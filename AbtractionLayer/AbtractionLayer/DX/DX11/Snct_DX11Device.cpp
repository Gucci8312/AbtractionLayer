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
		m_pDeviceContext.GetAddressOf());
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
		nullptr, tempRTV->GetRTVAddress());
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
		&descDepthStencilView, tempDSV->GetDSVAddress());
}
