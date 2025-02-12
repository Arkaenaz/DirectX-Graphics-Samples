#include "Cube.h"

#include "../ObjectManager.h"
//#include "Source/Window.h"


Cube::Cube(std::string name, float size, bool uvHorizontalFlip, bool uvVerticalFlip, float uTileFactor, float vTileFactor)
	    : Primitive(name)
{
    shapeInfo.vertexData.clear();
    shapeInfo.indexData.clear();
    shapeInfo.otherVertexData.clear();

    const float uCoordMin = uvHorizontalFlip ? uTileFactor : 0;
    const float uCoordMax = uvHorizontalFlip ? 0 : uTileFactor;
    const float vCoordMin = uvVerticalFlip ? vTileFactor : 0;
    const float vCoordMax = uvVerticalFlip ? 0 : vTileFactor;

    const glm::vec3 normals[] = {
        glm::vec3(0, 0, 1),
        glm::vec3(0, 0, -1),
        glm::vec3(1, 0, 0),
        glm::vec3(-1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, -1, 0),
    };

    const glm::vec2 texCoord[] = {
        glm::vec2(uCoordMax, vCoordMax), glm::vec2(uCoordMin, vCoordMax), glm::vec2(uCoordMin, vCoordMin),
        glm::vec2(uCoordMax, vCoordMin),
        glm::vec2(uCoordMin, vCoordMin), glm::vec2(uCoordMax, vCoordMin), glm::vec2(uCoordMax, vCoordMax),
        glm::vec2(uCoordMin, vCoordMax),
        glm::vec2(uCoordMax, vCoordMin), glm::vec2(uCoordMax, vCoordMax), glm::vec2(uCoordMin, vCoordMax),
        glm::vec2(uCoordMin, vCoordMin),
        glm::vec2(uCoordMax, vCoordMin), glm::vec2(uCoordMax, vCoordMax), glm::vec2(uCoordMin, vCoordMax),
        glm::vec2(uCoordMin, vCoordMin),
        glm::vec2(uCoordMin, vCoordMax), glm::vec2(uCoordMin, vCoordMin), glm::vec2(uCoordMax, vCoordMin),
        glm::vec2(uCoordMax, vCoordMax),
        glm::vec2(uCoordMax, vCoordMin), glm::vec2(uCoordMax, vCoordMax), glm::vec2(uCoordMin, vCoordMax),
        glm::vec2(uCoordMin, vCoordMin),
    };

    const glm::vec3 tangents[] = {
        glm::vec3(1, 0, 0),
        glm::vec3(-1, 0, 0),
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, 1),
        glm::vec3(1, 0, 0),
        glm::vec3(1, 0, 0),
    };

    for (int i = 0, j = 0; i < 6; i++, j += 4) {
        glm::vec3 normal = normals[i];
        glm::vec3 tangent = tangents[i];

        glm::vec3 side1 = glm::vec3(normal.y, normal.z, normal.x);
        glm::vec3 side2 = glm::cross(normal, side1);

        const int vertexCount = shapeInfo.vertexData.size();

        shapeInfo.indexData.push_back(static_cast<unsigned short>(vertexCount + 0));
        shapeInfo.indexData.push_back(static_cast<unsigned short>(vertexCount + 1));
        shapeInfo.indexData.push_back(static_cast<unsigned short>(vertexCount + 3));
        shapeInfo.indexData.push_back(static_cast<unsigned short>(vertexCount + 1));
        shapeInfo.indexData.push_back(static_cast<unsigned short>(vertexCount + 2));
        shapeInfo.indexData.push_back(static_cast<unsigned short>(vertexCount + 3));

        const float sideOverTwo = size * 0.5f;

        shapeInfo.vertexData.emplace_back(
            (normal - side1 - side2) * sideOverTwo,
            normal,
            tangent,
            texCoord[j]);

        shapeInfo.vertexData.emplace_back(
            (normal - side1 + side2) * sideOverTwo,
            normal,
            tangent,
            texCoord[j + 1]);

        shapeInfo.vertexData.emplace_back(
            (normal + side1 + side2) * sideOverTwo,
            normal,
            tangent,
            texCoord[j + 2]);

        shapeInfo.vertexData.emplace_back(
            (normal + side1 - side2) * sideOverTwo,
            normal,
            tangent,
            texCoord[j + 3]);

        shapeInfo.otherVertexData.emplace_back(
            (normal - side1 - side2) * sideOverTwo,
            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        shapeInfo.otherVertexData.emplace_back(
            (normal - side1 + side2) * sideOverTwo,
            glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

        shapeInfo.otherVertexData.emplace_back(
            (normal + side1 + side2) * sideOverTwo,
            glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

        shapeInfo.otherVertexData.emplace_back(
            (normal + side1 - side2) * sideOverTwo,
            glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    }

    calculateTangentSpace(shapeInfo);

    ShapeResources* createdPrimitive = new ShapeResources();
    // Ensure the contents are reset
    memset(createdPrimitive, 0, sizeof(ShapeResources));

    //Vertex
    createdPrimitive->vertexCount = static_cast<unsigned int>(shapeInfo.vertexData.size());
    createdPrimitive->vertexBuffer = DirectXUtil::AccelerationStructures::createBuffer(
        pDevice,
        static_cast<unsigned int>(sizeof(Structs::VertexPositionNormalTangentTexture) * shapeInfo.vertexData.size()),
        D3D12_RESOURCE_FLAG_NONE,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        AccelerationStructures::kUploadHeapProps
    );
    uint8_t* vBuffData;
    createdPrimitive->vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&vBuffData));
    memcpy(vBuffData, shapeInfo.vertexData.data(),
        static_cast<unsigned int>(sizeof(Structs::VertexPositionNormalTangentTexture) * shapeInfo.vertexData.size()));
    createdPrimitive->vertexBuffer->Unmap(0, nullptr);

    // other vertex buffer
    createdPrimitive->otherVertexCount = static_cast<unsigned int>(shapeInfo.otherVertexData.size());

    if (shapeInfo.otherVertexData.size() > 0)
    {
        createdPrimitive->otherVertexBuffer = DirectXUtil::AccelerationStructures::createBuffer(
            pDevice,
            static_cast<unsigned int>(sizeof(Structs::VertexPositionColor) * shapeInfo.otherVertexData.size()),
            D3D12_RESOURCE_FLAG_NONE,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            AccelerationStructures::kUploadHeapProps
        );

        uint8_t* ovBuffData;
        createdPrimitive->otherVertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&ovBuffData));
        memcpy(ovBuffData, shapeInfo.otherVertexData.data(),
            static_cast<unsigned int>(sizeof(Structs::VertexPositionColor) * shapeInfo.otherVertexData.size()));
        createdPrimitive->otherVertexBuffer->Unmap(0, nullptr);
    }

    //Index
    createdPrimitive->indexCount = static_cast<unsigned int>(shapeInfo.indexData.size());
    createdPrimitive->indexBuffer = DirectXUtil::AccelerationStructures::createBuffer(
        pDevice,
        static_cast<unsigned int>(sizeof(unsigned short) * shapeInfo.indexData.size()),
        D3D12_RESOURCE_FLAG_NONE,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        AccelerationStructures::kUploadHeapProps
    );
    uint8_t* iBuffData;
    createdPrimitive->indexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&iBuffData));
    memcpy(iBuffData, shapeInfo.indexData.data(),
        static_cast<unsigned int>(sizeof(unsigned short) * shapeInfo.indexData.size()));
    createdPrimitive->indexBuffer->Unmap(0, nullptr);
}