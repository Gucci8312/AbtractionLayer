#pragma once
#include "Snct_DXRender.h"

// Classes that manage DirectX12
class SnctDX12Render : public SnctDXRender
{
public:
	// Method
	void Build();
	void Begin();
	void End();
	void Draw();

private:
};

