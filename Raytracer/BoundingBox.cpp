#include "BoundingBox.h"
#include <algorithm>


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

float BoundingBox::calcSurfaceArea(Vector3 v) {
	return 2.0f * (
		v.z * v.x + // length * width
		v.z * v.y + // length * height
		v.x * v.y   // width * height
	);
}

bool BoundingBox::hit(const Ray& ray, float tMin, float tMax) const {
	float tx1 = (min().x - ray.o.x) / ray.d.x;
	float tx2 = (max().x - ray.o.x) / ray.d.x;

	float ty1 = (min().y - ray.o.y) / ray.d.y;
	float ty2 = (max().y - ray.o.y) / ray.d.y;

	float tz1 = (min().z - ray.o.z) / ray.d.z;
	float tz2 = (max().z - ray.o.z) / ray.d.z;

	float min = std::max({ std::min(tx1, tx2), std::min(ty1, ty2), std::min(tz1, tz2) });
	float max = std::min({ std::max(tx1, tx2), std::max(ty1, ty2), std::max(tz1, tz2) });

	//if (min < 0.0f) return false; // inside box
	if (max < 0.0f) return false; //box behind origin

	if (max < min || max < tMin || min > tMax) return false;

	return true;
}