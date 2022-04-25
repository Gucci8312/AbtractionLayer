#pragma once
#include "../Snct_Windows.h"
#include <d3d11.h>
#include <d3d12.h>

// How to use texture
typedef enum SNCT_USAGE
{
	USAGE_DEFAULT = 0,
	USAGE_IMMUTABLE = 1,
	USAGE_DYNAMIC = 2,
	USAGE_STAGING = 3
} 	SNCT_USAGE;


// Texture settings
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
	const virtual ISnctDXRTV* Get() = 0;
};


// DirectX11 render target view management class
class SnctDX11RTV : public ISnctDXRTV
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	// Getter
	const ISnctDXRTV* Get() override final { return this; }
	ID3D11RenderTargetView* GetRTV() { return m_rtv.Get(); }
	ID3D11RenderTargetView** GetRTVAddress() { return m_rtv.GetAddressOf(); }

	//Setter
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
	// Getter
	const SnctDX12RTV* Get() override final { return this; }
	const D3D12_CPU_DESCRIPTOR_HANDLE GetHandle() { return m_rtvHandle; }
	const D3D12_CPU_DESCRIPTOR_HANDLE* GetpHandle() { return &m_rtvHandle; }

	// Setter
	void SetHandle(D3D12_CPU_DESCRIPTOR_HANDLE handle) { m_rtvHandle = handle; }

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
	const virtual ISnctDXDSV* Get() = 0;
};


// DirectX11 render target view management class
class SnctDX11DSV : public ISnctDXDSV
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	const SnctDX11DSV* Get() override final { return this; }
	ID3D11DepthStencilView* GetDSV() { return m_dsv.Get(); }
	ID3D11DepthStencilView** GetDSVAddress() { return m_dsv.GetAddressOf(); }

	void SetDSV(ID3D11DepthStencilView* ppDSV) { m_dsv = ppDSV; }

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
	const SnctDX12DSV* Get() override final { return this; }
	const D3D12_CPU_DESCRIPTOR_HANDLE GetHandle() { return m_dsvHandle; }
	const D3D12_CPU_DESCRIPTOR_HANDLE* GetpHandle() { return &m_dsvHandle; }

	void SetHandle(D3D12_CPU_DESCRIPTOR_HANDLE handle) { m_dsvHandle = handle; }

	void IncrementHandlePointer(unsigned int size) { m_dsvHandle.ptr += size; }

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
	const virtual ISnctDXResource* Get() = 0;
};


// DirectX buffer management interface class
class ISnctDXBuffer : public ISnctDXResource
{
	// Nothing //
};


// DirectX11 buffer management clas
class SnctDX11Buffer : public ISnctDXBuffer
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	const SnctDX11Buffer* Get() override final { return this; }
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
	const SnctDX12Buffer* Get() override final { return this; }
	ID3D12Resource* GetBuffer() { return m_pBuffer.Get(); }
	ID3D12Resource** GetBufferAddress() { return m_pBuffer.GetAddressOf(); }

private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D12Resource> m_pBuffer;
};


// DirectX texture management interface class
class ISnctDXTexture : public ISnctDXBuffer
{
	// Nothing //
};


// DirectX11 texture management clas
class SnctDX11Texture : public ISnctDXTexture
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	const SnctDX11Texture* Get() override final { return this; }
	ID3D11Texture2D* GetTexture() { return m_pTexture.Get(); }
	ID3D11Texture2D** GetTextureAddress() { return m_pTexture.GetAddressOf(); }
	ID3D11Texture2D** SetTextureAddress() { return m_pTexture.ReleaseAndGetAddressOf(); }
	HRESULT Create(ID3D11Device* device, SNCT_TEXTURE2D_DESC desc);

private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D11Texture2D> m_pTexture;
};


// DirectX12 texture management clas
class SnctDX12Texture : public ISnctDXTexture
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	const SnctDX12Texture* Get() override final { return this; }
	ID3D12Resource* GetResource() { return m_pResouce.Get(); }
	ID3D12Resource** GetResourceAddress() { return m_pResouce.GetAddressOf(); }
	ID3D12Resource** SetResourceAddress() { return m_pResouce.ReleaseAndGetAddressOf(); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle() { return m_CPUHandle; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() { return m_GPUHandle; }

private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D12Resource> m_pResouce;
	D3D12_CPU_DESCRIPTOR_HANDLE m_CPUHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_GPUHandle;
};
