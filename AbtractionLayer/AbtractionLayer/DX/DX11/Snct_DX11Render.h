#pragma once
#include "../Snct_DXResource.h"
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

	void DrawIndexed(SnctDX11ObjectBuffer* pObject);
	void UpdateObjectBuffer(ID3D11Buffer* pObjectConstant);
	void UpdateCameraBuffer(ID3D11Buffer* pCameraConstant);

	// << TEST CODE >>
	ComPtr<ID3D11VertexShader>		TEST_CODE_m_pVertexShader;
	ComPtr<ID3D11PixelShader>		TEST_CODE_m_pPixelShader;

	ComPtr<ID3D11Buffer>			TEST_CODE_m_pCameraConstant;
	ComPtr<ID3D11InputLayout>		TEST_CODE_m_pInputLayout;

	void TEST_CODE_CreateCameraConstantBuffer();
	void TEST_CODE_CreateVSAndPS();
};