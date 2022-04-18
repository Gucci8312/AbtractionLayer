#pragma once
#include <iostream>
#include <cstdint>
#include <memory>
#include <thread>
#include <Windows.h>

constexpr const char*		g_className	= "AbtractionLayer";

// Clases that manage applications
class Snct_Application
{
private:
	HINSTANCE   m_hInst = nullptr;
	HWND        m_hwnd	= nullptr;
	LONG		m_width;
	LONG		m_height;
	const char* m_windowName;

public:
	Snct_Application(uint32_t Width, uint32_t Height,const char* WindowName);
	~Snct_Application();

	bool Initialize();
	void Finalize();
	bool InitWnd();
	void Run();
	void MainLoop();
};

