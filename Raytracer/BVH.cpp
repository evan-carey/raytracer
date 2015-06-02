#include "BVH.h"
#include "Ray.h"
#include "Console.h"
#include <limits>
#include <algorithm>

#define MAX_NUM_OBJECTS 8 // maximum number of objects in each leaf node
#define MAX_TREE_DEPTH 64 // maximum depth of BVH tree
#define NUM_SPLITS 32      // number of checks to find best splitting plane for each axis
                          // (higher number generally results in longer BVH build time, 
                          // but shorter render time)

static const float float_max = std::numeric_limits<float>::max();
static const float float_min = std::numeric_limits<float>::min();

BVH::BVH() {
	m_box.min = Vector3(float_max);
	m_box.max = Vector3(float_min);
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

// construct the bounding volume hierarchy
void BVH::build(Objects * objs, int depth) {

	StatsReporter::numBVHNodes++;

	// create bounding box
	if (m_box.min.x == float_max)
		createBoundingBox(m_box, objs);


	if (objs->size() <= MAX_NUM_OBJECTS || depth >= MAX_TREE_DEPTH) {
		// current node is a leaf

		StatsReporter::numBVHLeafNodes++;

		m_leaf = true;
		m_objects = objs;
		
	} else {

		// split and build recursively
		m_leaf = false;

		// find best splitting plane
		float minCost = float_max; // stores the cost of the best split found
		float planePos; // stores position of the best splitting plane
		int axis = 0; // stores axis of splitting plane (X=0,Y=1,Z=2)

		BoundingBox bestLeftBox, bestRightBox;

		// check all dimensions
		for (int i = 0; i < 3; i++) {
			Objects *left = new Objects();
			Objects *right = new Objects();
			BoundingBox leftBB;
			BoundingBox rightBB;

			// splitting planes of current axis
			float lowerBound = m_box.min[i];
			float upperBound = m_box.max[i];
			float midpoint = lowerBound + (upperBound - lowerBound) / 2.0f;

			for (int j = 0; j < objs->size(); j++) {
				// put objects into left and right buckets based on location
				// relative to midpoint
				if ((*objs)[j]->center()[i] < midpoint) {
					(*left).push_back((*objs)[j]);
				} else {
					(*right).push_back((*objs)[j]);
				}
			}

			createBoundingBox(leftBB, left);
			createBoundingBox(rightBB, right);

			// minimize costs of subtrees according to surface area heuristic
			// to compare and determine optimal split point
			for (int d = 0; d < NUM_SPLITS; d++) {
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

				if (leftCost > rightCost) {
					// left has higher cost ==> move objects from left to right
					upperBound = midpoint;
					midpoint = lowerBound + (upperBound - lowerBound) / 2.0f;
					for (int j = left->size() - 1; j >= 0; j--) {
						if ((*left)[j]->center()[i] > midpoint) {

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
							right->push_back((*left)[j]);
							//left->erase(left->begin() + j);
							std::swap((*left)[j], (*left)[left->size() - 1]);
							left->pop_back();
						}
					}
					createBoundingBox(leftBB, left);

				} else if (rightCost > leftCost) {
					// right has higher cost ==> move objects from right to left
					lowerBound = midpoint;
					midpoint = lowerBound + (upperBound - lowerBound) / 2.0f;
					for (int j = right->size() - 1; j >= 0; j--) {
						if ((*right)[j]->center()[i] < midpoint) {

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
							left->push_back((*right)[j]);
							//right->erase(right->begin() + j);
							std::swap((*right)[j], (*right)[right->size() - 1]);
							right->pop_back();
						}
					}
					createBoundingBox(rightBB, right);
				}
				midpoint = lowerBound + (upperBound - lowerBound) / 2.0f;

			}
			
			float leftCost = calcCost(leftBB, left);
			float rightCost = calcCost(rightBB, right);

			// check for new minimum cost and update
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
		m_axis = axis;
		m_plane = planePos;

		// add nodes according to best splitting plane
		Objects* left = new Objects();
		Objects* right = new Objects();
		for (int i = 0; i < objs->size(); i++) {
			if ((*objs)[i]->center()[m_axis] < m_plane) {
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
		if (!m_left->m_leaf) {
			left->erase(left->begin(), left->end());
			delete left;
		}
		m_right->build(right, depth + 1);
		if (!m_right->m_leaf) {
			right->erase(right->begin(), right->end());
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
	Vector3 min(float_max);
	Vector3 max(float_min);

	for (int i = 0; i < objs->size(); i++) {
		Vector3 currMin((*objs)[i]->min());
		Vector3 currMax((*objs)[i]->max());

		for (int j = 0; j < 3; j++) {
			min[j] = currMin[j] < min[j] ? currMin[j] : min[j];
			max[j] = currMax[j] > max[j] ? currMax[j] : max[j];
		}
	}

	box.min = min;
	box.max = max;
}

bool BVH::intersect(HitInfo& minHit, const Ray& ray, float tMin, float tMax) const {

	if (m_box.hit(ray, tMin, tMax)) {
		return intersectNode(minHit, ray, tMin, tMax);
	}
	return false;
}


bool BVH::intersectNode(HitInfo& minHit, const Ray& ray, float tMin, float tMax) const {
	
	StatsReporter::numRayBoxIntersections++;

    bool hit = false;
    minHit.t = tMax;

    HitInfo tempMinHit;
	
	if (m_leaf) {
		// intersect objects
		for (int i = 0; i < m_objects->size(); i++) {
			if ((*m_objects)[i]->intersect(tempMinHit, ray, tMin, minHit.t)) {

				StatsReporter::numRayTriangleIntersections++;

				if (tempMinHit.t < minHit.t) {
					minHit = tempMinHit;
					hit = true;
				}
			}
		}
		return hit;
	} else {
		
		
		float lt1 = tMin, lt2 = tMax;
		float rt1 = tMin, rt2 = tMax;

		bool lHit = m_left->m_box.hit(ray, lt1, lt2);
		bool rHit = m_right->m_box.hit(ray, rt1, rt2);


		if (lHit && rHit) { // hit both boxes

			// intersect closer child first
			if (lt1 < rt1) {
				if (m_left->intersectNode(tempMinHit, ray, tMin, minHit.t)) {
					minHit = tempMinHit;
					hit = true;
				}
				if (m_right->intersectNode(tempMinHit, ray, tMin, minHit.t)) {
					minHit = tempMinHit;
					hit = true;
				}
			} else {
				if (m_right->intersectNode(tempMinHit, ray, tMin, minHit.t)) {
					minHit = tempMinHit;
					hit = true;
				}
				if (m_left->intersectNode(tempMinHit, ray, tMin, minHit.t)) {
					minHit = tempMinHit;
					hit = true;
				}
			}
		} else if (lHit) { // only hit left box
			if (m_left->intersectNode(tempMinHit, ray, tMin, minHit.t)) {
				minHit = tempMinHit;
				hit = true;
			}
		} else if (rHit) { // only hit right box
			if (m_right->intersectNode(tempMinHit, ray, tMin, minHit.t)) {
				minHit = tempMinHit;
				hit = true;
			}
		} else return false; // miss
	}
	return hit;
}



bool BoundingBox::hit(const Ray& ray, float tMin, float tMax) const {
	
	float tempMin = float_min, tempMax = float_max;

	float dist1, dist2;
	for (int i = 0; i < 3; i++) { // check each dimension
		dist1 = (this->min[i] - ray.o[i]) / ray.d[i];
		dist2 = (this->max[i] - ray.o[i]) / ray.d[i];

		if (dist1 < dist2) {
			// dist1 (m_box.min) is closer
			if (dist1 > tempMin) tempMin = dist1;
			if (dist2 < tempMax) tempMax = dist2;
		} else {
			// dist2 (m_box.max) is closer
			if (dist2 > tempMin) tempMin = dist2;
			if (dist1 < tempMax) tempMax = dist1;
		}
	}
	if (tempMin > tempMax || tempMin > tMax || tempMax < tMin) 
		return false;

	return true;
}