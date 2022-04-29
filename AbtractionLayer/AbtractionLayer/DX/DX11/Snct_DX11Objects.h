#pragma once
#include "Snct_DX11.h"
#include "../../Snct_Utility.h"
#include "../../Snct_Windows.h"
#include "../Snct_DXResource.h"

struct SnctDX11ObjectBuffer
{
	UINT					nVertexSize;
	UINT					nIndexSize;
	UINT					nDescSize;

	SnctDX11Buffer	pVertexBuffer;
	SnctDX11Buffer	pIndexBuffer;
	SnctDX11Buffer	pConstantObject;

	//std::vector<ComPtr<ID3D11Buffer>>			pConstantObject;
};

class SnctDX11Objects
{
public:
	void AddSceneObject(ID3D11Device* device, HashKey key, Vertices* vertices, Indices* indices);
	void SubSceneObject(HashKey key);

	SnctDX11ObjectBuffer* GetObjectBuffer(HashKey key);

private:
	std::unordered_map<HashKey, SnctDX11ObjectBuffer> objectBufferMap{};
};