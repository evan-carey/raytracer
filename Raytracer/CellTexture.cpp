#include "CellTexture.h"
#include "Worley.h"
#include "Perlin.h"
#include <algorithm>

CellTexture::CellTexture(int numPoints, int w, int h) : m_numPoints(numPoints), m_grid(w, h) {
	// add random points to the grid
	for (int i = 0; i < numPoints; i++) {
		float u = ((float)rand() / RAND_MAX);
		float v = ((float)rand() / RAND_MAX);

		m_grid.add(TexPoint(u, v));
	}
}


CellTexture::~CellTexture() {
	//delete &m_grid;
}

float* CellTexture::closestNDistances(int n, TexPoint& p) {

	// Array of distances of n closest points
	float* Fn = new float[n];

	float (*delta)[2] = new float[n][2];
	unsigned long *ID = new unsigned long[n];

	// Location of p, as an array
	float at[2] = { p.u(), p.v() };
	WorleyNoise::noise2D(at, n, Fn, delta, ID);

	delete [] delta;
	delete [] ID;
	return Fn;
}



