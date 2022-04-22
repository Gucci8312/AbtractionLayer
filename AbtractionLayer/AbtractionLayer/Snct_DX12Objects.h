#pragma once
#include "DX/DX12/Snct_DX12.h"
#include "Snct_Utility.h"
#include "Snct_Windows.h"

struct SnctDX12ObjectBuffer
{
	UINT					nVertexSize;
	UINT					nIndexSize;
	UINT					nDescSize;

	ComPtr<ID3D12Resource>						pVertexBuffer;
	ComPtr<ID3D12Resource>						pIndexBuffer;
	ComPtr<ID3D12DescriptorHeap>				pObjectHeap;
	std::vector<ComPtr<ID3D12Resource>>			pConstantObject;
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>	objectCBV;
};

class SnctDX12Objects
{
public:
	void AddSceneObject(ID3D12Device* device, HashKey key, Vertices* vertices, Indices* indices);
	void SubSceneObject(HashKey key);

	SnctDX12ObjectBuffer* GetObjectBuffer(HashKey key);

private:
	std::unordered_map<HashKey, SnctDX12ObjectBuffer> objectBufferMap{};
};