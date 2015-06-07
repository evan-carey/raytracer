//#include <math.h>
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

//#include "assignment1.h"
//#include "assignment2.h"
#include "assignment3.h"



int
main(int argc, char*argv[])
{
	FreeImage_Initialise();
    // create a scene
	//makeTeapotScene2();
	//makeBunny1Scene();
	//makeBunny20Scene();
	//makeSponzaScene();
	//makeCornellBoxScene();
	makeFinalScene2();
	//testEnvMap();
	
    MiroWindow miro(&argc, argv);
    miro.mainLoop();

	FreeImage_DeInitialise();
    return 0; // never executed
}

