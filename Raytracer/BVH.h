#ifndef CSE168_BVH_H_INCLUDED
#define CSE168_BVH_H_INCLUDED

#include "Miro.h"
#include "Object.h"


struct BoundingBox {
	Vector3 min;
	Vector3 max;
	bool hit(const Ray& ray, float& tMin, float& tMax) const;
};


class BVH {
public:
	BVH();
	BVH(Objects* objs);
	~BVH();

    void build(Objects * objs, int depth = 0);

	bool intersect(HitInfo& hit, const Ray& ray, float tMin = 0.0f, float tMax = MIRO_TMAX) const;

    bool intersectNode(HitInfo& result, const Ray& ray, float tMin = 0.0f, float tMax = MIRO_TMAX) const;

protected:
	bool m_leaf;
    Objects * m_objects;
	BVH *m_left, *m_right;
	BoundingBox m_box;
	int m_axis;
	float m_plane;

private:
	void createBoundingBox(BoundingBox& box, Objects *objs);
	float surfaceArea(BoundingBox& box);
	float calcCost(BoundingBox& box, Objects* objs);
};

#endif // CSE168_BVH_H_INCLUDED
