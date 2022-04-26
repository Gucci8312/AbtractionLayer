#include "SnctDX12_Object.h"

SnctDX12Object::~SnctDX12Object()
{
}

HRESULT SnctDX12Object::Init(ISnctDxDevice* pDevice, Vertices* pVertices, Indices* pIndices)
{
	//SnctDX12Device* tempDevice = dynamic_cast<SnctDX12Device*>(pDevice);
	//// create vertex & index map
	//try
	//{
	//	TEST_CODE_m_pConstantObject.resize(2);
	//	TEST_CODE_m_objectCBV.resize(2);

	//	D3D12_HEAP_PROPERTIES heapProperties{};
	//	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	//	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	//	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	//	heapProperties.CreationNodeMask = 1;
	//	heapProperties.VisibleNodeMask = 1;

	//	D3D12_RESOURCE_DESC descResource{};
	//	descResource.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//	descResource.Alignment = 0;
	//	descResource.Height = 1;
	//	descResource.DepthOrArraySize = 1;
	//	descResource.MipLevels = 1;
	//	descResource.Format = DXGI_FORMAT_UNKNOWN;
	//	descResource.SampleDesc = { 1, 0 };
	//	descResource.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//	descResource.Flags = D3D12_RESOURCE_FLAG_NONE;

	//	m_vertexSize = (UINT)pVertices->size();
	//	m_indexSize = (UINT)pIndices->size();

	//	D3D12_RANGE readRange{ 0,0 };

	//	// vertex
	//	{
	//		descResource.Width = (UINT)sizeof(Vertex) * pVertices->size();
	//		
	//		if (FAILED(tempDevice->GetDevice()->CreateCommittedResource(
	//			&heapProperties,
	//			D3D12_HEAP_FLAG_NONE,
	//			&descResource,
	//			D3D12_RESOURCE_STATE_GENERIC_READ,
	//			nullptr,
	//			IID_PPV_ARGS(m_pVertexBuffer.GetAddressOf())
	//		)))
	//			throw "!Failed to crate vertex buffer";

	//		UINT8* pVertexDataBegin;

	//		if (FAILED(TEST_CODE_m_pVertexBuffer->Map(
	//			0,
	//			&readRange,
	//			reinterpret_cast<void**>(&pVertexDataBegin)
	//		)))
	//			throw "!Failed to copy vertex";

	//		memcpy(pVertexDataBegin, pVertices->data(), sizeof(Vertex) * pVertices->size());
	//		TEST_CODE_m_pVertexBuffer->Unmap(0, nullptr);
	//	}

	//	// index
	//	{
	//		descResource.Width = (UINT)sizeof(UINT) * pIndices->size();

	//		if (FAILED(tempDevice->GetDevice()->CreateCommittedResource(
	//			&heapProperties,
	//			D3D12_HEAP_FLAG_NONE,
	//			&descResource,
	//			D3D12_RESOURCE_STATE_GENERIC_READ,
	//			nullptr,
	//			IID_PPV_ARGS(TEST_CODE_m_pIndexBuffer.GetAddressOf())
	//		)))
	//			throw std::runtime_error("!Failed to crate index buffer");

	//		UINT8* pIndexDataBegin;

	//		if (FAILED(TEST_CODE_m_pIndexBuffer->Map(
	//			0,
	//			&readRange,
	//			reinterpret_cast<void**>(&pIndexDataBegin)
	//		)))
	//			throw std::runtime_error("!Failed to copy index");

	//		memcpy(pIndexDataBegin, pIndices->data(), sizeof(UINT) * pIndices->size());
	//		TEST_CODE_m_pIndexBuffer->Unmap(0, nullptr);
	//	}

	//	// constnt 
	//	{
	//		D3D12_HEAP_PROPERTIES heapProperties{};
	//		heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	//		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	//		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	//		heapProperties.CreationNodeMask = 1;
	//		heapProperties.VisibleNodeMask = 1;

	//		D3D12_RESOURCE_DESC descResource{};
	//		descResource.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//		descResource.Alignment = 0;
	//		descResource.Width = (UINT)((sizeof(XMConstantObject) + 0xff) & ~0xff);
	//		descResource.Height = 1;
	//		descResource.DepthOrArraySize = 1;
	//		descResource.MipLevels = 1;
	//		descResource.Format = DXGI_FORMAT_UNKNOWN;
	//		descResource.SampleDesc = { 1, 0 };
	//		descResource.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//		descResource.Flags = D3D12_RESOURCE_FLAG_NONE;

	//		D3D12_DESCRIPTOR_HEAP_DESC	descDescriptorHeap{};
	//		descDescriptorHeap.NumDescriptors = 2;
	//		descDescriptorHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//		descDescriptorHeap.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//		descDescriptorHeap.NodeMask = 0;

	//		tempDevice->GetDevice()->CreateDescriptorHeap(&descDescriptorHeap, IID_PPV_ARGS(TEST_CODE_m_pObjectHeap.GetAddressOf()));
	//		TEST_CODE_m_nDescSize = tempDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//		for (UINT i = 0; i < 2; ++i)
	//		{
	//			if (FAILED(tempDevice->GetDevice()->CreateCommittedResource(
	//				&heapProperties,
	//				D3D12_HEAP_FLAG_NONE,
	//				&descResource,
	//				D3D12_RESOURCE_STATE_GENERIC_READ,
	//				nullptr,
	//				IID_PPV_ARGS(TEST_CODE_m_pConstantObject[i].GetAddressOf())
	//			)))
	//				throw std::runtime_error("!Failed to create constant buffer");


	//			D3D12_CONSTANT_BUFFER_VIEW_DESC descConstant{};
	//			D3D12_CPU_DESCRIPTOR_HANDLE		cbvCpuHandle{};

	//			descConstant.BufferLocation = TEST_CODE_m_pConstantObject[i]->GetGPUVirtualAddress();
	//			descConstant.SizeInBytes = (UINT)((sizeof(XMConstantObject) + 0xff) & ~0xff);
	//			cbvCpuHandle.ptr = TEST_CODE_m_pObjectHeap->GetCPUDescriptorHandleForHeapStart().ptr + static_cast<unsigned long long>(i) * TEST_CODE_m_nDescSize;

	//			tempDevice->GetDevice()->CreateConstantBufferView(&descConstant, cbvCpuHandle);

	//			D3D12_GPU_DESCRIPTOR_HANDLE cbvGpuHandle{};
	//			cbvGpuHandle.ptr = TEST_CODE_m_pObjectHeap->GetGPUDescriptorHandleForHeapStart().ptr + static_cast<unsigned long long>(i) * TEST_CODE_m_nDescSize;
	//			TEST_CODE_m_objectCBV[i] = cbvGpuHandle;
	//		}
	//	}
	//}
	//catch (std::runtime_error& e)
	//{
	//	SnctRuntimeError(e);
	//}
    return S_OK;
}

void SnctDX12Object::Draw()
{
}
