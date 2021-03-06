#include "assignment1.h"
#include <math.h>
#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Console.h"

#include "PointLight.h"
#include "TriangleMesh.h"
#include "Triangle.h"
#include "Lambert.h"
#include "PhongMaterial.h"
#include "StoneTexture.h"
#include "Sphere.h"

void
makeBunnyScene() {
	g_camera = new Camera;
	g_scene = new Scene;
	g_image = new Image;

	g_image->resize(128, 128);

	// set up the camera
	g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
	g_camera->setEye(Vector3(-2, 3, 5));
	g_camera->setLookAt(Vector3(-.5, 1, 0));
	g_camera->setUp(Vector3(0, 1, 0));
	g_camera->setFOV(45);

	// create and place a point light source
	PointLight * light = new PointLight;
	light->setPosition(Vector3(-3, 15, 3));
	light->setColor(Vector3(1, 1, 1));
	light->setWattage(800);
	g_scene->addLight(light);

	Material* mat = new PhongMaterial(Vector3(1.0f), Vector3(0.0f), Vector3(0.0f), 1.0f, 1.0f);

	TriangleMesh * bunny = new TriangleMesh;
	bunny->load("res/models/bunny.obj");

	// create all the triangles in the bunny mesh and add to the scene
	for (int i = 0; i < bunny->numTris(); ++i) {
		Triangle* t = new Triangle;
		t->setIndex(i);
		t->setMesh(bunny);
		t->setMaterial(mat);
		g_scene->addObject(t);
	}

	// create the floor triangle
	TriangleMesh * floor = new TriangleMesh;
	floor->createSingleTriangle();
	floor->setV1(Vector3(0, 0, 10));
	floor->setV2(Vector3(10, 0, -10));
	floor->setV3(Vector3(-10, 0, -10));
	floor->setN1(Vector3(0, 1, 0));
	floor->setN2(Vector3(0, 1, 0));
	floor->setN3(Vector3(0, 1, 0));

	Triangle* t = new Triangle;
	t->setIndex(0);
	t->setMesh(floor);
	t->setMaterial(mat);
	g_scene->addObject(t);

	// let objects do pre-calculations if needed
	g_scene->preCalc();
}

void makeSphereScene() {
	g_camera = new Camera;
	g_scene = new Scene;
	g_image = new Image;

	g_image->resize(512, 512);

	// set up the camera
	g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.0f));
	g_camera->setEye(Vector3(0, 4, 7));
	g_camera->setLookAt(Vector3(0, 1, 0));
	g_camera->setUp(Vector3(0, 1, 0));
	g_camera->setFOV(45);

	// create and place a point light source
	PointLight * light = new PointLight;
	light->setPosition(Vector3(3, 15, -3));
	light->setColor(Vector3(1, 1, 1));
	light->setWattage(300);
	g_scene->addLight(light);

	PointLight* light2 = new PointLight();
	light2->setPosition(Vector3(-3, 2, 5));
	light2->setColor(Vector3(1, 1, 1));
	light2->setWattage(500);
	g_scene->addLight(light2);

	Material* floormat = new PhongMaterial(Vector3(1.0f));
	Material* spheremat = new PhongMaterial(Vector3(0.0f), Vector3(1.0f), Vector3(0.0f), 512.0f, 1.5f);

	Material* spheremat2 = new PhongMaterial(Vector3(0.0f), Vector3(0.0f), Vector3(1.0f), 1.0f, 1.15f);

	Material* spheremat3 = new PhongMaterial(Vector3(1.0f, 0.6, 0.6f), Vector3(0.0f), Vector3(0.0f),/*Vector3(0.8f, 0.8f, 1.0f),*/ 1.0f, 1.2f);

	Sphere* sphere1 = new Sphere();
	sphere1->setCenter(Vector3(-0.75, 0.0, -1));
	sphere1->setRadius(0.5f);
	sphere1->setMaterial(spheremat);
	g_scene->addObject(sphere1);

	Sphere* sphere2 = new Sphere();
	sphere2->setCenter(Vector3(0.75, 0.0, -1));
	sphere2->setRadius(0.5);
	sphere2->setMaterial(spheremat2);
	g_scene->addObject(sphere2);

	StoneTexture* stone = new StoneTexture();

	//spheremat3->applyTexture(stone);
	floormat->applyTexture(stone);

	TriangleMesh * sphere = new TriangleMesh;
	sphere->load("res/models/sphere.obj");

	// create all the triangles in the sphere mesh and add to the scene
	for (int i = 0; i < sphere->numTris(); ++i) {
		Triangle* t = new Triangle;
		t->setIndex(i);
		t->setMesh(sphere);
		t->setMaterial(spheremat3);
		g_scene->addObject(t);
	}

	// create the floor triangle
	TriangleMesh * floor = new TriangleMesh;
	floor->createSingleTriangle();
	floor->setV1(Vector3(0, 0, 10));
	floor->setV2(Vector3(10, 0, -10));
	floor->setV3(Vector3(-10, 0, -10));
	floor->setN1(Vector3(0, 1, 0));
	floor->setN2(Vector3(0, 1, 0));
	floor->setN3(Vector3(0, 1, 0));

	Triangle* t = new Triangle;
	t->setIndex(0);
	t->setMesh(floor);
	t->setMaterial(floormat);
	g_scene->addObject(t);

	// let objects do pre-calculations if needed
	g_scene->preCalc();
}

