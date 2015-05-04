#include "BVH.h"
#include "Ray.h"
#include "Console.h"
#include <limits>

static const float infinity = std::numeric_limits<float>::infinity();

void BVH::build(Objects * objs) {
    // construct the bounding volume hierarchy

	// create bounding box
	m_box = createBoundingBox(objs);

	if (objs->size() <= MAX_NUM_OBJECTS) {
		// current node is a leaf
		m_leaf = true;
		m_objects = objs;
	} else {
		// split and build recursively
		m_leaf = false;

		// find best splitting plane
		for (int i = 0; i < 3; i++) {
			Objects *left = new Objects();
			Objects *right = new Objects();
			BoundingBox leftBB, rightBB;
			float midpoint = (m_box.max()[i] + m_box.min()[i]) / 2.0f;

			for (int j = 0; j < objs->size(); j++) {
				// put objects into left and right buckets based on location
				// relative to midpoint
				if ((*objs)[j]->center()[i] < midpoint) {
					left->push_back((*objs)[j]);
				} else {
					right->push_back((*objs)[j]);
				}
			}
			leftBB = createBoundingBox(left);
			rightBB = createBoundingBox(right);

			// TODO: finish function
			
		}

	}
}

BoundingBox BVH::createBoundingBox(Objects *objs) {
	// create bounding box
	Vector3 min = Vector3(infinity);
	Vector3 max = Vector3(-infinity);

	for (int i = 0; i < objs->size(); i++) {
		Vector3 currMin = (*objs)[i]->min();
		Vector3 currMax = (*objs)[i]->max();

		for (int j = 0; j < 3; j++) {
			min[j] = currMin[j] < min[j] ? currMin[j] : min[j];
			max[j] = currMax[j] > max[j] ? currMax[j] : max[j];
		}
	}
	return BoundingBox(min, max);
}

bool BVH::intersect_bsp() {
	return false;
}


bool BVH::intersect(HitInfo& minHit, const Ray& ray, float tMin, float tMax) const {

    bool hit = false;
    HitInfo tempMinHit;
    minHit.t = tMax;

	if (m_box.hit(ray, tMin, tMax)) {
		if (m_leaf) {
			// intersect objects
			for (size_t i = 0; i < m_objects->size(); i++) {
				if ((*m_objects)[i]->intersect(tempMinHit, ray, tMin, tMax)) {
					hit = true;
					if (tempMinHit.t < minHit.t)
						minHit = tempMinHit;
				}
			}
			return hit;
		} else {
			// traverse tree checking nearest node first
			BSP_Node *nearChild = NULL, *farChild = NULL;
			if (ray.d[node->axis] > 0) {
				nearChild = node->left;
				farChild = node->right;
			} else {
				nearChild = node->right;
				farChild = node->left;
			}
			
			// distance to left child

		}

	}

	return false;
    /*for (size_t i = 0; i < m_objects->size(); ++i)
    {
        if ((*m_objects)[i]->intersect(tempMinHit, ray, tMin, tMax))
        {
            hit = true;
            if (tempMinHit.t < minHit.t)
                minHit = tempMinHit;
        }
    }
    
    return hit;*/
}
