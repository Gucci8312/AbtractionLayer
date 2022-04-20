// Interface class for DirectX Manegiment file...
#pragma once

// Interface class for DirectX command list
class ISnctDxCmdList
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	virtual void ClearRTV() = 0;
	virtual void ClearDSV() = 0;

private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------
};

