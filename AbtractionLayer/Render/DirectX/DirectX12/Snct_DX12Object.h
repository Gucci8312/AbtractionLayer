#pragma once
#include "../Snct_DXObject.h"
#include "Snct_DX12Device.h"
#include "Snct_DX12Context.h"

class SnctDX12Object : public SnctDXObject
{
public:
	bool Create(ISnctDXDevice* pDevice) override final;
	void Draw(ISnctDXContext* pCmdList)override final;

private:
};

