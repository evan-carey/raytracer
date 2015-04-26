#pragma once
#include "Texture.h"
#include "WorleyGrid.h"


class CellTexture : Texture {
public:
	CellTexture(int numPoints, int w, int h);
	virtual ~CellTexture();

	/* returns an array of the distances of the <n> points closest to <p> */
	float* closestNPoints(int n, TexPoint& p);

protected:
	int m_numPoints; // number of random points to place in grid
	WorleyGrid m_grid;
};

