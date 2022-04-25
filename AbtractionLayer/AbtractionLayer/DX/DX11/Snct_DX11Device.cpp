#include "Snct_DX11Device.h"

// デバイスの作成関連後で修正
HRESULT SnctDX11Device::Create(D3D_FEATURE_LEVEL Level)
{
	return D3D11CreateDevice(nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		&Level,
		1,
		D3D11_SDK_VERSION,
		m_pDevice.GetAddressOf(),
		nullptr,
		m_pDeviceContext.ReleaseAndGetAddressOf());
}

void SnctDX11Device::CreateDeferredContext(ID3D11DeviceContext** DeferredContext)
{
	m_pDevice->CreateDeferredContext(0, DeferredContext);
}

HRESULT SnctDX11Device::CreateRTV(ISnctDXBuffer* backbuffer, ISnctDXRTV* rtv)
{
	SnctDX11Buffer* tempBuffer = static_cast<SnctDX11Buffer*>(backbuffer);
	SnctDX11RTV* tempRTV = dynamic_cast<SnctDX11RTV*>(rtv);

	return m_pDevice->CreateRenderTargetView(tempBuffer->GetBuffer(),
		nullptr, tempRTV->SetRTVAddress());
}

HRESULT SnctDX11Device::CreateDSV(ISnctDXBuffer* buffer, ISnctDXDSV* dsv)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView{};
	descDepthStencilView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepthStencilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDepthStencilView.Flags = 0;

	SnctDX11Buffer* tempBuffer = static_cast<SnctDX11Buffer*>(buffer);
	SnctDX11DSV* tempDSV = dynamic_cast<SnctDX11DSV*>(dsv);

	return m_pDevice->CreateDepthStencilView(tempBuffer->GetBuffer(),
		&descDepthStencilView, tempDSV->SetDSVAddress());
}

void SnctDX11Device::ExecuteCmdList(ID3D11CommandList* cmdList)
{
	m_pDeviceContext->ExecuteCommandList(cmdList, false);
}

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

void SnctDX11Device::ClearRTV(ISnctDXRTV* Descriptors, UINT NumRects, RECT* pRects)
{
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	SnctDX11RTV* TempRTV = static_cast<SnctDX11RTV*>(Descriptors);
	m_pDeviceContext->ClearRenderTargetView(TempRTV->GetRTV(), clearColor);
}

void SnctDX11Device::ClearDSV(ISnctDXDSV* Descriptors, UINT Flag, float Depth, UINT8 Stencil, UINT NumRects, RECT* pRects)
{
	SnctDX11DSV* TempDSV = static_cast<SnctDX11DSV*>(Descriptors);
	m_pDeviceContext->ClearDepthStencilView(TempDSV->GetDSV(), static_cast<UINT>(Flag), Depth, Stencil);
}

void SnctDX11Device::SetRTV(UINT NumDescriptors, ISnctDXRTV* Descriptors, ISnctDXDSV* DSHandle, bool SingleHandleToDescriptorRange)
{
	SnctDX11RTV* TempRTV = static_cast<SnctDX11RTV*>(Descriptors);
	SnctDX11DSV* TempDSV = static_cast<SnctDX11DSV*>(DSHandle);
	m_pDeviceContext->OMSetRenderTargets(1, TempRTV->GetRTVAddress(), TempDSV->GetDSV());
}