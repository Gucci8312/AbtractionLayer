// Application Manegiment file...
#pragma once
#include <Windows.h>
#include <cstdlib>
#include <crtdbg.h>

#define _CRTDBG_MAP_ALLOC

#include "Snct_Utility.h"
#include "Snct_Scene.h"
#include "Render/DirectX/Snct_DXRender.h"
#include "Render/DirectX/Interface/ISnct_DXContext.h"
#include "Render/DirectX/DirectX11/Snct_DX11Render.h"
#include "Render/DirectX/DirectX12/Snct_DX12Render.h"

// Classes that manage applications
class SnctApplication
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	SnctApplication();
	~SnctApplication();

	// Method
	bool Initialize();
	void Finalize();
	bool InitWnd();
	void Run();
	void MainLoop();

private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------
	const char* m_windowName;
	const char* g_className = "AbtractionLayer";
	HINSTANCE   m_hInst = nullptr;
	HWND        m_hWnd = nullptr;
	LONG		m_width;
	LONG		m_height;

	std::unique_ptr<SnctDXRender>	m_pRender = nullptr;
	std::unique_ptr<ISnctScene>		m_pScene = nullptr;
	ISnctDXContext*					m_deferredContext=nullptr;
};

