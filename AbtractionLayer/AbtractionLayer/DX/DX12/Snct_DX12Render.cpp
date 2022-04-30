#include "Snct_DX12Render.h"

//------------------------------------------------------------------------------
/// Constructor
/// \param		none
//------------------------------------------------------------------------------
SnctDX12Render::SnctDX12Render()
{
	// Nothing //
}

//------------------------------------------------------------------------------
/// Destructor
/// \param		none
//------------------------------------------------------------------------------
SnctDX12Render::~SnctDX12Render()
{
	// Fence
	WaitGPU();

	// Release
	//m_cmdQueue.Reset();
	m_swapChain.Reset();
	m_heapRTV.Reset();
	m_heapDSV.Reset();
	m_fence.Reset();
	//m_depthBuffer.Reset();

	//for (auto Idx = 0; Idx < m_frameCount; ++Idx)
	//{
	//	m_cmdAllocator[Idx].Reset();
	//}

	// Event destroy
	if (m_fenceEvent != nullptr)
	{
		CloseHandle(m_fenceEvent);
		m_fenceEvent = nullptr;
	}
}

//------------------------------------------------------------------------------
/// Initial setting of DirectX12
/// \param[in]		Window handle pointer
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12Render::Build(HWND hWnd)
{
	// Debug option
#if defined(DEBUG) || defined(_DEBUG)
	{
		ComPtr<ID3D12Debug> Debug;
		auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(Debug.ReleaseAndGetAddressOf()));

		// Debuh layer enabled
		if (SUCCEEDED(hr))
		{
			Debug->EnableDebugLayer();
		}
	}
