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

	if (dot(hit.N, this->d) < 0) {
		// entering object
		n2 = hit.material->refractionIndex();
		n = hit.N;
	} else {
		// leaving object
		n1 = hit.material->refractionIndex();
		n = -hit.N;
	}

	// compute energy of refracted ray ( cos^2 (theta2) )
	float cosTheta1 = dot(this->d, n); // NOTE: should this be n or hit.N?
	float e = 1 - ((n1*n1) / (n2*n2)) * (1 - cosTheta1*cosTheta1);

	if (e <= 0.0f) { // total internal reflection
		return reflect(hit);
	}

	Vector3 dir = (n1 / n2) * (this->d - n * cosTheta1) - n * (sqrt(e));
	Vector3 origin = hit.P + (dir * epsilon);
	return Ray(origin, dir);
}