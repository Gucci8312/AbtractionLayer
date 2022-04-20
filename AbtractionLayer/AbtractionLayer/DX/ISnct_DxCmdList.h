// Interface class for DirectX Manegiment file...
#pragma once
#include "Snct_DXResource.h"

// Interface class for DirectX command list
class ISnctDxCmdList
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	virtual void ClearRTV(Product*) = 0;
	virtual void ClearDSV() = 0;

private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------
};

