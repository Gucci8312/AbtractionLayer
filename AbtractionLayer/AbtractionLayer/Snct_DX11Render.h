#pragma once
#include "Snct_DX11.h"
#include "Snct_DXRender.h"

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
	ComPtr<ID3D11Device> pDevice;

private:

};