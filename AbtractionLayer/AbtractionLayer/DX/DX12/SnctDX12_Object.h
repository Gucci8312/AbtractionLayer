#pragma once
#include "../Snct_DXObject.h"
#include "Snct_Dx12Device.h"
#include "Snct_DX12Context.h"
#include "../Snct_DXConstantParameter.h"

class SnctDX12Object : public SnctDXObject
{
public:
	void Create(ISnctDxDevice* pDevice, HashKey key, Vertices* pVertices, Indices* pIndices) override final;
	void Draw(int nowFrame, ISnctDXContext* pCmdList, XMConstantObject* pObjectData) override final;
	void Update() override final;
private:
	SnctDX12Buffer m_vertexBuffer;
	SnctDX12Buffer m_indexBuffer;

	ComPtr<ID3D12RootSignature> TEST_CODE_m_pRootSignature;
	ComPtr<ID3D12PipelineState> TEST_CODE_m_pPipelineState;
	ComPtr<ID3D12DescriptorHeap>				TEST_CODE_m_pObjectHeap;
	ComPtr<ID3D12DescriptorHeap>				TEST_CODE_m_pCameraHeap;
	std::vector<ComPtr<ID3D12Resource>>			TEST_CODE_m_pConstantObject;
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>	TEST_CODE_m_objectCBV;
	std::vector<ComPtr<ID3D12Resource>>			TEST_CODE_m_pCameraConstant;
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>	TEST_CODE_m_cameraCBV;
};

