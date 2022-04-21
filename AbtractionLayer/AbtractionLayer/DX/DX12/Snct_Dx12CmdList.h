#pragma once
#include "../ISnct_CmdList.h"
#include "../Snct_DXResource.h"
class SnctDx12CmdList :
    public ISnctCmdList
{
public:
    void Create();
    void ClearRTV(RTVProduct* RTV) ;
private:
    ID3D12GraphicsCommandList* m_cmdList;
};

