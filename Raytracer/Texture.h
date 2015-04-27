#pragma once
#include "Vector3.h"

/*
A point in a texture specified by its (u,v) coordinates
*/
class TexPoint {
public:
	TexPoint(float u = 0.0f, float v = 0.0f) : m_u(u), m_v(v) {}
	float u() { return m_u; }
	float v() { return m_v; }
protected:
	float m_u;
	float m_v;
};



class Texture {
public:
	Texture();
	virtual ~Texture();

	virtual Vector3 getColor(TexPoint& p) const { return Vector3(0.0f); }
	virtual Vector3 getColor3D(Vector3& v) const { return Vector3(0.0f); }
};

