#include "Snct_DX11Context.h"

//------------------------------------------------------------------------------
/// Set raster rizer state
/// \param[in]		Rasterize state
/// \return			none
//------------------------------------------------------------------------------
void SnctDX11Context::SetRasterizerState(ISnctDXRasterizerState* pRaseterizeState)
{
	SnctDX11RasterizerState* pTempRasterizerState = static_cast<SnctDX11RasterizerState*>(pRaseterizeState);
	m_pContext->RSSetState(pTempRasterizerState->GetRasterizerState());
}


//------------------------------------------------------------------------------
/// Set view port
/// \param[in]		Width
/// \param[in]		Height
/// \param[in]		Minimum depth
/// \param[in]		Maximum depth
/// \return			none
//------------------------------------------------------------------------------
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

void SnctDX11Context::ClearRTV(ISnctDXRTV* pRTV, float clearColor[4], UINT NumRects, RECT* pRects)
{
	SnctDX11RTV* TempRTV = static_cast<SnctDX11RTV*>(pRTV);
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

void SnctDX11Context::Close()
{
	//m_pContext->FinishCommandList(true, cmdList);
}

void SnctDX11Context::SetVertexBuffer(UINT bufferNum, ISnctDXBuffer* pBuffer, UINT stride, UINT num)
{
	SnctDX11Buffer* pTempBuffer = static_cast<SnctDX11Buffer*>(pBuffer);
	m_pContext->IASetVertexBuffers(0, 1, pTempBuffer->GetBufferAddress(), &stride, &num);
}

void SnctDX11Context::SetIndexBuffer(ISnctDXBuffer* pBuffer, DXGI_FORMAT format, UINT size)
{
	SnctDX11Buffer* pTempBuffer = static_cast<SnctDX11Buffer*>(pBuffer);
	m_pContext->IASetIndexBuffer(pTempBuffer->GetBuffer(), format, size);
}

void SnctDX11Context::SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology)
{
	m_pContext->IASetPrimitiveTopology(topology);
}

void SnctDX11Context::DrawIndexed(UINT indexCount, UINT startIndexLocation, UINT instanceLocation)
{
	m_pContext->DrawIndexedInstanced(indexCount, startIndexLocation, 0, 0, instanceLocation);
}

void SnctDX11Context::VSSetConstantBuffer(UINT startSlot, UINT bufferNum, ISnctDXBuffer* pBuffer)
{
	SnctDX11Buffer* pTempBuffer = static_cast<SnctDX11Buffer*>(pBuffer);
	m_pContext->VSSetConstantBuffers(startSlot, bufferNum, pTempBuffer->GetBufferAddress());
}

void SnctDX11Context::PSSetConstantBuffer(UINT startSlot, UINT bufferNum, ISnctDXBuffer* pBuffer)
{
	SnctDX11Buffer* pTempBuffer = static_cast<SnctDX11Buffer*>(pBuffer);
	m_pContext->PSSetConstantBuffers(startSlot, bufferNum, pTempBuffer->GetBufferAddress());
}

void SnctDX11Context::UpdateSubresource(ISnctDXBuffer* pBuffer, UINT dstSubResource, const void* pData, UINT srcRowPitch, UINT srcDepthPitch)
{
	SnctDX11Buffer* pTempBuffer = static_cast<SnctDX11Buffer*>(pBuffer);
	m_pContext->UpdateSubresource(pTempBuffer->GetBuffer(), dstSubResource, nullptr, pData, srcRowPitch, srcDepthPitch);
}

void SnctDX11Context::RegisterCmdList(ID3D11CommandList** cmdList)
{
	m_pContext->FinishCommandList(true, cmdList);
}
