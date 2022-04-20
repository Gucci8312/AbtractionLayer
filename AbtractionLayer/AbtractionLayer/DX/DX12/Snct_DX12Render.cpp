#include "Snct_DX12Render.h"

//------------------------------------------------------------------------------
/// Contructor
/// \param		none
//------------------------------------------------------------------------------
SnctDX12Render::SnctDX12Render()
{
	Factory* Factory = new RenderTargetViewFactory();
	Dx11RTV* aa;
	Product* GraphDev = Factory->Create(aa);
	GraphDev->Use<ID3D12DescriptorHeap>();
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
	m_device.Reset();
	m_cmdQueue.Reset();
	m_swapChain.Reset();
	m_cmdList.Reset();
	m_heapRTV.Reset();
	m_heapDSV.Reset();
	m_fence.Reset();
	m_depthBuffer.Reset();

	for (auto Idx = 0; Idx < m_frameCount; ++Idx)
	{
		m_cmdAllocator[Idx].Reset();
		m_colorBuffer[Idx].Reset();
	}

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
void SnctDX12Render::Build(HWND* hWnd)
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
		auto hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(m_device.ReleaseAndGetAddressOf()));
		if(FAILED(hr)) throw "DirectX12 device creation error ";

		// Command queue Setteings
		D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
		QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		QueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		QueueDesc.NodeMask = 0;

		// Create commnd queue
		hr = m_device->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(m_cmdQueue.ReleaseAndGetAddressOf()));
		if (FAILED(hr)) throw "DirectX12 command queue create error";

		// Create DXGI factoy
		ComPtr<IDXGIFactory4> Factory = nullptr;
		hr = CreateDXGIFactory1(IID_PPV_ARGS(Factory.ReleaseAndGetAddressOf()));
		if (FAILED(hr)) throw "DirectX12 DXGIFactory create error";

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
		SwapChainDesc.OutputWindow = *hWnd;
		SwapChainDesc.Windowed = TRUE;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// Create swapchain
		ComPtr<IDXGISwapChain> pSwapChain;
		hr = Factory->CreateSwapChain(m_cmdQueue.Get(), &SwapChainDesc, pSwapChain.ReleaseAndGetAddressOf());
		if (FAILED(hr)) throw "DirectX12 IDXGISwapchain create error";

		// Get IDXGISwapChain3
		hr = pSwapChain.As(&m_swapChain);
		if (FAILED(hr)) throw "DirectX12 IDXGISwapchain create error";

		// Get buckbuffer idx
		m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

		// Create commandallocator
		for (auto i = 0; i < m_frameCount; ++i)
		{
			hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(m_cmdAllocator[i].ReleaseAndGetAddressOf()));
			if (FAILED(hr)) throw "DirectX12 command allocator create error";
		}

		// Create commandlist
		hr = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_cmdAllocator[m_frameIndex].Get(), nullptr, IID_PPV_ARGS(m_cmdList.ReleaseAndGetAddressOf()));
		if (FAILED(hr)) throw "DirectX12 command list create error";

		// Render target view settings
		D3D12_DESCRIPTOR_HEAP_DESC RTVHeapDesc = {};
		RTVHeapDesc.NumDescriptors = m_frameCount;
		RTVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		RTVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		RTVHeapDesc.NodeMask = 0;

		// Create descriptor heap
		hr = m_device->CreateDescriptorHeap(&RTVHeapDesc, IID_PPV_ARGS(m_heapRTV.ReleaseAndGetAddressOf()));
		if (FAILED(hr)) throw "DirectX12 render target descriptor heap creation error";

		// Descriptor handle
		auto handle = m_heapRTV->GetCPUDescriptorHandleForHeapStart();

		// Get the size of render target view
		auto incrementSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		// Turn the buffer for a few minutes
		for (auto i = 0u; i < m_frameCount; ++i)
		{
			// Get color buffer
			hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(m_colorBuffer[i].ReleaseAndGetAddressOf()));
			if (FAILED(hr)) throw "DirectX12 color buffer create error";

			// Render target view settings
			D3D12_RENDER_TARGET_VIEW_DESC viewDesc = {};
			viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			viewDesc.Texture2D.MipSlice = 0;
			viewDesc.Texture2D.PlaneSlice = 0;

			// Create render target view
			m_device->CreateRenderTargetView(m_colorBuffer[i].Get(), &viewDesc, handle);

			m_handleRTV[i] = handle;
			handle.ptr += incrementSize;
		}

		// Reset fence counter
		for (auto i = 0u; i < m_frameCount; ++i)
		{
			m_fenceCounter[i] = 0;
		}

		// Create fence
		hr = m_device->CreateFence(m_fenceCounter[m_frameIndex],
			D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.ReleaseAndGetAddressOf()));
		if (FAILED(hr)) throw "DirectX12 fence create error";

		m_fenceCounter[m_frameIndex]++;

		// Create event
		m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (m_fenceEvent == nullptr) return;

		// End command recording
		m_cmdList->Close();

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
		D3D12_CLEAR_VALUE ClearValue;
		ClearValue.Format = DXGI_FORMAT_D32_FLOAT;
		ClearValue.DepthStencil.Depth = 1.0f;
		ClearValue.DepthStencil.Stencil = 0;

		// Create depth buffer
		hr = m_device->CreateCommittedResource(&DepthHeapProp, D3D12_HEAP_FLAG_NONE,
			&DepthResDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &ClearValue,
			IID_PPV_ARGS(m_depthBuffer.ReleaseAndGetAddressOf()));
		if (FAILED(hr)) throw "DirectX12 depth buffer create error";

		// Depth descriptor heap settings
		D3D12_DESCRIPTOR_HEAP_DESC DepthHeapDesc = {};
		DepthHeapDesc.NumDescriptors = 1;
		DepthHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		DepthHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		DepthHeapDesc.NodeMask = 0;

		// Create descriptor heap for depth
		hr = m_device->CreateDescriptorHeap(&DepthHeapDesc, IID_PPV_ARGS(m_heapDSV.ReleaseAndGetAddressOf()));
		if (FAILED(hr)) throw "DirectX12 depth stencil view descriptor heap creation error";

		// Get the handle of the descriptor heap for depth
		handle = m_heapDSV->GetCPUDescriptorHandleForHeapStart();

		// Get the size of the depth stencil view
		incrementSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

		// Depth stencil view settings
		D3D12_DEPTH_STENCIL_VIEW_DESC DepthViewDesc = {};
		DepthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		DepthViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		DepthViewDesc.Texture2D.MipSlice = 0;
		DepthViewDesc.Flags = D3D12_DSV_FLAG_NONE;

		// Create depth stencil view
		m_device->CreateDepthStencilView(m_depthBuffer.Get(), &DepthViewDesc, handle);

		// Set the size of the descriptor heap for depth
		m_handleDSV = handle;

		// View port settings
		m_viewPort.TopLeftX = 0;
		m_viewPort.TopLeftY = 0;
		m_viewPort.Width = static_cast<float>(g_screenWidth);
		m_viewPort.Height = static_cast<float>(g_screenHeight);
		m_viewPort.MinDepth = 0.0f;
		m_viewPort.MaxDepth = 1.0f;

		// Scissor rectangle settings
		m_scissor.left = 0;
		m_scissor.right = g_screenWidth;
		m_scissor.top = 0;
		m_scissor.bottom = g_screenHeight;
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
	// コマンド入力開始
	m_cmdAllocator[m_frameIndex]->Reset();
	m_cmdList->Reset(m_cmdAllocator[m_frameIndex].Get(), nullptr);

	SetResourceBarrier(m_colorBuffer[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	// Render target setting
	m_cmdList->OMSetRenderTargets(1, &m_handleRTV[m_frameIndex], false, &m_handleDSV);

	// Clear color settings
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	// Clear render targt view
	m_cmdList->ClearRenderTargetView(m_handleRTV[m_frameIndex], clearColor, 0, nullptr);

	// Clear depth stencil view
	m_cmdList->ClearDepthStencilView(m_handleDSV, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// Set screen parameter
	m_cmdList->RSSetViewports(1, &m_viewPort);
	m_cmdList->RSSetScissorRects(1, &m_scissor);
}


//------------------------------------------------------------------------------
/// Post-drawing processing
/// \param			none
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12Render::RenderEnd()
{
	SetResourceBarrier(m_colorBuffer[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	// End command recording
	m_cmdList->Close();

	// Execute command
	ID3D12CommandList* ppCmdLists[] = { m_cmdList.Get() };
	m_cmdQueue->ExecuteCommandLists(1, ppCmdLists);


	// Display on screen
	m_swapChain->Present(1, 0);

	// Signal processing
	const auto currentValue = m_fenceCounter[m_frameIndex];
	m_cmdQueue->Signal(m_fence.Get(), currentValue);

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
}


//------------------------------------------------------------------------------
/// Used after setting the barrier
/// \param[in]		Resource
/// \param[in]		Before
/// \param[in]		After
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12Render::SetResourceBarrier(ID3D12Resource* Resource, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After)
{
	// Resource barrier settings
	D3D12_RESOURCE_BARRIER BarrierDesc;
	ZeroMemory(&BarrierDesc, sizeof(BarrierDesc));
	BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	BarrierDesc.Transition.pResource = Resource;
	BarrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	BarrierDesc.Transition.StateBefore = Before;
	BarrierDesc.Transition.StateAfter = After;

	// Set resource barrier
	m_cmdList->ResourceBarrier(1, &BarrierDesc);
}


//------------------------------------------------------------------------------
/// Wait until GPU processing is finished
/// \param			none
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12Render::WaitGPU()
{
	m_cmdQueue->Signal(m_fence.Get(), m_fenceCounter[m_frameIndex]);

	// When GPU procesing is completed
	m_fence->SetEventOnCompletion(m_fenceCounter[m_frameIndex], m_fenceEvent);

	// Wait procesing
	WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);

	// Counter increase
	m_fenceCounter[m_frameIndex]++;
}
