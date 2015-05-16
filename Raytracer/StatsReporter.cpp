#include "StatsReporter.h"

int StatsReporter::numBVHNodes = 0;
int StatsReporter::numBVHLeafNodes = 0;
unsigned long StatsReporter::numRays = 0;
unsigned long StatsReporter::numRayBoxIntersections = 0;
unsigned long StatsReporter::numRayTriangleIntersections = 0;
double StatsReporter::BVHBuildTime = 0.0;
double StatsReporter::renderTime = 0.0;