#include "Snct_DX12Device.h"

HRESULT SnctDX12Device::Create(D3D_FEATURE_LEVEL Level)
{
    return D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(m_pDevice.ReleaseAndGetAddressOf()));
}

HRESULT SnctDX12Device::CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE commandListType, ID3D12CommandAllocator** cmdAllocator)
{
    return m_pDevice->CreateCommandAllocator(commandListType,
        IID_PPV_ARGS(cmdAllocator));
}

HRESULT SnctDX12Device::CreateCommandQueue(D3D12_COMMAND_QUEUE_DESC queueDesc, ID3D12CommandQueue** commandqueue)
{
    return m_pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(commandqueue));
}

unsigned int SnctDX12Device::GetIncrementHandleSize(D3D12_DESCRIPTOR_HEAP_TYPE type)
{
    return m_pDevice->GetDescriptorHandleIncrementSize(type);
}

void SnctDX12Device::CreateRTV(ID3D12Resource* buffer, D3D12_RENDER_TARGET_VIEW_DESC rtvDesc, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle)
{
    m_pDevice->CreateRenderTargetView(buffer, &rtvDesc, rtvHandle);
}
