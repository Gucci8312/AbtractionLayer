#include "Snct_Dx12Device.h"

HRESULT SnctDx12Device::Create(D3D_FEATURE_LEVEL Level)
{
    return D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(m_device.ReleaseAndGetAddressOf()));
}
