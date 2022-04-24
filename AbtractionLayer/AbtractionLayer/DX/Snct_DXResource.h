#pragma once
#include "../Snct_Windows.h"
#include <d3d11.h>
#include <d3d12.h>

typedef
enum DEPTH_CLEAR_FLAGS
{
	CLEAR_FLAG_DEPTH = 0x1,
	CLEAR_FLAG_STENCIL = 0x2,
} 	DEPTH_CLEAR_FLAGS;

DEFINE_ENUM_FLAG_OPERATORS(DEPTH_CLEAR_FLAGS);

typedef enum SNCT_USAGE
{
	USAGE_DEFAULT = 0,
	USAGE_IMMUTABLE = 1,
	USAGE_DYNAMIC = 2,
	USAGE_STAGING = 3
} 	SNCT_USAGE;

typedef struct SNCT_TEXTURE2D_DESC
{
	UINT Width;
	UINT Height;
	UINT MipLevels;
	UINT ArraySize;
	DXGI_FORMAT Format;
	DXGI_SAMPLE_DESC SampleDesc;
	SNCT_USAGE Usage;
	UINT BindFlags;
	UINT CPUAccessFlags;
	UINT MiscFlags;
} 	SNCT_TEXTURE2D_DESC;

// Render target view management interface class
class ISnctDXRTV
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	virtual ~ISnctDXRTV() {}
	virtual ISnctDXRTV* Get() = 0;
};


// DirectX11 render target view management class
class SnctDX11RTV : public ISnctDXRTV
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	ISnctDXRTV* Get() override final { return this; }
	ID3D11RenderTargetView* GetRTV() { return m_rtv.Get(); }
	ID3D11RenderTargetView** GetRTVAddress() { return m_rtv.GetAddressOf(); }
	ID3D11RenderTargetView** SetRTVAddress() { return m_rtv.ReleaseAndGetAddressOf(); }
private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D11RenderTargetView> m_rtv;
};


// DirectX12 render target view management class
class SnctDX12RTV : public ISnctDXRTV
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	SnctDX12RTV* Get() override final { return this; }
	void SetHandle(D3D12_CPU_DESCRIPTOR_HANDLE handle) { m_rtvHandle = handle; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetHandle() { return m_rtvHandle; }
	D3D12_CPU_DESCRIPTOR_HANDLE* GetpHandle() { return &m_rtvHandle; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTV() { return m_rtvHandle; }
	void IncrementHandlePointer(unsigned int size) { m_rtvHandle.ptr += size; }
private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	D3D12_CPU_DESCRIPTOR_HANDLE m_rtvHandle = {};
};


// Render target view management interface class
class ISnctDXDSV
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	virtual ~ISnctDXDSV() {}
	virtual ISnctDXDSV* Get() = 0;
};


// DirectX11 render target view management class
class SnctDX11DSV : public ISnctDXDSV
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	SnctDX11DSV* Get() override final { return this; }
	ID3D11DepthStencilView* GetDSV() { return m_dsv.Get(); }
	ID3D11DepthStencilView** GetDSVAddress() { return m_dsv.GetAddressOf(); }
	ID3D11DepthStencilView** SetDSVAddress() { return m_dsv.ReleaseAndGetAddressOf(); }
private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D11DepthStencilView> m_dsv;
};


// DirectX12 render target view management class
class SnctDX12DSV : public ISnctDXDSV
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	SnctDX12DSV* Get() override final { return this; }
	void SetHandle(D3D12_CPU_DESCRIPTOR_HANDLE handle) { m_dsvHandle = handle; }
	D3D12_CPU_DESCRIPTOR_HANDLE* GetpHandle() { return &m_dsvHandle; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSV() { return m_dsvHandle; }
private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	D3D12_CPU_DESCRIPTOR_HANDLE m_dsvHandle = {};
};


// DirectX resource management class
class ISnctDXResource
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	virtual ~ISnctDXResource() {}
	virtual ISnctDXResource* Get() = 0;
};


// DirectX buffer management interface class
class ISnctDXBuffer : public ISnctDXResource
{
};


// DirectX11 buffer management clas
class SnctDX11Buffer : public ISnctDXBuffer
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	SnctDX11Buffer* Get() override final { return this; }
	ID3D11Buffer* GetBuffer() { return m_pBuffer.Get(); }
	ID3D11Buffer** GetBufferAddress() { return m_pBuffer.GetAddressOf(); }

private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D11Buffer> m_pBuffer;
};


// DirectX12 buffer management class
class SnctDX12Buffer : public ISnctDXBuffer
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	SnctDX12Buffer* Get() override final { return this; }
	ID3D12Resource* GetBuffer() { return m_pBuffer.Get(); }
	ID3D12Resource** GetBufferAddress() { return m_pBuffer.GetAddressOf(); }
private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D12Resource> m_pBuffer;
};

class ISnctDXTexture : public ISnctDXBuffer
{

};

class SnctDX11Texture : public ISnctDXTexture
{
public:
	SnctDX11Texture* Get() override final { return this; }
	ID3D11Texture2D* GetTexture() { return m_pTexture.Get(); }
	ID3D11Texture2D** GetTextureAddress() { return m_pTexture.GetAddressOf(); }
	ID3D11Texture2D** SetTextureAddress() { return m_pTexture.ReleaseAndGetAddressOf(); }
	HRESULT Create(ID3D11Device* device, SNCT_TEXTURE2D_DESC desc);

private:
	ComPtr<ID3D11Texture2D> m_pTexture;
};

class SnctDX12Texture : public ISnctDXTexture
{
public:
	SnctDX12Texture* Get() override final { return this; }
	ID3D12Resource* GetResource() { return m_pResouce.Get(); }
	ID3D12Resource** GetResourceAddress() { return m_pResouce.GetAddressOf(); }
	ID3D12Resource** SetResourceAddress() { return m_pResouce.ReleaseAndGetAddressOf(); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle() { return m_CPUHandle; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() { return m_GPUHandle; }
private:
	ComPtr<ID3D12Resource> m_pResouce;
	D3D12_CPU_DESCRIPTOR_HANDLE m_CPUHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_GPUHandle;
};