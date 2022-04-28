#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;

#pragma region Parameter
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

typedef enum SNCT_HEAP_TYPE
{
	HEAP_TYPE_DEFAULT = 1,
	HEAP_TYPE_UPLOAD = 2,
	HEAP_TYPE_READBACK = 3,
	HEAP_TYPE_CUSTOM = 4
} 	SNCT_HEAP_TYPE;

typedef enum SNCT_CPU_PAGE_PROPERTY
{
	CPU_PAGE_PROPERTY_UNKNOWN = 0,
	CPU_PAGE_PROPERTY_NOT_AVAILABLE = 1,
	CPU_PAGE_PROPERTY_WRITE_COMBINE = 2,
	CPU_PAGE_PROPERTY_WRITE_BACK = 3
} 	SNCT_CPU_PAGE_PROPERTY;

typedef enum SNCT_MEMORY_POOL
{
	MEMORY_POOL_UNKNOWN = 0,
	MEMORY_POOL_L0 = 1,
	MEMORY_POOL_L1 = 2
} 	MEMORY_POOL;

typedef enum SNCT_RESOURCE_DIMENSION
{
	RESOURCE_DIMENSION_UNKNOWN = 0,
	RESOURCE_DIMENSION_BUFFER = 1,
	RESOURCE_DIMENSION_TEXTURE1D = 2,
	RESOURCE_DIMENSION_TEXTURE2D = 3,
	RESOURCE_DIMENSION_TEXTURE3D = 4
} 	RESOURCE_DIMENSION;

template<typename T>
struct ConstantBufferView
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC Desc;
	D3D12_CPU_DESCRIPTOR_HANDLE     HandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE     HandleGPU;
	T* pBuffer;
};

#pragma endregion


#pragma region SuperClass
// DirectX resource management class
class ISnctDXResource
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	// Getter
	const virtual ISnctDXResource* Get() = 0;

	// Method
	virtual ~ISnctDXResource() {}
};
#pragma endregion


#pragma region View
// Render target view management interface class
class ISnctDXRTV : public ISnctDXResource
{
public:
	// Nothing //	
};

// Render target view management interface class
class ISnctDXDSV
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	virtual ~ISnctDXDSV(){};
	const virtual ISnctDXDSV* Get() = 0;
};

// DirectX11 render target view management class
class SnctDX11RTV : public ISnctDXRTV
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	~SnctDX11RTV() { m_pRTV.Reset(); }
	// Getter
	const ISnctDXRTV* Get() override final { return this; }
	ID3D11RenderTargetView* GetRTV() { return m_pRTV.Get(); }
	ID3D11RenderTargetView** GetRTVAddress() { return m_pRTV.GetAddressOf(); }

	//Setter
	void SetRTV(ID3D11RenderTargetView* ppDSV) { m_pRTV = ppDSV; }

private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D11RenderTargetView> m_pRTV;
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

	// Method
	void IncrementHandlePointer(unsigned int size) { m_rtvHandle.ptr += size; }

private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	D3D12_CPU_DESCRIPTOR_HANDLE m_rtvHandle = {};
};





// DirectX11 render target view management class
class SnctDX11DSV : public ISnctDXDSV
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	~SnctDX11DSV() { m_pDSV.Reset(); }

	const SnctDX11DSV* Get() override final { return this; }
	ID3D11DepthStencilView* GetDSV() { return m_pDSV.Get(); }
	ID3D11DepthStencilView** GetDSVAddress() { return m_pDSV.GetAddressOf(); }

	void SetDSV(ID3D11DepthStencilView* ppDSV) { m_pDSV = ppDSV; }

private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D11DepthStencilView> m_pDSV;
};


