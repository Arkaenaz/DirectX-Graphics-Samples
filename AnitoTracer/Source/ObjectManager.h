#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "Object.h"

#include "Primitives/Cube.h"
#include "Primitives/Sphere.h"
#include "Primitives/Quad.h"
#include "Structs/AccelerationStructureBuffers.h"

class ObjectManager
{
public:
	typedef std::unordered_map<std::string, Object*> ObjectTable;
	typedef std::vector<Object*> ObjectList;

	enum PrimitiveType
	{
		SPHERE,
		CUBE,
		QUAD
	};

private:
	ObjectTable m_ObjectTable;
	ObjectList m_ObjectList;

	int nPrimitiveCount = 0;

	SampleFramework::ID3D12Device5Ptr pDevice;
	SampleFramework::ID3D12GraphicsCommandList4Ptr pCmdList;

	std::vector<DirectXUtil::Structs::AccelerationStructureBuffers> mpBottomLevelAS;

	std::vector <DirectXUtil::Primitive::ShapeResources*> shapes;

public:
	void CreatePrimitive(PrimitiveType type);

	void CreateCube(std::string name = "Cube", float size = 1, bool uvHorizontalFlip = false, bool uvVerticalFlip = false, float uTileFactor = 1, float vTileFactor = 1);
	void CreateSphere(std::string name = "Sphere", float diameter = 1, int tessellation = 32, bool uvHorizontalFlip = false, bool uvVerticalFlip = false);
	void CreateQuad(std::string name = "Quad", int size = 1, bool uvHorizontalFlip = false, bool uvVerticalFlip = false, float uTileFactor = 1, float vTileFactor = 1);

	std::vector<DirectXUtil::Structs::AccelerationStructureBuffers> BuildBottomLevelAS();

public:
	void Update(float deltaTime);
	void Draw(int width, int height);

public:
	ObjectList GetAllObjects();
	Object* FindObjectByName(std::string name);
	void AddObject(Object* gameObject);
	void DeleteObject(Object* gameObject);
	void DeleteObjectByName(std::string name);
	void DeleteAllObjects();
	Object* GetLastObject();

	int GetPrimitiveCount();
	std::vector <DirectXUtil::Primitive::ShapeResources*> GetShapes();

	SampleFramework::ID3D12Device5Ptr GetDevice();

private:
	static ObjectManager* P_SHARED_INSTANCE;

private:
	ObjectManager() = default;
	~ObjectManager() = default;
	ObjectManager(const ObjectManager&) = default;
	ObjectManager& operator = (const ObjectManager&) = default;

public:
	static ObjectManager* GetInstance();
	static void Initialize(SampleFramework::ID3D12Device5Ptr device, SampleFramework::ID3D12GraphicsCommandList4Ptr pCmdList);
	static void Destroy();
};

