#include "Snct_Application.h"

#include "Snct_Scene01_Impl.h"

//------------------------------------------------------------------------------
/// Window Procedure
/// \param[in]      Window handle
/// \param[in]      Message
/// \param[in]      parameter
/// \param[in]      parameter
//------------------------------------------------------------------------------
LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	break;

	default:
	{
		// Nothing //
	}
	break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//------------------------------------------------------------------------------
/// Contructor
/// \param		none
//------------------------------------------------------------------------------
SnctApplication::SnctApplication() :m_width(g_screenWidth), m_height(g_screenHeight), m_windowName("App")
{
	// Nothing //
}


//------------------------------------------------------------------------------
/// Destructor
//------------------------------------------------------------------------------
SnctApplication::~SnctApplication()
{
	// Nothing //
}


//------------------------------------------------------------------------------
/// Initialization process
/// \param		none
/// \return		none
//------------------------------------------------------------------------------
bool SnctApplication::Initialize()
{
	if (!InitWnd()) return false;

	// Console window destroy
	FreeConsole();

	m_pRender = std::make_unique<SnctDX12Render>();
	m_pScene = std::make_unique<SnctScene01>();

	if (!m_pRender->Build(m_hWnd)) return false;
	m_deferredContext = new SnctDX12Context;
	m_pRender->CreateCommandList(&m_deferredContext);

	m_pScene->SetRender(m_pRender.get());
	m_pScene->Initialize();

	return true;
}


//------------------------------------------------------------------------------
/// Application end processing
/// param		none
/// \return		none
//------------------------------------------------------------------------------
void SnctApplication::Finalize()
{
	m_pRender.reset();
	delete m_deferredContext;
	m_deferredContext = nullptr;
}


//------------------------------------------------------------------------------
/// Window	initlize
/// \param		none
/// \return		none
//------------------------------------------------------------------------------
bool SnctApplication::InitWnd()
{
	// Get instance handle
	auto hInst = GetModuleHandle(nullptr);
	if (hInst == nullptr)
	{
		return false;
	}

	// Window class param
	WNDCLASSEX		wcex;
	wcex.hInstance = m_hInst;
	wcex.lpszClassName = g_className;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.style = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hIcon = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon((HINSTANCE)NULL, IDI_WINLOGO);
	wcex.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);
	wcex.lpszMenuName = 0;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	// Rigister window
	if (!RegisterClassEx(&wcex)) return FALSE;

	// Register as a member of the class
	m_hInst = hInst;

	// Set window size
	RECT rc = {};
	rc.right = static_cast<LONG>(m_width);
	rc.bottom = static_cast<LONG>(m_height);

	// Adjust window size
	auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	AdjustWindowRect(&rc, style, FALSE);

	// Create window
	m_hWnd = CreateWindow(g_className, m_windowName, WS_CAPTION | WS_SYSMENU, 0,
		0, rc.right - rc.left, rc.bottom - rc.top, HWND_DESKTOP,
		(HMENU)NULL, m_hInst, (LPVOID)NULL);

	// Check window handle
	if (!m_hWnd) return false;

	// Update window
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);

	// Set window focus
	SetFocus(m_hWnd);

	// Success
	return true;
}


//------------------------------------------------------------------------------
/// Application running
/// \param		none
/// \return		none
//------------------------------------------------------------------------------
void SnctApplication::Run()
{
	if (!Initialize())	return;

	MainLoop();

	Finalize();
}


//------------------------------------------------------------------------------
/// Main loop
/// \param		none
/// \return		none
//------------------------------------------------------------------------------
void SnctApplication::MainLoop()
{
	// To receive a message param
	MSG msg = {};

	// Message loop
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// Process
			m_pRender->BeforeRender(m_deferredContext);

			m_pScene->Draw(m_deferredContext);

			m_pRender->AfterRender(m_deferredContext);
		}
	}
}
