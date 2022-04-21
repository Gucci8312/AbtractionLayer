#include "Snct_Dx12CmdList.h"

HRESULT SnctDx12CmdList::Create(D3D12_COMMAND_LIST_TYPE Type, ID3D12Device* Device, ID3D12CommandAllocator* CmdAllocator)
{
	return Device->CreateCommandList(0, Type,
		CmdAllocator, nullptr, IID_PPV_ARGS(m_cmdList.ReleaseAndGetAddressOf()));
}

void SnctDx12CmdList::ClearRTV(ISnctRTV* RTV)
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	RTV->Get(&rtvHandle);
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}
