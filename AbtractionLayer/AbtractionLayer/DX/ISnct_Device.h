// Interface class for DirectX device Manegiment file...
#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

// Device interface class
class IDevice
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	virtual ~IDevice() {};
	virtual IDevice* GetDvice() = 0;
};

// Device management interface class
class ISnctDevice
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	template <class T> void Get(T* ReturnRTV)
	{
		return reinterpret_cast<T*>(m_device);
	}

	 //CmdList for DirectX11
	template <> void Get<ID3D11Device>(ID3D11Device* ReturnDevice)
	{
		ReturnDevice = reinterpret_cast<ID3D11Device*>(m_device);
	}

	// Dx12Device for DirectX12
	template <> void Get<ID3D12Device>(ID3D12Device* ReturnCmdList)
	{
		ReturnCmdList = reinterpret_cast<ID3D12Device*>(m_device);
	}

protected:
	//---------------------------------------------------------------------------
	// protected variables.
	//---------------------------------------------------------------------------	
	IDevice* m_device;
};


class Device :public ISnctDevice
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	template<class T> Device(T* DeviceResource) { m_device = reinterpret_cast<IDevice*>(DeviceResource); }
	~Device() {}
private:
};

// Create DirectX command list
class ISnctCreateDevice
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	ISnctCreateDevice() {}
	~ISnctCreateDevice() {}
	template<class T>  ISnctDevice* Convert(T* returnDevice)
	{
		ISnctDevice* p = new Device(returnDevice);
		return p;
	}
};

