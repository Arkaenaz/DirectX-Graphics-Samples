#pragma once

#include "InterfacePointers.h"
#include "Structs/AccelerationStructureBuffers.h"
#include <cstdint>
#include <d3d12.h>
#include <vector>
#include "string"
#include "../Source/Primitives/Primitive.h"

//#include "Source/ObjectManager.h"

namespace DirectXUtil
{
	// Number of geometry types, we only have sphere and plane.
	static const int kDefaultNumDesc = 2;

	// NUmber of instances, plane:0, sphere:1-3
	static const int kInstancesNum = 4;

	class AccelerationStructures
	{
	public:
		static const D3D12_HEAP_PROPERTIES kUploadHeapProps;
		static const D3D12_HEAP_PROPERTIES kDefaultHeapProps;

		static SampleFramework::ID3D12ResourcePtr createBuffer(
			SampleFramework::ID3D12Device5Ptr pDevice,
			uint64_t size,
			D3D12_RESOURCE_FLAGS flags,
			D3D12_RESOURCE_STATES initState,
			const D3D12_HEAP_PROPERTIES& heapProps
		);

		//static DirectXUtil::Primitive::ShapeResources* createModel(std::string path, SampleFramework::ID3D12Device5Ptr pDevice);

		//static DirectXUtil::Structs::AccelerationStructureBuffers createPrimitiveBottomLevelAS(
		//	SampleFramework::ID3D12Device5Ptr pDevice,
		//	SampleFramework::ID3D12GraphicsCommandList4Ptr pCmdList
		//);

		//static DirectXUtil::Structs::AccelerationStructureBuffers createModelBottomLevelAS(
		//	std::string path,
		//	SampleFramework::ID3D12Device5Ptr pDevice,
		//	SampleFramework::ID3D12GraphicsCommandList4Ptr pCmdList
		//);

		static void buildTopLevelAS(
			SampleFramework::ID3D12Device5Ptr pDevice,
			SampleFramework::ID3D12GraphicsCommandList4Ptr pCmdList,
			std::vector<Structs::AccelerationStructureBuffers> pBottomLevelAS,
			uint64_t& tlasSize,
			DirectXUtil::Structs::AccelerationStructureBuffers& buffers
		);

		static DirectXUtil::Structs::AccelerationStructureBuffers createBottomLevelAS(
			SampleFramework::ID3D12Device5Ptr pDevice,
			SampleFramework::ID3D12GraphicsCommandList4Ptr pCmdList,
			DirectXUtil::Primitive::ShapeResources* shape);
	};
}
