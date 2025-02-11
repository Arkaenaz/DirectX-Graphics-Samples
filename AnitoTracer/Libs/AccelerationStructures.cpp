#include "AccelerationStructures.h"

#include <vector>

#include "../Source/Primitives/Primitive.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/random.hpp"
#include "Helper.h"
#include "../Source/Primitives/Quad.h"
#include "../Source/Primitives/Sphere.h"
//#include "../Source/ModelImporter.h"
//#include "../Source/Model.h"
#include "../Source/ObjectManager.h"

//Init static instance reference

//Class

const D3D12_HEAP_PROPERTIES DirectXUtil::AccelerationStructures::kUploadHeapProps =
{
	D3D12_HEAP_TYPE_UPLOAD,
	D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
	D3D12_MEMORY_POOL_UNKNOWN,
	0,
	0,
};

const D3D12_HEAP_PROPERTIES DirectXUtil::AccelerationStructures::kDefaultHeapProps =
{
	D3D12_HEAP_TYPE_DEFAULT,
	D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
	D3D12_MEMORY_POOL_UNKNOWN,
	0,
	0
};

SampleFramework::ID3D12ResourcePtr DirectXUtil::AccelerationStructures::createBuffer(
	SampleFramework::ID3D12Device5Ptr pDevice,
	const uint64_t size, const D3D12_RESOURCE_FLAGS flags,
	const D3D12_RESOURCE_STATES initState,
	const D3D12_HEAP_PROPERTIES& heapProps)
{
	D3D12_RESOURCE_DESC bufDesc;
	bufDesc.Alignment = 0;
	bufDesc.DepthOrArraySize = 1;
	bufDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufDesc.Flags = flags;
	bufDesc.Format = DXGI_FORMAT_UNKNOWN;
	bufDesc.Height = 1;
	bufDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	bufDesc.MipLevels = 1;
	bufDesc.SampleDesc.Count = 1;
	bufDesc.SampleDesc.Quality = 0;
	bufDesc.Width = size;

	SampleFramework::ID3D12ResourcePtr pBuffer;
	ThrowIfFailed(pDevice->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&bufDesc,
		initState,
		nullptr,
		IID_PPV_ARGS(&pBuffer)
	));

	return pBuffer;
}

//DirectXUtil::Primitive::ShapeResources* DirectXUtil::AccelerationStructures::createModel(
//	std::string path, SampleFramework::ID3D12Device5Ptr pDevice)
//{
//	//DirectXUtil::Primitive primitive;
//	//DirectXUtil::Primitive::Shape shape;
//	//ModelImporter* importer = new ModelImporter();
//	//
//	//Model model = Model("Model", importer->LoadFile(path.c_str()));
//	//shape = model.GetShape();
//
//	//DirectXUtil::Primitive::ShapeResources* primitiveData = new DirectXUtil::Primitive::ShapeResources;
//
//	////Vertex
//	//primitiveData->vertexCount = static_cast<unsigned int>(shape.vertexData.size());
//	//primitiveData->vertexBuffer = createBuffer(
//	//	pDevice,
//	//	static_cast<unsigned int>(sizeof(Structs::VertexPositionNormalTangentTexture) * shape.vertexData.size()),
//	//	D3D12_RESOURCE_FLAG_NONE,
//	//	D3D12_RESOURCE_STATE_GENERIC_READ,
//	//	kUploadHeapProps
//	//);
//	//uint8_t* vBuffData;
//	//primitiveData->vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&vBuffData));
//	//memcpy(vBuffData, shape.vertexData.data(),
//	//	static_cast<unsigned int>(sizeof(Structs::VertexPositionNormalTangentTexture) * shape.vertexData.size()));
//	//primitiveData->vertexBuffer->Unmap(0, nullptr);
//
//	//// other vertex buffer
//	//primitiveData->otherVertexCount = static_cast<unsigned int>(shape.otherVertexData.size());
//	//primitiveData->otherVertexBuffer = createBuffer(
//	//	pDevice,
//	//	static_cast<unsigned int>(sizeof(Structs::VertexPositionColor) * shape.otherVertexData.size()),
//	//	D3D12_RESOURCE_FLAG_NONE,
//	//	D3D12_RESOURCE_STATE_GENERIC_READ,
//	//	kUploadHeapProps
//	//);
//
//	//uint8_t* ovBuffData;
//	//primitiveData->otherVertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&ovBuffData));
//	//memcpy(ovBuffData, shape.otherVertexData.data(),
//	//	static_cast<unsigned int>(sizeof(Structs::VertexPositionColor) * shape.otherVertexData.size()));
//	//primitiveData->otherVertexBuffer->Unmap(0, nullptr);
//
//	////Index
//	//primitiveData->indexCount = static_cast<unsigned int>(shape.indexData.size());
//	//primitiveData->indexBuffer = createBuffer(
//	//	pDevice,
//	//	static_cast<unsigned int>(sizeof(unsigned short) * shape.indexData.size()),
//	//	D3D12_RESOURCE_FLAG_NONE,
//	//	D3D12_RESOURCE_STATE_GENERIC_READ,
//	//	kUploadHeapProps
//	//);
//	//uint8_t* iBuffData;
//	//primitiveData->indexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&iBuffData));
//	//memcpy(iBuffData, shape.indexData.data(),
//	//	static_cast<unsigned int>(sizeof(unsigned short) * shape.indexData.size()));
//	//primitiveData->indexBuffer->Unmap(0, nullptr);
//
//	//return primitiveData;
//}

