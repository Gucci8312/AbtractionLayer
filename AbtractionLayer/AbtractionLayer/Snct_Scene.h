#pragma once
#include "Snct_Utility.h"

class ISnctRender;

class ISnctScene {

public:
	ISnctScene(ISnctRender* pRender) : m_pRender(pRender) {}

	virtual void Initialize();
	virtual void Update();
	virtual void Draw();
	virtual void Finalize();

protected:
	ISnctRender* m_pRender;

private:

};