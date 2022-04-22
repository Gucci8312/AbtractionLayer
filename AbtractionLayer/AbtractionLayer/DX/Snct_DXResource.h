#pragma once
#include <d3d11.h>
#include <d3d12.h>

// Render target view management interface class
class ISnctDxRTV
{
public:
	virtual ~ISnctDxRTV() {}
	virtual ISnctDxRTV* Get() = 0;
private:
};


// DirectX11 render target view management class
class SnctDx11RTV : public ISnctDxRTV
{
public:
	SnctDx11RTV* Get() override final { return this; }
	ID3D11RenderTargetView* GetRTV() { return m_rtv; }
private:
	ID3D11RenderTargetView* m_rtv;
};


// DirectX12 render target view management class
class SnctDx12RTV : public ISnctDxRTV
{
public:
	SnctDx12RTV* Get() override final { return this; }
	void SetHandle(D3D12_CPU_DESCRIPTOR_HANDLE handle) { m_rtvHandle = handle; }
	D3D12_CPU_DESCRIPTOR_HANDLE* GetpHandle() { return &m_rtvHandle; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTV() { return m_rtvHandle; }
private:
	D3D12_CPU_DESCRIPTOR_HANDLE m_rtvHandle;
};


// Render target view management interface class
class ISnctDxDSV
{
public:
	virtual ~ISnctDxDSV() {}
	virtual ISnctDxDSV* Get() = 0;
private:
};


// DirectX11 render target view management class
class SnctDx11DSV : public ISnctDxDSV
{
public:
	SnctDx11DSV* Get() override final { return this; }
	ID3D11DepthStencilView* GetRTV() { return m_dsv; }
private:
	ID3D11DepthStencilView* m_dsv;
};


// DirectX12 render target view management class
class SnctDx12DSV : public ISnctDxDSV
{
public:
	SnctDx12DSV* Get() override final { return this; }
	void SetHandle(D3D12_CPU_DESCRIPTOR_HANDLE handle) { m_dsvHandle = handle; }
	D3D12_CPU_DESCRIPTOR_HANDLE* GetpHandle() { return &m_dsvHandle; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSV() { return m_dsvHandle; }
private:
	D3D12_CPU_DESCRIPTOR_HANDLE m_dsvHandle;
};