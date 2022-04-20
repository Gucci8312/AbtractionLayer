// DirectX12 Management file...
#pragma once
#include "../Snct_DXRender.h"
#include "Snct_DX12.h"

// Classes that manage DirectX12
class SnctDX12Render : public SnctDXRender
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	SnctDX12Render();
	~SnctDX12Render();
	void Build(HWND* hWnd)	override final;
	void RenderBegin()		override final;
	void RenderEnd()		override final;
	void Draw(HashKey key, SNCT_DRAW_FLAG drawFlag) override final;
	void CreateObject(HashKey key, Vertices* pVertices, Indices* pIndices) override final;
	void SetResourceBarrier(ID3D12Resource* Resource, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After);
	void WaitGPU();
private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	static const uint32_t				   m_frameCount = 2;
	ComPtr<ID3D12Device>                   m_device;
	ComPtr<ID3D12CommandQueue>             m_cmdQueue;
	ComPtr<IDXGISwapChain3>                m_swapChain;
	ComPtr<ID3D12CommandAllocator>         m_cmdAllocator[m_frameCount];
	ComPtr<ID3D12GraphicsCommandList>      m_cmdList;
	ComPtr<ID3D12DescriptorHeap>           m_heapRTV;
	ComPtr<ID3D12DescriptorHeap>           m_heapDSV;
	ComPtr<ID3D12Resource>                 m_colorBuffer[m_frameCount];
	ComPtr<ID3D12Fence>                    m_fence;
	ComPtr<ID3D12Resource>				   m_depthBuffer;
	HANDLE								   m_fenceEvent = {};
	uint64_t							   m_fenceCounter[m_frameCount];
	uint32_t                               m_frameIndex = 0;
	D3D12_CPU_DESCRIPTOR_HANDLE            m_handleRTV[m_frameCount];
	D3D12_CPU_DESCRIPTOR_HANDLE            m_handleDSV = {};
	D3D12_VIEWPORT                         m_viewPort = {};
	D3D12_RECT                             m_scissor = {};
};

