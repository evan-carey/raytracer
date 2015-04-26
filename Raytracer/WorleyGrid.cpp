#include "WorleyGrid.h"


WorleyGrid::WorleyGrid(int w, int h) : m_w(w), m_h(h) {
	m_cells = new cell*[h];
	for (int i = 0; i < h; i++)
		m_cells[i] = new cell[w];
}


WorleyGrid::~WorleyGrid() {
	for (int i = 0; i < m_h; i++) {
		delete m_cells[i];
	}
	delete m_cells;
}

std::vector<TexPoint>& WorleyGrid::pointsInCell(int i, int j) {
	return m_cells[i][j].m_points;
}

int WorleyGrid::add(TexPoint p) {
	if (p.u() < 0.0f || p.v() < 0.0f || p.u() > 1.0f || p.v() > 1.0f) {
		// point is out of texture bounds
		return 0;
	}
	int i = int(p.v() * height());
	int j = int(p.u() * width());
	return m_cells[i][j].add(p);
}