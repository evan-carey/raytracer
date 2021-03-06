#include "Ray.h"

Ray Ray::reflect(const HitInfo& hit) const {
	// ray's direction is the mirror reflection of current ray
	Vector3 dir = (this->d - 2 * dot(d, hit.N) * hit.N).normalized();
	// ray's origin is the hitpoint (accounting for error)
	Vector3 origin = hit.P + (dir * epsilon);

	return Ray(origin, dir);
}

Ray Ray::refract(const HitInfo& hit) const {

	Vector3 n;

	// indices of refraction
	float n1 = 1.0f;
	float n2 = 1.0f;

	if (dot(this->d, hit.N) < 0.0f) {
		// entering object
		n2 = hit.material->refractionIndex();
		n = hit.N;
	} else {
		// leaving object
		n1 = hit.material->refractionIndex();
		n = -hit.N;
	}

	// compute energy of refracted ray ( cos^2 (theta2) )
	float cosTheta1 = dot(this->d, n);
	float e = 1 - ((n1*n1) / (n2*n2)) * (1 - cosTheta1*cosTheta1);
	if (e < 0.0f) {
		return reflect(hit);
	}

	// create refraction ray
	Vector3 dir = (n1 / n2) * (this->d - n * cosTheta1) - n * (sqrt(e));
	Vector3 origin = hit.P + (dir * epsilon);
	return Ray(origin, dir);
}

float Ray::calcFresnel(const HitInfo& hit) const {
	float n1 = 1.0, n2 = 1.0f;
	Vector3 n;

	if (dot(this->d, hit.N) < 0.0f) {
		// entering object
		n2 = hit.material->refractionIndex();
		n = hit.N;
	} else {
		// leaving object
		n1 = hit.material->refractionIndex();
		n = -hit.N;
	}
	float cosTheta_i = dot(-d, n);
	float sinTheta_i = sin(acos(cosTheta_i));

	// calculate perpendicular reflectance (from http://en.wikipedia.org/wiki/Fresnel_equations)
	float n1n2sinTheta_i = (n1 / n2) * sinTheta_i;
	float n1n2sinThetaSquared = n1n2sinTheta_i * n1n2sinTheta_i;
	if (n1n2sinThetaSquared > 1.0f) return 1.0f; // above critical angle
	float n2sqrt = n2 * sqrtf(1.0f - n1n2sinThetaSquared);
	
	float res = (n1 * cosTheta_i - n2sqrt) / (n1 * cosTheta_i + n2sqrt);

	return res * res;
}

/********************************
Ray Ray::randomRay(const HitInfo& hit) const {

	// Important sampling method
	float s = (float)rand() / (float)RAND_MAX;
	float t = (float)rand() / (float)RAND_MAX;

	float u = 2.0f * PI * s;
	float v = sqrtf(1.0f - t);

	float p_x = v * cos(u);
	float p_y = sqrtf(t);
	float p_z = v * sin(u);

	Vector3 dir(p_x, p_y, p_z);
	Vector3 origin(hit.P + (dir * epsilon));
	return Ray(origin, dir);
}
**********************************/

Ray Ray::indirectRay(const HitInfo& hit) const {

	float theta = asin(sqrt((float)rand() / (float)RAND_MAX));
	float phi = 2.0f * PI * ((float)rand() / (float)RAND_MAX);

	Vector3 t_1 = cross(Vector3(0, 0, 1), hit.N);
	if (t_1.length() < epsilon) { 
		// if cross product equals 0, hit.N is parallel to the z-axis --> use y-axis instead
		t_1 = cross(Vector3(0, 1, 0), hit.N);
	}
	Vector3 t_2 = cross(t_1, hit.N);

	// convert spherical coordinates to vector
	Vector3 dir = cos(phi) * sin(theta) * t_1 +
		sin(phi) * sin(theta) * t_2 +
		cos(theta) * hit.N;

	dir.normalize();

	Vector3 origin = hit.P + (dir * epsilon);
	return Ray(origin, dir);
}