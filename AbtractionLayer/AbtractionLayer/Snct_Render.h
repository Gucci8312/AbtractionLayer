#pragma once
#include "Snct_Utility.h"
#include "Snct_Windows.h"

enum class SNCT_DRAW_FLAG {
	SNCT_DRAW_FLAG_IMMADIATE,
	SNCT_DRAW_FLAG_DEFERRED,
	SNCT_DRAW_FLAG_MULTITHREAD
};

class ISnctRender
{
public:
	virtual void Build(HWND hWnd) = 0;
	virtual void RenderBegin() = 0;
	virtual void RenderEnd() = 0;

	virtual void Draw(HashKey key, SNCT_DRAW_FLAG drawFlag) = 0;

	/// <summary>
	/// create object vertex & index buffer 
	/// </summary>
	virtual void CreateObject(HashKey key, Vertices* pVertices, Indices* pIndices) = 0;

	virtual void SetObject(Transform* pTransform) = 0;
	virtual void SetCamera(Camera* pCamera) = 0;

protected:

private:

};