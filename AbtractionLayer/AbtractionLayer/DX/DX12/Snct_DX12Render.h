// DirectX12 Management file...
#pragma once
#include "../Snct_DXRender.h"
#include "Snct_DX12.h"
#include "Snct_Dx12Context.h"
#include "Snct_Dx12Device.h"
#include "Snct_DX12CmdQueue.h"
#include "../../Snct_DX12Objects.h"
#include "../Snct_DXResource.h"
#include "../Snct_DXConstantParameter.h"

// A class that manages DirectX12 render core
class SnctDX12Render : public SnctDXRender
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	SnctDX12Render();
	~SnctDX12Render();
	void Build(HWND hWnd)	override final;
	void RenderBegin()		override final;
	void RenderEnd()		override final;
	void Draw(HashKey key, SNCT_DRAW_FLAG drawFlag) override final;
	void CreateObject(HashKey key, Vertices* pVertices, Indices* pIndices) override final;
	void WaitGPU();

private:
	//---------------------------------------------------------------------------
	// private variables
	//---------------------------------------------------------------------------	
	static const uint32_t				   m_frameCount = 2;
	SnctDX12Device						   m_device;
	SnctDX12CmdQueue			           m_cmdQueue;
	ComPtr<IDXGISwapChain3>                m_swapChain;
	ComPtr<ID3D12CommandAllocator>         m_cmdAllocator[m_frameCount];
	SnctDX12Context						   m_cmdList;
	ComPtr<ID3D12DescriptorHeap>           m_heapRTV;
	ComPtr<ID3D12DescriptorHeap>           m_heapDSV;
	SnctDX12Buffer						   m_colorBuffer[m_frameCount];
	ComPtr<ID3D12Fence>                    m_fence;
	SnctDX12Buffer						   m_depthBuffer;
	HANDLE								   m_fenceEvent = {};
	uint64_t							   m_fenceCounter[m_frameCount];
	uint32_t                               m_frameIndex = 0;
	SnctDX12RTV					           m_handleRTV[m_frameCount];
	SnctDX12DSV				               m_handleDSV = {};
	D3D12_VIEWPORT                         m_viewPort = {};
	D3D12_RECT                             m_scissor = {};

	std::unique_ptr<SnctDX12Objects>		m_pSceneObjects;

	//---------------------------------------------------------------------------
	// private methods
	//---------------------------------------------------------------------------	
	void DrawIndexed(SnctDX12ObjectBuffer* pObject);
	void UpdateCameraBuffer(ID3D12Resource* pCameraConstant);
	void UpdateObjectBuffer(ID3D12Resource* pObjectConstant);


	// << TEST CODE >>
	ComPtr<ID3D12DescriptorHeap>				TEST_CODE_m_pCameraHeap;
	std::vector<ComPtr<ID3D12Resource>>			TEST_CODE_m_pCameraConstant;
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>	TEST_CODE_m_cameraCBV;

	ComPtr<ID3D12RootSignature> TEST_CODE_m_pRootSignature;
	ComPtr<ID3D12PipelineState> TEST_CODE_m_pPipelineState;

	void TEST_CODE_CreateCameraConstantBuffer();
	void TEST_CODE_CreateRootSignature();
	void TEST_CODE_CreatePipelineState();
};

