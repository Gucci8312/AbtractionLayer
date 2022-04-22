#pragma once
#include "Snct_DX12.h"
#include "../../Snct_Windows.h"
#include "../Snct_DXResource.h"

class ISnctDxCommandList
{
public:
	virtual void SetViewPort(float Width, float Height, float MinDepth, float MaxDepth) = 0;
	virtual void SetScissorRects(float Width, float Height) = 0;
	virtual void Close() = 0;
	virtual void SetRTV(UINT NumDescriptors, ISnctDxRTV* DescriptorHandle, bool SingleHandleToDescriptorRange, ISnctDxDSV* DSHandle)=0;

private:

};
