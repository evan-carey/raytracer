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


void
Triangle::renderGL()
{
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



bool
Triangle::intersect(HitInfo& result, const Ray& r,float tMin, float tMax)
{
	// Get vertices from mesh
	TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
	const Vector3 & v0 = m_mesh->vertices()[ti3.x]; //vertex a of triangle
	const Vector3 & v1 = m_mesh->vertices()[ti3.y]; //vertex b of triangle
	const Vector3 & v2 = m_mesh->vertices()[ti3.z]; //vertex c of triangle

	// Get normal
	Vector3 vAB = v1 - v0;
	Vector3 vAC = v2 - v0;
	Vector3 norm = cross(vAB, vAC).normalized();

	// Triangle plane intersection distance
	result.t = dot(norm, v0 - r.o) / dot(norm, r.d);

	Vector3 q = r.o + result.t * r.d; // hit point

	// Compute barycentric coordinates
	Vector3 vAQ = q - v0;

	float D = dot(vAB, vAB) * dot(vAC, vAC) - dot(vAB, vAC) * dot(vAB, vAC);

	float beta = (dot(vAC, vAC) * dot(vAQ, vAB) - dot(vAB, vAC) * dot(vAQ, vAC)) / D;
	float gamma = (dot(vAB, vAB) * dot(vAQ, vAC) - dot(vAB, vAC) * dot(vAQ, vAB)) / D;

	// If it misses, return false
	if (beta < 0.0f || gamma < 0.0f) return false;
	if (beta + gamma > 1.0f) return false;
	if (result.t < tMin || result.t > tMax) return false;

	// It's a hit, so update result and return true
	result.P = q;
	result.N = norm;
	result.material = this->m_material;

	return true;
}
