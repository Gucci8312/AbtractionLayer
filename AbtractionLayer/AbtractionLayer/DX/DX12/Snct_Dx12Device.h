#pragma once
#include "Snct_DX12.h"
#include "../ISnct_DxDevice.h"
#include "../../Snct_Windows.h"
#include "../Snct_DXResource.h"

class SnctDx12Device :
	public ISnctDxDevice
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	HRESULT Create(D3D_FEATURE_LEVEL Level);
	ID3D12Device* GetDevice() { return m_device.Get(); }
	SnctDx12Device* Get() { return this; }
private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D12Device> m_device;
};

