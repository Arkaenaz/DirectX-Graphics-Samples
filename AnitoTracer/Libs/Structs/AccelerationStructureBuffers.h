#pragma once

#include "InterfacePointers.h"

namespace DirectXUtil
{
	namespace Structs
	{
		struct AccelerationStructureBuffers
		{
			//Scratch memory
			SampleFramework::ID3D12ResourcePtr pScratch;

			SampleFramework::ID3D12ResourcePtr pResult;

			// Used only for top-level AS
			SampleFramework::ID3D12ResourcePtr pInstanceDesc;
		};
	}
}
