#pragma once
#include "../Interface/ISnct_DXDevice.h"
#include "Snct_DX11Context.h"
// A class that manages DirectX11 device 

class SnctDX11Device : public ISnctDXDevice
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	// Getter
	SnctDX11Device* Get() { return this; }
	ID3D11Device* GetDevice() { return m_pDevice.Get(); }
	ID3D11DeviceContext* GetDeviceContext() { return m_pDeviceContext.Get(); }

	// Create
	HRESULT CreateDevice(D3D_FEATURE_LEVEL Level)override final;
	HRESULT CreateRTV(ISnctDXBuffer* pBuckBuffer, ISnctDXRTV* pRTV) override final;
	HRESULT CreateDSV(ISnctDXBuffer* buffer, ISnctDXDSV* pDSVHandle) override final;
	HRESULT CreateCmdList(ISnctDXContext** pCmdList)override final;

	// Device
	void ExecuteCmdList(ID3D11CommandList* cmdList);


	// Context command
	void SetViewPort(float Width, float Height, float MinDepth, float MaxDepth);
	void ClearRTV(ISnctDXRTV* pRenderTargetView, float clearColor[4], UINT NumRects = 1, RECT* pRects = 0);
	void ClearDSV(ISnctDXDSV* pDepthStencilView, UINT Flag, float Depth,
		UINT8 Stencil, UINT NumRects = 0, RECT* pRects = 0);
	void SetRTV(UINT viewNum, ISnctDXRTV* pRTV, ISnctDXDSV* pDSV = nullptr,
		bool SingleHandleToDescriptorRange = 0);
	void SetRasterizerState(ID3D11RasterizerState* rasterizerState);
private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D11Device> m_pDevice;
	ComPtr<ID3D11DeviceContext> m_pDeviceContext;
};

