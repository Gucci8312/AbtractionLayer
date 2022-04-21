#pragma once
#include "../ISnct_CmdList.h"
#include "../../Snct_Windows.h"
#include "../Snct_DXResource.h"
class SnctDx12CmdList :
    public ISnctCmdList
{
public:
    HRESULT Create(D3D12_COMMAND_LIST_TYPE Type, ID3D12Device* Device,ID3D12CommandAllocator* CmdAllocator);
    void ClearRTV(ISnctRTV* RTV) ;
    ID3D12GraphicsCommandList* Get() { return m_cmdList.Get(); }
private:
    ComPtr<ID3D12GraphicsCommandList> m_cmdList;
};

