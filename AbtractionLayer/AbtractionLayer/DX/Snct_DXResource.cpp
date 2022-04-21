#include "Snct_DXResource.h"

void SnctDx11RTV::Create(ID3D11Device* Device, ID3D11Texture2D* BackBuffer)
{
	Device->CreateRenderTargetView(BackBuffer, nullptr, &m_RenderTargetView);
}

void SnctDx12RTV::Create(ID3D12Device* device, D3D12_RENDER_TARGET_VIEW_DESC RTVDesc, D3D12_CPU_DESCRIPTOR_HANDLE startHandle,int BackBufferNum,ID3D12Resource** Buffer)
{
	auto incremntsize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	auto handle = startHandle;
	m_handle = new D3D12_CPU_DESCRIPTOR_HANDLE[BackBufferNum];
	for (int Idx = 0; Idx < BackBufferNum; Idx++)
	{
		// Render target view settings
		D3D12_RENDER_TARGET_VIEW_DESC viewDesc = {};
		viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipSlice = 0;
		viewDesc.Texture2D.PlaneSlice = 0;

		// Create render target view
		device->CreateRenderTargetView(Buffer[Idx], &viewDesc, handle);
		m_handle[Idx] = handle;
		handle.ptr += incremntsize;
	}
}
