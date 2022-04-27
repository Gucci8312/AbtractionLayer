#pragma once
#include "Snct_DXResource.h"
#include "Snct_DXConstantParameter.h"
#include "../Snct_DXShaderLibrary.h"
#include "Interface/ISnct_DXDevice.h"
#include "Snct_DX12Context.h"
#include <memory>

class SnctDXObject
{
public:
	virtual void Create(ISnctDxDevice* pDevice, HashKey key, Vertices* pVertices, Indices* pIndices) = 0;
	virtual void Update() = 0;
	virtual void Draw(int nowFrame,ISnctDXContext* pCmdList, XMConstantObject* pObjectData) = 0;

protected:
	std::unique_ptr<SnctShaderLibrary> m_pShaderLibrary;
	unsigned int					m_vertexSize;
	unsigned int					m_indexSize;
	unsigned int					m_descSize;

};

