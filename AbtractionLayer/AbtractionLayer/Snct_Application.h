// Application Manegiment file...
#pragma once
#include "Snct_Windows.h"

// Classes that manage applications
class SnctApplication
{
public:
	SnctApplication();
	~SnctApplication();

	// Method
	bool Initialize();
	void Finalize();
	bool InitWnd();
	void Run();
	void MainLoop();

private:
	// Member
	const char* m_windowName;
	const char* g_className	= "AbtractionLayer";
	HINSTANCE   m_hInst		= nullptr;
	HWND        m_hwnd		= nullptr;
	LONG		m_width;
	LONG		m_height;
};

