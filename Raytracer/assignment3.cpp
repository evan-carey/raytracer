#include "assignment3.h"
#include <math.h>
#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"

#include "PointLight.h"
#include "TriangleMesh.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Lambert.h"

#include "PhongMaterial.h"
#include "StoneTexture.h"



// local helper function declarations
namespace {
	void addMeshTrianglesToScene(TriangleMesh * mesh, Material * material);
	inline Matrix4x4 translate(float x, float y, float z);
	inline Matrix4x4 scale(float x, float y, float z);
	inline Matrix4x4 rotate(float angle, float x, float y, float z);
} // namespace

void makeFinalScene() {
	g_camera = new Camera;
	g_scene = new Scene;
	g_image = new Image;

	g_image->resize(512, 512);

	// set up the camera
	g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.0f));
	g_camera->setEye(Vector3(2.75, 2.75, 5.25));
	g_camera->setLookAt(Vector3(2.75, 2.75, 0));
	g_camera->setUp(Vector3(0, 1, 0));
	g_camera->setFOV(55);

	// create and place a point light source
	PointLight* light = new PointLight;
	//SphereLight * light = new SphereLight;
	//SquareLight* light = new SquareLight;
	//light->setNormal(Vector3(0, -1, 0));
	//PointLight* light = new PointLight;
	light->setPosition(Vector3(2.75, 5.0f, -2.75));
	//light->setSize(1.0f);
	//light->setRadius(0.2f);
	light->setColor(Vector3(1, 1, 1));
	light->setWattage(160);
	g_scene->addLight(light);


	Material* whiteDiffuse = new PhongMaterial(Vector3(1.0f, 1.0f, 1.0f));
	Material* redDiffuse = new PhongMaterial(Vector3(1.0f, 0.4f, 0.4f));
	Material* greenDiffuse = new PhongMaterial(Vector3(0.4f, 1.0f, 0.4f));
	TriangleMesh * mesh = new TriangleMesh;
	mesh->load("res/models/cornell_box_empty.obj");

	// add triangles to scene
	for (int i = 0; i < mesh->numTris(); i++) {
		Triangle* t = new Triangle;
		t->setIndex(i);
		t->setMesh(mesh);
		switch (i) {
		case 4: case 5: t->setMaterial(redDiffuse);   break;
		case 6: case 7: t->setMaterial(greenDiffuse); break;
		default:        t->setMaterial(whiteDiffuse); break;
		}
		g_scene->addObject(t);
	}

	// add spheres
	/*Sphere* sphereReflect = new Sphere();
	sphereReflect->setCenter(Vector3(1.5f, 1.0f, -1.0f));
	sphereReflect->setRadius(1.0f);
	Material* reflectMat = new PhongMaterial(Vector3(0.0f), Vector3(1.0f), Vector3(0.0f), 256.0f, 1.0f);
	sphereReflect->setMaterial(reflectMat);
	g_scene->addObject(sphereReflect);

	Sphere* sphereRefract = new Sphere();
	sphereRefract->setCenter(Vector3(4.0f, 1.0f, -2.5f));
	sphereRefract->setRadius(1.0f);
	Material* refractMat = new PhongMaterial(Vector3(0.0f), Vector3(0.0f), Vector3(1.0f), 1.0f, 1.5f);
	sphereRefract->setMaterial(refractMat);
	g_scene->addObject(sphereRefract);*/

	/*
	mesh = new TriangleMesh;
	Matrix4x4 xform;
	xform *= translate(1.5, 1.0, -3.5);
	xform *= scale(0.76, 0.76, 0.76);
	mesh->load("res/models/only_quad_sphere2.obj", xform);
	addMeshTrianglesToScene(mesh, new PhongMaterial(Vector3(0.0f), Vector3(1.0f), Vector3(0.0f), 16.0f, 1.0f));

	mesh = new TriangleMesh;
	xform.setIdentity();
	xform *= translate(4.0, 1.0, -2.25);
	xform *= scale(0.76, 0.76, 0.76);
	mesh->load("res/models/only_quad_sphere2.obj", xform);
	addMeshTrianglesToScene(mesh, new PhongMaterial(Vector3(0.0f), Vector3(0.0f), Vector3(1.0f), 1.0f, 1.5f));
	*/
	
	mesh = new TriangleMesh;
	Matrix4x4 xform;
	xform *= translate(1.5, 0.0, -3.5);
	mesh->load("res/models/sphere4.obj", xform);
	addMeshTrianglesToScene(mesh, new PhongMaterial(Vector3(0.0f), Vector3(1.0f), Vector3(0.0f), 16.0f, 1.0f));
	
	
	mesh = new TriangleMesh;
	xform.setIdentity();
	xform *= translate(4, 0.00, -2.25);
	mesh->load("res/models/sphere4.obj", xform);
	addMeshTrianglesToScene(mesh, new PhongMaterial(Vector3(0.0f), Vector3(0.0f), Vector3(1.0f), 1.0f, 1.5f));
	
	
	/*
	mesh = new TriangleMesh;
	xform.setIdentity();
	xform *= translate(3, 0.00, -2.35);
	xform *= scale(0.75, 0.75, 0.75);
	mesh->load("res/models/wineglassgoblet2.obj", xform);
	addMeshTrianglesToScene(mesh, new PhongMaterial(Vector3(0.0f), Vector3(0.0f), Vector3(1.0f), 1.0f, 1.5f));
	*/

	// let objects do pre-calculations if needed
	g_scene->preCalc();
}

