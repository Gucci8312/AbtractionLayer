#include "DX/Snct_DXConstantParameter.h"
#include "Snct_DX12Objects.h"

//------------------------------------------------------------------------------
/// Create and add object primitive data.
/// \param			device
/// \param			hash key
/// \param			vertcies
/// \param			indices
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12Objects::AddSceneObject(ID3D12Device* device, HashKey key, Vertices* vertices, Indices* indices)
{
	// create vertex & index map
	try
	{
		D3D12_HEAP_PROPERTIES heapProperties{};
		heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProperties.CreationNodeMask = 1;
		heapProperties.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC descResource{};
		descResource.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		descResource.Alignment = 0;
		descResource.Height = 1;
		descResource.DepthOrArraySize = 1;
		descResource.MipLevels = 1;
		descResource.Format = DXGI_FORMAT_UNKNOWN;
		descResource.SampleDesc = { 1, 0 };
		descResource.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		descResource.Flags = D3D12_RESOURCE_FLAG_NONE;

		SnctDX12ObjectBuffer object{};

		if (vertices->size() <= 0) {
			
		}

		object.nVertexSize = (UINT)vertices->size();
		object.nIndexSize = (UINT)indices->size();

		D3D12_RANGE readRange{ 0,0 };

		// vertex
		{
			descResource.Width = (UINT)sizeof(Vertex) * vertices->size();

			if (FAILED(device->CreateCommittedResource(
				&heapProperties,
				D3D12_HEAP_FLAG_NONE,
				&descResource,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(object.pVertexBuffer.GetAddressOf())
			)))
				throw "!Failed to crate vertex buffer";

			UINT8* pVertexDataBegin;

			if (FAILED(object.pVertexBuffer->Map(
				0,
				&readRange,
				reinterpret_cast<void**>(&pVertexDataBegin)
			)))
				throw "!Failed to copy vertex";

			memcpy(pVertexDataBegin, vertices->data(), sizeof(Vertex) * vertices->size());
			object.pVertexBuffer->Unmap(0, nullptr);
		}

		// index
		{
			descResource.Width = (UINT)sizeof(UINT) * indices->size();

			if (FAILED(device->CreateCommittedResource(
				&heapProperties,
				D3D12_HEAP_FLAG_NONE,
				&descResource,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(object.pIndexBuffer.GetAddressOf())
			)))
				throw std::runtime_error("!Failed to crate index buffer");

			UINT8* pIndexDataBegin;

			if (FAILED(object.pIndexBuffer->Map(
				0,
				&readRange,
				reinterpret_cast<void**>(&pIndexDataBegin)
			)))
				throw std::runtime_error("!Failed to copy index");

			memcpy(pIndexDataBegin, indices->data(), sizeof(UINT) * indices->size());
			object.pIndexBuffer->Unmap(0, nullptr);
		}

		// constnt 
		{
			object.pConstantObject.resize(2);
			object.objectCBV.resize(2);

			D3D12_HEAP_PROPERTIES heapProperties{};
			heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
			heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			heapProperties.CreationNodeMask = 1;
			heapProperties.VisibleNodeMask = 1;

			D3D12_RESOURCE_DESC descResource{};
			descResource.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			descResource.Alignment = 0;
			descResource.Width = (UINT)((sizeof(XMConstantObject) + 0xff) & ~0xff);
			descResource.Height = 1;
			descResource.DepthOrArraySize = 1;
			descResource.MipLevels = 1;
			descResource.Format = DXGI_FORMAT_UNKNOWN;
			descResource.SampleDesc = { 1, 0 };
			descResource.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			descResource.Flags = D3D12_RESOURCE_FLAG_NONE;

			D3D12_DESCRIPTOR_HEAP_DESC	descDescriptorHeap{};
			descDescriptorHeap.NumDescriptors = 2;
			descDescriptorHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			descDescriptorHeap.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			descDescriptorHeap.NodeMask = 0;

			device->CreateDescriptorHeap(&descDescriptorHeap, IID_PPV_ARGS(object.pObjectHeap.GetAddressOf()));
			object.nDescSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			for (UINT i = 0; i < 2; ++i)
			{
				if (FAILED(device->CreateCommittedResource(
					&heapProperties,
					D3D12_HEAP_FLAG_NONE,
					&descResource,
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(object.pConstantObject[i].GetAddressOf())
				)))
					throw std::runtime_error("!Failed to create constant buffer");


				D3D12_CONSTANT_BUFFER_VIEW_DESC descConstant{};
				D3D12_CPU_DESCRIPTOR_HANDLE		cbvCpuHandle{};

				descConstant.BufferLocation = object.pConstantObject[i]->GetGPUVirtualAddress();
				descConstant.SizeInBytes = (UINT)((sizeof(XMConstantObject) + 0xff) & ~0xff);
				cbvCpuHandle.ptr = object.pObjectHeap->GetCPUDescriptorHandleForHeapStart().ptr + static_cast<unsigned long long>(i) * object.nDescSize;

				device->CreateConstantBufferView(&descConstant, cbvCpuHandle);

				D3D12_GPU_DESCRIPTOR_HANDLE cbvGpuHandle{};
				cbvGpuHandle.ptr = object.pObjectHeap->GetGPUDescriptorHandleForHeapStart().ptr + static_cast<unsigned long long>(i) * object.nDescSize;
				object.objectCBV[i] = cbvGpuHandle;
			}
		}

		objectBufferMap.emplace(key, object);
	}
	catch (std::runtime_error& e)
	{
		SnctRuntimeError(e);
	}
}

//------------------------------------------------------------------------------
/// delete object data.
/// \param			hash key
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12Objects::SubSceneObject(HashKey key)
{
	objectBufferMap.erase(key);
}

//------------------------------------------------------------------------------
/// return Object data
/// \param			hash key
/// \return			object data
//------------------------------------------------------------------------------
SnctDX12ObjectBuffer* SnctDX12Objects::GetObjectBuffer(HashKey key)
{
	return &objectBufferMap.at(key);
}