#endif

	try
	{
		// Device 
		m_device.CreateDevice(D3D_FEATURE_LEVEL_11_0);

		// Command queue Setteings
		D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
		QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		QueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		QueueDesc.NodeMask = 0;

		// Create commnd queue
		auto hr = m_device.CreateCommandQueue(QueueDesc, m_cmdQueue.GetCmdQueueAddress());
		if (FAILED(hr)) throw std::runtime_error("DirectX12 command queue create error");

		// Create DXGI factoy
		ComPtr<IDXGIFactory4> Factory = nullptr;
		hr = CreateDXGIFactory1(IID_PPV_ARGS(Factory.ReleaseAndGetAddressOf()));
		if (FAILED(hr)) throw std::runtime_error("DirectX12 DXGIFactory create error");

		// Swapchain Setteings
		DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
		SwapChainDesc.BufferDesc.Width = g_screenWidth;
		SwapChainDesc.BufferDesc.Height = g_screenHeight;
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SwapChainDesc.SampleDesc.Count = 1;
		SwapChainDesc.SampleDesc.Quality = 0;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.BufferCount = m_frameCount;
		SwapChainDesc.OutputWindow = hWnd;
		SwapChainDesc.Windowed = TRUE;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// Create swapchain
		ComPtr<IDXGISwapChain> pSwapChain;
		hr = Factory->CreateSwapChain(m_cmdQueue.GetCmdQueue(), &SwapChainDesc, pSwapChain.ReleaseAndGetAddressOf());
		if (FAILED(hr)) throw std::runtime_error("DirectX12 IDXGISwapchain create error");

		// Get IDXGISwapChain3
		hr = pSwapChain.As(&m_swapChain);
		if (FAILED(hr)) throw std::runtime_error("DirectX12 IDXGISwapchain create error");

		// Get buckbuffer idx
		m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

		// Create commandallocator
		for (auto i = 0; i < m_frameCount; ++i)
		{
			hr = m_device.GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(m_cmdList.GetCmdAllocatorAddress()));
			if (FAILED(hr)) throw std::runtime_error("DirectX12 command allocator create error");
		}

		m_pShaderLibrary = std::make_unique<SnctShaderLibrary>();

		m_pShaderLibrary->CreateShaderFromFile("n_vertex.hlsl", L"../../AbtractionLayer/AbtractionLayer/DX/Shader/n_vertex.hlsl", DX_SHADER_TYPE::VS);
		m_pShaderLibrary->CreateShaderFromFile("n_pixel.hlsl" , L"../../AbtractionLayer/AbtractionLayer/DX/Shader/n_pixel.hlsl" , DX_SHADER_TYPE::PS);
			
		TEST_CODE_CreateRootSignature();
		TEST_CODE_CreatePipelineState();

		// Create commandlist
		//hr = m_cmdList.Create(D3D12_COMMAND_LIST_TYPE_DIRECT, m_device.GetDevice(),TEST_CODE_m_pPipelineState.Get(), m_cmdAllocator[m_frameIndex].Get());
		//if (FAILED(hr)) throw std::runtime_error("DirectX12 command list create error");
		ISnctDXContext* pTempCmdList = dynamic_cast<ISnctDXContext*>(&m_cmdList);
		m_device.CreateCmdList(&pTempCmdList);
		//m_device.c

		// Render target view settings
		D3D12_DESCRIPTOR_HEAP_DESC RTVHeapDesc = {};
		RTVHeapDesc.NumDescriptors = m_frameCount;
		RTVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		RTVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		RTVHeapDesc.NodeMask = 0;

		// Create descriptor heap
		hr = m_device.GetDevice()->CreateDescriptorHeap(&RTVHeapDesc, IID_PPV_ARGS(m_heapRTV.ReleaseAndGetAddressOf()));
		if (FAILED(hr)) throw std::runtime_error("DirectX12 render target descriptor heap creation error");

		// Descriptor handle
		auto handle = m_heapRTV->GetCPUDescriptorHandleForHeapStart();

		// Get the size of render target view
		auto incrementSize = m_device.GetIncrementHandleSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		// Turn the buffer for a few minutes
		for (auto i = 0u; i < m_frameCount; ++i)
		{
			// Get color buffer
			hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(m_colorBuffer[i].GetBufferAddress()));
			if (FAILED(hr)) throw std::runtime_error("DirectX12 color buffer create error");

			SnctDX12RTV TempRTV = {};
			TempRTV.SetHandle(handle);

			// Create render target view
			m_device.CreateRTV(&m_colorBuffer[i], &TempRTV);

			m_handleRTV[i].SetHandle(handle);
			handle.ptr += incrementSize;
		}

		// Reset fence counter
		for (auto i = 0u; i < m_frameCount; ++i)
		{
			m_fenceCounter[i] = 0;
		}

		// Create fence
		hr = m_device.GetDevice()->CreateFence(m_fenceCounter[m_frameIndex],
			D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.ReleaseAndGetAddressOf()));
		if (FAILED(hr)) throw std::runtime_error("DirectX12 fence create error");

		m_fenceCounter[m_frameIndex]++;

		// Create event
		m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (m_fenceEvent == nullptr) return;

		// End command recording
		m_cmdList.Close();

		// Depth stencil buffer settings
		D3D12_HEAP_PROPERTIES DepthHeapProp = {};
		DepthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
		DepthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		DepthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		DepthHeapProp.CreationNodeMask = 1;
		DepthHeapProp.VisibleNodeMask = 1;

		// Resource settings for depth
		D3D12_RESOURCE_DESC DepthResDesc = {};
		DepthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		DepthResDesc.Alignment = 0;
		DepthResDesc.Width = g_screenWidth;
		DepthResDesc.Height = g_screenHeight;
		DepthResDesc.DepthOrArraySize = 1;
		DepthResDesc.MipLevels = 1;
		DepthResDesc.Format = DXGI_FORMAT_D32_FLOAT;
		DepthResDesc.SampleDesc.Count = 1;
		DepthResDesc.SampleDesc.Quality = 0;
		DepthResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		DepthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		// Depth buffer clear value
		D3D12_CLEAR_VALUE ClearValue = {};
		ClearValue.Format = DXGI_FORMAT_D32_FLOAT;
		ClearValue.DepthStencil.Depth = 1.0f;
		ClearValue.DepthStencil.Stencil = 0;

		// Create depth buffer
		hr = m_device.GetDevice()->CreateCommittedResource(&DepthHeapProp, D3D12_HEAP_FLAG_NONE,
			&DepthResDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &ClearValue,
			IID_PPV_ARGS(m_depthBuffer.GetBufferAddress()));
		if (FAILED(hr)) throw std::runtime_error("DirectX12 depth buffer create error");

		// Depth descriptor heap settings
		D3D12_DESCRIPTOR_HEAP_DESC DepthHeapDesc = {};
		DepthHeapDesc.NumDescriptors = 1;
		DepthHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		DepthHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		DepthHeapDesc.NodeMask = 0;

		// Create descriptor heap for depth
		hr = m_device.GetDevice()->CreateDescriptorHeap(&DepthHeapDesc, IID_PPV_ARGS(m_heapDSV.ReleaseAndGetAddressOf()));
		if (FAILED(hr)) throw std::runtime_error("DirectX12 depth stencil view descriptor heap creation error");

		// Get the handle of the descriptor heap for depth
		handle = m_heapDSV->GetCPUDescriptorHandleForHeapStart();

		// Get the size of the depth stencil view
		incrementSize = m_device.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

		SnctDX12DSV TempDSV = {};
		TempDSV.SetHandle(handle);

		// Create depth stencil view
		m_device.CreateDSV(&m_depthBuffer, &TempDSV);

		// Set the size of the descriptor heap for depth
		m_handleDSV.SetHandle(handle);

		// Create object manager
		m_pSceneObjects = std::make_unique<SnctDX12Objects>();

		TEST_CODE_CreateCameraConstantBuffer();

	}
	catch (std::runtime_error& e) {
		SnctRuntimeError(e);
	}
}


