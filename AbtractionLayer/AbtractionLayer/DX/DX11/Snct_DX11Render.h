#pragma once
#include "../Snct_DXRender.h"

#include "Snct_DX11.h"
#include "Snct_DX11Device.h"
#include "Snct_DX11Context.h"
#include "Snct_DX11Objects.h"

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
	SnctDX11DSV						m_pDepthStencileView;

	SnctDX11Context					m_pDeferredContext;
	ComPtr<ID3D11CommandList>		m_pCommandList;

	std::unique_ptr<SnctDX11Objects> m_pSceneObjects;

	void UpdateObjectBuffer(ISnctDXBuffer* pObjectConstant);
	void UpdateCameraBuffer(ISnctDXBuffer* pCameraConstant);

	SnctDXVertexShader				m_pVertexShader;
	SnctDXPixelShader				m_pPixelShader;

	SnctDX11Buffer					m_pCameraConstant;
	ComPtr<ID3D11InputLayout>		m_pInputLayout;
};