#ifndef CSE168_MATERIAL_H_INCLUDED
#define CSE168_MATERIAL_H_INCLUDED

#include "Miro.h"
#include "Vector3.h"

class Material
{
public:
    Material();
    virtual ~Material();

    virtual void preCalc() {}
    
    virtual Vector3 shade(const Ray& ray, const HitInfo& hit,
                          const Scene& scene) const;

	Vector3 getDiffuse() const { return m_diffuse; }
	Vector3 getSpecular() const { return m_specular; }
	Vector3 getTransparency() const { return m_transparency; }

	bool isDiffuse() const;
	bool isSpecular() const;
	bool isTransparent() const;

	float refractionIndex() const { return m_refractionIndex; }
	
protected:
	Vector3 m_diffuse;
	Vector3 m_specular; // specular reflection vector
	Vector3 m_transparency; // specular refraction vector

	float m_shininess;
	float m_refractionIndex;
};

#endif // CSE168_MATERIAL_H_INCLUDED
