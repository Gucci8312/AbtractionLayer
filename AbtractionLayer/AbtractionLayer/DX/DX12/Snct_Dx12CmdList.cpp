#include "Snct_Dx12CmdList.h"

void SnctDx12CmdList::ClearRTV(Product* RTV)
{
	RTV->Use<ID3D12DescriptorHeap>();
	//m_cmdList->ClearRenderTargetView(m_handleRTV[m_frameIndex], clearColor, 0, nullptr);
}
