#include "Snct_DX12FrameResource.h"

SnctDX12FrameResource::SnctDX12FrameResource(ID3D12Device* pDevice, ID3D12PipelineState* pPso, UINT frameResourceIndex, ID3D12DescriptorHeap* pCbvSrvHeap)
{
	m_fenceValue		= 0;
	m_pPipelineState	= pPso;

	try
	{
		//　スレッド実行しないコマンドリスト
		for (UINT i = 0; i < CommandListCount; ++i) 
		{
			if(FAILED(pDevice->CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(m_pCommandAllocators[i].GetAddressOf())
			)))
				throw std::runtime_error("!Failed to create cmdAlloc");

			if (FAILED(pDevice->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				m_pCommandAllocators[i].Get(),
				m_pPipelineState.Get(),
				IID_PPV_ARGS(m_pCommandLists[i].GetAddressOf())
			)))
				throw std::runtime_error("!Failed to create cmdList");

			m_pCommandLists[i]->Close();
		}


		// スレッド内部で呼び出される描画コマンド。　サンプルではオブジェクト、影（今回はなし）を生成している
		for (UINT i = 0; i < NumContexts; ++i)
		{
			if (FAILED(pDevice->CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(m_pObjectCommandAllocators[i].GetAddressOf())
			)))
				throw std::runtime_error("!Failed to create cmdAlloc");

			if (FAILED(pDevice->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				m_pObjectCommandAllocators[i].Get(),
				m_pPipelineState.Get(),
				IID_PPV_ARGS(m_pObjectCommandLists[i].GetAddressOf())
			)))
				throw std::runtime_error("!Failed to create cmdList");

			m_pObjectCommandLists[i]->Close();
		}


		// Constant buffer　の作成とマップ
		D3D12_HEAP_PROPERTIES heapProperties{};
		heapProperties.Type					= D3D12_HEAP_TYPE_UPLOAD;
		heapProperties.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProperties.CreationNodeMask		= 1;
		heapProperties.VisibleNodeMask		= 1;

		D3D12_RESOURCE_DESC descResource{};
		descResource.Dimension			= D3D12_RESOURCE_DIMENSION_BUFFER;
		descResource.Alignment			= 0;
		descResource.Height				= 1;
		descResource.DepthOrArraySize	= 1;
		descResource.MipLevels			= 1;
		descResource.Format				= DXGI_FORMAT_UNKNOWN;
		descResource.SampleDesc			= { 1, 0 };
		descResource.Layout				= D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		descResource.Flags				= D3D12_RESOURCE_FLAG_NONE;

		const UINT cameraConstantSize	= (sizeof(XMConstantCamera) + 0xff) & ~0xff;
		const UINT objectConstantSize	= (sizeof(XMConstantObject) + 0xff) & ~0xff;


		// 作成
		{
			descResource.Width	= cameraConstantSize;
			
			if (FAILED(pDevice->CreateCommittedResource(
				&heapProperties,
				D3D12_HEAP_FLAG_NONE,
				&descResource,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(m_pCameraConstantBuffer.GetAddressOf())
			)))
				throw std::runtime_error("!Failed to create camera constant");
			
			descResource.Width	= objectConstantSize;

			if (FAILED(pDevice->CreateCommittedResource(
				&heapProperties,
				D3D12_HEAP_FLAG_NONE,
				&descResource,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(m_pObjectConstantBuffer.GetAddressOf())
			)))
				throw std::runtime_error("!Failed to create camera constant");
		}

		// マップ
		{
			D3D12_RANGE range{};

			if (FAILED(m_pCameraConstantBuffer->Map(
				0,
				&range,
				reinterpret_cast<void**>(m_pConstantCamera)
			)))
				throw std::runtime_error("!Failed to create camera constan data");

			if (FAILED(m_pObjectConstantBuffer->Map(
				0,
				&range,
				reinterpret_cast<void**>(m_pConstantObject)
			)))
				throw std::runtime_error("!Failed to create object constan data");
		}


		// Descriptor等の作成
		const UINT cbvSrvDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		
		D3D12_GPU_DESCRIPTOR_HANDLE		cbvSrvGpuHandle{};
		D3D12_CPU_DESCRIPTOR_HANDLE		cbvSrvCpuHandle{};
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc		{};

		// Camera CBV desc
		cbvDesc.SizeInBytes		= cameraConstantSize;
		cbvDesc.BufferLocation	= m_pCameraConstantBuffer->GetGPUVirtualAddress();

		pDevice->CreateConstantBufferView(&cbvDesc, cbvSrvCpuHandle);
		m_cameraCbvHandle = cbvSrvGpuHandle;
		
		cbvSrvCpuHandle.ptr = pCbvSrvHeap->GetCPUDescriptorHandleForHeapStart().ptr + cbvSrvDescriptorSize;
		cbvSrvGpuHandle.ptr = pCbvSrvHeap->GetGPUDescriptorHandleForHeapStart().ptr + cbvSrvDescriptorSize;

		// Object CBV desc
		cbvDesc.SizeInBytes		= objectConstantSize;
		cbvDesc.BufferLocation	= m_pObjectConstantBuffer->GetGPUVirtualAddress();

		pDevice->CreateConstantBufferView(&cbvDesc, cbvSrvCpuHandle);
		m_objectCbvHandle = cbvSrvGpuHandle;


		// commandList 一括実行用
		memcpy(m_pBatchSubmit, m_pObjectCommandLists, _countof(m_pObjectCommandLists) * sizeof(ID3D12CommandList*));
	}
	catch(std::runtime_error &e)
	{
		SnctRuntimeError(e);
	}
}

void SnctDX12FrameResource::Bind(ID3D12GraphicsCommandList* pCommandList, D3D12_CPU_DESCRIPTOR_HANDLE* pRtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE* pDsvHandle)
{
	// コマンドリストに対してRTVとDSVをバインド
	pCommandList->SetGraphicsRootDescriptorTable(1, m_objectCbvHandle);
	pCommandList->OMSetRenderTargets(1, pRtvHandle, false, pDsvHandle);
}

void SnctDX12FrameResource::CmdReset()
{
	// Cmdのリセット
	for (int i = 0; i < CommandListCount; ++i) 
	{
		m_pCommandAllocators[i]->Reset();
		m_pCommandLists[i]->Reset(m_pCommandAllocators[i].Get(), m_pPipelineState.Get());
	}

	for (int i = 0; i < NumContexts; ++i)
	{
		m_pObjectCommandAllocators[i]->Reset();
		m_pObjectCommandLists[i]->Reset(m_pObjectCommandAllocators[i].Get(), m_pPipelineState.Get());
	}
}

void SnctDX12FrameResource::UpdateConstantBuffers(XMConstantCamera* cameraConstant, XMConstantObject* objectConstant)
{
	// Render 側のコンスタントデータをフレームリソース内にコピー
	memcpy(m_pConstantCamera, &cameraConstant, sizeof(XMConstantCamera));
	memcpy(m_pConstantObject, &objectConstant, sizeof(XMConstantObject));
}
