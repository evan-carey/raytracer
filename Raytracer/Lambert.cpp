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
Lambert::shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const
{
    Vector3 L = Vector3(0.0f, 0.0f, 0.0f);
    
    const Vector3 viewDir = -ray.d; // d is a unit vector
    
    const Lights *lightlist = scene.lights();
    
    // loop over all of the lights
    Lights::const_iterator lightIter;
    for (lightIter = lightlist->begin(); lightIter != lightlist->end(); lightIter++)
    {
        PointLight* pLight = *lightIter;
    
        Vector3 l = pLight->position() - hit.P;

#ifdef SHADOWS
		// check for shadow
		Ray shadow(hit.P + (l.normalized() * epsilon), l);
		HitInfo shadowHit;
		// if theree's an object between hitpoint and light source, don't shade it
		if (scene.trace(shadowHit, shadow, 0.0f, l.length())) {
			if (!shadowHit.material->isTransparent()) {
				continue;
			}
			if (dot(shadowHit.N, l) < 0.0f) {
				continue;
			}
		}
#endif
        
        // the inverse-squared falloff
        float falloff = l.length2();
        
        // normalize the light direction
        l /= sqrt(falloff);

        // get the diffuse component
        float nDotL = dot(hit.N, l);
        Vector3 result = pLight->color();
        result *= m_kd;
        
        L += std::max(0.0f, nDotL/falloff * pLight->wattage() / PI) * result;
    }
    
    // add the ambient component
    L += m_ka;
    
    return L;
}
