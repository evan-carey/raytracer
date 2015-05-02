#include "BoundingBox.h"


BoundingBox::BoundingBox() {
	m_min = Vector3(0.0f, 0.0f, 0.0f);
	m_max = Vector3(0.0f, 0.0f, 0.0f);
	m_surfaceArea = 0.0f;
}

BoundingBox::BoundingBox(const Vector3& min, const Vector3& max) : m_min(min), m_max(max) {
	m_surfaceArea = calcSurfaceArea(max - min);
}

BoundingBox::~BoundingBox() {
}

float calcSurfaceArea(Vector3 v) {
	return 2.0f * (
		v.z * v.x + // length * width
		v.z * v.y + // length * height
		v.x * v.y   // width * height
	);
}