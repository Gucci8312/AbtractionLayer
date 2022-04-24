#include "Snct_DX12Device.h"

//------------------------------------------------------------------------------
/// Create Directx12 device
/// \param[in]		Feature level
/// \return			HRESULT
//------------------------------------------------------------------------------
HRESULT SnctDX12Device::Create(D3D_FEATURE_LEVEL Level)
{
    return D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(m_pDevice.ReleaseAndGetAddressOf()));
}


//------------------------------------------------------------------------------
/// Create Directx12 command allocator
/// \param[in]		Command allocator
/// \return			HRESULT
//------------------------------------------------------------------------------
HRESULT SnctDX12Device::CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE commandListType, ID3D12CommandAllocator** cmdAllocator)
{
    return m_pDevice->CreateCommandAllocator(commandListType,
        IID_PPV_ARGS(cmdAllocator));
}


//------------------------------------------------------------------------------
/// Create Directx12 command queue
/// \param[in]		Command queue desc
/// \param[in]		Command queue 
/// \return			HRESULT
//------------------------------------------------------------------------------
HRESULT SnctDX12Device::CreateCommandQueue(D3D12_COMMAND_QUEUE_DESC queueDesc, ID3D12CommandQueue** commandqueue)
{
    return m_pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(commandqueue));
}


//------------------------------------------------------------------------------
/// return increment handle size
/// \param[in]		Descriptor heap type
/// \return			unsigned int
//------------------------------------------------------------------------------
unsigned int SnctDX12Device::GetIncrementHandleSize(D3D12_DESCRIPTOR_HEAP_TYPE type)
{
    return m_pDevice->GetDescriptorHandleIncrementSize(type);
}


//------------------------------------------------------------------------------
/// Create render target view
/// \param[in]		Render target view desc
/// \param[in]		Render target view handle
/// \return			none
//------------------------------------------------------------------------------
HRESULT SnctDX12Device::CreateRTV(ISnctDXBuffer* buffer,  ISnctDXRTV* rtvHandle)
{
    // Render target view settings
    D3D12_RENDER_TARGET_VIEW_DESC viewDesc = {};
    viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D.MipSlice = 0;
    viewDesc.Texture2D.PlaneSlice = 0;

    SnctDX12Buffer* tempBuffer = static_cast<SnctDX12Buffer*>(buffer);
    SnctDX12RTV* tempRTVHandle = static_cast<SnctDX12RTV*>(rtvHandle);
    m_pDevice->CreateRenderTargetView(tempBuffer->GetBuffer(), &viewDesc, tempRTVHandle->GetRTV());

    if (tempRTVHandle != nullptr) return E_FAIL;
    return S_OK;
}

//------------------------------------------------------------------------------
/// Create render target view
/// \param[in]		Render target view desc
/// \param[in]		Render target view handle
/// \return			none
//------------------------------------------------------------------------------
HRESULT SnctDX12Device::CreateDSV(ISnctDXBuffer* buffer,  ISnctDXDSV* dsvHandle)
{
    // Render target view settings
        // Depth stencil view settings
    D3D12_DEPTH_STENCIL_VIEW_DESC DepthViewDesc = {};
    DepthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    DepthViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    DepthViewDesc.Texture2D.MipSlice = 0;
    DepthViewDesc.Flags = D3D12_DSV_FLAG_NONE;

    SnctDX12Buffer* tempBuffer = static_cast<SnctDX12Buffer*>(buffer);
    SnctDX12DSV* tempDSVHandle = static_cast<SnctDX12DSV*>(dsvHandle);

    // Create depth stencil view
    m_pDevice->CreateDepthStencilView(tempBuffer->GetBuffer(), &DepthViewDesc, tempDSVHandle->GetDSV());

    if (tempDSVHandle != nullptr) return E_FAIL;
    return S_OK;
}
