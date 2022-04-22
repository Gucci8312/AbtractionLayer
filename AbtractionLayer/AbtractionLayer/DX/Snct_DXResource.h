#pragma once
#include "../Snct_Windows.h"
#include <d3d11.h>
#include <d3d12.h>

enum class DEPTH_CLEAR_FLAGS
{
	CLEAR_FLAG_DEPTH = 0x1,
	CLEAR_FLAG_STENCIL = 0x2,
};


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
	ID3D11RenderTargetView* GetRTV() { return m_rtv; }
private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ID3D11RenderTargetView* m_rtv;
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
	D3D12_CPU_DESCRIPTOR_HANDLE* GetpHandle() { return &m_rtvHandle; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTV() { return m_rtvHandle; }
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
	ID3D11DepthStencilView* GetRTV() { return m_dsv; }
private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ID3D11DepthStencilView* m_dsv;
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
class SnctDXResource
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	virtual ~SnctDXResource() {}
	virtual SnctDXResource* Get() = 0;
};


// DirectX buffer management interface class
class ISnctDXBuffer : public SnctDXResource
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