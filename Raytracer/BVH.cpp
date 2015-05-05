#include "BVH.h"
#include "Ray.h"
#include "Console.h"
#include <limits>

#define DEPTH 32

static const float infinity = 99999.9f;

BVH::BVH() {
	m_box.min = Vector3(infinity);
	m_box.max = Vector3(-infinity);
}

BVH::BVH(Objects* objs) {
	this->build(objs);
}

BVH::~BVH() {
	if (m_objects != NULL) {
		m_objects->erase(m_objects->begin(), m_objects->end());
		delete m_objects;
		m_objects = NULL;
	}
	if (m_left != NULL) {
		delete m_left;
		m_left = NULL;
	}
	if (m_right != NULL) {
		delete m_right;
		m_right = NULL;
	}
	
}

void BVH::build(Objects * objs, int depth) {
    // construct the bounding volume hierarchy
	// create bounding box
	//m_box = new BoundingBox();
	if (m_box.min.x == infinity)
		createBoundingBox(m_box, objs);


	if (objs->size() <= MAX_NUM_OBJECTS || depth >= DEPTH) {
		// current node is a leaf
		m_leaf = true;
		m_objects = objs;
	} else {
		// split and build recursively
		m_leaf = false;

		// find best splitting plane
		float minCost = infinity; // stores the cost of the best split found
		float planePos; // stores position of the best splitting plane
		int axis = 0; // stores axis of splitting plane (X=0,Y=1,Z=2)

		BoundingBox bestLeftBox, bestRightBox;

		for (int i = 0; i < 3; i++) {
			Objects *left = new Objects();
			Objects *right = new Objects();
			BoundingBox leftBB;
			BoundingBox rightBB;

			// splitting planes of current axis
			float lowerBound = m_box.min[i];
			float upperBound = m_box.max[i];
			float midpoint = (upperBound + lowerBound) / 2.0f;

			for (int j = 0; j < objs->size(); j++) {
				// put objects into left and right buckets based on location
				// relative to midpoint
				if ((*objs)[j]->center()[i] < midpoint) {
					(*left).push_back((*objs)[j]);
				} else {
					(*right).push_back((*objs)[j]);
				}
			}
			/*leftBB = createBoundingBox(left);
			rightBB = createBoundingBox(right);*/
			createBoundingBox(leftBB, left);
			createBoundingBox(rightBB, right);

			// minimize costs of subtrees to compare and determine optimal split point
			for (int d = 0; d < DEPTH; d++) {
				// calculate costs
				float leftCost = calcCost(leftBB, left);
				float rightCost = calcCost(rightBB, right);
				
				if (leftCost + rightCost < minCost) {
					// new minimum cost
					minCost = leftCost + rightCost;
					planePos = midpoint;
					axis = i;

					bestLeftBox.min = leftBB.min;
					bestLeftBox.max = leftBB.max;
					bestRightBox.min = rightBB.min;
					bestRightBox.max = rightBB.max;
				}
				// compare subtree costs

				int numObjsMoved = 0;
				if (leftCost > rightCost) {
					// left has higher cost ==> move objects from left to right
					upperBound = midpoint;

					for (int j = left->size() - 1; j >= 0; j--) {
						if ((*left)[j]->center()[i] > midpoint) {
							numObjsMoved++;

							// get bounding box of object being moved 
							// (might have to update right's bounding box)
							Vector3 objBoxMin = Vector3((*left)[j]->min());
							Vector3 objBoxMax = Vector3((*left)[j]->max());
							for (int k = 0; k < 3; k++) {
								if (objBoxMin[k] < rightBB.min[k]) {
									rightBB.min[k] = objBoxMin[k];
								}
								if (objBoxMax[k] > rightBB.max[k]) {
									rightBB.max[k] = objBoxMax[k];
								}

							}
							// move object to right child
							(*right).push_back((*left)[j]);
							std::swap((*left)[j], (*left)[(*left).size() - 1]);
							(*left).pop_back();
						}
					}
					//leftBB = createBoundingBox(left);
					createBoundingBox(leftBB, left);

				} else if (rightCost > leftCost) {
					// right has higher cost ==> move objects from right to left
					lowerBound = midpoint;

					for (int j = right->size() - 1; j >= 0; j--) {
						if ((*right)[j]->center()[i] < midpoint) {
							numObjsMoved++;

							// get bounding box of object being moved 
							// (might have to update left's bounding box)
							Vector3 objBoxMin = Vector3((*right)[j]->min());
							Vector3 objBoxMax = Vector3((*right)[j]->max());

							for (int k = 0; k < 3; k++) {
								if (objBoxMin[k] < leftBB.min[k]) {
									leftBB.min[k] = objBoxMin[k];
								}
								if (objBoxMax[k] > leftBB.max[k]) {
									leftBB.max[k] = objBoxMax[k];
								}

							}
							// move object to left child
							(*left).push_back((*right)[j]);
							std::swap((*right)[j], (*right)[(*right).size() - 1]);
							//(*right).erase((*right).begin() + j);
							(*right).pop_back();
						}
					}
					//rightBB = createBoundingBox(right);
					createBoundingBox(rightBB, right);
				}
				midpoint = (upperBound + lowerBound) / 2.0f;

			}

			float leftCost = calcCost(leftBB, left);
			float rightCost = calcCost(rightBB, right);

			if (leftCost + rightCost < minCost) {
				minCost = leftCost + rightCost;
				axis = i;
				planePos = midpoint;

				bestLeftBox.min = leftBB.min;
				bestLeftBox.max = leftBB.max;
				bestRightBox.min = rightBB.min;
				bestRightBox.max = rightBB.max;
			}
			left->erase(left->begin(), left->end());
			delete left;
			right->erase(right->begin(), right->end());
			delete right;

		}

		// add nodes according to best splitting plane
		Objects* left = new Objects();
		Objects* right = new Objects();
		for (int i = 0; i < objs->size(); i++) {
			if ((*objs)[i]->center()[axis] < planePos) {
				left->push_back((*objs)[i]);
			} else {
				right->push_back((*objs)[i]);
			}
		}

		// recurse
		m_left = new BVH();
		m_right = new BVH();

		m_left->m_box.min = bestLeftBox.min;
		m_left->m_box.max = bestLeftBox.max;
		m_right->m_box.min = bestRightBox.min;
		m_right->m_box.max = bestRightBox.max;
		
		m_left->build(left, depth + 1);
		if (m_left->m_leaf) {
			//left->erase(left->begin(), left->end());
			delete left;
		}
		m_right->build(right, depth + 1);
		if (m_right->m_leaf) {
			//right->erase(right->begin(), right->end());
			delete right;
		}
	}
}

