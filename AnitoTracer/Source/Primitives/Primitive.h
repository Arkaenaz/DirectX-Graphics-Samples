#pragma once
#define _USE_MATH_DEFINES

#include <vector>

#include "Object.h"
#include "InterfacePointers.h"
#include "Structs/VertexPositionNormalTangentTexture.h"

namespace DirectXUtil
{
	class Primitive : public Object
	{
	public:
		Primitive();
		Primitive(std::string name);
		~Primitive();

		/**
		 * General Shape information
		 */
		struct Shape
		{
			std::vector<Structs::VertexPositionNormalTangentTexture> vertexData;
			std::vector<Structs::VertexPositionColor> otherVertexData;
			std::vector<unsigned short> indexData;
		};

		struct ShapeResources
		{
			//Vertex
			SampleFramework::ID3D12ResourcePtr vertexBuffer;
			unsigned int vertexCount;
			SampleFramework::ID3D12ResourcePtr otherVertexBuffer;
			unsigned int otherVertexCount;
			//Index
			SampleFramework::ID3D12ResourcePtr indexBuffer;
			unsigned int indexCount;
		};

		void CreateShapeResources(SampleFramework::ID3D12Device5Ptr pDevice);

		Shape GetShape();
		ShapeResources* GetCreatedPrimitive();

	protected:
		static void calculateTangentSpace(Shape& shape);

		ShapeResources* createdPrimitive;
		Shape shapeInfo;
	};
}
