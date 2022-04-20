#pragma once
#include "../ISnct_DxCmdList.h"
class SnctDx12CmdList :
    public ISnctDxCmdList
{
public:
    void ClearRTV(Product* RTV) ;
};

