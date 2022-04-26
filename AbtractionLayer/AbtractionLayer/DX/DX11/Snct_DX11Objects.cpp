#include "Snct_DX11Objects.h"
#include "../Snct_DXConstantParameter.h"

void SnctDX11Objects::AddSceneObject(ID3D11Device* device, HashKey key, Vertices* vertices, Indices* indices)
{
	// create vertex & index map
	try
	{
		SnctDX11ObjectBuffer object{};
		objectBufferMap.emplace(key, object);
		auto itr = &objectBufferMap.at(key);

		// vertex
		{
			itr->nVertexSize = (UINT)vertices->size();

			D3D11_BUFFER_DESC descVBuffer{};
			descVBuffer.ByteWidth = (UINT)sizeof(Vertex) * (UINT)vertices->size();
			descVBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			descVBuffer.Usage = D3D11_USAGE_DEFAULT;
			descVBuffer.CPUAccessFlags = 0;
			descVBuffer.MiscFlags = 0;
			descVBuffer.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA dataVBuffer{};
			dataVBuffer.pSysMem = vertices->data();
			dataVBuffer.SysMemPitch = 0;
			dataVBuffer.SysMemSlicePitch = 0;

			if (FAILED(device->CreateBuffer(&descVBuffer, &dataVBuffer, itr->pVertexBuffer.GetAddressOf())))
				throw  std::runtime_error("!Failed to Create Vertex Buffer");
		}

		// index
		{
			itr->nIndexSize = (UINT)indices->size();

			D3D11_BUFFER_DESC descIndex{};
			descIndex.ByteWidth = (UINT)sizeof(UINT) * (UINT)indices->size();
			descIndex.BindFlags = D3D11_BIND_INDEX_BUFFER;
			descIndex.Usage = D3D11_USAGE_DEFAULT;
			descIndex.CPUAccessFlags = 0;
			descIndex.MiscFlags = 0;
			descIndex.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA dataIndex{};
			dataIndex.pSysMem = indices->data();
			dataIndex.SysMemPitch = 0;
			dataIndex.SysMemSlicePitch = 0;

			if (FAILED(device->CreateBuffer(&descIndex, &dataIndex, itr->pIndexBuffer.GetAddressOf())))
				throw std::runtime_error("!Failed to Create Index Buffer");
		}

		// constant
		{
			D3D11_BUFFER_DESC descConstantBuffer{};

			descConstantBuffer.ByteWidth = (UINT)sizeof(XMConstantObject);
			descConstantBuffer.Usage = D3D11_USAGE_DEFAULT;
			descConstantBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			descConstantBuffer.CPUAccessFlags = 0;
			descConstantBuffer.MiscFlags = 0;
			descConstantBuffer.StructureByteStride = 0;

			if (FAILED(device->CreateBuffer(
				&descConstantBuffer,
				nullptr,
				itr->pConstantObject.GetAddressOf()
			)))
				throw std::runtime_error("!Failed to Create Constant Matrix Buffer");
		}

	}
	catch (std::runtime_error& e)
	{
		SnctRuntimeError(e);
	}
}

void SnctDX11Objects::SubSceneObject(HashKey key)
{
	objectBufferMap.erase(key);
}

SnctDX11ObjectBuffer* SnctDX11Objects::GetObjectBuffer(HashKey key)
{
	return &objectBufferMap.at(key);
}
