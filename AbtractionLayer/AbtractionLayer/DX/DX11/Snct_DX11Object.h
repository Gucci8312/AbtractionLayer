#pragma once
#include "../Snct_DXObject.h"
#include "Snct_DX11Device.h"
class SnctDX11Object :public SnctDXObject
{
public:
	void Create(ISnctDxDevice* pDevice, HashKey key, Vertices* pVertices, Indices* pIndices);
	void Draw(int nowFrame, ISnctDXContext* pCmdList, XMConstantObject* pObjectData);
	void Update();
private:

};

