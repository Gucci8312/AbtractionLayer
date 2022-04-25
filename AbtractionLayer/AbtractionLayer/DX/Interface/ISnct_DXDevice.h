// Interface class for DirectX device Management...
#pragma once
#include "../../Snct_Windows.h"
#include "../Snct_DXResource.h"

class ISnctDxDevice
{
public:
	virtual ISnctDxDevice* Get() = 0;
	virtual HRESULT CreateRTV(ISnctDXBuffer* buffer, ISnctDXRTV* rtvHandle) = 0;
	virtual HRESULT CreateDSV(ISnctDXBuffer* buffer, ISnctDXDSV* rtvHandle) = 0;
	virtual HRESULT CreateDevice(D3D_FEATURE_LEVEL Level) = 0;
private:
};