// DirectX12 Management file...
#pragma once
#include "../Snct_DXRender.h"
#include "Snct_DX12.h"
#include "Snct_Dx12CmdList.h"
#include "Snct_Dx12Device.h"
#include "../Snct_DXResource.h"

// Classes that manage DirectX12 core
class SnctDX12Render : public SnctDXRender
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	SnctDX12Render();
	~SnctDX12Render();

	static SnctDX12Render* Get() { return s_app; }

	void Build(HWND* hWnd)	override final;
	void RenderBegin()		override final;
	void RenderEnd()		override final;
	void Draw(HashKey key, SNCT_DRAW_FLAG drawFlag) override final;
	void CreateObject(HashKey key, Vertices* pVertices, Indices* pIndices) override final;
	void WaitGPU();

	void DrawCallPool();
	void DrawThread	 (UINT threadIndex);

	static SnctDX12Render* s_app;



private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	

	struct ThreadParameter
	{
		UINT threadIndex;
	};
	ThreadParameter m_threadParameters[ThreadNum];

	HANDLE m_hBeginFrame	[ThreadNum];
	HANDLE m_hFinishFrame	[ThreadNum];
	HANDLE m_hThread		[ThreadNum];

	static const uint32_t				   m_frameCount = 2;
	SnctDX12Device						   m_device;
	ComPtr<ID3D12CommandQueue>             m_cmdQueue;
	ComPtr<IDXGISwapChain3>                m_swapChain;
	ComPtr<ID3D12CommandAllocator>         m_cmdAllocator[m_frameCount];
	SnctDX12CmdList						   m_cmdList;
	ComPtr<ID3D12DescriptorHeap>           m_heapRTV;
	ComPtr<ID3D12DescriptorHeap>           m_heapDSV;
	ComPtr<ID3D12Resource>                 m_colorBuffer[m_frameCount];
	ComPtr<ID3D12Fence>                    m_fence;
	ComPtr<ID3D12Resource>				   m_depthBuffer;
	HANDLE								   m_fenceEvent = {};
	uint64_t							   m_fenceCounter[m_frameCount];
	uint32_t                               m_frameIndex = 0;
	SnctDx12RTV					           m_handleRTV[m_frameCount];
	SnctDx12DSV				               m_handleDSV = {};
	D3D12_VIEWPORT                         m_viewPort = {};
	D3D12_RECT                             m_scissor = {};
};

