#pragma once
#include <stdio.h>
class StatsReporter {
public:
	static int numBVHNodes;
	static int numBVHLeafNodes;
	static int numRays;
	static int numRayBoxIntersections;
	static int numRayTriangleIntersections;

	void writeToFile(char* fileName) {
		FILE* file = fopen(fileName, "wb");
		if (!file) {
			fprintf(stderr, "Couldn't open file %s for writing\n", fileName);
		} else {
			fprintf(file, "Total BVH nodes: %d\n", numBVHNodes);
			fprintf(file, "Total BVH leaf nodes: %d\n", numBVHLeafNodes);
			fprintf(file, "Total number of rays: %d\n", numRays);
			fprintf(file, "Ray-box intersections: %d\n", numRayBoxIntersections);
			fprintf(file, "Ray-triangle intersections: %d\n", numRayTriangleIntersections);
			fclose(file);
		}
	}
};

int StatsReporter::numBVHNodes = 0;
int StatsReporter::numBVHLeafNodes = 0;
int StatsReporter::numRays = 0;
int StatsReporter::numRayBoxIntersections = 0;
int StatsReporter::numRayTriangleIntersections = 0;