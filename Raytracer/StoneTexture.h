#pragma once
#include "Texture.h"

/*
Stone texture
*/
class StoneTexture : public Texture {
public:
	StoneTexture() {}
	virtual ~StoneTexture() {}
	virtual Vector3 getColor(TexPoint& p) const;
	virtual Vector3 getColor3D(Vector3& v) const;
};
