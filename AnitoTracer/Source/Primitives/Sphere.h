#pragma once
#include "Primitive.h"

class Sphere : public DirectXUtil::Primitive
{
public:
	Sphere(std::string name = "Sphere", float diameter = 2 , int tessellation = 32, bool uvHorizontalFlip = false,
		bool uvVerticalFlip = false);
};


