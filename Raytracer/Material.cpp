#include "Material.h"

Material::Material()
{
	m_diffuse = Vector3(1.0f);
	m_specular = Vector3(0.0f);
	m_transparency = Vector3(0.0f);
	m_shininess = 0.0f;
	m_refractionIndex = 1.0f;
}

Material::~Material()
{
}

bool Material::isDiffuse() const {
	return m_diffuse.x > 0.0f || m_diffuse.y > 0.0f || m_diffuse.z > 0.0f;
}

bool Material::isSpecular() const { 
	return m_specular.x > 0.0f || m_specular.y > 0.0f || m_specular.z > 0.0f;
}
bool Material::isTransparent() const { 
	return m_transparency.x > 0.0f || m_transparency.y > 0.0f || m_transparency.z > 0.0f; 
}


Vector3
Material::shade(const Ray&, const HitInfo&, const Scene&) const
{
    return Vector3(1.0f, 1.0f, 1.0f);
}
