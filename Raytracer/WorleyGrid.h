#pragma once
#include "Texture.h"
#include <vector>

/*
A cell of the Worley noise grid
*/
struct cell {
	// the points in the cell, specified by their (u,v) coordinates
	std::vector<TexPoint> m_points;

	int add(TexPoint p) { m_points.push_back(p); return (int)m_points.size(); }
};

/*
The grid used to store random points for Worley noise.
Used for cellular texturing.
*/
class WorleyGrid {
public:
	WorleyGrid(int w, int h);
	virtual ~WorleyGrid();

	int width() { return m_w; }
	int height() { return m_h; }

	std::vector<TexPoint>& pointsInCell(int i, int j);
	int add(TexPoint p);
protected:
	int m_w, m_h; // grid dimensions

	// The grid is a 2D array of cells
	cell** m_cells;
};

