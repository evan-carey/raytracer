#ifndef CSE168_OBJECT_H_INCLUDED
#define CSE168_OBJECT_H_INCLUDED

#include <vector>
#include "Miro.h"
#include "Material.h"

class Object
{
public:
    Object() {}
    virtual ~Object() {}

    void setMaterial(const Material* m) {m_material = m;}

    virtual void renderGL() {}
    virtual void preCalc() {}


    virtual bool intersect(HitInfo& result, const Ray& ray,
                           float tMin = 0.0f, float tMax = MIRO_TMAX) = 0;

	
	virtual Vector3 min() const { return m_min; }
	virtual Vector3 max() const { return m_max; }
	
	virtual const Vector3& center() const { return m_center; }

protected:
    const Material* m_material;

	// object bounds
	Vector3 m_min, m_max;
	// object centroid
	Vector3 m_center;
};

typedef std::vector<Object*> Objects;

#endif // CSE168_OBJECT_H_INCLUDED
