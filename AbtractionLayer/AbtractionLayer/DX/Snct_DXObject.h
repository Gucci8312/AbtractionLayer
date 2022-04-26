#pragma once
#include "../Snct_Utility.h"
#include "Snct_DXResource.h"
#include "Interface/ISnct_DXDevice.h"
#include <memory>

class SnctDXObject
{
public:
	virtual ~SnctDXObject() {};
	ISnctDXResource* GetVertexBuffer() { return m_pVertexBuffer.get(); }

	virtual HRESULT Init(ISnctDxDevice* pDevice, Vertices* pVertices, Indices* pIndices) = 0;
	virtual void Draw() = 0;


protected:
	unsigned int						m_vertexSize;
	unsigned int						m_indexSize;
	unsigned int						m_descSize;

	std::unique_ptr<ISnctDXResource>	m_pVertexBuffer;
	std::unique_ptr<ISnctDXResource>	mpIndexBuffer;
};

