// Interface class for DirectX device Management...
#include "../Snct_DXResource.h"
#include "../Interface/ISnct_DXContext.h"

#pragma once
class ISnctDXDevice
{
public:
	virtual ISnctDXDevice* Get() = 0;
	virtual HRESULT CreateRTV(ISnctDXBuffer* buffer, ISnctDXRTV* rtvHandle) = 0;
	virtual HRESULT CreateDSV(ISnctDXBuffer* buffer, ISnctDXDSV* rtvHandle) = 0;
	virtual HRESULT CreateDevice(D3D_FEATURE_LEVEL Level) = 0;
	virtual HRESULT CreateCmdList(ISnctDXContext** pCmdList) = 0;
};

