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
#include "assignment3.h"



int
main(int argc, char*argv[])
{
    // create a scene
	//makeTeapotScene2();
	//makeBunny1Scene();
	//makeBunny20Scene();
	//makeSponzaScene();
	//makeCornellBoxScene();
	makeFinalScene();

    MiroWindow miro(&argc, argv);
    miro.mainLoop();

    return 0; // never executed
}

