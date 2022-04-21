#include "Snct_Dx12CmdList.h"

void SnctDx12CmdList::ClearRTV(RTVProduct* RTV)
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	RTV->Get(&rtvHandle);
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}
