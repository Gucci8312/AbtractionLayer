#pragma once
#include "Snct_Utility.h"

class ISnctObject {

public:
	ISnctObject() {}
	virtual ~ISnctObject() {}

	virtual void Initialize()	= 0;
	virtual void Update()		= 0;
	virtual void Draw()			= 0;
	virtual void Finalize()		= 0;

protected:
	Transform	m_Transform;
	Vertices	m_Vertices;
	Indices		m_Indices;

private:

};