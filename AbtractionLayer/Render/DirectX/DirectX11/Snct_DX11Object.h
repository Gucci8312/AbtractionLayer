#pragma once
#include "../Snct_DXObject.h"
#include "Snct_DX11Device.h"
#include "Snct_DX11Context.h"

class SnctDX11Object : SnctDXObject
{
public:
	bool Create(ISnctDXDevice* pDevice) override final;
	void Draw(ISnctDXContext* pCmdList)override final;

private:

};

