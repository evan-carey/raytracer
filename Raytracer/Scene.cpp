#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Console.h"
#include "Sphere.h"
#include "PhongMaterial.h"



#ifdef OPEN_MP
#include <omp.h>
#endif

using namespace std;

Scene * g_scene = 0;

Scene::Scene() 
	: m_photonMap(NUM_PHOTONS * MAX_LIGHTS * NUM_PHOTON_CALLS), 
	  m_causticMap(NUM_CAUSTIC_PHOTONS * MAX_LIGHTS * NUM_PHOTON_CALLS) {

}

void Scene::openGL(Camera *cam) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cam->drawGL();

    // draw objects
    for (size_t i = 0; i < m_objects.size(); ++i)
        m_objects[i]->renderGL();

    glutSwapBuffers();
}

void
Scene::preCalc() {

	// Pre-calc scene objects
    Objects::iterator it;
    for (it = m_objects.begin(); it != m_objects.end(); it++) {
        Object* pObject = *it;
        pObject->preCalc();
    }

	// Pre-calc scene lights
    Lights::iterator lit;
    for (lit = m_lights.begin(); lit != m_lights.end(); lit++) {
        PointLight* pLight = *lit;
        pLight->preCalc();
    }

	// Build BVH
	clock_t start = std::clock(); // track BVH building time
    m_bvh.build(&m_objects);
	StatsReporter::calcBVHBuildTime(start);

#ifdef USE_PHOTON_MAPPING
	// Build photon map
	calcPhotonMap(m_photonMap, false);
	calcPhotonMap(m_causticMap, true);
#endif
}

void
Scene::raytraceImage(Camera *cam, Image *img) {   

	// track rendering time
	clock_t start = std::clock();

    // loop over all pixels in the image
#ifdef OPEN_MP
	#pragma omp parallel for schedule(dynamic)
#endif
    for (int j = 0; j < img->height(); ++j)
    {
        for (int i = 0; i < img->width(); ++i)
        {
			Vector3 finalResult(0.0f);

#if defined USE_PATH_TRACING || defined USE_DOF
			// Sample each pixel multiple times
			for (int k = 0; k < SAMPLES; k++) 
#endif
			{ 
				Vector3 shadeResult(0.0f);
				Ray ray;
#if defined USE_PATH_TRACING || defined USE_DOF
				// create ray through random point in pixel (i,j)
				ray = cam->randomEyeRay(i, j, img->width(), img->height());
#else
				ray = cam->eyeRay(i, j, img->width(), img->height());
#endif
				//if (trace(hitInfo, ray))
				if (trace(ray, 0, shadeResult)) {
					//shadeResult = hitInfo.material->shade(ray, hitInfo, *this);
					//img->setPixel(i, j, shadeResult);
					finalResult += shadeResult;
				} else {
					//img->setPixel(i, j, cam->bgColor());
					finalResult += bgColor(ray);
					//break;
				}
			}
#if defined USE_PATH_TRACING || defined USE_DOF
			finalResult /= (float)SAMPLES;
#endif
			img->setPixel(i, j, finalResult);
        }

        img->drawScanline(j);

        glFinish();
#ifdef OPEN_MP
		if (omp_get_thread_num() == 0) 
#endif
		{
			printf("Rendering Progress: %.3f%%\r", j / float(img->height())*100.0f);
			fflush(stdout);
		}
    }
    
	StatsReporter::calcRenderTime(start);

    printf("Rendering Progress: 100.000%\n");
    debug("done Raytracing!\n");

	StatsReporter::writeToFile("README.txt");
}

bool
Scene::trace(HitInfo& minHit, const Ray& ray, float tMin, float tMax) const
{
   return m_bvh.intersect(minHit, ray, tMin, tMax);
}

