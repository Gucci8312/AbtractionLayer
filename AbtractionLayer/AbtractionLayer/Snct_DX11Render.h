#pragma once
#include "Snct_DX11.h"
#include "Snct_DXRender.h"

class SnctDX11Render : public SnctDXRender
{

public:
	// render override
	void Build()	override;
	void Begin()	override;
	void End()		override;

	void Draw(HashKey key, SNCT_DRAW_FLAG drawFlag) override;

	void CreateObject(HashKey key, Vertices* pVertices, Indices* pIndices) override;
protected:

private:

};