#pragma once
#include "../Snct_DXRender.h"
#include "Snct_DX12Device.h"
#include "Snct_DX12Context.h"
#include "Snct_DX12CmdQueue.h"

// A class that manages DirectX12 render core
class SnctDX12Render : public SnctDXRender
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	SnctDX12Render();
	~SnctDX12Render() override;
	inline ISnctDXDevice* GetDevice() override final
	{
		return &m_device;
	}

	bool Build(HWND hWnd)	override final;
	void BeforeRender(ISnctDXContext* pCmdList)		override final;
	void AfterRender(ISnctDXContext* pCmdList)		override final;
	void WaitGPU();
	HRESULT CreateCommandList(ISnctDXContext** pCmdList)override final;

private:
	//---------------------------------------------------------------------------
	// private variables
	//---------------------------------------------------------------------------	
	static const uint32_t				   m_frameCount = 2;
	SnctDX12Device						   m_device;
	SnctDX12CmdQueue			           m_cmdQueue;
	ComPtr<IDXGISwapChain3>                m_swapChain;
	//ComPtr<ID3D12CommandAllocator>         m_cmdAllocator[m_frameCount];
	//SnctDX12Context						   m_cmdList;
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

};

