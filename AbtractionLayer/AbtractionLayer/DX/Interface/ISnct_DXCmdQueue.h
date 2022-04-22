// Interface class for DirectX command queue Management...
#pragma once
#include "../../Snct_Windows.h"
#include "../Snct_DXResource.h"
#include "ISnct_DxCmdList.h"

// A class that manages DirectX command queue interface
class ISnctDXCmdQueue
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	virtual void Execute(int Num, ISnctDXCmdList** cmdList) = 0;
private:
};