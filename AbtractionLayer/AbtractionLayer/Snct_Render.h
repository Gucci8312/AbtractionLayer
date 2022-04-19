#pragma once
#include "Snct_Utility.h"

enum class SNCT_DRAW_FLAG {
	SNCT_DRAW_FLAG_IMMADIATE,
	SNCT_DRAW_FLAG_DEFERRED,
	SNCT_DRAW_FLAG_MULTITHREAD
};

class ISnctRender
{
public:
	virtual void Build() = 0;
	virtual void Begin() = 0;
	virtual void End() = 0;

	virtual void CreateObject() = 0;
	virtual void Draw(std::string szKey, SNCT_DRAW_FLAG drawFlag) = 0;

	virtual void SetObject() = 0;
	virtual void SetCamera() = 0;

protected:

private:

};