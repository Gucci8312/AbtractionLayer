#pragma once
#include <iostream>
#include <cstdint>
#include <memory>
#include <thread>
#include <Windows.h>

constexpr const char*	g_className	= "AbtractionLayer";

// Classes that manage applications
class SnctApplication
{
public:
	SnctApplication(uint32_t Width, uint32_t Height,const char* WindowName);
	~SnctApplication();

	// Method
	bool Initialize();
	void Finalize();
	bool InitWnd();
	void Run();
	void MainLoop();

private:
	// Member
	HINSTANCE   m_hInst = nullptr;
	HWND        m_hwnd = nullptr;
	LONG		m_width;
	LONG		m_height;
	const char* m_windowName;
};