float BVH::calcCost(BoundingBox& box, Objects* objs) {
	if (objs->size() <= 0) {
		return 0.0f;
	} else {
		// cost = surface area of box * number of objects
		return surfaceArea(box) * objs->size();
	}
}

float BVH::surfaceArea(BoundingBox& box) {
	Vector3 v = Vector3(box.max - box.min);
	return 2.0f * (
		v.z * v.x + // length * width
		v.z * v.y + // length * height
		v.x * v.y   // width * height
		);
}

void BVH::createBoundingBox(BoundingBox& box, Objects *objs) {
	if (objs == NULL) return;
	// create bounding box
	Vector3 min(infinity);
	Vector3 max(-infinity);

	for (int i = 0; i < objs->size(); i++) {
		Vector3 currMin((*objs)[i]->min());
		Vector3 currMax((*objs)[i]->max());

		for (int j = 0; j < 3; j++) {
			min[j] = currMin[j] < min[j] ? currMin[j] : min[j];
			max[j] = currMax[j] > max[j] ? currMax[j] : max[j];
		}
	}

	box.min = Vector3(min);
	box.max = Vector3(max);
}

bool BVH::intersect_bsp() {
	return false;
}


bool BVH::intersect(HitInfo& minHit, const Ray& ray, float tMin, float tMax) const {

    bool hit = false;
    HitInfo tempMinHit;
    minHit.t = tMax;

	//if (m_box.hit(ray, tMin, tMax)) {
	//	if (m_leaf) {
	//		// intersect objects
	//		for (size_t i = 0; i < m_objects->size(); i++) {
	//			if ((*m_objects)[i]->intersect(tempMinHit, ray, tMin, tMax)) {
	//				hit = true;
	//				if (tempMinHit.t < minHit.t)
	//					minHit = tempMinHit;
	//			}
	//		}
	//		return hit;
	//	} else {
	//		// traverse tree checking nearest node first
	//		BSP_Node *nearChild = NULL, *farChild = NULL;
	//		if (ray.d[node->axis] > 0) {
	//			nearChild = node->left;
	//			farChild = node->right;
	//		} else {
	//			nearChild = node->right;
	//			farChild = node->left;
	//		}
	//		
	//		// distance to left child

	//	}

	//}

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
