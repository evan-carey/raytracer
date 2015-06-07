#ifndef CSE168_POINTLIGHT_H_INCLUDED
#define CSE168_POINTLIGHT_H_INCLUDED

#include <vector>
#include "Vector3.h"

class PointLight {
public:
	PointLight() {}
	virtual ~PointLight() {}

	void setPosition(const Vector3& v) { m_position = v; }
	void setColor(const Vector3& v) { m_color = v; }
	void setWattage(float f) { m_wattage = f; }

	float wattage() const { return m_wattage; }
	const Vector3 & color() const { return m_color; }
	const Vector3& position() const { return m_position; }

	virtual void preCalc() {} // use this if you need to

	virtual Vector3 getPhotonOrigin(const Vector3& hitPos = NULL) { return m_position; }
	virtual Vector3 getPhotonDirection() {
		float x, y, z;
		do {
			x = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
			y = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
			z = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
		} while (x*x + y*y + z*z > 1.0f);
		return Vector3(x, y, z).normalize();
	}

	virtual Vector3 calcLightDir(const Vector3& pOrigin, const Vector3& hitpoint) {
		return pOrigin - hitpoint;
	}

protected:
	Vector3 m_position;
	Vector3 m_color;
	float m_wattage;
    
};

typedef std::vector<PointLight*> Lights;


class AreaLight : public PointLight {
public:

protected:

};

class SquareLight : public AreaLight {
public:
	SquareLight() { m_normal = Vector3(0, 1, 0); }
	virtual ~SquareLight() {}

	virtual void setNormal(const Vector3& n) { m_normal = n.normalized(); }
	void setSize(float s) { m_size[0] = s; m_size[1] = s; }
	void setSize(float s, float t) { m_size[0] = s; m_size[1] = t; }

	virtual const Vector3& normal() { return m_normal; }
	float* size() { return m_size; }


	virtual void preCalc() {
		// set surface tangents
		m_tan1 = cross(Vector3(0, 0, 1), m_normal);
		if (m_tan1.length2() < epsilon)
			m_tan1 = cross(Vector3(0, 1, 0), m_normal);
		m_tan2 = cross(m_tan1, m_normal);
	}

	virtual Vector3 getPhotonOrigin(const Vector3& hitPos = NULL) {
		float dx = (1.0f - (2.0f * ((float)rand() / (float)RAND_MAX))) * (m_size[0] / 2.0f);
		float dy = (1.0f - (2.0f * ((float)rand() / (float)RAND_MAX))) * (m_size[1] / 2.0f);

		return m_position + dx * m_tan1 + dy * m_tan2;
	}
	virtual Vector3 getPhotonDirection() {
		/*float u = (float)rand() / (float)RAND_MAX;
		float v = 2.0f * PI * ((float)rand() / (float)RAND_MAX);
		return Vector3(cos(v) * sqrtf(u), sin(v) * sqrtf(u), sqrtf(1.0f - u)).normalize();*/

		float phi = 2.0f * PI * ((float)rand() / (float)RAND_MAX);
		float theta = asin(sqrt((float)rand() / (float)RAND_MAX));

		float u1 = sin(theta) * cos(phi);
		float u2 = sin(theta) * sin(phi);
		float u3 = cos(theta);

		Vector3 t1 = cross(Vector3(0, 0, 1), m_normal);
		if (t1.length2() < epsilon) t1 = cross(Vector3(0, 1, 0), m_normal);
		Vector3 dir = (u1 * t1 + u2 * cross(t1, m_normal) + u3 * m_normal);
		return dir.normalized();

		//Vector3 dir = cos(phi) * sin(theta) * m_tan1 + sin(phi) * sin(theta) * m_tan2 + cos(theta) * m_normal;
		//return dir.normalized();

	}

	virtual Vector3 calcLightDir(const Vector3& pOrigin, const Vector3& hitpoint) {
		return -m_normal;
	}

protected:
	Vector3 m_normal; // light direction
	Vector3 m_tan1, m_tan2; // surface tangents
	float m_size[2];
};


class SphereLight : public AreaLight {
public:
	SphereLight(float rad = 1.0f) : m_radius(rad) {}
	virtual ~SphereLight() {}

	void setRadius(float rad) { m_radius = rad; }
	float radius() { return m_radius; }

	virtual void preCalc() {}

	virtual Vector3 getPhotonOrigin(const Vector3& hitPos = NULL) {
		if (hitPos != NULL) {
			// disc sampling
			Vector3 tan1, tan2;
			Vector3 l = m_position - hitPos;
			tan1 = cross(Vector3(0, 0, 1), l);
			if (tan1.length2() < epsilon) tan1 = cross(Vector3(1, 0, 0), l);
			tan2 = cross(tan1, l);

			float x, y;
			do {
				x = (1 - 2 * ((float)rand() / (float)RAND_MAX)) * m_radius;
				y = (1 - 2 * ((float)rand() / (float)RAND_MAX)) * m_radius;
			} while (x*x + y*y > m_radius*m_radius);

			return m_position + tan1 * x + tan2 * y;
		} else {
			float x, y,z;
			do {
				x = (1 - 2 * ((float)rand() / (float)RAND_MAX)) * m_radius;
				y = (1 - 2 * ((float)rand() / (float)RAND_MAX)) * m_radius;
				z = (1 - 2 * ((float)rand() / (float)RAND_MAX)) * m_radius;
			} while (x*x + y*y + z*z > m_radius*m_radius);

			return m_position + Vector3(x,y,z);
		}
	}


protected:
	float m_radius;
};

class SpotLight : public SquareLight {
public:
	SpotLight(Vector3 dir = Vector3(0,1,0), float rad = 1.0f, float theta = PI/12.0f, float phi = PI/4.0f) 
		: m_radius(rad), m_theta(theta), m_phi(phi) {
		setNormal(dir);
	}

	void setRadius(float rad) { m_radius = rad; }
	float radius() { return m_radius; }

	void setTheta(float theta) { m_theta = theta; }
	float theta() { return m_theta; }

	void setPhi(float phi) { m_phi = phi; }
	float phi() { return m_phi; }

	virtual Vector3 getPhotonOrigin(const Vector3& hitPos = NULL) {
		float x, y;
		do {
			x = (1 - 2 * ((float)rand() / (float)RAND_MAX)) * m_radius;
			y = (1 - 2 * ((float)rand() / (float)RAND_MAX)) * m_radius;
		} while (x*x + y*y > m_radius*m_radius);

		return m_position + m_tan1 * x + m_tan2 * y;
	}

	
protected:
	float m_radius;
	float m_theta; // inner cone (in radians)
	float m_phi; // outer cone (in radians)
};

#endif // CSE168_POINTLIGHT_H_INCLUDED
