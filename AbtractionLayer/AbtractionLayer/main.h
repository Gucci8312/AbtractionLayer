#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "Snct_Application.h"

constexpr const char*	NAME			= "APIPractice";
constexpr const char*	TITLE			= "Renderer";
constexpr int			TIMER_ID		= 1;
constexpr float			FRAME_RATE		= (1000 / 60);
constexpr int			SCREEN_WIDTH	= 1200;
constexpr int			SCREEN_HEIGHT	= 800;
