#pragma once
#include "Snct_Math.h"

class ISnctRender
{

public:
	virtual void Build()	= 0;
	virtual void Begin()	= 0;
	virtual void End()		= 0;

	virtual void CreateObject() = 0;
	virtual void Draw()			= 0;

	virtual void SetObject()	= 0;
	virtual void SetCamera()	= 0;

protected:


private:


};