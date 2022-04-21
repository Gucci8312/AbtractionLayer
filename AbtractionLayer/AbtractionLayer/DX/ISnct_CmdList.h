// Interface class for DirectX command list Manegiment file...
#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

// Command list interface class
class ICmdList
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	virtual ~ICmdList() {};
	virtual ICmdList* GetCmdList() = 0;
};

// Interface class for DirectX command list management class
class ISnctCmdList
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	template <class T> void Get(T* ReturnRTV)
	{
		return static_cast<T*>(m_CmdList);
	}

	// CmdList for DirectX11
	//template <> void Get<ID3D11RenderTargetView>(ID3D11RenderTargetView* ReturnRtv)
	//{
	//	ReturnRtv = reinterpret_cast<ID3D11RenderTargetView*>(CmdList);
	//}

	// CmdList for DirectX12
	template <> void Get<ID3D12GraphicsCommandList>(ID3D12GraphicsCommandList* ReturnCmdList)
	{
		ReturnCmdList = reinterpret_cast<ID3D12GraphicsCommandList*>(m_CmdList);
	}

protected:
	//---------------------------------------------------------------------------
	// protected variables.
	//---------------------------------------------------------------------------	
	ICmdList* m_CmdList;
};

class CmdList :public ISnctCmdList
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	template<class T> CmdList(T* RTVResource) { m_CmdList = new T; }
	~CmdList() {}
private:
};


// Create DirectX command list
class ISnctCreateCmdList
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	ISnctCreateCmdList() {}
	~ISnctCreateCmdList() {}
	template<class T>  ISnctCmdList* Convert(T* rtv)
	{
		ISnctCmdList* p = new CmdList(rtv);
		return p;
	}
};

