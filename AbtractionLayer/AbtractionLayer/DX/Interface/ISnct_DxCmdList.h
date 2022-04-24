#pragma once
#include "../../Snct_Windows.h"
#include "../Snct_DXResource.h"

// A class that manages DirectX command list interface
class ISnctDXCmdList
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	virtual void SetViewPort(float Width, float Height, float MinDepth, float MaxDepth) = 0;
	virtual void ClearRTV(ISnctDXRTV* Descriptors, UINT NumRects, RECT* pRects = 0) = 0;
	virtual void ClearDSV(ISnctDXDSV* Descriptors, UINT Flag, float Depth,
		UINT8 Stencil, UINT NumRects = 0, RECT* pRects = 0) = 0;
	virtual void SetRTV(UINT NumDescriptors, ISnctDXRTV* DescriptorHandle, ISnctDXDSV* DSHandle, bool SingleHandleToDescriptorRange) = 0;
};
