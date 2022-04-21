#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#pragma region Render target view
// View management interface class
class ISnctView
{
public:
	virtual ~ISnctView() {};
	virtual void Get(void* ReceiveData) = 0;
	//virtual ISnctView* GetView() = 0;
};

// 生成していくRTVの元
class ISnctRTV
{
public:
	// Return as areturn value
	template <class T> void Get(T* ReturnRTV)
	{
		
	}

	// RTV for DirectX11
	template <> void Get<ID3D11RenderTargetView>(ID3D11RenderTargetView* ReturnRtv)
	{
		ReturnRtv = reinterpret_cast<ID3D11RenderTargetView*>(m_RenderTargetView);
	}

	// RTV for DirectX12
	template <> void Get<D3D12_CPU_DESCRIPTOR_HANDLE>(D3D12_CPU_DESCRIPTOR_HANDLE* ReturnRtv)
	{
		ReturnRtv = reinterpret_cast<D3D12_CPU_DESCRIPTOR_HANDLE*>(m_RenderTargetView);
	}


protected:
	//---------------------------------------------------------------------------
	// protected variables.
	//---------------------------------------------------------------------------	
	ISnctView* m_RenderTargetView;
};

class RTV :public ISnctRTV
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	template<class T> RTV(T* RTVResource) { m_RenderTargetView = reinterpret_cast<ISnctView*>(RTVResource); }
	~RTV() {}
private:
};

// RTV
class ISnctCreateRTV 
{
public:
	ISnctCreateRTV() {}
	~ISnctCreateRTV() {}
	template<class T>  ISnctRTV* Convert(T* rtv)
	{
		ISnctRTV* p = new RTV(rtv);
		return p;
	}
};

// Dx11用レンダーターゲットビュー
class SnctDx11RTV :public ISnctView
{
private:
	ID3D11RenderTargetView* m_RenderTargetView;
public:
	ISnctView* Get() { return (ISnctView*)m_RenderTargetView; }
	//void Get(void* ReceiveData) { ReceiveData = m_RenderTargetView; }
	void Create(ID3D11Device* Device, ID3D11Texture2D* BackBuffer);
};

// Dx12用レンダーターゲットビュー
class SnctDx12RTV :public ISnctView
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	D3D12_CPU_DESCRIPTOR_HANDLE* Get() { return m_handle; }
	void Get(void* ReceiveData) { ReceiveData = m_handle; }
	void Create(ID3D12Device* device, D3D12_RENDER_TARGET_VIEW_DESC RTVDesc, D3D12_CPU_DESCRIPTOR_HANDLE startHandle, int BackBufferNum, ID3D12Resource** Buffer);
private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	D3D12_CPU_DESCRIPTOR_HANDLE* m_handle;
};

#pragma endregion
