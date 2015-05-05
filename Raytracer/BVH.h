#ifndef CSE168_BVH_H_INCLUDED
#define CSE168_BVH_H_INCLUDED

#include "Miro.h"
#include "Object.h"
//#include "BoundingBox.h"

#define MAX_NUM_OBJECTS 8

struct BoundingBox {
	Vector3 min;
	Vector3 max;
};

struct BSP_Node {
	Vector3 min, max;
	float plane_pos;
	BSP_Node *left, *right;
	int axis;
};

class BVH {
public:
	BVH();
	BVH(Objects* objs);
	~BVH();

    void build(Objects * objs, int depth = 0);

	bool intersect_bsp();

    bool intersect(HitInfo& result, const Ray& ray,
                   float tMin = 0.0f, float tMax = MIRO_TMAX) const;

	void createBoundingBox(BoundingBox& box, Objects *objs);
	float surfaceArea(BoundingBox& box);
protected:
	bool m_leaf;
    Objects * m_objects;
	BVH *m_left, *m_right;
	BoundingBox m_box;
	//BSP_Node* node;

private:
	float calcCost(BoundingBox& box, Objects* objs);
};

#endif // CSE168_BVH_H_INCLUDED
