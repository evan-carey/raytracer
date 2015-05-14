#include "assignment2.h"
#include <math.h>
#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"

#include "PointLight.h"
#include "TriangleMesh.h"
#include "Triangle.h"
#include "Lambert.h"

#include "PhongMaterial.h"

// local helper function declarations
namespace {
	void addMeshTrianglesToScene(TriangleMesh * mesh, Material * material);
	inline Matrix4x4 translate(float x, float y, float z);
	inline Matrix4x4 scale(float x, float y, float z);
	inline Matrix4x4 rotate(float angle, float x, float y, float z);
} // namespace


void
makeTeapotScene2() {
	g_camera = new Camera;
	g_scene = new Scene;
	g_image = new Image;

	g_image->resize(512, 512);

	// set up the camera
	g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
	g_camera->setEye(Vector3(0, 3, 6));
	g_camera->setLookAt(Vector3(0, 0, 0));
	g_camera->setUp(Vector3(0, 1, 0));
	g_camera->setFOV(45);

	// create and place a point light source
	PointLight * light = new PointLight;
	light->setPosition(Vector3(10, 10, 10));
	light->setColor(Vector3(1, 1, 1));
	light->setWattage(2800);
	g_scene->addLight(light);

	Material* material = new PhongMaterial(Vector3(1.0f));
	TriangleMesh * teapot = new TriangleMesh;
	teapot->load("res/models/teapot.obj");
	addMeshTrianglesToScene(teapot, material);

	// create the floor triangle
	TriangleMesh * floor = new TriangleMesh;
	floor->createSingleTriangle();
	floor->setV1(Vector3(-10, 0, -10));
	floor->setV2(Vector3(0, 0, 10));
	floor->setV3(Vector3(10, 0, -10));
	floor->setN1(Vector3(0, 1, 0));
	floor->setN2(Vector3(0, 1, 0));
	floor->setN3(Vector3(0, 1, 0));

	Triangle* t = new Triangle;
	t->setIndex(0);
	t->setMesh(floor);
	t->setMaterial(material);
	g_scene->addObject(t);

	// let objects do pre-calculations if needed
	g_scene->preCalc();
}


void
makeBunny1Scene() {
	g_camera = new Camera;
	g_scene = new Scene;
	g_image = new Image;

	g_image->resize(512, 512);

	// set up the camera
	g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
	g_camera->setEye(Vector3(0, 5, 15));
	g_camera->setLookAt(Vector3(0, 0, 0));
	g_camera->setUp(Vector3(0, 1, 0));
	g_camera->setFOV(45);

	// create and place a point light source
	PointLight * light = new PointLight;
	light->setPosition(Vector3(10, 20, 10));
	light->setColor(Vector3(1, 1, 1));
	light->setWattage(1000);
	//light->setWattage(2000);
	g_scene->addLight(light);

	Material* material = new Lambert(Vector3(1.0f));
	//Material* material = new PhongMaterial(Vector3(1.0f));
	TriangleMesh * bunny = new TriangleMesh;
	bunny->load("res/models/bunny.obj");
	addMeshTrianglesToScene(bunny, material);

	// create the floor triangle
	TriangleMesh * floor = new TriangleMesh;
	floor->createSingleTriangle();
	floor->setV1(Vector3(-100, -1, -100));
	floor->setV2(Vector3(0, -1, 100));
	floor->setV3(Vector3(100, -1, -100));
	floor->setN1(Vector3(0, 1, 0));
	floor->setN2(Vector3(0, 1, 0));
	floor->setN3(Vector3(0, 1, 0));

	Triangle* t = new Triangle;
	t->setIndex(0);
	t->setMesh(floor);
	t->setMaterial(material);
	g_scene->addObject(t);

	// let objects do pre-calculations if needed
	g_scene->preCalc();
}



