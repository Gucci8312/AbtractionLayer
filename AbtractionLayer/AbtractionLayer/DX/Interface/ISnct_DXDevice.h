// Interface class for DirectX device Management...
#pragma once
#include "../../Snct_Windows.h"
#include "../Snct_DXResource.h"

class ISnctDxDevice
{
public:
	virtual ISnctDxDevice* Get() = 0;
};