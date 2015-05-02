#pragma once
#include "Vector3.h"

class BoundingBox {
public:
	
	BoundingBox();
	BoundingBox(const Vector3& min, const Vector3& max);
	virtual ~BoundingBox();

	Vector3 min() { return m_min; }
	Vector3 max() { return m_max; }
	float surfaceArea() { return m_surfaceArea; }

protected:
	Vector3 m_min; // minimum x,y,z bounds
	Vector3 m_max; // maximum x,y,z bounds

	float m_surfaceArea;
};

