#pragma once
#include "Snct_DX11.h"
#include "../Snct_DXResource.h"
#include "../Snct_DXRender.h"
#include "Snct_DX11Device.h"
#include "Snct_DX11CmdList.h"

class SnctDX11Render : public SnctDXRender
{
public:
	SnctDX11Render() {}
	~SnctDX11Render() {}
	// render override
	void Build(HWND hWnd)	override;
	void RenderBegin()		override;
	void RenderEnd()		override;

	void Draw(HashKey key, SNCT_DRAW_FLAG drawFlag) override;
	void CreateObject(HashKey key, Vertices* pVertices, Indices* pIndices) override;
protected:

private:
	ComPtr<IDXGISwapChain>			m_pSwapChain;

	SnctDX11Device					m_pDevice;
	SnctDX11RTV						m_pBackBufferView;
	ComPtr<ID3D11DepthStencilState>	m_pDepthState;
	//ComPtr<ID3D11DepthStencilView>	m_pDepthStencileView;
	SnctDX11DSV						m_pDepthStencileView;

	D3D11_VIEWPORT					m_viewport{};

	SnctDX11CmdList					m_pDeferredContext;
	ComPtr<ID3D11CommandList>		m_pCommandList;


	//std::unique_ptr<Snct_DXShaderLibrary>	m_pShaderLibrary;
	//std::unique_ptr<SnctCommandList>	m_pCommandList;
	//std::unique_ptr<SnctResorceLibrary>	m_pResourceLibrary;



};