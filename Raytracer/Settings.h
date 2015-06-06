#ifndef SETTINGS_H
#define SETTINGS_H


/**************************
* Global Settings
*************************/

#define OPEN_MP // enable multi-threading


/* Ray Tracing */
#define SHADOWS // use shadows
const int NUM_TRACE_CALLS = 5; // number of bounces to trace each ray
const float AREA_LIGHT_SAMPLES = 40.0f; // number of samples to take from an area light for a point on a surface


/* BVH */
const int MAX_NUM_OBJECTS = 8; // maximum number of objects in each leaf node
const int MAX_TREE_DEPTH = 64; // maximum depth of BVH tree
const int NUM_SPLITS = 32;     // number of checks to find best splitting plane for each axis
                               // (higher number generally results in longer BVH build time, 
                               // but shorter render time)



/* Path Tracing */
//#define USE_PATH_TRACING  // uncomment to use Monte Carlo path tracing
const int SAMPLES = 50; // number of samples per pixel

//#define USE_RUSSIAN_ROULETTE // uncomment to use Russian Roulette
const float CHANCE_TO_TERMINATE = 0.5f;



/* Photon Mapping */
// Global
//#define USE_PHOTON_MAPPING // uncomment to use photon mapping
const int NUM_PHOTONS = 20000; // global photons per light source
const int GLOBAL_PHOTONS_TO_USE = 500; // global photons to use in irradiance estimate
const float GLOBAL_MAX_DISTANCE = 1.0f; // max distance to look for photons in irradiance estimate
// Caustic
const int NUM_CAUSTIC_PHOTONS = 5000; // caustic photons per light source
const int CAUSTIC_PHOTONS_TO_USE = 50; // caustic photons to use in irradiance estimate
const float CAUSTIC_MAX_DISTANCE = 5.0f; // max distance to look for photons in irradiance estimate

const int NUM_PHOTON_CALLS = 5; // max number of bounces to trace each photon
const int MAX_LIGHTS = 1; // max lights in scene (for photon map KD tree size)

//#define DEBUG_PHOTONS // draw global photons in OpenGL mode
//#define DEBUG_CAUSTICS // draw caustic photons in OpenGL mode


/* Depth of Field */
//#define USE_DOF // uncomment to use a depth-of-field camera
const float APERTURE_RADIUS = 0.15f; // radius of camera aperture
const float FOCUS_DISTANCE = 5.75f; // plane in focus (distance from camera)
#endif