#include "Ray.h"

Ray Ray::reflect(const HitInfo& hit) const {
	// ray's direction is the mirror reflection of current ray
	Vector3 dir = (this->d - 2 * dot(d, hit.N) * hit.N).normalized();
	// ray's origin is the hitpoint (accounting for error)
	Vector3 origin = hit.P + (dir * epsilon);

	return Ray(origin, dir);
}

Ray Ray::refract(const HitInfo& hit) const {
	Ray res;
	res.o = hit.P;

	
	
	return res;
}