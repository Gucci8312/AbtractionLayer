#include "Snct_DX12CmdQueue.h"

//------------------------------------------------------------------------------
/// Get gpu status
/// \param[in]		fence
/// \return			HRESULT
//------------------------------------------------------------------------------
void SnctDX12CmdQueue::Signal(ID3D12Fence* fence, uint64_t& value)
{
	m_pCmdQueue->Signal(fence, value);
}

void SnctDX12CmdQueue::Execute(int Num, ISnctDXCmdList** cmdList)
{
	std::vector<ID3D12CommandList*> ppCmdLists = {};
	for (int Idx = 0; Idx < Num; Idx++)
	{
		SnctDX12CmdList* CmdList = static_cast<SnctDX12CmdList*>(*cmdList);
		ppCmdLists.push_back(CmdList->Get());
	}

	m_pCmdQueue->ExecuteCommandLists(1, ppCmdLists.data());
}
