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
	HRESULT Create(D3D_FEATURE_LEVEL Level);
	HRESULT CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE commandListType,
		ID3D12CommandAllocator** cmdAllocator);
	HRESULT CreateCommandQueue(D3D12_COMMAND_QUEUE_DESC queueDesc, ID3D12CommandQueue** commandqueue);
	unsigned int GetIncrementHandleSize(D3D12_DESCRIPTOR_HEAP_TYPE type);
	HRESULT CreateRTV(ISnctDXBuffer* buffer, ISnctDXRTV* rtvHandle) override final;
	HRESULT CreateDSV(ISnctDXBuffer* buffer, ISnctDXDSV* rtvHandle) override final;
private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D12Device> m_pDevice;
};

