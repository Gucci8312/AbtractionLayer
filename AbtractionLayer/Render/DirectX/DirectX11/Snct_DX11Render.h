#pragma once
#include "../Snct_DXRender.h"
#include "Snct_Dx11Context.h"
#include "Snct_Dx11Device.h"

class SnctDX11Render : public SnctDXRender
{
public:
	SnctDX11Render() {}
	~SnctDX11Render() {}

	inline ISnctDXDevice* GetDevice() override final
	{
		return &m_device;
	}


	// render override
	bool Build(HWND hWnd)	override final;
	void BeforeRender(ISnctDXContext* pCmdList)		override final;
	void AfterRender(ISnctDXContext* pCmdList)		override final;
	HRESULT CreateCommandList(ISnctDXContext** pCmdList)override final;

private:
	ComPtr<IDXGISwapChain>			m_swapChain;

	SnctDX11Device					m_device;
	SnctDX11RTV						m_backBufferView;
	ComPtr<ID3D11DepthStencilState>	m_pDepthState;
	SnctDX11DSV						m_depthStencileView;
	ComPtr<ID3D11RasterizerState> rasterizerState;

	//SnctDX11Context					m_deferredContext;
};

