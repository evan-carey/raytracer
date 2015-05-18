#ifndef CSE168_RAY_H_INCLUDED
#define CSE168_RAY_H_INCLUDED

#include "Material.h"
#include "StatsReporter.h"

//! Contains information about a ray hit with a surface.
/*!
HitInfos are used by object intersection routines. They are useful in
order to return more than just the hit distance.
*/
class HitInfo {
public:
	float t;                            //!< The hit distance
	Vector3 P;                          //!< The hit point
	Vector3 N;                          //!< Shading normal vector
	const Material* material;           //!< Material of the intersected object

	//! Default constructor.
	explicit HitInfo(float t = 0.0f,
		const Vector3& P = Vector3(),
		const Vector3& N = Vector3(0.0f, 1.0f, 0.0f)) :
		t(t), P(P), N(N), material(0) {
		// empty
	}
};

class Ray
{
public:
    Vector3 o,      //!< Origin of ray
            d;      //!< Direction of ray

    Ray() : o(), d(Vector3(0.0f,0.0f,1.0f))
    {
		StatsReporter::numRays++;
    }

    Ray(const Vector3& o, const Vector3& d) : o(o), d(d)
    {
		StatsReporter::numRays++;
    }

	// create reflection ray
	Ray reflect(const HitInfo& hit) const;

	// create refraction ray
	Ray refract(const HitInfo& hit) const;

	// create random ray using important sampling
	/*Ray randomRay(const HitInfo& hit) const;*/

	// create random indirect diffuse lighting ray (for Monte Carlo path tracing)
	Ray indirectRay(const HitInfo& hit) const;
};


#endif // CSE168_RAY_H_INCLUDED
