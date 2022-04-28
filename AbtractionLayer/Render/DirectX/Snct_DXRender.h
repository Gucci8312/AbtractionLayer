#pragma once
#include "../ISnct_Render.h"
#include "Snct_DXResource.h"
#include "Interface/ISnct_DXDevice.h"
#include "Interface/ISnct_DXContext.h"

class SnctDXRender : public ISnctRender
{
public:
	~SnctDXRender() override {}
	virtual ISnctDXDevice* GetDevice() = 0;

	virtual bool Build(HWND hWnd) = 0;
	virtual void BeforeRender(ISnctDXContext* pCmdList) = 0;
	virtual void AfterRender(ISnctDXContext* pCmdList) = 0;
	virtual HRESULT CreateCommandList(ISnctDXContext** pCmdList) = 0;
};