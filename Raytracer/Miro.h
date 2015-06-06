#ifndef __MIRO_H__
#define __MIRO_H__

// #ifndef max
// #define max(a,b) ((a>b)?a:b)
// #endif

const float MIRO_TMAX = 1e10f;
const float epsilon   = 0.001f; 
const float PI = 3.1415926535897932384626433832795028841972f;
const float DegToRad = PI/180.0f;
const float RadToDeg = 180.0f/PI; 

#include <stdlib.h>
#include "OpenGL.h"
#include <stdio.h>
#include <iostream>

class Ray;
class HitInfo;

class Object;
class Sphere;
class Triangle;
class TriangleMesh;
class Instance;

class PointLight;

class Camera;
class Image;
class Scene;
class Material;

extern void ParseFile(FILE* fp);
extern void initOpenGL();
extern Camera* g_camera;
extern Scene* g_scene;
extern Image* g_image;


/**************************
 * Global Settings
 *************************/

#define NUM_TRACE_CALLS 8 // number of bounces to trace each ray
#define OPEN_MP // enable multi-threading

/* Ray Tracing */
#define SHADOWS // use shadows
#define AREA_LIGHT_SAMPLES 1.0f // number of samples to take from an area light for a point on a surface


/* Path Tracing*/
//#define USE_PATH_TRACING  // uncomment to use Monte Carlo path tracing
#define SAMPLES 50 // number of samples per pixel

//#define USE_RUSSIAN_ROULETTE // uncomment to use Russian Roulette
#define CHANCE_TO_TERMINATE 0.5f



/* Photon Mapping*/
// Global
//#define USE_PHOTON_MAPPING // uncomment to use photon mapping
#define NUM_PHOTONS 200000 // global photons per light source
#define GLOBAL_PHOTONS_TO_USE 500 // global photons to use in irradiance estimate
#define GLOBAL_MAX_DISTANCE 1.0f // max distance to look for photons in irradiance estimate
// Caustic
#define NUM_CAUSTIC_PHOTONS 50000 // caustic photons per light source
#define CAUSTIC_PHOTONS_TO_USE 500 // caustic photons to use in irradiance estimate
#define CAUSTIC_MAX_DISTANCE 5.0f // max distance to look for photons in irradiance estimate

#define NUM_PHOTON_CALLS 5 // max number of bounces to trace each photon
#define MAX_LIGHTS 1 // max lights in scene (for photon map KD tree size)

//#define DEBUG_PHOTONS // draw global photons in OpenGL mode
//#define DEBUG_CAUSTICS // draw caustic photons in OpenGL mode


/* Depth of Field */
#define USE_DOF // uncomment to use a depth-of-field camera
#define APERTURE_RADIUS 0.15f // radius of camera aperture
#define FOCUS_DISTANCE 8.75f // plane in focus (distance from camera)

#endif
