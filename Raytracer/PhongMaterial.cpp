#include "PhongMaterial.h"
#include "Ray.h"
#include "Scene.h"
#include <algorithm>


PhongMaterial::PhongMaterial(const Vector3& kd, const Vector3& ks, const Vector3& kt, const float shine, const float ri) 
	: m_diffuse(kd), m_specular(ks), m_transparency(kt), m_shininess(shine), m_refractionIndex(ri) {


}


PhongMaterial::~PhongMaterial() {
}

Vector3 PhongMaterial::shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const {

	Vector3 L = Vector3(0.0f, 0.0f, 0.0f);

	const Vector3 viewDir = -ray.d; // d is a unit vector

	const Lights *lightlist = scene.lights();

	// loop over all of the lights
	Lights::const_iterator lightIter;
	for (lightIter = lightlist->begin(); lightIter != lightlist->end(); lightIter++) {
		PointLight* pLight = *lightIter;

		Vector3 l = pLight->position() - hit.P;


		// Reflection vector: r = d + 2(d dot n)n
		Vector3 r = (-l + 2 * dot(l, hit.N) * hit.N).normalized();

		// the inverse-squared falloff
		float falloff = l.length2();

		// normalize the light direction
		l /= sqrt(falloff);

		falloff = 1.0f / (falloff * 4.0f * PI*PI);

		// get the diffuse component
		float nDotL = dot(hit.N, l);

		Vector3 result = pLight->color();

		L += (std::max(0.0f, nDotL * falloff * pLight->wattage() ) * m_diffuse * m_diffuse) * result;

		// get the specular component
		//float eDotR = dot(viewDir, r);
		float eDotR = std::max(0.0f, std::min(1.0f, dot(-ray.d, r)));
		L += std::max(0.0f, eDotR * falloff * pLight->wattage());
	}



	return L;
}