// DirectX12 render target view management class
class SnctDX12DSV : public ISnctDXDSV
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	// Getter
	const SnctDX12DSV* Get() override final { return this; }
	const D3D12_CPU_DESCRIPTOR_HANDLE GetHandle() { return m_DSVHandle; }
	const D3D12_CPU_DESCRIPTOR_HANDLE* GetpHandle() { return &m_DSVHandle; }

	// Setter
	void SetHandle(D3D12_CPU_DESCRIPTOR_HANDLE handle) { m_DSVHandle = handle; }

	// Method
	void IncrementHandlePointer(unsigned int size) { m_DSVHandle.ptr += size; }

private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	D3D12_CPU_DESCRIPTOR_HANDLE m_DSVHandle = {};
};
#pragma endregion


#pragma region Buffer
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
	~SnctDX11Buffer() { m_pBuffer.Reset(); }
	// Getter
	const SnctDX11Buffer* Get() override final { return this; }
	ID3D11Buffer* GetBuffer() { return m_pBuffer.Get(); }
	ID3D11Buffer** GetBufferAddress() { return m_pBuffer.GetAddressOf(); }

	// Setter
	void SetBufferAddress(ID3D11Buffer* pBuffer) { m_pBuffer = pBuffer; }

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
	~SnctDX12Buffer() { m_pBuffer.Reset(); }

	// Getter
	const SnctDX12Buffer* Get() override final { return this; }
	ID3D12Resource* GetBuffer() { return m_pBuffer.Get(); }
	ID3D12Resource** GetBufferAddress() { return m_pBuffer.GetAddressOf(); }

	// Setter
	void SetBufferAddress(ID3D12Resource* pBuffer) { m_pBuffer = pBuffer; }

private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D12Resource> m_pBuffer;
};
#pragma endregion


#pragma region Texture
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
	~SnctDX11Texture() { m_pTexture.Reset(); }

	// Getter
	const SnctDX11Texture* Get() override final { return this; }
	ID3D11Texture2D* GetTexture() { return m_pTexture.Get(); }
	ID3D11Texture2D** GetTextureAddress() { return m_pTexture.GetAddressOf(); }

	// Setter
	void SetTextureAddress(ID3D11Texture2D* pTex) { m_pTexture = pTex; }

	// Process
	HRESULT Create(ID3D11Device* device, SNCT_TEXTURE2D_DESC desc)
	{
		D3D11_TEXTURE2D_DESC* tempDesc = reinterpret_cast<D3D11_TEXTURE2D_DESC*>(&desc);
		return device->CreateTexture2D(tempDesc, nullptr, m_pTexture.GetAddressOf());
	}

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
	~SnctDX12Texture() { m_pResouce.Reset(); }

	// Getter
	const SnctDX12Texture* Get() override final { return this; }
	ID3D12Resource* GetTexture() { return m_pResouce.Get(); }
	ID3D12Resource** GetTextureAddress() { return m_pResouce.GetAddressOf(); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle() { return m_CPUHandle; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() { return m_GPUHandle; }

	// Setter
	void SetTextureAddress(ID3D12Resource* pTex) { m_pResouce = pTex; }

private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D12Resource> m_pResouce;
	D3D12_CPU_DESCRIPTOR_HANDLE m_CPUHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_GPUHandle;
};
#pragma endregion


#pragma region Shader
class SnctDXShader : public ISnctDXResource
{
public:
protected:
	ComPtr<ID3DBlob> m_pShaderBlob;
};


class SnctDX11Shader : public SnctDXShader
{
	virtual HRESULT Create(const void* pShaderBytecode, SIZE_T BytecodeLength) = 0;
};


class SnctDX12Shader : public SnctDXShader
{
public:
	void Set(const void* pByteCode, SIZE_T size)
	{
		m_shaderData.pShaderBytecode = pByteCode;
		m_shaderData.BytecodeLength = size;
	}

	const D3D12_SHADER_BYTECODE GetShaderDesc() { return m_shaderData; }

protected:
	D3D12_SHADER_BYTECODE m_shaderData;
};
#pragma endregion