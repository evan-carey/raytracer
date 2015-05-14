#include "StatsReporter.h"

int StatsReporter::numBVHNodes = 0;
int StatsReporter::numBVHLeafNodes = 0;
int StatsReporter::numRays = 0;
int StatsReporter::numRayBoxIntersections = 0;
int StatsReporter::numRayTriangleIntersections = 0;
double StatsReporter::BVHBuildTime = 0.0;
double StatsReporter::renderTime = 0.0;