void
makeBunny20Scene() {
	g_camera = new Camera;
	g_scene = new Scene;
	g_image = new Image;

	g_image->resize(512, 512);

	// set up the camera
	g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
	g_camera->setEye(Vector3(0, 5, 15));
	g_camera->setLookAt(Vector3(0, 0, 0));
	g_camera->setUp(Vector3(0, 1, 0));
	g_camera->setFOV(45);

	// create and place a point light source
	PointLight * light = new PointLight;
	light->setPosition(Vector3(10, 20, 10));
	light->setColor(Vector3(1, 1, 1));
	light->setWattage(1000);
	g_scene->addLight(light);

	TriangleMesh * mesh;
	Material* material = new Lambert(Vector3(1.0f));
	//Material* material = new PhongMaterial(Vector3(1.0f));
	Matrix4x4 xform;
	Matrix4x4 xform2;
	xform2 *= rotate(110, 0, 1, 0);
	xform2 *= scale(.6, 1, 1.1);


	// bunny 1
	xform.setIdentity();
	xform *= scale(0.3, 2.0, 0.7);
	xform *= translate(-1, .4, .3);
	xform *= rotate(25, .3, .1, .6);
	mesh = new TriangleMesh;
	mesh->load("res/models/bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 2
	xform.setIdentity();
	xform *= scale(.6, 1.2, .9);
	xform *= translate(7.6, .8, .6);
	mesh = new TriangleMesh;
	mesh->load("res/models/bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 3
	xform.setIdentity();
	xform *= translate(.7, 0, -2);
	xform *= rotate(120, 0, .6, 1);
	mesh = new TriangleMesh;
	mesh->load("res/models/bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 4
	xform.setIdentity();
	xform *= translate(3.6, 3, -1);
	mesh = new TriangleMesh;
	mesh->load("res/models/bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 5
	xform.setIdentity();
	xform *= translate(-2.4, 2, 3);
	xform *= scale(1, .8, 2);
	mesh = new TriangleMesh;
	mesh->load("res/models/bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 6
	xform.setIdentity();
	xform *= translate(5.5, -.5, 1);
	xform *= scale(1, 2, 1);
	mesh = new TriangleMesh;
	mesh->load("res/models/bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 7
	xform.setIdentity();
	xform *= rotate(15, 0, 0, 1);
	xform *= translate(-4, -.5, -6);
	xform *= scale(1, 2, 1);
	mesh = new TriangleMesh;
	mesh->load("res/models/bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 8
	xform.setIdentity();
	xform *= rotate(60, 0, 1, 0);
	xform *= translate(5, .1, 3);
	mesh = new TriangleMesh;
	mesh->load("res/models/bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 9
	xform.setIdentity();
	xform *= translate(-3, .4, 6);
	xform *= rotate(-30, 0, 1, 0);
	mesh = new TriangleMesh;
	mesh->load("res/models/bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 10
	xform.setIdentity();
	xform *= translate(3, 0.5, -2);
	xform *= rotate(180, 0, 1, 0);
	xform *= scale(1.5, 1.5, 1.5);
	mesh = new TriangleMesh;
	mesh->load("res/models/bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 11
	xform = xform2;
	xform *= scale(0.3, 2.0, 0.7);
	xform *= translate(-1, .4, .3);
	xform *= rotate(25, .3, .1, .6);
	mesh = new TriangleMesh;
	mesh->load("res/models/bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 12
	xform = xform2;
	xform *= scale(.6, 1.2, .9);
	xform *= translate(7.6, .8, .6);
	mesh = new TriangleMesh;
	mesh->load("res/models/bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 13
	xform = xform2;
	xform *= translate(.7, 0, -2);
	xform *= rotate(120, 0, .6, 1);
	mesh = new TriangleMesh;
	mesh->load("res/models/bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 14
	xform = xform2;
	xform *= translate(3.6, 3, -1);
	mesh = new TriangleMesh;
	mesh->load("res/models/bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 15
	xform = xform2;
	xform *= translate(-2.4, 2, 3);
	xform *= scale(1, .8, 2);
	mesh = new TriangleMesh;
	mesh->load("res/models/bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 16
	xform = xform2;
	xform *= translate(5.5, -.5, 1);
	xform *= scale(1, 2, 1);
	mesh = new TriangleMesh;
	mesh->load("res/models/bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 17
	xform = xform2;
	xform *= rotate(15, 0, 0, 1);
	xform *= translate(-4, -.5, -6);
	xform *= scale(1, 2, 1);
	mesh = new TriangleMesh;
	mesh->load("res/models/bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 18
	xform = xform2;
	xform *= rotate(60, 0, 1, 0);
	xform *= translate(5, .1, 3);
	mesh = new TriangleMesh;
	mesh->load("res/models/bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 19
	xform = xform2;
	xform *= translate(-3, .4, 6);
	xform *= rotate(-30, 0, 1, 0);
	mesh = new TriangleMesh;
	mesh->load("res/models/bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 20
	xform = xform2;
	xform *= translate(3, 0.5, -2);
	xform *= rotate(180, 0, 1, 0);
	xform *= scale(1.5, 1.5, 1.5);
	mesh = new TriangleMesh;
	mesh->load("res/models/bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);


	// create the floor triangle
	mesh = new TriangleMesh;
	mesh->createSingleTriangle();
	mesh->setV1(Vector3(-100, 0, -100));
	mesh->setV2(Vector3(0, 0, 100));
	mesh->setV3(Vector3(100, 0, -100));
	mesh->setN1(Vector3(0, 1, 0));
	mesh->setN2(Vector3(0, 1, 0));
	mesh->setN3(Vector3(0, 1, 0));

	Triangle* t = new Triangle;
	t->setIndex(0);
	t->setMesh(mesh);
	t->setMaterial(material);
	g_scene->addObject(t);

	// let objects do pre-calculations if needed
	g_scene->preCalc();
}


void
makeSponzaScene() {
	g_camera = new Camera;
	g_scene = new Scene;
	g_image = new Image;

	g_image->resize(512, 512);

	// set up the camera
	g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
	g_camera->setEye(Vector3(8, 1.5, 1));
	g_camera->setLookAt(Vector3(0, 2.5, -1));
	g_camera->setUp(Vector3(0, 1, 0));
	g_camera->setFOV(55);

	// create and place a point light source
	PointLight * light = new PointLight;
	light->setPosition(Vector3(0, 10.0, 0));
	light->setColor(Vector3(1, 1, 1));
	light->setWattage(200);
	g_scene->addLight(light);

	Material* material = new Lambert(Vector3(1.0f));
	TriangleMesh * mesh = new TriangleMesh;
	mesh->load("res/models/sponza.obj");
	addMeshTrianglesToScene(mesh, material);

	// let objects do pre-calculations if needed
	g_scene->preCalc();
}

void makeCornellBoxScene() {
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
	PointLight * light = new PointLight;
	light->setPosition(Vector3(2.75, 5.5 - epsilon, -2.75));
	light->setColor(Vector3(1, 1, 1));
	light->setWattage(150);
	g_scene->addLight(light);

	Material* whiteDiffuse = new PhongMaterial(Vector3(1.0f));
	Material* redDiffuse = new PhongMaterial(Vector3(1.0f, 0.0f, 0.0f));
	Material* greenDiffuse = new PhongMaterial(Vector3(0.0f, 1.0f, 0.0f));
	TriangleMesh * mesh = new TriangleMesh;
	mesh->load("res/models/cornell_box.obj");
	
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

