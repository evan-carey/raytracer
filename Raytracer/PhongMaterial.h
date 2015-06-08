#pragma once
#include "Material.h"
class PhongMaterial : public Material {
public:
	PhongMaterial(const Vector3& diffuse = Vector3(1.0f), 
		const Vector3& specular = Vector3(0.0f), 
		const Vector3& transparency = Vector3(0.0f), 
		const float shininess = 1.0f, 
		const float refractionIndex = 1.0f);

	virtual ~PhongMaterial();

	virtual Vector3 shade(const Ray& ray, const HitInfo& hit,
		const Scene& scene) const;

protected:

};

