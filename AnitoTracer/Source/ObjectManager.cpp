#include "ObjectManager.h"

#include <iostream>

#include "AccelerationStructures.h"


ObjectManager* ObjectManager::P_SHARED_INSTANCE = nullptr;

void ObjectManager::CreatePrimitive(PrimitiveType type)
{
	switch (type)
	{
	case PrimitiveType::CUBE:
		{
			this->CreateCube();
			break;
		}

	case PrimitiveType::SPHERE:
		{
			this->CreateSphere();
			break;
		}

	case PrimitiveType::QUAD:
		{
			return this->CreateQuad();
			break;
		}
	}
}

void ObjectManager::CreateCube(std::string name, float size, bool uvHorizontalFlip, bool uvVerticalFlip, float uTileFactor, float vTileFactor)
{
	Cube* cube = new Cube(name, size, uvHorizontalFlip, uvVerticalFlip, uTileFactor, vTileFactor);
	this->AddObject(cube);

	shapes.push_back(cube->GetCreatedPrimitive());
}

void ObjectManager::CreateSphere(std::string name, float diameter, int tessellation, bool uvHorizontalFlip, bool uvVerticalFlip)
{
	Sphere* sphere = new Sphere(name, diameter, tessellation, uvHorizontalFlip, uvVerticalFlip);
	this->AddObject(sphere);

	shapes.push_back(sphere->GetCreatedPrimitive());

}

void ObjectManager::CreateQuad(std::string name, int size, bool uvHorizontalFlip, bool uvVerticalFlip, float uTileFactor, float vTileFactor)
{
	Quad* quad = new Quad(name, size, uvHorizontalFlip, uvVerticalFlip, uTileFactor, vTileFactor);
	this->AddObject(quad);

	shapes.push_back(quad->GetCreatedPrimitive());
}

std::vector<DirectXUtil::Structs::AccelerationStructureBuffers> ObjectManager::BuildBottomLevelAS()
{
	if (mpBottomLevelAS.empty())
	{
		std::vector<DirectXUtil::Structs::AccelerationStructureBuffers> asBuffers;  // Declare asBuffers here

		for (auto shape : shapes)
		{
			asBuffers.push_back(DirectXUtil::AccelerationStructures::createBottomLevelAS(
				pDevice,
				pCmdList,
				shape // Pass each shape individually
			));
		}

		mpBottomLevelAS.insert(mpBottomLevelAS.end(), asBuffers.begin(), asBuffers.end());
	}

	return mpBottomLevelAS;
}

void ObjectManager::Update(float deltaTime)
{
}

void ObjectManager::Draw(int width, int height)
{
}

ObjectManager::ObjectList ObjectManager::GetAllObjects()
{
	return m_ObjectList;
}

Object* ObjectManager::FindObjectByName(std::string name)
{
	if (this->m_ObjectTable[name] != NULL)
	{
		return this->m_ObjectTable[name];
	}
	else
	{
		std::cout << "Object " + name + " is not found\n";
		return NULL;
	}
}

void ObjectManager::AddObject(Object* object)
{
	std::string name = object->GetName();

	if (this->m_ObjectTable[object->GetName()] != NULL)
	{
		int count = 1;
		name = object->GetName() + " " + std::to_string(count);

		while (this->m_ObjectTable[name] != NULL)
		{
			count++;
			name = object->GetName() + " " + std::to_string(count);
		}
		object->SetName(name);
		this->m_ObjectTable[name] = object;
	}
	else
	{
		this->m_ObjectTable[name] = object;
	}

	this->m_ObjectList.push_back(object);

	if (object->GetTypeString() == "Primitive")
	{
		nPrimitiveCount++;
	}
}

void ObjectManager::DeleteObject(Object* gameObject)
{
}

void ObjectManager::DeleteObjectByName(std::string name)
{
}

void ObjectManager::DeleteAllObjects()
{
}

Object* ObjectManager::GetLastObject()
{
	if (this->m_ObjectList.empty())
	{
		return nullptr;
	}
	return this->m_ObjectList.back();
}

int ObjectManager::GetPrimitiveCount()
{
	return this->nPrimitiveCount;
}

std::vector<DirectXUtil::Primitive::ShapeResources*> ObjectManager::GetShapes()
{
	return this->shapes;
}

SampleFramework::ID3D12Device5Ptr ObjectManager::GetDevice()
{
	return this->pDevice;
}

ObjectManager* ObjectManager::GetInstance()
{
	return P_SHARED_INSTANCE;
}

void ObjectManager::Initialize(SampleFramework::ID3D12Device5Ptr device, SampleFramework::ID3D12GraphicsCommandList4Ptr pCmdList)
{
	if (P_SHARED_INSTANCE)
	{
		std::cout << "[EXCEPTION]: Object Manager Already Initialized" << std::endl;
		throw std::exception("[EXCEPTION]: Object Manager Already Initialized");
	}
	P_SHARED_INSTANCE = new ObjectManager();

	GetInstance()->pDevice = device;
	GetInstance()->pCmdList = pCmdList;

	GetInstance()->m_ObjectTable = ObjectTable(); 
	GetInstance()->m_ObjectList = ObjectList(); 

	GetInstance()->nPrimitiveCount = 0; 
	GetInstance()->shapes.clear();
}

void ObjectManager::Destroy()
{
	delete P_SHARED_INSTANCE;
}