DirectXUtil::Structs::AccelerationStructureBuffers DirectXUtil::AccelerationStructures::createBottomLevelAS(
	SampleFramework::ID3D12Device5Ptr pDevice,
	SampleFramework::ID3D12GraphicsCommandList4Ptr pCmdList,
	DirectXUtil::Primitive::ShapeResources* shape)
{
	std::vector<D3D12_RAYTRACING_GEOMETRY_DESC> geomDescs;
	geomDescs.reserve(1);

		D3D12_RAYTRACING_GEOMETRY_DESC geomDesc = {};
		geomDesc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
		geomDesc.Triangles.VertexBuffer.StartAddress = shape->vertexBuffer->GetGPUVirtualAddress();
		geomDesc.Triangles.VertexBuffer.StrideInBytes = sizeof(DirectXUtil::Structs::VertexPositionNormalTangentTexture);
		geomDesc.Triangles.VertexFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		geomDesc.Triangles.VertexCount = shape->vertexCount;
		geomDesc.Triangles.IndexBuffer = shape->indexBuffer->GetGPUVirtualAddress();
		geomDesc.Triangles.IndexCount = shape->indexCount;
		geomDesc.Triangles.IndexFormat = DXGI_FORMAT_R16_UINT;
		geomDesc.Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;

		geomDescs.push_back(geomDesc);

	// Set up the AS inputs
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs = {};
	inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
	inputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
	inputs.NumDescs = static_cast<UINT>(geomDescs.size());
	inputs.pGeometryDescs = geomDescs.data();
	inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;

	// Get prebuild info
	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO prebuildInfo;
	pDevice->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &prebuildInfo);

	// Allocate buffers
	DirectXUtil::Structs::AccelerationStructureBuffers buffers;
	buffers.pScratch = createBuffer(
		pDevice,
		prebuildInfo.ScratchDataSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		kDefaultHeapProps
	);

	buffers.pResult = createBuffer(
		pDevice,
		prebuildInfo.ResultDataMaxSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
		kDefaultHeapProps
	);

	// Build the bottom-level AS
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC asDesc = {};
	asDesc.Inputs = inputs;
	asDesc.DestAccelerationStructureData = buffers.pResult->GetGPUVirtualAddress();
	asDesc.ScratchAccelerationStructureData = buffers.pScratch->GetGPUVirtualAddress();

	pCmdList->BuildRaytracingAccelerationStructure(&asDesc, 0, nullptr);

	// Insert UAV barrier
	D3D12_RESOURCE_BARRIER uavBarrier = {};
	uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrier.UAV.pResource = buffers.pResult.Get();
	pCmdList->ResourceBarrier(1, &uavBarrier);

	return buffers;
}

//DirectXUtil::Structs::AccelerationStructureBuffers DirectXUtil::AccelerationStructures::createPrimitiveBottomLevelAS(
//	SampleFramework::ID3D12Device5Ptr pDevice, SampleFramework::ID3D12GraphicsCommandList4Ptr pCmdList)
//{
//
//	createdPrimitive = ObjectManager::GetInstance()->CreatePrimitive(ObjectManager::CUBE);
//
//	//Create the AS
//	return createBottomLevelAS(pDevice, pCmdList, createdPrimitive);
//}

