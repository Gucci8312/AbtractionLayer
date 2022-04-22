// Interface class for DirectX device Management...
#pragma once
#include "../../Snct_Windows.h"
#include "../Snct_DXResource.h"

class ISnctDxDevice
{
public:
	virtual ISnctDxDevice* Get() = 0;
	virtual void CreateRTV(ISnctDXBuffer* buffer, ISnctDXRTV* rtvHandle)=0;
	virtual void CreateDSV(ISnctDXBuffer* buffer, ISnctDXDSV* rtvHandle)=0;
private:
};