void makeTeapotScene() {
	g_camera = new Camera;
	g_scene = new Scene;
	g_image = new Image;

	g_image->resize(256, 256);

	// set up the camera
	g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.0f));
	g_camera->setEye(Vector3(-2, 3, 5));
	g_camera->setLookAt(Vector3(0, 1, 0));
	g_camera->setUp(Vector3(0, 1, 0));
	g_camera->setFOV(45);

	// create and place a point light source
	PointLight * light = new PointLight;
	light->setPosition(Vector3(-3, 8, 3));
	light->setColor(Vector3(1, 1, 1));
	light->setWattage(400);
	g_scene->addLight(light);

	PointLight* light2 = new PointLight();
	light2->setPosition(Vector3(3, 2, -5));
	light2->setColor(Vector3(1, 1, 1));
	light2->setWattage(100);
	g_scene->addLight(light2);


	//Material* floormat = new Lambert(Vector3(1.0f));
	Material* floormat = new PhongMaterial(Vector3(0.8f, 0.8f, 1.0f), Vector3(0.0f), Vector3(0.0f), 32.0f, 1.0f);
	Material* mat = new PhongMaterial(Vector3(0.0f), Vector3(0.0f),Vector3(1.0f), 8.0f, 1.2f);

	// texture
	StoneTexture* stoneTexture = new StoneTexture();
	//mat->applyTexture(stoneTexture);
	floormat->applyTexture(stoneTexture);

	TriangleMesh * teapot = new TriangleMesh;
	teapot->load("res/models/teapot.obj");
	

	// create all the triangles in the teapot mesh and add to the scene
	for (int i = 0; i < teapot->numTris(); ++i) {
		Triangle* t = new Triangle;
		t->setIndex(i);
		t->setMesh(teapot);
		t->setMaterial(mat);
		g_scene->addObject(t);
	}

	// create the floor triangle
	TriangleMesh * floor = new TriangleMesh;
	floor->createSingleTriangle();
	floor->setV1(Vector3(0, 0, 10));
	floor->setV2(Vector3(10, 0, -10));
	floor->setV3(Vector3(-10, 0, -10));
	floor->setN1(Vector3(0, 1, 0));
	floor->setN2(Vector3(0, 1, 0));
	floor->setN3(Vector3(0, 1, 0));

	Triangle* t = new Triangle;
	t->setIndex(0);
	t->setMesh(floor);
	t->setMaterial(floormat);
	g_scene->addObject(t);

	// let objects do pre-calculations if needed
	g_scene->preCalc();
}
