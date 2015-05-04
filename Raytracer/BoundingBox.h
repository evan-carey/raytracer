#pragma once
#include "Vector3.h"
#include "Ray.h"

class BoundingBox {
public:
	
	BoundingBox();
	BoundingBox(const Vector3& min, const Vector3& max);
	virtual ~BoundingBox();

	Vector3 min() const { return m_min; }
	Vector3 max() const { return m_max; }
	float surfaceArea() const { return m_surfaceArea; }

	bool hit(const Ray& ray, float tMin, float tMax) const;

	

protected:
	Vector3 m_min; // minimum x,y,z bounds
	Vector3 m_max; // maximum x,y,z bounds

	float m_surfaceArea;

private:
	float calcSurfaceArea(Vector3 v);
};