//------------------------------------------------------------------------------
/// Pre-drawing processing
/// \param			none
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12Render::RenderBegin()
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	// Start command input
	//m_cmdAllocator[m_frameIndex]->Reset();
	//m_cmdList.Reset(m_cmdAllocator[m_frameIndex].Get(), nullptr);
	m_cmdList.Reset(TEST_CODE_m_pPipelineState.Get());

	m_cmdList.SetResourceBarrier(&m_colorBuffer[m_frameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	// Render target setting
	m_cmdList.SetRTV(1, &m_handleRTV[m_frameIndex], &m_handleDSV, false);

	m_cmdList.SetPipelineState(TEST_CODE_m_pPipelineState.Get());
	m_cmdList.SetGraphicsRootSignature(TEST_CODE_m_pRootSignature.Get());

	// Clear render targt view
	m_cmdList.ClearRTV(&m_handleRTV[m_frameIndex], clearColor, 0, nullptr);

	// Clear depth stencil view
	m_cmdList.ClearDSV(&m_handleDSV, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// Set screen parameter
	m_cmdList.SetViewPort(g_screenWidth, g_screenHeight, 0.0f, 1.0f);
	m_cmdList.SetScissorRects(g_screenWidth, g_screenHeight);

	// Set camera constant buffer
	UpdateCameraBuffer(TEST_CODE_m_pCameraConstant[m_frameIndex].Get());
}


//------------------------------------------------------------------------------
/// Post-drawing processing
/// \param			none
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12Render::RenderEnd()
{
	m_cmdList.SetResourceBarrier(&m_colorBuffer[m_frameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	// End command recording
	m_cmdList.Close();

	// Execute command
	ISnctDXContext* ppCmdLists[] = { static_cast<ISnctDXContext*>(&m_cmdList) };
	m_cmdQueue.Execute(1, ppCmdLists);

	// Display on screen
	m_swapChain->Present(1, 0);

	// Signal processing
	uint64_t  currentValue = m_fenceCounter[m_frameIndex];
	m_cmdQueue.Signal(m_fence.Get(), currentValue);

	// Update back buffer number
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

	// Fence processing
	WaitGPU();

	// Increased frame counter
	m_fenceCounter[m_frameIndex] = currentValue + 1;
}


//------------------------------------------------------------------------------
/// Drawing directX12 object
/// \param[in]		HashKey
/// \param[in]		drawFlag
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12Render::Draw(HashKey key, SNCT_DRAW_FLAG drawFlag)
{
	SnctDX12ObjectBuffer* object = m_pSceneObjects->GetObjectBuffer(key);

	// Set object constant buffer
	UpdateObjectBuffer(object->pConstantObject[m_frameIndex].Get());

	D3D12_VERTEX_BUFFER_VIEW		vertexBufferView{};
	vertexBufferView.BufferLocation = object->pVertexBuffer->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = sizeof(Vertex);
	vertexBufferView.SizeInBytes = (UINT)sizeof(Vertex) * object->nVertexSize;

	D3D12_INDEX_BUFFER_VIEW			indexBufferView{};
	indexBufferView.BufferLocation = object->pIndexBuffer->GetGPUVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	indexBufferView.SizeInBytes = (UINT)sizeof(UINT) * object->nIndexSize;


	// Future : include factory pattern method
	m_cmdList.GetContext()->SetDescriptorHeaps(1, TEST_CODE_m_pCameraHeap.GetAddressOf());
	m_cmdList.GetContext()->SetGraphicsRootDescriptorTable(0, TEST_CODE_m_cameraCBV[m_frameIndex]);

	m_cmdList.GetContext()->SetDescriptorHeaps(1, object->pObjectHeap.GetAddressOf());
	m_cmdList.GetContext()->SetGraphicsRootDescriptorTable(1, object->objectCBV[m_frameIndex]);

	m_cmdList.GetContext()->IASetVertexBuffers(0, 1, &vertexBufferView);
	m_cmdList.GetContext()->IASetIndexBuffer(&indexBufferView);
	m_cmdList.GetContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_cmdList.GetContext()->DrawIndexedInstanced(object->nIndexSize, 1, 0, 0, 0);
}


//------------------------------------------------------------------------------
/// Create directX12 object
/// \param[in]		HashKey
/// \param[in]		pVertices
/// \param[in]		pIndices
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12Render::CreateObject(HashKey key, Vertices* pVertices, Indices* pIndices)
{
	m_pSceneObjects->AddSceneObject(m_device.GetDevice(), key, pVertices, pIndices);
}

//------------------------------------------------------------------------------
/// Wait until GPU processing is finished
/// \param			none
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12Render::WaitGPU()
{
	uint64_t currentValue = m_fenceCounter[m_frameIndex];
	m_cmdQueue.Signal(m_fence.Get(), currentValue);

	// When GPU procesing is completed
	m_fence->SetEventOnCompletion(m_fenceCounter[m_frameIndex], m_fenceEvent);

	// Wait procesing
	WaitForSingleObjectEx(m_fenceEvent, INFINITE, false);

	// Counter increase
	m_fenceCounter[m_frameIndex]++;
}

//------------------------------------------------------------------------------
/// Draw object from library
/// \param			object pointer
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12Render::DrawIndexed(SnctDX12ObjectBuffer* pObject)
{
	//// Set object constant buffer
	//UpdateObjectBuffer(pObject->pConstantObject[m_frameIndex].Get());
	//
	//D3D12_VERTEX_BUFFER_VIEW		vertexBufferView{};
	//vertexBufferView.BufferLocation		= pObject->pVertexBuffer->GetGPUVirtualAddress();
	//vertexBufferView.StrideInBytes		= sizeof(Vertex);
	//vertexBufferView.SizeInBytes		= (UINT)sizeof(Vertex) * pObject->nVertexSize;

	//D3D12_INDEX_BUFFER_VIEW			indexBufferView{};
	//indexBufferView.BufferLocation		= pObject->pIndexBuffer->GetGPUVirtualAddress();
	//indexBufferView.Format				= DXGI_FORMAT_R32_UINT;
	//indexBufferView.SizeInBytes			= (UINT)sizeof(UINT) * pObject->nIndexSize;

	//
	//// Future : include factory pattern method
	//m_cmdList.Get()->SetDescriptorHeaps(1, TEST_CODE_m_pCameraHeap.GetAddressOf());
	//m_cmdList.Get()->SetGraphicsRootDescriptorTable(0, TEST_CODE_m_cameraCBV[m_frameIndex]);

	//m_cmdList.Get()->SetDescriptorHeaps(1, pObject->pObjectHeap.GetAddressOf());
	//m_cmdList.Get()->SetGraphicsRootDescriptorTable(1, pObject->objectCBV[m_frameIndex]);
	//
	//m_cmdList.Get()->IASetVertexBuffers(0, 1,&vertexBufferView);
	//m_cmdList.Get()->IASetIndexBuffer(&indexBufferView);
	//m_cmdList.Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//m_cmdList.Get()->DrawIndexedInstanced(pObject->nIndexSize, 1, 0, 0, 0);
}

//------------------------------------------------------------------------------
/// Update camera constant buffer 
/// \param			object unique constant buffer
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12Render::UpdateCameraBuffer(ID3D12Resource* pCameraConstant)
{
	void* pCameraDataBegin;
	D3D12_RANGE range{ 0,0 };

	pCameraConstant->Map(
		0,
		&range,
		&pCameraDataBegin
	);
	
	memcpy(pCameraDataBegin, m_pConstantCamera.get(), sizeof(XMConstantCamera));
	pCameraConstant->Unmap(0, nullptr);
}

//------------------------------------------------------------------------------
/// Update object constant buffer 
/// \param			object unique constant buffer
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12Render::UpdateObjectBuffer(ID3D12Resource* pObjectConstant)
{
	void* pObjectDataBegin;
	D3D12_RANGE range{ 0,0 };

	pObjectConstant->Map(
		0,
		&range,
		&pObjectDataBegin
	);

	memcpy(pObjectDataBegin, m_pConstantObject.get(), sizeof(XMConstantObject));
	pObjectConstant->Unmap(0, nullptr);
}

//------------------------------------------------------------------------------
/// create camera constant buffer 
/// \param			object unique constant buffer
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12Render::TEST_CODE_CreateCameraConstantBuffer()
{
	try {

		TEST_CODE_m_pCameraConstant.resize(m_frameCount);
		TEST_CODE_m_cameraCBV.resize(m_frameCount);

		D3D12_HEAP_PROPERTIES heapProperties{};
		heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProperties.CreationNodeMask = 1;
		heapProperties.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC descResource{};
		descResource.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		descResource.Alignment = 0;
		descResource.Width = (UINT)((sizeof(XMConstantCamera) + 0xff) & ~0xff);
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

		m_device.GetDevice()->CreateDescriptorHeap(&descDescriptorHeap, IID_PPV_ARGS(TEST_CODE_m_pCameraHeap.GetAddressOf()));
		UINT nDescSize = m_device.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		for (UINT i = 0; i < m_frameCount; ++i)
		{
			if (FAILED(m_device.GetDevice()->CreateCommittedResource(
				&heapProperties,
				D3D12_HEAP_FLAG_NONE,
				&descResource,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(TEST_CODE_m_pCameraConstant[i].GetAddressOf())
			)))
				throw std::runtime_error("!Failed to create constant buffer");


			D3D12_CONSTANT_BUFFER_VIEW_DESC descConstant{};
			D3D12_CPU_DESCRIPTOR_HANDLE		cbvCpuHandle{};

			descConstant.BufferLocation = TEST_CODE_m_pCameraConstant[i]->GetGPUVirtualAddress();
			descConstant.SizeInBytes = (UINT)((sizeof(XMConstantCamera) + 0xff) & ~0xff);
			cbvCpuHandle.ptr = TEST_CODE_m_pCameraHeap->GetCPUDescriptorHandleForHeapStart().ptr + static_cast<unsigned long long>(i) * nDescSize;

			m_device.GetDevice()->CreateConstantBufferView(&descConstant, cbvCpuHandle);

			D3D12_GPU_DESCRIPTOR_HANDLE cbvGpuHandle{};
			cbvGpuHandle.ptr = TEST_CODE_m_pCameraHeap->GetGPUDescriptorHandleForHeapStart().ptr + static_cast<unsigned long long>(i) * nDescSize;
			TEST_CODE_m_cameraCBV[i] = cbvGpuHandle;
		}
	}
	catch (std::runtime_error& e)
	{
		SnctRuntimeError(e);
	}
}

//------------------------------------------------------------------------------
/// << ! TEST CODE >> create rootsignature
/// \param			none
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12Render::TEST_CODE_CreateRootSignature()
{
	// root signature
	try
	{
		D3D12_ROOT_PARAMETER	rootParams[3] = {};
		D3D12_DESCRIPTOR_RANGE	camRange{}, objRange{}, srvRange{};
		
		// constant (b0)
		camRange.NumDescriptors						= 1;
		camRange.BaseShaderRegister					= 0;
		camRange.RangeType							= D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		camRange.OffsetInDescriptorsFromTableStart	= D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		rootParams[0].ParameterType					= D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParams[0].ShaderVisibility				= D3D12_SHADER_VISIBILITY_ALL;
		rootParams[0].DescriptorTable				= {1, &camRange};
		
		// constant (b1)
		objRange.NumDescriptors						= 1;
		objRange.BaseShaderRegister					= 1;
		objRange.RangeType							= D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		objRange.OffsetInDescriptorsFromTableStart	= D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		rootParams[1].ParameterType					= D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParams[1].ShaderVisibility				= D3D12_SHADER_VISIBILITY_ALL;
		rootParams[1].DescriptorTable				= {1, &objRange};

		// texture	(t0)
		srvRange.NumDescriptors						= 1;
		srvRange.BaseShaderRegister					= 0;
		srvRange.RangeType							= D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		srvRange.OffsetInDescriptorsFromTableStart	= D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		
		rootParams[2].ParameterType					= D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParams[2].ShaderVisibility				= D3D12_SHADER_VISIBILITY_PIXEL;
		rootParams[2].DescriptorTable				= {1, &srvRange};

		// sampler	(s0)
		D3D12_STATIC_SAMPLER_DESC descStaticSampler{};
		descStaticSampler.Filter							= D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		descStaticSampler.AddressU							= D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		descStaticSampler.AddressV							= D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		descStaticSampler.AddressW							= D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		descStaticSampler.MipLODBias						= 0.0f;
		descStaticSampler.MaxAnisotropy						= 16;
		descStaticSampler.ComparisonFunc					= D3D12_COMPARISON_FUNC_NEVER;
		descStaticSampler.BorderColor						= D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		descStaticSampler.MinLOD							= 0.0f;
		descStaticSampler.MaxLOD							= D3D12_FLOAT32_MAX;
		descStaticSampler.ShaderRegister					= 0;
		descStaticSampler.RegisterSpace						= 0;
		descStaticSampler.ShaderVisibility					= D3D12_SHADER_VISIBILITY_ALL;

		D3D12_ROOT_SIGNATURE_DESC descRootSignature{};
		descRootSignature.Flags								= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		descRootSignature.pParameters						= rootParams;
		descRootSignature.NumParameters						= _countof(rootParams);
		descRootSignature.pStaticSamplers					= &descStaticSampler;
		descRootSignature.NumStaticSamplers					= 1;

		ComPtr<ID3DBlob> signature;

		if(FAILED(D3D12SerializeRootSignature(
			&descRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1,
			&signature,
			nullptr)
		))
			throw std::runtime_error("!Failed to serialize root signature");

		if (FAILED(m_device.GetDevice()->CreateRootSignature(
			0,
			signature->GetBufferPointer(),
			signature->GetBufferSize(),
			IID_PPV_ARGS(TEST_CODE_m_pRootSignature.GetAddressOf()))
		))
			throw std::runtime_error("!Failed to create root signature");

	}
	catch (std::runtime_error& e) 
	{
		SnctRuntimeError(e);
	}
}

//------------------------------------------------------------------------------
/// << ! TEST CODE >> create pipeline state 
/// \param			none
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12Render::TEST_CODE_CreatePipelineState()
{
	// create pipeline
	try
	{
		D3D12_INPUT_ELEMENT_DESC descInputLayout[] = {
			{"POSITION"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"NORMAL"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"COLOR"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"TEXCOORD"		, 0, DXGI_FORMAT_R32G32_FLOAT		, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		};

		{
			D3D12_GRAPHICS_PIPELINE_STATE_DESC descPipeline{};
			descPipeline.pRootSignature = TEST_CODE_m_pRootSignature.Get();

			// vertex
			{
				ID3DBlob* vertex = m_pShaderLibrary->GetShaderBlob("n_vertex.hlsl");

				D3D12_SHADER_BYTECODE shaderBytecode{};
				shaderBytecode.pShaderBytecode	= vertex->GetBufferPointer();
				shaderBytecode.BytecodeLength	= vertex->GetBufferSize();

				descPipeline.InputLayout.pInputElementDescs		= descInputLayout;
				descPipeline.InputLayout.NumElements			= _countof(descInputLayout);
				descPipeline.VS									= shaderBytecode;
			}

			// pixe
			{
				ID3DBlob* pixel = m_pShaderLibrary->GetShaderBlob("n_pixel.hlsl");

				D3D12_SHADER_BYTECODE shaderBytecode{};
				shaderBytecode.pShaderBytecode	= pixel->GetBufferPointer();
				shaderBytecode.BytecodeLength	= pixel->GetBufferSize();
				descPipeline.PS					= shaderBytecode;
			}

			// raterizer 
			{
				D3D12_RASTERIZER_DESC descRasterize{};
				descRasterize.FillMode				= D3D12_FILL_MODE_SOLID;
				descRasterize.CullMode				= D3D12_CULL_MODE_NONE;
				descRasterize.FrontCounterClockwise = false;
				descRasterize.DepthBias				= D3D12_DEFAULT_DEPTH_BIAS;
				descRasterize.DepthBiasClamp		= D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
				descRasterize.SlopeScaledDepthBias	= D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
				descRasterize.DepthClipEnable		= true;
				descRasterize.MultisampleEnable		= false;
				descRasterize.AntialiasedLineEnable	= false;
				descRasterize.ForcedSampleCount		= 0;
				descRasterize.ConservativeRaster	= D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
				
				descPipeline.RasterizerState = descRasterize;
			}

			// blend
			{
				D3D12_BLEND_DESC descBlend{};
				descBlend.AlphaToCoverageEnable		= false;
				descBlend.IndependentBlendEnable	= false;

				for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
				{
					descBlend.RenderTarget[i].BlendEnable			= false;
					descBlend.RenderTarget[i].LogicOpEnable			= false;
					descBlend.RenderTarget[i].SrcBlend				= D3D12_BLEND_ONE;
					descBlend.RenderTarget[i].DestBlend				= D3D12_BLEND_ZERO;
					descBlend.RenderTarget[i].BlendOp				= D3D12_BLEND_OP_ADD;
					descBlend.RenderTarget[i].SrcBlendAlpha			= D3D12_BLEND_ONE;
					descBlend.RenderTarget[i].DestBlendAlpha		= D3D12_BLEND_ZERO;
					descBlend.RenderTarget[i].BlendOpAlpha			= D3D12_BLEND_OP_ADD;
					descBlend.RenderTarget[i].LogicOp				= D3D12_LOGIC_OP_NOOP;
					descBlend.RenderTarget[i].RenderTargetWriteMask	= D3D12_COLOR_WRITE_ENABLE_ALL;
				}
				descPipeline.BlendState = descBlend;
			}

			// depth
			{
				D3D12_DEPTH_STENCIL_DESC descDepth;
				descDepth.DepthEnable					= true;
				descDepth.DepthFunc						= D3D12_COMPARISON_FUNC_LESS;
				descDepth.DepthWriteMask				= D3D12_DEPTH_WRITE_MASK_ALL;
				descDepth.StencilEnable					= false;
				descDepth.StencilReadMask				= D3D12_DEFAULT_STENCIL_READ_MASK;
				descDepth.StencilWriteMask				= D3D12_DEFAULT_STENCIL_WRITE_MASK;

				descDepth.FrontFace.StencilFailOp		= D3D12_STENCIL_OP_KEEP;
				descDepth.FrontFace.StencilDepthFailOp	= D3D12_STENCIL_OP_KEEP;
				descDepth.FrontFace.StencilPassOp		= D3D12_STENCIL_OP_KEEP;
				descDepth.FrontFace.StencilFunc			= D3D12_COMPARISON_FUNC_ALWAYS;

				descDepth.BackFace.StencilFailOp		= D3D12_STENCIL_OP_KEEP;
				descDepth.BackFace.StencilDepthFailOp	= D3D12_STENCIL_OP_KEEP;
				descDepth.BackFace.StencilPassOp		= D3D12_STENCIL_OP_KEEP;
				descDepth.BackFace.StencilFunc			= D3D12_COMPARISON_FUNC_ALWAYS;

				descPipeline.DepthStencilState = descDepth;
			}

			descPipeline.PrimitiveTopologyType			= D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			descPipeline.NumRenderTargets				= 1;
			descPipeline.RTVFormats[0]					= DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			descPipeline.DSVFormat						= DXGI_FORMAT_D32_FLOAT;
			descPipeline.SampleMask						= UINT_MAX;
			descPipeline.SampleDesc						= {1,0};

			if (FAILED(m_device.GetDevice()->CreateGraphicsPipelineState(
				&descPipeline,
				IID_PPV_ARGS(TEST_CODE_m_pPipelineState.GetAddressOf())
			)))
				throw std::runtime_error("!Failed to create pso");
		}
	}
	catch (std::runtime_error &e)
	{
		SnctRuntimeError(e);
	}
}
