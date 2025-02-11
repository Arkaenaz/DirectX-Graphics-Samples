#pragma once
#include "Primitive.h"

class Quad : public DirectXUtil::Primitive
{
public:
	Quad(std::string name = "Quad", int size = 1, bool uvHorizontalFlip = false, bool uvVerticalFlip = false, 
		float uTileFactor = 1, float vTileFactor = 1);
};

