#include "Snct_DX11Context.h"

void SnctDX11Context::SetRasterRizerState(ID3D11RasterizerState* raseterRizeState)
{
	m_pContext->RSSetState(raseterRizeState);
}

void SnctDX11Context::SetViewPort(float Width, float Height, float MinDepth, float MaxDepth)
{
	D3D11_VIEWPORT viewPort;
	viewPort.Width = Width;
	viewPort.Height = Height;
	viewPort.MinDepth = MinDepth;
	viewPort.MaxDepth = MaxDepth;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;

	m_pContext->RSSetViewports(1, &viewPort);
}

void SnctDX11Context::ClearRTV(ISnctDXRTV* Descriptors, UINT NumRects, RECT* pRects)
{
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	SnctDX11RTV* TempRTV = static_cast<SnctDX11RTV*>(Descriptors);
	m_pContext->ClearRenderTargetView(TempRTV->GetRTV(), clearColor);
}

void SnctDX11Context::ClearDSV(ISnctDXDSV* Descriptors, UINT Flag, float Depth, UINT8 Stencil, UINT NumRects, RECT* pRects)
{
	SnctDX11DSV* TempDSV = static_cast<SnctDX11DSV*>(Descriptors);
	m_pContext->ClearDepthStencilView(TempDSV->GetDSV(), static_cast<UINT>(Flag), Depth, Stencil);
}

void SnctDX11Context::SetRTV(UINT NumDescriptors, ISnctDXRTV* Descriptors, ISnctDXDSV* DSHandle, bool SingleHandleToDescriptorRange)
{
	SnctDX11RTV* TempRTV = static_cast<SnctDX11RTV*>(Descriptors);
	SnctDX11DSV* TempDSV = static_cast<SnctDX11DSV*>(DSHandle);
	m_pContext->OMSetRenderTargets(1, TempRTV->GetRTVAddress(), TempDSV->GetDSV());
}

void SnctDX11Context::Close(bool RestoreDeferredContextState, ID3D11CommandList** cmdList)
{
	m_pContext->FinishCommandList(true, cmdList);

}
