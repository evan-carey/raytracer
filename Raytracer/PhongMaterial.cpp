#include "PhongMaterial.h"
#include "Ray.h"
#include "Scene.h"
#include <algorithm>

#define PHONG_CONSTANT 1


PhongMaterial::PhongMaterial(const Vector3& kd, const Vector3& ks, const Vector3& kt, const float shine, const float ri) {

	m_diffuse = kd;
	m_specular = ks;
	m_transparency = kt;
	m_shininess = shine;
	m_refractionIndex = ri;
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



		// the inverse-squared falloff 
		float falloff = 1.0f / (4.0f * PI * l.length2());

		// normalize the light direction
		l.normalize();

		// get the diffuse component
		float nDotL = dot(hit.N, l);

		Vector3 result = pLight->color();
		result *= m_diffuse;

		// E = (phi * (n dot l)) / 4 * PI * r^2
		float irradiance = pLight->wattage() * nDotL * falloff;

		L += (std::max(0.0f, irradiance ) ) * result;


		// get the specular component

		// Reflection vector: r = d + 2(d dot n)n
		Vector3 r = (-l + 2 * dot(l, hit.N) * hit.N).normalized();

		float eDotR = dot(viewDir, r);
		//float eDotR = std::max(0.0f, std::min(1.0f, dot(-ray.d, r)));
		eDotR = 0.0f > eDotR ? 0.0f : 1.0f < eDotR ? 1.0f : eDotR; // clamp it to [0..1]
		eDotR = pow(eDotR, PHONG_CONSTANT);
		L += std::max(0.0f, eDotR * falloff * pLight->wattage());
	}



	return L;
}