#pragma once
#include "../ISnct_Device.h"
#include "../../Snct_Windows.h"
#include "../Snct_DXResource.h"

class SnctDx12Device :
	public ISnctDevice
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	HRESULT Create(D3D_FEATURE_LEVEL Level);
	ID3D12Device* Get() { return m_device.Get(); }
private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D12Device> m_device;
};

