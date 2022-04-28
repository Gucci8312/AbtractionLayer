#include "Snct_DX12Device.h"

//------------------------------------------------------------------------------
/// Create Directx12 device
/// \param[in]		Feature level
/// \return			Whether it was created
//------------------------------------------------------------------------------
HRESULT SnctDX12Device::CreateDevice(D3D_FEATURE_LEVEL Level)
{
	// Debug option
#if defined(DEBUG) || defined(_DEBUG)
	{
		ComPtr<ID3D12Debug> Debug;
		auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(Debug.ReleaseAndGetAddressOf()));

		// Debuh layer enabled
		if (SUCCEEDED(hr))
		{
			Debug->EnableDebugLayer();
		}
	}
#endif

	return D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(m_pDevice.ReleaseAndGetAddressOf()));
}


//------------------------------------------------------------------------------
/// Create DirectX12 command queue
/// \param[in]		Command queue desc
/// \param[in]		Command queue double pointer
/// \return			Whether it was created
//------------------------------------------------------------------------------
HRESULT SnctDX12Device::CreateCommandQueue(D3D12_COMMAND_QUEUE_DESC CmdQueueDesc, ID3D12CommandQueue** ppCommandQueue)
{
	return m_pDevice->CreateCommandQueue(&CmdQueueDesc, IID_PPV_ARGS(ppCommandQueue));
}


//------------------------------------------------------------------------------
/// return increment handle size
/// \param[in]		Descriptor heap type
/// \return			Descriptor increment size
//------------------------------------------------------------------------------
unsigned int SnctDX12Device::GetIncrementHandleSize(D3D12_DESCRIPTOR_HEAP_TYPE type)
{
	return m_pDevice->GetDescriptorHandleIncrementSize(type);
}


//------------------------------------------------------------------------------
/// Create render target view
/// \param[in]		Buck buffer pointer
/// \param[out]		Render target view handle pointer
/// \return			Whether it was created
//------------------------------------------------------------------------------
HRESULT SnctDX12Device::CreateRTV(ISnctDXBuffer* pBuckBuffer, ISnctDXRTV* pRTV)
{
	// Render target view settings
	D3D12_RENDER_TARGET_VIEW_DESC viewDesc = {};
	viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipSlice = 0;
	viewDesc.Texture2D.PlaneSlice = 0;

	// For temporary storage
	ID3D12Resource* tempBuffer = static_cast<SnctDX12Buffer*>(pBuckBuffer)->GetBuffer();
	SnctDX12RTV* tempRTV = static_cast<SnctDX12RTV*>(pRTV);

	// Create render target
	m_pDevice->CreateRenderTargetView(tempBuffer, &viewDesc, tempRTV->GetHandle());

	// Error checking
	if (tempRTV->GetHandle().ptr) return S_OK;
	return E_FAIL;
}


//------------------------------------------------------------------------------
/// Create render target view
/// \param[in]		Depth texture pointer
/// \param[out]		Render target view handle pointer
/// \return			Whether it was created
//------------------------------------------------------------------------------
HRESULT SnctDX12Device::CreateDSV(ISnctDXBuffer* pDepthTexture, ISnctDXDSV* pDSV)
{
	// Depth stencil view settings
	D3D12_DEPTH_STENCIL_VIEW_DESC DepthViewDesc = {};
	DepthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DepthViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DepthViewDesc.Texture2D.MipSlice = 0;
	DepthViewDesc.Flags = D3D12_DSV_FLAG_NONE;

	// For temporary storage
	ID3D12Resource* tempBuffer = static_cast<SnctDX12Buffer*>(pDepthTexture)->GetBuffer();
	SnctDX12DSV* tempDSV = static_cast<SnctDX12DSV*>(pDSV);

	m_pDevice->CreateDepthStencilView(tempBuffer, &DepthViewDesc, tempDSV->GetHandle());

	//// Create depth stencil view
	//D3D12_CPU_DESCRIPTOR_HANDLE tempCreateHandle = {};
	//m_pDevice->CreateDepthStencilView(tempBuffer, &DepthViewDesc, tempCreateHandle);
	//tempDSV->SetHandle(tempCreateHandle);

	// Error checking
	if (tempDSV->GetHandle().ptr) return S_OK;
	return E_FAIL;
}

HRESULT SnctDX12Device::CreateCmdList(ISnctDXContext** pCmdList)
{
	SnctDX12Context* tempCmdList = static_cast<SnctDX12Context*>(*pCmdList);
	auto hr= m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(tempCmdList->GetCmdAllocatorAddress()));

	hr= m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		tempCmdList->GetCmdAllocator(), nullptr, IID_PPV_ARGS(tempCmdList->GetContextAddress()));
	tempCmdList->Close();

	return hr;
}
