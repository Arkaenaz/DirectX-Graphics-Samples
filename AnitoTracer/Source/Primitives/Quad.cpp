#include "Quad.h"

#include "../ObjectManager.h"

Quad::Quad(std::string name, int size, bool uvHorizontalFlip, bool uvVerticalFlip, float uTileFactor, float vTileFactor)
	: Primitive("Quad")
{
	shapeInfo.vertexData.clear();
	shapeInfo.indexData.clear();
	shapeInfo.otherVertexData.clear();

	shapeInfo.vertexData =
	{
		// Indexed Quad
		DirectXUtil::Structs::VertexPositionNormalTangentTexture(glm::vec3(-size, 0, -size),
		                                                         glm::vec3(0, 1, 0),
		                                                         glm::vec3(0),
		                                                         glm::vec2(0)),
		DirectXUtil::Structs::VertexPositionNormalTangentTexture(glm::vec3(size, 0, -size),
		                                                         glm::vec3(0, 1, 0),
		                                                         glm::vec3(0),
		                                                         glm::vec2(0)),
		DirectXUtil::Structs::VertexPositionNormalTangentTexture(glm::vec3(size, 0, size),
		                                                         glm::vec3(0, 1, 0),
		                                                         glm::vec3(0),
		                                                         glm::vec2(0)),
		DirectXUtil::Structs::VertexPositionNormalTangentTexture(glm::vec3(-size, 0, size),
																 glm::vec3(0, 1, 0),
																 glm::vec3(0),
																 glm::vec2(0)),
	};

	// this should be temporary
	shapeInfo.otherVertexData =
	{
		// Indexed Quad
		DirectXUtil::Structs::VertexPositionColor(glm::vec3(-size, 0, -size),
												  glm::vec4(1, 0, 0, 1)),
		DirectXUtil::Structs::VertexPositionColor(glm::vec3(size, 0, -size),
												  glm::vec4(0, 1, 0, 1)),
		DirectXUtil::Structs::VertexPositionColor(glm::vec3(size, 0, size),
												  glm::vec4(0, 0, 1, 1)),
		DirectXUtil::Structs::VertexPositionColor(glm::vec3(-size, 0, size),
												  glm::vec4(1, 1, 0, 1)),
	};

	shapeInfo.indexData = { 0, 1, 2, 0, 2, 3 };

	CreateShapeResources(ObjectManager::GetInstance()->GetDevice());
}
