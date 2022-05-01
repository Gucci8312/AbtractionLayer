#pragma once
#include "../Snct_DXResource.h"

// A class that manages DirectX command list interface
class ISnctDXContext
{
public:
	virtual ~ISnctDXContext() {}
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	virtual void SetViewPort(float Width, float Height, float MinDepth, float MaxDepth) = 0;
	virtual void ClearRTV(ISnctDXRTV* Descriptors, float clearColor[4], UINT NumRects, RECT* pRects = 0) = 0;
	virtual void ClearDSV(ISnctDXDSV* Descriptors, UINT Flag, float Depth,
		UINT8 Stencil, UINT NumRects = 0, RECT* pRects = 0) = 0;
	virtual void SetRTV(UINT NumDescriptors, ISnctDXRTV* DescriptorHandle, ISnctDXDSV* DSHandle, bool SingleHandleToDescriptorRange) = 0;
	virtual void SetVertexBuffer(UINT bufferNum, ISnctDXBuffer* pBuffer, UINT stride, UINT num) = 0;
	virtual void SetIndexBuffer(ISnctDXBuffer* pBuffer, DXGI_FORMAT format, UINT size) = 0;
	virtual void SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology) = 0;
	virtual void DrawIndexed(UINT indexCount, UINT startIndexLocation, UINT instanceLocation) = 0;
	virtual void Close()=0;
};
