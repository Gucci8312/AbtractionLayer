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
	virtual ISnctView* GetView() = 0;
};

// Dx11�p�����_�[�^�[�Q�b�g�r���[
class Dx11RTV :public ISnctView
{
private:
	ID3D11RenderTargetView* m_RenderTargetView;
public:
	ISnctView* GetView() { return (ISnctView*)m_RenderTargetView; }
};

// Dx12�p�����_�[�^�[�Q�b�g�r���[
class Dx12RTV :public ISnctView
{
private:
	ID3D12DescriptorHeap* m_RenderTargetView;
public:
	ISnctView* GetView() { return (ISnctView*)m_RenderTargetView; }
};

// �������Ă���RTV�̌�
class RTVProduct
{
public:
	// Return as areturn value
	template <class T> T* Get() { return static_cast<T*>(RenderTargetView); }

	// Return as an argument
	template <class T> void Get(T* ReturnRTV)
	{
		ReturnRTV = nullptr;
	}

	// RTV for DirectX11
	template <> void Get<ID3D11RenderTargetView>(ID3D11RenderTargetView* ReturnRtv)
	{
		ReturnRtv = reinterpret_cast<ID3D11RenderTargetView*>(RenderTargetView);
	}

	// RTV for DirectX12
	template <> void Get<ID3D12DescriptorHeap>(ID3D12DescriptorHeap* ReturnRtv)
	{
		ReturnRtv = reinterpret_cast<ID3D12DescriptorHeap*>(RenderTargetView);
	}

protected:
	ISnctView* RenderTargetView;
};

class RTV :public RTVProduct
{
public:
	template<class T> RTV(T* RTVResource) { RenderTargetView = new T; }
	~RTV() {}
private:
};


// RTV
class RenderTargetViewFactory 
{
public:
	RenderTargetViewFactory() {}
	~RenderTargetViewFactory() {}
	template<class T>  RTVProduct* Convert(T* rtv)
	{
		RTVProduct* p = new RTV(rtv);
		return p;
	}
};

#pragma endregion
