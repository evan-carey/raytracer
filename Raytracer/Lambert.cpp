#include "Lambert.h"
#include "Ray.h"
#include "Scene.h"
#include <algorithm>

#define SHADOWS

Lambert::Lambert(const Vector3 & kd, const Vector3 & ka) :
    m_kd(kd), m_ka(ka)
{

}

Lambert::~Lambert()
{
}

Vector3
Lambert::shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const {
    Vector3 L = Vector3(0.0f, 0.0f, 0.0f);
    
    const Vector3 viewDir = -ray.d; // d is a unit vector
    
    const Lights *lightlist = scene.lights();
    
    // loop over all of the lights
    Lights::const_iterator lightIter;
	for (lightIter = lightlist->begin(); lightIter != lightlist->end(); lightIter++) {
		PointLight* pLight = *lightIter;

		float contrib;
		float samples = 1.0f;
		if (dynamic_cast<SphereLight*>(*lightIter) || dynamic_cast<SquareLight*>(*lightIter)) {
			samples = 32.0f;
		}
		for (int i = 0; i < samples; i++) {
			Vector3 l = pLight->getPhotonOrigin(hit.P) - hit.P;

			float falloff = l.length2();
			l /= sqrt(falloff);
#ifdef SHADOWS
			// check for shadow
			Ray shadow(hit.P + (l * epsilon), l);
			HitInfo shadowHit;
			// if theree's an object between hitpoint and light source, don't shade it
			if (scene.trace(shadowHit, shadow, 0.0f, sqrt(falloff))) {

				continue;
			}
#endif
			// get the diffuse component
			float nDotL = dot(hit.N, l);
			Vector3 result = pLight->color();
			result *= m_kd;

			L += std::max(0.0f, nDotL / falloff * (pLight->wattage() / samples) / PI) * result;
		}
	}
	
	
    
    // add the ambient component
    L += m_ka;
    
    return L;
}
