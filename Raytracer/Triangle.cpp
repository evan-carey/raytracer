#include "Triangle.h"
#include "TriangleMesh.h"
#include "Ray.h"


Triangle::Triangle(TriangleMesh * m, unsigned int i) :
    m_mesh(m), m_index(i)
{

}


Triangle::~Triangle()
{

}

void Triangle::preCalc() {

	TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
	const Vector3 & vA = m_mesh->vertices()[ti3.x]; //vertex a of triangle
	const Vector3 & vB = m_mesh->vertices()[ti3.y]; //vertex b of triangle
	const Vector3 & vC = m_mesh->vertices()[ti3.z]; //vertex c of triangle

	// calculate bounds
	Vector3 v[3] = { vA, vB, vC };
	m_min = m_max = vA;
	for (int i = 1; i < 3; i++) {
		// calc min
		m_min.x = v[i].x < m_min.x ? v[i].x : m_min.x;
		m_min.y = v[i].y < m_min.y ? v[i].y : m_min.y;
		m_min.z = v[i].z < m_min.z ? v[i].z : m_min.z;
		// calc max
		m_max.x = v[i].x > m_max.x ? v[i].x : m_max.x;
		m_max.y = v[i].y > m_max.y ? v[i].y : m_max.y;
		m_max.z = v[i].z > m_max.z ? v[i].z : m_max.z;
	}
	

	// calculate centroid
	Vector3 AB = vB - vA;
	Vector3 AC = vC - vA;
	m_center = vA + (AB / 3.0f) + (AC / 3.0f);
}

void Triangle::renderGL() {
    TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
    const Vector3 & v0 = m_mesh->vertices()[ti3.x]; //vertex a of triangle
    const Vector3 & v1 = m_mesh->vertices()[ti3.y]; //vertex b of triangle
    const Vector3 & v2 = m_mesh->vertices()[ti3.z]; //vertex c of triangle

    glBegin(GL_TRIANGLES);
        glVertex3f(v0.x, v0.y, v0.z);
        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
    glEnd();
}



bool Triangle::intersect(HitInfo& result, const Ray& r,float tMin, float tMax) {
	// Get vertices from mesh
	TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
	const Vector3 & v0 = m_mesh->vertices()[ti3.x]; //vertex a of triangle
	const Vector3 & v1 = m_mesh->vertices()[ti3.y]; //vertex b of triangle
	const Vector3 & v2 = m_mesh->vertices()[ti3.z]; //vertex c of triangle

	// Get normals from mesh
	TriangleMesh::TupleI3 ti3_norm = m_mesh->nIndices()[m_index];
	const Vector3& n0 = m_mesh->normals()[ti3_norm.x]; // normal of vertex a
	const Vector3& n1 = m_mesh->normals()[ti3_norm.y]; // normal of vertex b
	const Vector3& n2 = m_mesh->normals()[ti3_norm.z]; // normal of vertex c

	// Get triangle normal
	Vector3 vAB = v1 - v0;
	Vector3 vAC = v2 - v0;
	Vector3 norm = cross(vAB, vAC).normalized();

	// Triangle plane intersection distance
	result.t = dot(norm, v0 - r.o) / dot(norm, r.d);

	Vector3 q = r.o + result.t * r.d; // hit point

	// Compute barycentric coordinates
	Vector3 vAQ = q - v0;

	float dAB_AB = dot(vAB, vAB);
	float dAB_AC = dot(vAB, vAC);
	float dAC_AC = dot(vAC, vAC);
	float dAQ_AB = dot(vAQ, vAB);
	float dAQ_AC = dot(vAQ, vAC);

	float D = dAB_AB * dAC_AC - dAB_AC * dAB_AC; // determinant

	if (D == 0.0f) return false; // ray parallel to triangle plane

	float beta = (dAC_AC * dAQ_AB - dAB_AC * dAQ_AC) / D;
	float gamma = (dAB_AB * dAQ_AC - dAB_AC * dAQ_AB) / D;

	// If it misses, return false
	if (beta < 0.0f || gamma < 0.0f) return false;
	if (beta > 1.0f || gamma > 1.0f) return false;
	if (beta + gamma > 1.0f) return false;
	if (result.t < tMin || result.t > tMax) return false;

	// It's a hit, so update result and return true
	result.P = q;
	// interpolate normal
	float alpha = 1.0f - beta - gamma;
	result.N = ((n0 * alpha) + (n1 * beta) + (n2 * gamma)).normalized();
	result.material = this->m_material;

	return true;
}


void Triangle::calcBounds() {

}