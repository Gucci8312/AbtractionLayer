#pragma once
// �g�p�p�x��������ˑ��w�b�_�[���܂Ƃ߂�
// Summarize frequently used independent headers

#include <list>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

#include "Snct_Math.h"

using HashKey = std::string;

constexpr int g_screenWidth  = 1200;
constexpr int g_screenHeight = 800;

inline void SnctRuntimeError(std::runtime_error& e) 
{
	_ASSERT(e.what());
}