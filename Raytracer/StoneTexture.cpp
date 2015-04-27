#include "StoneTexture.h"
#include "Worley.h"
#include "Perlin.h"
#include <algorithm>

Vector3 StoneTexture::getColor(TexPoint& p) const {
	float r, g, b;
	r = g = b = 0.0f;
	int n = 2;

	float u = p.u() * 15.0f;
	float v = p.v() * 15.0f;

	float* Fn = new float[n];
	float(*delta)[2] = new float[n][2];
	unsigned long *ID = new unsigned long[n];

	// Location of p, as an array
	float at[2] = { u, v };

	WorleyNoise::noise2D(at, n, Fn, delta, ID);


	float color = Fn[1] - Fn[0];
	if (color < 0.1f) { // thicker cell walls
		color = 0.0f;
	} else {
		// add Perlin noise
		float noise = PerlinNoise::noise(u, v, 0.0f);
		color += fabs(noise);
	}

	// changing these will change the overall color palette of the texture
	r = color + (float)(ID[0] % 10) / 10.0f;
	g = color + (float)(ID[0] % 10) / 20.0f;
	b = color + (float)(ID[0] % 10) / 20.0f;

	delete[] Fn;
	delete[] delta;
	delete[] ID;
	return Vector3(r, g, b);

}

Vector3 StoneTexture::getColor3D(Vector3& v) const {
	float r, g, b;
	r = g = b = 0.0f;

	// small values here result in larger cells
	float x = v.x * 10.0f;
	float y = v.y * 10.0f;
	float z = v.z * 10.0f;


	// Worley texturing
	int n = 2; // order
	float* Fn = new float[n]; // min n distances
	float(*delta)[3] = new float[n][3]; // min distance vectors
	unsigned long *ID = new unsigned long[n];
	float at[3] = { x, y, z }; // input sample location

	WorleyNoise::noise3D(at, n, Fn, delta, ID);


	float color = Fn[1] - Fn[0];
	if (color < 0.1f) { // thicker cell walls
		color = 0.0f;
	} else {
		// add Perlin noise
		float noise = PerlinNoise::noise(x, y, z);
		color += fabs(noise);
	}

	// changing these will change the overall color palette of the texture
	r = color + (float)(ID[0] % 10) / 10.0f;
	g = color + (float)(ID[0] % 10) / 20.0f;
	b = color + (float)(ID[0] % 10) / 20.0f;


	delete[] Fn;
	delete[] delta;
	delete[] ID;

	return Vector3(r, g, b);
}