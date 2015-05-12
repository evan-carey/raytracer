#pragma once
#include <stdio.h>

#ifdef _WIN32
// disable useless warnings
#pragma warning(disable:4996)
#endif

class StatsReporter {
public:
	static int numBVHNodes;
	static int numBVHLeafNodes;
	static int numRays;
	static int numRayBoxIntersections;
	static int numRayTriangleIntersections;

	static void writeToFile(char* fileName) {
		FILE* file = fopen(fileName, "w");
		if (!file) {
			fprintf(stderr, "Couldn't open file %s for writing\n", fileName);
		} else {
			fprintf(file, "--------------------\n");
			fprintf(file, "Rendering Statistics\n");
			fprintf(file, "--------------------\n\n");
			fprintf(file, "Total BVH nodes: %d\n", numBVHNodes);
			fprintf(file, "Total BVH leaf nodes: %d\n", numBVHLeafNodes);
			fprintf(file, "Total number of rays: %d\n", numRays);
			fprintf(file, "Ray-box intersections: %d\n", numRayBoxIntersections);
			fprintf(file, "Ray-triangle intersections: %d\n", numRayTriangleIntersections);
			fclose(file);
		}
		print();
	}

	static void print() {
		fprintf(stdout, "\n--------------------\n");
		fprintf(stdout, "Rendering Statistics\n");
		fprintf(stdout, "--------------------\n\n");
		fprintf(stdout, "Total BVH nodes: %d\n", numBVHNodes);
		fprintf(stdout, "Total BVH leaf nodes: %d\n", numBVHLeafNodes);
		fprintf(stdout, "Total number of rays: %d\n", numRays);
		fprintf(stdout, "Ray-box intersections: %d\n", numRayBoxIntersections);
		fprintf(stdout, "Ray-triangle intersections: %d\n", numRayTriangleIntersections);
	}
};

