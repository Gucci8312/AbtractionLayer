#pragma once

#include <list>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <assert.h>
#include <stdexcept>
#include <unordered_map>
#include "Snct_Math.h"

constexpr int g_screenWidth = 1200;
constexpr int g_screenHeight = 800;

using HashKey = std::string;

inline void SnctRuntimeError(std::runtime_error& e)
{
	_RPT0(_CRT_ASSERT, e.what());
}