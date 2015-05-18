#include "PhongMaterial.h"
#include "Ray.h"
#include "Scene.h"
#include <algorithm>

#define SHADOWS

PhongMaterial::PhongMaterial(const Vector3& kd, const Vector3& ks, const Vector3& kt, const float shine, const float ri) {

	m_diffuse = kd;
	m_specular = ks;
	m_transparency = kt;
	m_shininess = shine;
	m_refractionIndex = ri;

	// NOTE: d,s,t >= 0 && sum(d,s,t) <= 1
	for (int i = 0; i < 2; i++) {
		m_diffuse[i] = m_diffuse[i] < 0.0f ? 0.0f : m_diffuse[i] > 1.0f ? 1.0f : m_diffuse[i];
		m_specular[i] = m_specular[i] < 0.0f ? 0.0f : m_specular[i] > 1.0f ? 1.0f : m_specular[i];
		m_transparency[i] = m_transparency[i] < 0.0f ? 0.0f : m_transparency[i] > 1.0f ? 1.0f : m_transparency[i];
		
		m_transparency[i] = m_transparency[i] < 1.0f - m_specular[i] ? m_transparency[i] : 1.0f - m_specular[i];
		m_diffuse[i] = m_diffuse[i] < 1.0f - m_specular[i] - m_transparency[i] ? m_diffuse[i] : 1.0f - m_specular[i] - m_transparency[i];
	}
}


PhongMaterial::~PhongMaterial() {
}

Vector3 PhongMaterial::shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const {

	Vector3 L = Vector3(0.0f, 0.0f, 0.0f);

	const Vector3 viewDir = -ray.d; // d is a unit vector

	const Lights *lightlist = scene.lights();

	// diffuse color
	Vector3 color(m_diffuse);

	if (hit.material->isTextured()) {
		color = m_texture->getColor3D(Vector3(hit.P));
	}

	// loop over all of the lights
	Lights::const_iterator lightIter;
	for (lightIter = lightlist->begin(); lightIter != lightlist->end(); lightIter++) {
		PointLight* pLight = *lightIter;

		Vector3 l = pLight->position() - hit.P;

		// the inverse-squared falloff
		float falloff = l.length2();

		// normalize the light direction
		l /= sqrt(falloff);

#ifdef SHADOWS
		// check for shadow
		Ray shadow(hit.P + (l * epsilon), l);
		HitInfo shadowHit;
		// if theree's an object between hitpoint and light source, don't shade it
		if (scene.trace(shadowHit, shadow, 0.0f, sqrt(falloff))) {
			if (!shadowHit.material->isTransparent()) {
				continue;
			}
			if (dot(shadowHit.N, l) < 0.0f) {
				continue;
			}
			continue;
		}
#endif

		// the inverse-squared falloff 
		falloff = 1.0f / (4.0f * PI * falloff);

		// normalize the light direction
		//l.normalize();

		// get the diffuse component
		float nDotL = dot(hit.N, l);

		Vector3 result = pLight->color();
		result *= color;

		// E = (phi * (n dot l)) / 4 * PI * r^2
		float irradiance = pLight->wattage() * nDotL * falloff;

		L += (std::max(0.0f, irradiance ) ) * result;


		// get the specular component
		if (isSpecular()) {
			// Reflection vector: r = d + 2(d dot n)n
			Vector3 r = (-l + 2 * dot(l, hit.N) * hit.N).normalized();

			float eDotR = dot(viewDir, r);
			eDotR = 0.0f > eDotR ? 0.0f : 1.0f < eDotR ? 1.0f : eDotR; // clamp it to [0..1]
			eDotR = pow(eDotR, (int)shininess());
			L += eDotR * falloff * pLight->wattage();
		}
	}



	return L;
}