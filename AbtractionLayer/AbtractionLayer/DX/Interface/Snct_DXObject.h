#pragma once
#include <memory>
#include "../Snct_DXResource.h"
#include "../../Snct_Math.h"
#include "../../Snct_Utility.h"

class SnctDXObject
{
public:
	virtual void Draw() = 0;

private:
	unsigned int					m_vertexSize;
	unsigned int					m_indexSize;
	unsigned int					m_descSize;

	std::unique_ptr<ISnctDXBuffer> m_vetexBuffer;
	std::unique_ptr<ISnctDXBuffer> m_indexBuffer;
};

