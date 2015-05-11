#include <math.h>
#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Console.h"

#include "PointLight.h"
#include "Sphere.h"
#include "TriangleMesh.h"
#include "Triangle.h"
#include "Lambert.h"
#include "MiroWindow.h"

#include "assignment1.h"
#include "assignment2.h"

/* Render a Lorenz attractor */
void makeLorenzScene() {
	g_camera = new Camera();
	g_scene = new Scene();
	g_image = new Image();

	g_image->resize(512, 512);

	// set up the camera
	g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.0f));
	g_camera->setEye(Vector3(0, 0, -39));
	g_camera->setLookAt(Vector3(0, 0, 0));
	g_camera->setUp(Vector3(0, 1, 0));
	g_camera->setFOV(45);

	// create and place a point light source
	PointLight *light = new PointLight();
	light->setPosition(Vector3(0, 0, 0));
	light->setColor(Vector3(1.0f, 1.0f, 1.0f));
	light->setWattage(500);
	g_scene->addLight(light);

	// Creat Lorenz system
	//Material* mat = new Lambert(Vector3(1.0f, 0.0f, 0.0f));
	const int maxI = 10000;
	const float a = 0.15f;
	const float rho = 28.0f;
	const float sigma = 10.0f;
	const float beta = 8.0f / 3.0f;

	float x = 0.1f;
	float y = 0.0f;
	float z = 0.0f;

	float dt = 0.01f;

	for (int i = 1; i < maxI; ++i) {
		float t = i / float(maxI);
		float theta = 4 * PI*t;
		float r = a * theta;

		float dx = sigma * (y - x);
		float dy = (x * (rho - z) - y);
		float dz = ((x * y) - (beta * z));

		x += dx*dt;
		y += dy*dt;
		z += dz*dt;

		Sphere* sphere = new Sphere();
		sphere->setCenter(Vector3(x, y, z));
		sphere->setRadius(r/4.5f);
		sphere->setMaterial(new Lambert(Vector3(fabs(y*0.5f), fabs(x*0.3f), fabs(r*2.5f))));
		if (r > 0.15f) g_scene->addObject(sphere);
	}

	// let objects do pre-calculations if needed
	g_scene->preCalc();
}

void makeSpiralScene() {
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);
    
    // set up the camera
    g_camera->setBGColor(Vector3(1.0f, 1.0f, 1.0f));
    g_camera->setEye(Vector3(-5, 2, 3));
    g_camera->setLookAt(Vector3(0, 0, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(-3, 15, 3));
    light->setColor(Vector3(1.0f, 0.5f, 0.5f));
    light->setWattage(1000);
    g_scene->addLight(light);

    // create a spiral of spheres
    //Material* mat = new Lambert(Vector3(1.0f, 0.0f, 0.0f));
    const int maxI = 200;
    const float a = 0.15f;
    for (int i = 1; i < maxI; ++i)
    {
        float t = i/float(maxI);
        float theta = 4*PI*t;
		
        float r = a*theta;
        float x = r*cos(theta);
        float y = r*sin(theta);
        float z = 2*(2*PI*a - r);

		

        Sphere * sphere = new Sphere;
        sphere->setCenter(Vector3(x,y,z));
        sphere->setRadius(r/10);
        sphere->setMaterial(new Lambert(Vector3(fabs(y*0.5),fabs(x),fabs(r))));
        g_scene->addObject(sphere);
    }
    
    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

/*
void makeBunnyScene()
{
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
    light->setWattage(500);
    g_scene->addLight(light);

    Material* mat = new Lambert(Vector3(1.0f));

    TriangleMesh * bunny = new TriangleMesh;
    bunny->load("res/models/bunny.obj");
    
    // create all the triangles in the bunny mesh and add to the scene
    for (int i = 0; i < bunny->numTris(); ++i)
    {
        Triangle* t = new Triangle;
        t->setIndex(i);
        t->setMesh(bunny);
        t->setMaterial(mat); 
        g_scene->addObject(t);
    }
    
    // create the floor triangle
    TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();
    floor->setV1(Vector3(  0, 0,  10));
    floor->setV2(Vector3( 10, 0, -10));
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
*/

int
main(int argc, char*argv[])
{
    // create a scene
    //makeSpiralScene();
	//makeBunnyScene();
	//makeSphereScene();
	makeTeapotScene();

    MiroWindow miro(&argc, argv);
    miro.mainLoop();

    return 0; // never executed
}

