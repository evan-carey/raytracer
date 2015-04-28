#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Console.h"

#define NUM_TRACE_CALLS 5
#define OPEN_MP 1

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
Scene::preCalc()
{
    Objects::iterator it;
    for (it = m_objects.begin(); it != m_objects.end(); it++)
    {
        Object* pObject = *it;
        pObject->preCalc();
    }
    Lights::iterator lit;
    for (lit = m_lights.begin(); lit != m_lights.end(); lit++)
    {
        PointLight* pLight = *lit;
        pLight->preCalc();
    }

    m_bvh.build(&m_objects);
}

void
Scene::raytraceImage(Camera *cam, Image *img) 
{   
    // loop over all pixels in the image
#ifdef OPEN_MP
	#pragma omp parallel for schedule(dynamic)
#endif
    for (int j = 0; j < img->height(); ++j)
    {
        for (int i = 0; i < img->width(); ++i)
        {

            Ray ray = cam->eyeRay(i, j, img->width(), img->height());
			Vector3 shadeResult;
            //if (trace(hitInfo, ray))
			if (trace(ray, 0, shadeResult))
            {
                //shadeResult = hitInfo.material->shade(ray, hitInfo, *this);
                img->setPixel(i, j, shadeResult);
			} else {
				img->setPixel(i, j, cam->bgColor());
			}
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
    
    printf("Rendering Progress: 100.000%\n");
    debug("done Raytracing!\n");
}

bool
Scene::trace(HitInfo& minHit, const Ray& ray, float tMin, float tMax) const
{
   return m_bvh.intersect(minHit, ray, tMin, tMax);
}

bool Scene::trace(const Ray& ray, int numCalls, Vector3& res) {
	HitInfo hit;
	if (numCalls < NUM_TRACE_CALLS && trace(hit, ray)) {
			res = hit.material->shade(ray, hit, *this);

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
	}
	return false;
}