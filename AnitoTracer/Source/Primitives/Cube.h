#pragma once
#include "Primitive.h"

class Cube : public DirectXUtil::Primitive
{
private:
	Cube(std::string name = "Cube", float size = 1, bool uvHorizontalFlip = false, bool uvVerticalFlip = false,
		float uTileFactor = 1, float vTileFactor = 1);

	friend class ObjectManager;
};

