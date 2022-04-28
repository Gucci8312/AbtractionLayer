#include "Snct_DX11Object.h"

bool SnctDX11Object::Create(ISnctDXDevice* pDevice)
{
    SnctDX11Device* pTempDevice = static_cast<SnctDX11Device*>(pDevice);

    return true;
}

void SnctDX11Object::Draw(ISnctDXContext* pCmdList)
{
    SnctDX11Context* pTempCmdList = static_cast<SnctDX11Context*>(pCmdList);

}
