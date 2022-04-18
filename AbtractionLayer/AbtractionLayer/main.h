#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "Snct_Application.h"

constexpr const char*	g_name			= "APIPractice";
constexpr const char*	g_title			= "Renderer";
constexpr int			g_screenWidth	= 1200;
constexpr int			g_screenHeight	= 800;
