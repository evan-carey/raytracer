#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Console.h"

#define NUM_TRACE_CALLS 3

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
    Ray ray;
    HitInfo hitInfo;
    Vector3 shadeResult;
    
    // loop over all pixels in the image
    for (int j = 0; j < img->height(); ++j)
    {
        for (int i = 0; i < img->width(); ++i)
        {
            ray = cam->eyeRay(i, j, img->width(), img->height());
            //if (trace(hitInfo, ray))
			if (trace(ray, 0, shadeResult))
            {
                //shadeResult = hitInfo.material->shade(ray, hitInfo, *this);
                img->setPixel(i, j, shadeResult);
            }
        }
        img->drawScanline(j);
        glFinish();
        printf("Rendering Progress: %.3f%%\r", j/float(img->height())*100.0f);
        fflush(stdout);
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
		numCalls++;

		// check reflection
		if (hit.material->isSpecular()) {
			Ray reflection = ray.reflect(hit);
			Vector3 reflectionRes;
			// recurse on reflection ray
			if (trace(reflection, numCalls, reflectionRes)) {
				res += reflectionRes * hit.material->getSpecular();
			}
		}

		// check refraction
		if (hit.material->isTransparent()) {
			Ray refraction = ray.refract(hit);
			Vector3 refractionRes;
			if (trace(refraction, numCalls, refractionRes)) {
				res += refractionRes * hit.material->getTransparency();
			}
		}
		return true;
	}
	return false;
}