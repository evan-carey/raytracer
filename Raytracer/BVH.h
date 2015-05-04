#ifndef CSE168_BVH_H_INCLUDED
#define CSE168_BVH_H_INCLUDED

#include "Miro.h"
#include "Object.h"
#include "BoundingBox.h"

#define MAX_NUM_OBJECTS 8


struct BSP_Node {
	Vector3 min, max;
	float plane_pos;
	BSP_Node *left, *right;
	int axis;
};

class BVH {
public:
    void build(Objects * objs);

	bool intersect_bsp();

    bool intersect(HitInfo& result, const Ray& ray,
                   float tMin = 0.0f, float tMax = MIRO_TMAX) const;

	BoundingBox createBoundingBox(Objects *objs);

protected:
	bool m_leaf;
    Objects * m_objects;
	BVH *m_left, *m_right;
	BoundingBox m_box;
	BSP_Node* node;
};

#endif // CSE168_BVH_H_INCLUDED
