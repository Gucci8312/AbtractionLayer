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
void SnctDX12Device::CreateRTV(ISnctDXBuffer* buffer, D3D12_RENDER_TARGET_VIEW_DESC rtvDesc, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle)
{
    SnctDX12Buffer* tempBuffer = static_cast<SnctDX12Buffer*>(buffer);
    m_pDevice->CreateRenderTargetView(tempBuffer->GetBuffer(), &rtvDesc, rtvHandle);
}
