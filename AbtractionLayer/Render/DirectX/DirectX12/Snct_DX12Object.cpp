#include "Snct_DX12Object.h"

bool SnctDX12Object::Create(ISnctDXDevice* pDevice)
{
	SnctDX12Device* pTempDevice = static_cast<SnctDX12Device*>(pDevice);

	return false;
}

void SnctDX12Object::Draw(ISnctDXContext* pCmdList)
{
	SnctDX12Context* pTempCmdList = static_cast<SnctDX12Context*>(pCmdList);
}
