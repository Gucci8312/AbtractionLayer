#pragma once
#include "Snct_DX12.h"
#include "../ISnct_DxDevice.h"
#include "../../Snct_Windows.h"
#include "../Snct_DXResource.h"

class SnctDX12Device :
	public ISnctDxDevice
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	HRESULT Create(D3D_FEATURE_LEVEL Level);
	HRESULT CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE commandListType,
		ID3D12CommandAllocator** cmdAllocator);
	ID3D12Device* GetDevice() { return m_pDevice.Get(); }
	SnctDX12Device* Get() { return this; }
	HRESULT CreateCommandQueue(D3D12_COMMAND_QUEUE_DESC queueDesc, ID3D12CommandQueue** commandqueue);
	unsigned int GetIncrementHandleSize(D3D12_DESCRIPTOR_HEAP_TYPE type);
	void CreateRTV(ID3D12Resource* buffer, D3D12_RENDER_TARGET_VIEW_DESC rtvDesc, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle);
private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D12Device> m_pDevice;
};