bool Scene::trace(const Ray& ray, int numCalls, Vector3& res) {
	res = Vector3(0.0f);
	HitInfo hit;
	if (numCalls <= NUM_TRACE_CALLS) {
		if (trace(hit, ray, 0.0f, MIRO_TMAX)) {

			res = hit.material->shade(ray, hit, *this);


			// indirect diffuse lighting
			if (hit.material->isDiffuse()) {
#ifdef USE_PHOTON_MAPPING
				// apply photon map
				float P[3] = { hit.P.x, hit.P.y, hit.P.z };
				float N[3] = { hit.N.x, hit.N.y, hit.N.z };
				float E[3] = { 0.0f, 0.0f, 0.0f }; // irradiance
				float C[3] = { 0.0f, 0.0f, 0.0f }; // caustic

				m_photonMap.irradiance_estimate(E, P, N, GLOBAL_MAX_DISTANCE, GLOBAL_PHOTONS_TO_USE);
				m_causticMap.irradiance_estimate(C, P, N, CAUSTIC_MAX_DISTANCE, CAUSTIC_PHOTONS_TO_USE);
				res += Vector3(E[0], E[1], E[2]);
				res += Vector3(C[0], C[1], C[2]);
#endif
#ifdef USE_PATH_TRACING
				{
					Ray indirect = ray.indirectRay(hit);
					Vector3 indirectRes;
					if (trace(indirect, numCalls + 1, indirectRes)) {
						res += indirectRes * hit.material->getDiffuse() * 0.7f;
					}
				}
#endif // USE_PATH_TRACING
			}

			// check reflection
			if (hit.material->isSpecular()) {
				Ray reflection = ray.reflect(hit);
				Vector3 reflectionRes;
				// recurse on reflection ray
				if (trace(reflection, numCalls + 1, reflectionRes)) {
					res += reflectionRes * hit.material->getSpecular();
				}
			}

			// check refraction
			if (hit.material->isTransparent()) {
				// calc reflectance 
				float fresnel = ray.calcFresnel(hit);
				if (fresnel > 0.01) {
					Ray reflection = ray.reflect(hit);
					Vector3 reflectionRes;
					if (trace(reflection, numCalls + 1, reflectionRes)) {
						res += reflectionRes * hit.material->getTransparency() * fresnel;
					}
				}
				// calc transmission
				Ray refraction = ray.refract(hit);
				Vector3 refractionRes;
				// recurse on refraction ray
				if (trace(refraction, numCalls + 1, refractionRes)) {
					res += refractionRes * hit.material->getTransparency() * (1.0f - fresnel);
				}
			}
			return true;
		} else {
			res = bgColor(ray);
			return true;
		}
	}
	return false;
}

void Scene::calcPhotonMap(Photon_map& map, bool isCaustic) {

	int Max_Photons = isCaustic ? NUM_CAUSTIC_PHOTONS : NUM_PHOTONS;

	if (Max_Photons == 0) {
		map.balance();
		return;
	}

	int photonsEmitted = 0;
	int photonsAdded = 0;

	// trace photons from each light
	Lights::const_iterator lightIter;
	for (lightIter = lights()->begin(); lightIter != lights()->end(); lightIter++) {
		if (!(dynamic_cast<AreaLight*>(*lightIter))) {
			continue;
		}
		AreaLight* aLight = dynamic_cast<AreaLight*>(*lightIter);
		while (photonsAdded < Max_Photons) {
#ifdef OPEN_MP
#pragma omp parallel for
#endif
			for (int i = 0; i < 100000; i++) {
				if (photonsAdded < Max_Photons) {
					Vector3 pFlux = aLight->color() * aLight->wattage();
					SpotLight* spLight = dynamic_cast<SpotLight*>(aLight);
					if (spLight) {
						//if (isCaustic) pFlux *= PI * spLight->radius() * spLight->radius();
						//else 
						pFlux *= PI * spLight->radius() * spLight->radius();
					}
					//SquareLight* sqLight = dynamic_cast<SquareLight*>(aLight);
					//if (sqLight) {
					//	//float r = sqLight->size()[0] / 2.0f;
					//	//pFlux *= PI * r * r;
					//}
					Vector3 pOrigin = aLight->getPhotonOrigin();
					Vector3 pDir = aLight->getPhotonDirection();
					int numPhotons = tracePhoton(pFlux, pOrigin, pDir, 0, isCaustic);

#pragma omp critical
					{
						photonsAdded += numPhotons;
						photonsEmitted++;

					}
				}
				if (omp_get_thread_num() == 0) {
					printf("Building Photon Map: %.3f%%\r", 100.0f * ((float)photonsAdded) / (float)(Max_Photons * m_lights.size()));
					fflush(stdout);
				}
			}
		}
	}
	printf("Building Photon Map: 100.000%\n");
	map.scale_photon_power(1.0f / (float)photonsEmitted);
	map.balance();
}



