#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Console.h"

#define NUM_TRACE_CALLS 32
#define OPEN_MP

#define USE_PATH_TRACING  // uncomment to use Monte Carlo path tracing
#ifdef USE_PATH_TRACING
    #define SAMPLES 10

    //#define USE_RUSSIAN_ROULETTE // uncomment to use Russian Roulette
    #define CHANCE_TO_TERMINATE 0.5f
#endif


#ifdef OPEN_MP
#include <omp.h>
#endif

Scene * g_scene = 0;

void
Scene::openGL(Camera *cam)
{
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

#ifdef USE_PATH_TRACING
			// Sample each pixel multiple times for path tracing
			for (int k = 0; k < SAMPLES; k++) 
#endif
			{ 
				Vector3 shadeResult(0.0f);
				Ray ray;
#ifdef USE_PATH_TRACING
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
					//finalResult = cam->bgColor();
					//break;
				}
			}
#ifdef USE_PATH_TRACING
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
	if (numCalls < NUM_TRACE_CALLS) {
		if (trace(hit, ray)) {

#ifndef USE_RUSSIAN_ROULETTE
			res = hit.material->shade(ray, hit, *this);
#endif

#ifdef USE_PATH_TRACING
			// indirect diffuse lighting
			if (hit.material->isDiffuse()) {
#ifdef USE_RUSSIAN_ROULETTE
				if ((float)rand()/(float)RAND_MAX < CHANCE_TO_TERMINATE) {
					res = 2.0f * hit.material->shade(ray, hit, *this);
				} else 
#endif // USE_RUSSIAN_ROULETTE
				{
					Ray indirect = ray.indirectRay(hit);
					Vector3 indirectRes;
					if (trace(indirect, numCalls + 1, indirectRes)) {
						res += indirectRes * hit.material->getDiffuse() * 0.7f;
					}
				}
			}
#endif // USE_PATH_TRACING

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
				Ray refraction = ray.refract(hit);
				Vector3 refractionRes;
				// recurse on refraction ray
				if (trace(refraction, numCalls + 1, refractionRes)) {
					res += refractionRes * hit.material->getTransparency();
				}
			}
			return true;
		} else {
			res = g_camera->bgColor();
			return true;
		}
	}
	return false;
}