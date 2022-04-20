#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")


// View management interface class
class ISnctView
{
public:
	virtual ~ISnctView() {};
	virtual ISnctView* GetView() = 0;
};

class Dx11RTV :public ISnctView
{
private:
	ID3D11RenderTargetView* m_RenderTargetView;
public:
	ISnctView* GetView() { return (ISnctView*)m_RenderTargetView; }
};

class Dx12RTV :public ISnctView
{
private:
	ID3D12DescriptorHeap* m_RenderTargetView;
public:
	ISnctView* GetView() { return (ISnctView*)m_RenderTargetView; }
};

class Product
{
public:
	template <class T> T* Use() { return static_cast<T*>(RenderTargetView); };
protected:
	ISnctView* RenderTargetView;
};

class Factory
{
public:
	template<class T>  Product* Create(T* Object)
	{
		Product* p = CreateProduct(Object);
		//RegisterProduct(p);
		return p;
	}

	virtual Product* CreateProduct(Dx11RTV* Dev) = 0;
	virtual Product* CreateProduct(Dx12RTV* Dev) = 0;
};


class RTV :public Product
{
public:
	template<class T> RTV(T* RTVResource) { RenderTargetView = new T; }
	~RTV() {}
	////ISnctView* Use()  { return RenderTargetView; };
	//ISnctView* Use() final;
private:
};



class RenderTargetViewFactory :public Factory
{
private:
	std::list<std::string> Devices;

public:
	RenderTargetViewFactory() {}
	~RenderTargetViewFactory() {}
	Product* CreateProduct(Dx11RTV* rtv) { return new RTV(rtv); }
	Product* CreateProduct(Dx12RTV* rtv) { return new RTV(rtv); }
	//void RegisterProduct(Product* Product);
};

