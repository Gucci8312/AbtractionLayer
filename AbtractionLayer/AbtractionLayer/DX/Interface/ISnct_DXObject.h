#pragma once
#include "../Snct_DXResource.h"
#include "../../Snct_Math.h"
#include "../../Snct_Utility.h"

class SnctDXObject
{
public:
	//void AddSceneObject(ISnctDxDevice* device, HashKey key, Vertices* vertices, Indices* indices);
	//void SubSceneObject(HashKey key);

	//ISnctDXObject* GetObjectBuffer(HashKey key);
private:
	unsigned int					m_vertexSize;
	unsigned int					m_indexSize;
	unsigned int					m_descSize;
	
	ISnctDXBuffer*				m_vetexBuffer;
	ISnctDXBuffer*				m_indexBuffer;
	std::unordered_map<HashKey, ISnctDXBuffer*> objectBufferMap{};

};