int Scene::tracePhoton(const Vector3& flux, const Vector3& origin, const Vector3& dir, int numCalls, bool isCaustic) {
	if (numCalls > NUM_PHOTON_CALLS) {
		return 0;
	}
	

	numCalls++;

	// photon path (as a ray, so we can reuse reflecting/refracting properties)
	Ray photonRay(origin + (dir * epsilon), dir);
	
	HitInfo hit;
	if (trace(hit, photonRay)) {
		// Russian roulette - randomly choose what type of bounce based on hit material

		Vector3 diffuse(hit.material->getDiffuse());
		if (hit.material->isTextured()) {
			diffuse = hit.material->getTexture()->getColor3D(hit.P);
		}
		
		float w_d = avg(diffuse); // diffuse weight
		float w_s = avg(hit.material->getSpecular()); // specular reflection weight
		float w_t = avg(hit.material->getTransparency()); // refraction weight

		float r = ((float)rand() / (float)RAND_MAX);// *(w_d + w_s + w_t);

		if (r < w_d) {
			// Diffuse
			int photonsAdded = 0;
			if (numCalls > 1) {
				float a_flux[3] = { flux.x, flux.y, flux.z };
				float a_pos[3] = { hit.P.x, hit.P.y, hit.P.z };
				float a_dir[3] = { dir.x, dir.y, dir.z };
#ifdef OPEN_MP
#pragma omp critical
#endif
				{
					// store photon in map
					
					if (isCaustic) {
						m_causticMap.store(a_flux, a_pos, a_dir);
#ifdef DEBUG_CAUSTICS
						Sphere* caus = new Sphere;
						caus->setCenter(hit.P);
						caus->setRadius(0.02f);
						Vector3 pow = flux.normalized();
						caus->setMaterial(new PhongMaterial(pow.x, pow.y, pow.z));
						addObject(caus);
#endif
					} else {
						m_photonMap.store(a_flux, a_pos, a_dir);
#ifdef DEBUG_PHOTONS
						Sphere* caus = new Sphere;
						caus->setCenter(hit.P);
						caus->setRadius(0.02f);
						caus->setMaterial(new PhongMaterial(flux.x, flux.y, flux.z));
						addObject(caus);
#endif
					}
					photonsAdded++;

				}
			} else {
				if (isCaustic) return 0;
			}
			// bounce
			Ray indirectPhoton = photonRay.indirectRay(hit);
			return photonsAdded + tracePhoton(diffuse * flux / w_d, indirectPhoton.o, indirectPhoton.d, numCalls, isCaustic);
			
		} else if (r < w_d + w_s) {
			// Reflect
			if (numCalls == 1 && !isCaustic) return 0;

			Ray reflectPhoton = photonRay.reflect(hit);
			return tracePhoton(flux, reflectPhoton.o, reflectPhoton.d, numCalls, isCaustic);
		} else if (r < w_d + w_s + w_t) {
			// Refract
			if (numCalls == 1 && !isCaustic) return 0;
			
			if (((float)rand() / (float)RAND_MAX) < photonRay.calcFresnel(hit)) {
				// reflect
				Ray reflectPhoton = photonRay.reflect(hit);
				return tracePhoton(flux, reflectPhoton.o, reflectPhoton.d, numCalls, isCaustic);
			} else 
			{
				// refract
				Ray refractPhoton = photonRay.refract(hit);
				return tracePhoton(flux, refractPhoton.o, refractPhoton.d, numCalls, isCaustic);
			}
		} else {
			// Absorb
			return 0;
		}
	}
	return 0;
}

Vector3 Scene::bgColor(const Ray& ray) {
	if (!m_envMap) {
		return g_camera->bgColor();
	}

	float phi = atan2(ray.d.x, ray.d.z) + m_mapRotation[0] + PI;
	float theta = asin(ray.d.y) + m_mapRotation[1];

	// clamp values
	if (phi > 2.0f * PI) {
		phi -= 2.0f * PI;
	}
	if (theta > PI / 2.0f) {
		theta -= 2.0f * (theta - PI / 2.0f);
		phi += PI;
	}

	// convert to texture coordinates
	float u = phi / (2.0f * PI);
	float v = 0.5f + theta / PI;

	return m_envMap->getColor2D(TexPoint(u, v));
}