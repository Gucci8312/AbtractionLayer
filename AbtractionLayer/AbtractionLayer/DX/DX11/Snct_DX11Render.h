#pragma once
#include "Snct_DX11.h"
#include "../Snct_DXRender.h"

class SnctDX11Render : public SnctDXRender
{

public:
	// render override
	void Build(HWND* hWnd)	override;
	void RenderBegin()		override;
	void RenderEnd()		override;

	void Draw(HashKey key, SNCT_DRAW_FLAG drawFlag) override;

	void CreateObject(HashKey key, Vertices* pVertices, Indices* pIndices) override;
protected:

	ComPtr<IDXGISwapChain>			m_pSwapChain;

	ComPtr<ID3D11Device>			m_pDevice;
	ComPtr<ID3D11DeviceContext>		m_pDeviceContext;
	ComPtr<ID3D11RenderTargetView>	m_pBackBufferView;
	ComPtr<ID3D11DepthStencilState>	m_pDepthState;
	ComPtr<ID3D11DepthStencilView>	m_pDepthStencileView;

	D3D11_VIEWPORT					m_viewport{};

	ComPtr<ID3D11DeviceContext>		m_pDeferredContext;
	ComPtr<ID3D11CommandList>		m_pCommandList;


	std::unique_ptr<SnctShaderLibrary>	m_pShaderLibrary;
	//std::unique_ptr<SnctCommandList>	m_pCommandList;
	//std::unique_ptr<SnctResorceLibrary>	m_pResourceLibrary;


private:

};