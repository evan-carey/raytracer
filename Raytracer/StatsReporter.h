#pragma once
#include <stdio.h>
#include <ctime>

#ifdef _WIN32
// disable useless warnings
#pragma warning(disable:4996)
#endif

class StatsReporter {
public:
	static int numBVHNodes;
	static int numBVHLeafNodes;
	static unsigned long numRays;
	static unsigned long numRayBoxIntersections;
	static unsigned long numRayTriangleIntersections;
	static double BVHBuildTime;
	static double renderTime;

	static void writeToFile(char* fileName) {
		FILE* file = fopen(fileName, "w");
		if (!file) {
			fprintf(stderr, "Couldn't open file %s for writing\n", fileName);
		} else {
			fprintf(file, "--------------------\n");
			fprintf(file, "Rendering Statistics\n");
			fprintf(file, "--------------------\n\n");
			fprintf(file, "Total rendering time: %.3f seconds\n", renderTime);
			fprintf(file, "Total BVH nodes: %d\n", numBVHNodes);
			fprintf(file, "Total BVH leaf nodes: %d\n", numBVHLeafNodes);
			fprintf(file, "BVH build time: %.3f seconds\n", BVHBuildTime);
			fprintf(file, "Total number of rays: %lu\n", numRays);
			fprintf(file, "Ray-box intersections: %lu\n", numRayBoxIntersections);
			fprintf(file, "Ray-triangle intersections: %lu\n", numRayTriangleIntersections);
			fclose(file);
		}
		print();
		resetStats();
	}

	static void print() {
		fprintf(stdout, "\n--------------------\n");
		fprintf(stdout, "Rendering Statistics\n");
		fprintf(stdout, "--------------------\n\n");
		fprintf(stdout, "Total rendering time: %.3f seconds\n", renderTime);
		fprintf(stdout, "Total BVH nodes: %d\n", numBVHNodes);
		fprintf(stdout, "Total BVH leaf nodes: %d\n", numBVHLeafNodes);
		fprintf(stdout, "BVH build time: %.3f\n", BVHBuildTime);
		fprintf(stdout, "Total number of rays: %lu\n", numRays);
		fprintf(stdout, "Ray-box intersections: %lu\n", numRayBoxIntersections);
		fprintf(stdout, "Ray-triangle intersections: %lu\n\n\n", numRayTriangleIntersections);
	}

	static void calcBVHBuildTime(clock_t startTime) {
		BVHBuildTime = (std::clock() - startTime) / (double)CLOCKS_PER_SEC;
	}
	static void calcRenderTime(clock_t startTime) {
		renderTime = (std::clock() - startTime) / (double)CLOCKS_PER_SEC;
	}

	static void resetStats() {
		numRays = 0;
		numRayBoxIntersections = 0;
		numRayTriangleIntersections = 0;
		BVHBuildTime = 0.0;
		renderTime = 0.0;
	}
};

