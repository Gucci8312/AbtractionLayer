#include "Snct_DXResource.h"

HRESULT SnctDX11Texture::Create(ID3D11Device* device, SNCT_TEXTURE2D_DESC desc)
{
	D3D11_TEXTURE2D_DESC* tempDesc=reinterpret_cast<D3D11_TEXTURE2D_DESC*>(&desc);
	return device->CreateTexture2D(tempDesc, nullptr, m_pTexture.GetAddressOf());
}
