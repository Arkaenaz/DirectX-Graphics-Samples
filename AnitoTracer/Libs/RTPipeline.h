#pragma once

#include <vector>
#include "Structs/DxilLibrary.h"

#define NV_D3D_CALL(a) {HRESULT hr_ = a; if(FAILED(hr_)) { d3dTraceHR( #a, hr_); }}
#define NV_ARRAYSIZE(a) (sizeof(a)/sizeof(a[0]))
#define NV_ALIGN_TO(_alignment, _val) (((_val + _alignment - 1) / _alignment) * _alignment)

namespace DirectXUtil
{
	class RTPipeline
	{
	public:
		struct RootSignatureDesc
		{
			D3D12_ROOT_SIGNATURE_DESC desc = {};
			std::vector<D3D12_DESCRIPTOR_RANGE> range;
			std::vector<D3D12_ROOT_PARAMETER> rootParams;
		};

		static const WCHAR* kRayGenShader;
		static const WCHAR* kMissShader;
		static const WCHAR* kShadowMiss;
		static const WCHAR* kClosestHitShader;
		static const WCHAR* kHitGroup;

		static DirectXUtil::Structs::DxilLibrary createDxilLibrary();
		static SampleFramework::IDxcBlobPtr compileLibrary(const WCHAR* filename, const WCHAR* targetString);

		static RootSignatureDesc createRayGenRootDesc();
		static RootSignatureDesc CreateHitRootDesc();
		static RootSignatureDesc CreateMissRootDesc();
	};
}
