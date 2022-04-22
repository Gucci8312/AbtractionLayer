#pragma once
#include "Snct_DX12.h"
#include "../Interface/ISnct_DXCmdQueue.h"
#include "Snct_DX12CmdList.h"
#include <vector>

// A class that manages DirectX12 command queue
class SnctDX12CmdQueue : public ISnctDXCmdQueue
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	ID3D12CommandQueue* GetCmdQueue() { return m_pCmdQueue.Get(); }
	ID3D12CommandQueue** GetCmdQueueAddress() { return m_pCmdQueue.GetAddressOf(); }
	void Signal(ID3D12Fence* fence, uint64_t& value);
	void Execute(int Num, ISnctDXCmdList** cmdList) override final;
private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D12CommandQueue> m_pCmdQueue;
};

