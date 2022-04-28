#pragma once
#include "Snct_DX12.h"
#include "../Interface/ISnct_DxDevice.h"

// A class that manages DirectX12 devices
class SnctDX12Device : public ISnctDxDevice
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	// Getter
	SnctDX12Device* Get() { return this; }
	ID3D12Device* GetDevice() { return m_pDevice.Get(); }

	// Create
	HRESULT CreateDevice(D3D_FEATURE_LEVEL Level)override final;
	HRESULT CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE commandListType,
		ID3D12CommandAllocator** ppCmdAllocator);
	HRESULT CreateCommandQueue(D3D12_COMMAND_QUEUE_DESC CmdQueueDesc, ID3D12CommandQueue** ppCommandQueue);
	unsigned int GetIncrementHandleSize(D3D12_DESCRIPTOR_HEAP_TYPE type);
	HRESULT CreateRTV(ISnctDXBuffer* pBuckBuffer, ISnctDXRTV* pRTV) override final;
	HRESULT CreateDSV(ISnctDXBuffer* pDepthTexture, ISnctDXDSV* pDSV) override final;
private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D12Device> m_pDevice;
};

