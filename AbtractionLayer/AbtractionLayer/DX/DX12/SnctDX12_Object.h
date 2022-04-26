#pragma once
#include "../Snct_DXObject.h"
#include "../../Snct_Utility.h"
#include "Snct_Dx12Device.h"
#include "../Snct_DXConstantParameter.h"

class SnctDX12Object : public SnctDXObject
{
public:
	~SnctDX12Object() override final;
	HRESULT Init(ISnctDxDevice* pDevice, Vertices* pVertices, Indices* pIndices) override final;
	void Draw() override final;
private:
	std::vector<ComPtr<ID3D12Resource>>			TEST_CODE_m_pConstantObject;
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>	TEST_CODE_m_objectCBV;
};

