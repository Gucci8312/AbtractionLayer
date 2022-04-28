// Interface class for DirectX command queue Management...
#pragma once
#include "../Snct_DXResource.h"
#include "ISnct_DxContext.h"

// A class that manages DirectX command queue interface
class ISnctDXCmdQueue
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	virtual void Execute(int Num, ISnctDXContext** cmdList) = 0;
private:
};