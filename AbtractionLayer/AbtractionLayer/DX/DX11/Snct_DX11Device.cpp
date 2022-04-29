#include "Snct_DX11Device.h"

//------------------------------------------------------------------------------
/// Create Directx12 device
/// \param[in]		Feature level
/// \return			Whether it was created
//------------------------------------------------------------------------------
HRESULT SnctDX11Device::CreateDevice(D3D_FEATURE_LEVEL Level)
{
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
	// If the project is in a debug build, enable the debug layer.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	return D3D11CreateDevice(nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		&Level,
		1,
		D3D11_SDK_VERSION,
		m_pDevice.GetAddressOf(),
		nullptr,
		m_pDeviceContext.ReleaseAndGetAddressOf());
}


//------------------------------------------------------------------------------
/// Create DirectX11 deferred context
/// \param[out]		deferred context double pointer
/// \return			Whether it was created
//------------------------------------------------------------------------------
HRESULT SnctDX11Device::CreateCmdList(ISnctDXContext** pCmdList)
{
	SnctDX11Context* tempCmdList = static_cast<SnctDX11Context*>(*pCmdList);
	return m_pDevice->CreateDeferredContext(0, tempCmdList->GetContextAddress());
}


//------------------------------------------------------------------------------
/// Create DirectX11 render target view
/// \param[in]		Back buffer pointer
/// \param[out]		Render target view pointer
/// \return			Whether it was created
//------------------------------------------------------------------------------
HRESULT SnctDX11Device::CreateRTV(ISnctDXBuffer* pBuckBuffer, ISnctDXRTV* pRTV)
{
	ID3D11Resource* tempResource = static_cast<SnctDX11Buffer*>(pBuckBuffer)->GetBuffer();
	SnctDX11RTV* tempRTV = dynamic_cast<SnctDX11RTV*>(pRTV);

	//ID3D11RenderTargetView* TempCreateRTV;
	return m_pDevice->CreateRenderTargetView(tempResource,
		nullptr, tempRTV->GetRTVAddress());
}


//------------------------------------------------------------------------------
/// Create DirectX11 depth stencil view
/// \param[in]		Back buffer pointer
/// \param[out]		Depth stencil view pointer
/// \return			Whether it was created
//------------------------------------------------------------------------------
HRESULT SnctDX11Device::CreateDSV(ISnctDXBuffer* buffer, ISnctDXDSV* dsv)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView{};
	descDepthStencilView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepthStencilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDepthStencilView.Flags = 0;

	SnctDX11Buffer* tempBuffer = static_cast<SnctDX11Buffer*>(buffer);
	SnctDX11DSV* tempDSV = dynamic_cast<SnctDX11DSV*>(dsv);

	//ID3D11DepthStencilView* tempCreateDSV = {};
	return m_pDevice->CreateDepthStencilView(tempBuffer->GetBuffer(),
		&descDepthStencilView, tempDSV->GetDSVAddress());
}


//------------------------------------------------------------------------------
/// Execution command list
/// \param[in]		Command list pointer
/// \return			none
//------------------------------------------------------------------------------
void SnctDX11Device::ExecuteCmdList(ID3D11CommandList* cmdList)
{
	m_pDeviceContext->ExecuteCommandList(cmdList, false);
}


//------------------------------------------------------------------------------
/// Set view port
/// \param[in]		Window width
/// \param[in]		Window heiht
/// \param[in]		Maximum depth
/// \param[in]		Command list pointer
/// \return			none
//------------------------------------------------------------------------------
void SnctDX11Device::SetViewPort(float Width, float Height, float MinDepth, float MaxDepth)
{
	D3D11_VIEWPORT viewPort;
	viewPort.Width = Width;
	viewPort.Height = Height;
	viewPort.MinDepth = MinDepth;
	viewPort.MaxDepth = MaxDepth;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;

	m_pDeviceContext->RSSetViewports(1, &viewPort);
}


//------------------------------------------------------------------------------
/// Clear render target view
/// \param[in]		Render target view pointer
/// \param[in]		Clear color
/// \param[in]		Rects num
/// \param[in]		Rects poiner
/// \return			none
//------------------------------------------------------------------------------
void SnctDX11Device::ClearRTV(ISnctDXRTV* pRenderTargetView, float clearColor[4], UINT NumRects, RECT* pRects)
{
	SnctDX11RTV* TempRTV = static_cast<SnctDX11RTV*>(pRenderTargetView);
	m_pDeviceContext->ClearRenderTargetView(TempRTV->GetRTV(), clearColor);
}


//------------------------------------------------------------------------------
/// Clear depth stencil view
/// \param[in]		Descriptor handle
/// \param[in]		Clear flag
/// \param[in]		Depth
/// \param[in]		Stencil
/// \param[in]		Num
/// \param[in]		Rects
/// \return			none
//------------------------------------------------------------------------------
void SnctDX11Device::ClearDSV(ISnctDXDSV* pDepthStencilView, UINT Flag, float Depth, UINT8 Stencil, UINT NumRects, RECT* pRects)
{
	SnctDX11DSV* TempDSV = static_cast<SnctDX11DSV*>(pDepthStencilView);
	m_pDeviceContext->ClearDepthStencilView(TempDSV->GetDSV(), static_cast<UINT>(Flag), Depth, Stencil);
}

//------------------------------------------------------------------------------
/// Set render target view
/// \param[in]		Num
/// \param[in]		Descriptor handle
/// \return			none
//------------------------------------------------------------------------------
void SnctDX11Device::SetRTV(UINT viewNum, ISnctDXRTV* pRTV, ISnctDXDSV* pDSV, bool SingleHandleToDescriptorRange)
{
	SnctDX11RTV* TempRTV = static_cast<SnctDX11RTV*>(pRTV);
	SnctDX11DSV* TempDSV = static_cast<SnctDX11DSV*>(pDSV);
	m_pDeviceContext->OMSetRenderTargets(viewNum, TempRTV->GetRTVAddress(), TempDSV->GetDSV());
}


//------------------------------------------------------------------------------
/// Set rasterizer state
/// \param[in]		Rasterizer state pointer
/// \return			none
//------------------------------------------------------------------------------
void SnctDX11Device::SetRasterizerState(ISnctDXRasterizerState* rasterizerState)
{
	SnctDX11RasterizerState* pTempRasterizerState = static_cast<SnctDX11RasterizerState*>(rasterizerState);
	m_pDeviceContext->RSSetState(pTempRasterizerState->GetRasterizerState());
}

void SnctDX11Device::PSSetSamplers(UINT SamplerNum,ISnctDXStaticSampler* pSampler)
{
	SnctDX11Sampler* pTempSampler = static_cast<SnctDX11Sampler*>(pSampler);
	m_pDeviceContext->PSSetSamplers(0, SamplerNum, pTempSampler->GetSamplerAddress());
}
