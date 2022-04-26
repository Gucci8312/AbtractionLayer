#pragma once
#include "Snct_DXResource.h"
#include "Interface/ISnct_DXDevice.h"
#include <memory>

class SnctDXObject
{
public:
	ISnctDXResource* GetVertexBuffer() { return m_vertexBuffer; }
private:
	unsigned int					m_vertexSize;
	unsigned int					m_indexSize;
	unsigned int					m_descSize;

	ISnctDXResource*				m_vertexBuffer;
	ISnctDXResource*				m_indexBuffer;
};

