#pragma once
#include "Texture.h"
#include "WorleyGrid.h"

/*
Cellular texture using Worley noise
*/
class CellTexture : public Texture {
public:
	CellTexture(int numPoints, int w, int h);
	virtual ~CellTexture();

	/* returns an array of the distances of the <n> points closest to <p> */
	float* closestNDistances(int n, TexPoint& p);

protected:
	int m_numPoints; // number of random points to place in grid
	WorleyGrid m_grid;
};

