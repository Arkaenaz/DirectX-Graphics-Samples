#include "Sphere.h"

#include "../ObjectManager.h"


Sphere::Sphere(std::string name, float diameter, int tessellation, bool uvHorizontalFlip, bool uvVerticalFlip)
	: Primitive(name)
{
    shapeInfo.vertexData.clear();
    shapeInfo.indexData.clear();
    shapeInfo.otherVertexData.clear();

    const int verticalSegments = tessellation;
    const int horizontalSegments = tessellation * 2;
    float uIncrement = 1.f / horizontalSegments;
    float vIncrement = 1.f / verticalSegments;
    const float radius = diameter / 2;

    uIncrement *= uvHorizontalFlip ? 1 : -1;
    vIncrement *= uvVerticalFlip ? 1 : -1;

    float u = uvHorizontalFlip ? 0 : 1;
    float v = uvVerticalFlip ? 0 : 1;

    // Start with a single vertex at the bottom of the sphere.
    for (int i = 0; i < horizontalSegments; i++) {
        u += uIncrement;

        glm::vec3 position = glm::vec3(0, -1, 0) * radius + this->GetPosition();  // Apply position offset
        DirectXUtil::Structs::VertexPositionNormalTangentTexture vertex(position,
                                                                        glm::vec3(0, -1, 0),
                                                                        glm::vec3(0),
                                                                        glm::vec2(u, v));

        shapeInfo.vertexData.push_back(vertex);
    }

    // Create rings of vertices at progressively higher latitudes.
    v = uvVerticalFlip ? 0 : 1;
    for (int i = 0; i < verticalSegments - 1; i++) {
        const float latitude = (((i + 1) * static_cast<float>(M_PI)) / verticalSegments) - static_cast<float>(M_PI) / 2;
        u = uvHorizontalFlip ? 0 : 1;
        v += vIncrement;
        const float dy = static_cast<float>(sin(latitude));
        const float dxz = static_cast<float>(cos(latitude));

        // Create a single ring of vertices at this latitude.
        for (int j = 0; j <= horizontalSegments; j++) {
            const float longitude = j * static_cast<float>(M_PI) * 2 / horizontalSegments;

            const float dx = static_cast<float>(cos(longitude)) * dxz;
            const float dz = static_cast<float>(sin(longitude)) * dxz;

            const glm::vec3 normal(dx, dy, dz);
            const glm::vec2 texCoord(u, v);
            u += uIncrement;

            glm::vec3 position = normal * radius + this->GetPosition();  // Apply position offset
            DirectXUtil::Structs::VertexPositionNormalTangentTexture vertex(position, normal, glm::vec3(0), texCoord);

            shapeInfo.vertexData.push_back(vertex);
        }
    }

    // Finish with a single vertex at the top of the sphere.
    v = uvVerticalFlip ? 1 : 0;
    u = uvHorizontalFlip ? 0 : 1;
    for (int i = 0; i < horizontalSegments; i++) {
        u += uIncrement;

        glm::vec3 position = glm::vec3(0, 1, 0) * radius + this->GetPosition();  // Apply position offset
        DirectXUtil::Structs::VertexPositionNormalTangentTexture vertex(position,
                                                                        glm::vec3(0, 1, 0),
                                                                        glm::vec3(0),
                                                                        glm::vec2(u, v));

        shapeInfo.vertexData.push_back(vertex);
    }


    // Create a fan connecting the bottom vertex to the bottom latitude ring.
    for (int i = 0; i < horizontalSegments; i++)
    {
        shapeInfo.indexData.push_back(static_cast<unsigned short>(i));

        shapeInfo.indexData.push_back(static_cast<unsigned short>(1 + i + horizontalSegments));

        shapeInfo.indexData.push_back(static_cast<unsigned short>(i + horizontalSegments));
    }

    // Fill the sphere body with triangles joining each pair of latitude rings.
    for (int i = 0; i < verticalSegments - 2; i++)
    {
        for (int j = 0; j < horizontalSegments; j++)
        {
            const int nextI = i + 1;
            const int nextJ = j + 1;
            const int num = horizontalSegments + 1;

            const int i1 = horizontalSegments + (i * num) + j;
            const int i2 = horizontalSegments + (i * num) + nextJ;
            const int i3 = horizontalSegments + (nextI * num) + j;
            const int i4 = i3 + 1;

            shapeInfo.indexData.push_back(static_cast<unsigned short>(i1));

            shapeInfo.indexData.push_back(static_cast<unsigned short>(i2));

            shapeInfo.indexData.push_back(static_cast<unsigned short>(i3));

            shapeInfo.indexData.push_back(static_cast<unsigned short>(i2));

            shapeInfo.indexData.push_back(static_cast<unsigned short>(i4));

            shapeInfo.indexData.push_back(static_cast<unsigned short>(i3));
        }
    }

    // Create a fan connecting the top vertex to the top latitude ring.
    for (int i = 0; i < horizontalSegments; i++)
    {
        shapeInfo.indexData.push_back(static_cast<unsigned short>(shapeInfo.vertexData.size() - 1 - i));

        shapeInfo.indexData.push_back(
            static_cast<unsigned short>(shapeInfo.vertexData.size() - horizontalSegments - 2 - i));

        shapeInfo.indexData.push_back(
            static_cast<unsigned short>(shapeInfo.vertexData.size() - horizontalSegments - 1 - i));
    }

    calculateTangentSpace(shapeInfo);

    CreateShapeResources(ObjectManager::GetInstance()->GetDevice());
}