//DirectXUtil::Structs::AccelerationStructureBuffers DirectXUtil::AccelerationStructures::createModelBottomLevelAS(
//	std::string path, SampleFramework::ID3D12Device5Ptr pDevice, SampleFramework::ID3D12GraphicsCommandList4Ptr pCmdList)
//{
//	//Delete if necessary (probably never)
//	if (createdPrimitive)
//	{
//		delete createdPrimitive;
//	}
//
//	//Create the Model
//	createdPrimitive = createModel(path, pDevice);
//
//	if (!createdPrimitive)
//	{
//		ThrowError("createdPrimitive is invalid");
//	}
//
//	//Create the AS
//	return createBottomLevelAS(pDevice, pCmdList, createdPrimitive);
//}

void DirectXUtil::AccelerationStructures::buildTopLevelAS(SampleFramework::ID3D12Device5Ptr pDevice,
                                                          SampleFramework::ID3D12GraphicsCommandList4Ptr pCmdList,
                                                          std::vector<Structs::AccelerationStructureBuffers>
                                                          pBottomLevelAS,
                                                          uint64_t& tlasSize,
                                                          DirectXUtil::Structs::AccelerationStructureBuffers& buffers)
{
	static const int instances = ObjectManager::GetInstance()->GetPrimitiveCount();

	// First, get the size of the TLAS buffers and create them
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs = {};
	inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
	inputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
	inputs.NumDescs = instances;
	inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info;
	pDevice->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &info);

	// Create the buffers
	buffers.pScratch = createBuffer(
		pDevice,
		info.ScratchDataSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		kDefaultHeapProps
	);
	buffers.pResult = createBuffer(
		pDevice,
		info.ResultDataMaxSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
		kDefaultHeapProps);
	buffers.pInstanceDesc = createBuffer(
		pDevice,
		sizeof(D3D12_RAYTRACING_INSTANCE_DESC) * instances,
		D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		kUploadHeapProps
	);
	tlasSize = info.ResultDataMaxSizeInBytes;

	// Map the instance desc buffer
	D3D12_RAYTRACING_INSTANCE_DESC* instanceDescs;
	buffers.pInstanceDesc->Map(0, nullptr, reinterpret_cast<void**>(&instanceDescs));
	ZeroMemory(instanceDescs, sizeof(D3D12_RAYTRACING_INSTANCE_DESC));

	// The transformation matrices for the instances
	std::vector<glm::mat4> transformation;

	for (int i = 0; i < instances; i++)
	{
		transformation.push_back(glm::mat4(1.0f));
	}

	transformation[0] = translate(glm::mat4(1), glm::vec3(0, -1.0f, 0.0f)); // floor

	for (int i = 1; i < instances; i++)
	{
		float scaleValue = glm::linearRand(0.25f, 1.0f);
		glm::mat4 randomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaleValue));

		glm::vec3 randomTranslation = glm::vec3(
			glm::linearRand(-5.0f, 5.0f),  
			glm::linearRand(1.0f, 1.0f),
			glm::linearRand(3.0f, 3.0f)
		);

		transformation[i] = translate(glm::mat4(1), glm::vec3(i-3, 0.0f, 3.0f)); // floor
	}

	glm::mat3x4 mat;
	for (int i = 0; i < instances; i++)
	{
		instanceDescs[i].InstanceID = i; // This value will be exposed to the shader via InstanceID()
		instanceDescs[i].InstanceContributionToHitGroupIndex = 0;
		instanceDescs[i].Flags = D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
		mat = glm::mat3x4(glm::transpose(transformation[i]));
		memcpy(instanceDescs[i].Transform, &mat, sizeof(instanceDescs[i].Transform));
		instanceDescs[i].AccelerationStructure = pBottomLevelAS[i].pResult->GetGPUVirtualAddress();
		instanceDescs[i].InstanceMask = 0xFF;
	}

	// Unmap
	buffers.pInstanceDesc->Unmap(0, nullptr);

	// Create the TLAS
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC asDesc = {};
	asDesc.Inputs = inputs;
	asDesc.Inputs.InstanceDescs = buffers.pInstanceDesc->GetGPUVirtualAddress();
	asDesc.DestAccelerationStructureData = buffers.pResult->GetGPUVirtualAddress();
	asDesc.ScratchAccelerationStructureData = buffers.pScratch->GetGPUVirtualAddress();

	pCmdList->BuildRaytracingAccelerationStructure(&asDesc, 0, nullptr);

	// We need to insert a UAV barrier before using the acceleration structures in a raytracing operation
	D3D12_RESOURCE_BARRIER uavBarrier = {};
	uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrier.UAV.pResource = buffers.pResult.Get();
	pCmdList->ResourceBarrier(1, &uavBarrier);
}