void makeFinalScene2() {
	g_camera = new Camera;
	g_scene = new Scene;
	g_image = new Image;

	g_image->resize(512, 512);

	// set up the camera
	g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
	g_camera->setEye(Vector3(0, 4, 6));
	g_camera->setLookAt(Vector3(0, 0, 0));
	g_camera->setUp(Vector3(0, 1, 0));
	g_camera->setFOV(45);

	// create and place a point light source
	SphereLight * light = new SphereLight;
	//PointLight* light = new PointLight;
	light->setPosition(Vector3(6, 8, -5));
	light->setRadius(0.2f);
	light->setColor(Vector3(1, 1, 1));
	light->setWattage(2100);
	g_scene->addLight(light);


	Material* material = new PhongMaterial(Vector3(0.0f), Vector3(0.0f), Vector3(1.0f), 1.0f, 1.5f);
	//Material* material = new Lambert(1.0f);
	//Material* material = new PhongMaterial(Vector3(1.0f));
	//material->applyTexture(new StoneTexture());
	TriangleMesh * mesh = new TriangleMesh;
	Matrix4x4 xform;
	xform.setIdentity();
	//xform *= scale(0.005, 0.005, 0.005);
	//mesh->load("res/models/teapot.obj");
	//mesh->load("res/models/only_quad_sphere2.obj", xform);
	mesh->load("res/models/Glassobj2.obj", xform);
	//mesh->load("res/models/knives_obj.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// create the floor triangle
	TriangleMesh * floor = new TriangleMesh;
	floor->createSingleTriangle();
	floor->setV1(Vector3(-15, 0, -15));
	floor->setV2(Vector3(0, 0, 15));
	floor->setV3(Vector3(15, 0, -15));
	floor->setN1(Vector3(0, 1, 0));
	floor->setN2(Vector3(0, 1, 0));
	floor->setN3(Vector3(0, 1, 0));

	StoneTexture* stonetex = new StoneTexture();
	
	Triangle* t = new Triangle;
	t->setIndex(0);
	t->setMesh(floor);
	Material* floormat = new PhongMaterial(Vector3(1.0f, 0.8f, 0.8f));
	//floormat->applyTexture(stonetex);
	t->setMaterial(floormat);
	g_scene->addObject(t);

	// let objects do pre-calculations if needed
	g_scene->preCalc();
}

// local helper function definitions
namespace {

	void
		addMeshTrianglesToScene(TriangleMesh * mesh, Material * material) {
		// create all the triangles in the bunny mesh and add to the scene
		for (int i = 0; i < mesh->numTris(); ++i) {
			Triangle* t = new Triangle;
			t->setIndex(i);
			t->setMesh(mesh);
			t->setMaterial(material);
			g_scene->addObject(t);
		}
	}


	inline Matrix4x4
		translate(float x, float y, float z) {
		Matrix4x4 m;
		m.setColumn4(Vector4(x, y, z, 1));
		return m;
	}


	inline Matrix4x4
		scale(float x, float y, float z) {
		Matrix4x4 m;
		m.m11 = x;
		m.m22 = y;
		m.m33 = z;
		return m;
	}

	// angle is in degrees
	inline Matrix4x4
		rotate(float angle, float x, float y, float z) {
		float rad = angle*(PI / 180.);

		float x2 = x*x;
		float y2 = y*y;
		float z2 = z*z;
		float c = cos(rad);
		float cinv = 1 - c;
		float s = sin(rad);
		float xy = x*y;
		float xz = x*z;
		float yz = y*z;
		float xs = x*s;
		float ys = y*s;
		float zs = z*s;
		float xzcinv = xz*cinv;
		float xycinv = xy*cinv;
		float yzcinv = yz*cinv;

		Matrix4x4 m;
		m.set(x2 + c*(1 - x2), xy*cinv + zs, xzcinv - ys, 0,
			xycinv - zs, y2 + c*(1 - y2), yzcinv + xs, 0,
			xzcinv + ys, yzcinv - xs, z2 + c*(1 - z2), 0,
			0, 0, 0, 1);
		return m;
	}

} // namespace

