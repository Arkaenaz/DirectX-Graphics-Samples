#pragma once

#include <cstdint>
#include "InterfacePointers.h"

namespace DirectXUtil
{
	namespace Structs
	{
		struct HeapData
		{
			SampleFramework::ID3D12DescriptorHeapPtr pHeap;
			uint32_t usedEntries = 0;
		};
	}
}
