#pragma once
#include "Snct_Render.h"
#include "Snct_Utility.h"

class ISnctObject {

public:
	void SetRender(ISnctRender* pRender) { m_pRender = pRender; }
	
	ISnctObject() {}
	virtual ~ISnctObject() {}
	
	virtual void Initialize()	= 0;
	virtual void Update()		= 0;
	virtual void Draw()			= 0;
	virtual void Finalize()		= 0;

protected:
	std::string m_name;
	Transform	m_transform;
	Vertices	m_vertices;
	Indices		m_indices;

	ISnctRender* m_pRender = nullptr;
private:
};