#include "Snct_DX11Render.h"

//------------------------------------------------------------------------------
/// Constructor
/// \param		none
//------------------------------------------------------------------------------
SnctDX11Render::SnctDX11Render()
{
	// Nothing //
}


//------------------------------------------------------------------------------
/// Destructor
/// \param		none
//------------------------------------------------------------------------------
SnctDX11Render::~SnctDX11Render()
{
	m_swapChain.Reset();
	m_pDepthState.Reset();
	rasterizerState.Reset();
}

bool SnctDX11Render::Build(HWND hWnd)
{
	try
	{
		ComPtr<IDXGIFactory> factory = nullptr;

		if (FAILED(CreateDXGIFactory(IID_PPV_ARGS(factory.GetAddressOf()))))
		{
			throw std::runtime_error("!Failed to create factory");
		}

		{
			if (FAILED(m_device.CreateDevice(D3D_FEATURE_LEVEL_11_1)))
			{
				throw std::runtime_error("!Failed to create device");
			}
		}

		{
			DXGI_SWAP_CHAIN_DESC descSwapChain{};
			descSwapChain.BufferCount = 1;
			descSwapChain.BufferDesc.Width = g_screenWidth;
			descSwapChain.BufferDesc.Height = g_screenHeight;
			descSwapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			descSwapChain.BufferDesc.RefreshRate.Numerator = 60;
			descSwapChain.BufferDesc.RefreshRate.Denominator = 1;
			descSwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			descSwapChain.OutputWindow = hWnd;
			descSwapChain.SampleDesc.Count = 1;
			descSwapChain.SampleDesc.Quality = 0;
			descSwapChain.Windowed = true;
			descSwapChain.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			if (FAILED(factory->CreateSwapChain(
				m_device.GetDevice(),
				&descSwapChain,
				m_swapChain.GetAddressOf()
			)))
			{
				throw std::runtime_error("!Failed to create swapchain");
			}
		}

		{
			// デファードコンテクストの作成
			//m_device.CreateDeferredContext(m_deferredContext.GetContextAddress());

			// 型自体も　Immadiateと変わらないので　同じ設定ができる。もしくは、する必要がある。
		}

		{
			SnctDX11Texture pBackBuffer;
			ID3D11Texture2D* TempRTVBuffer;
			if (FAILED(m_swapChain->GetBuffer(
				0,
				IID_PPV_ARGS(&TempRTVBuffer)
			)))
			{
				throw std::runtime_error("!Failed to get back buffer");
			}
			pBackBuffer.SetTextureAddress(TempRTVBuffer);
			if (FAILED(m_device.CreateRTV(&pBackBuffer, &m_backBufferView)))
			{
				throw std::runtime_error("!Failed to create render target buffer");
			}
			TempRTVBuffer->Release();
		}

		{
			D3D11_DEPTH_STENCIL_DESC descDepthStencil{};
			descDepthStencil.DepthEnable = true;
			descDepthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			descDepthStencil.DepthFunc = D3D11_COMPARISON_LESS;
			descDepthStencil.StencilEnable = FALSE;

			if (FAILED(m_device.GetDevice()->CreateDepthStencilState(
				&descDepthStencil,
				m_pDepthState.GetAddressOf()
			)))
			{
				throw std::runtime_error("!Failed to create depth stancil state");
			}

			SnctDX11Texture pDepthStencilTex;
			SNCT_TEXTURE2D_DESC	descDepthTex{};
			descDepthTex.Width = g_screenWidth;
			descDepthTex.Height = g_screenHeight;
			descDepthTex.MipLevels = 1;
			descDepthTex.ArraySize = 1;
			descDepthTex.Format = DXGI_FORMAT_R24G8_TYPELESS;
			descDepthTex.SampleDesc = { 1, 0 };
			descDepthTex.Usage = USAGE_DEFAULT;
			descDepthTex.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			descDepthTex.CPUAccessFlags = 0;
			descDepthTex.MiscFlags = 0;

			if (FAILED(pDepthStencilTex.Create(m_device.GetDevice(),
				descDepthTex)))
			{
				throw std::runtime_error("!Failed to create depth stencil buffer texture");
			}

			D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView{};
			descDepthStencilView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDepthStencilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDepthStencilView.Flags = 0;

			if (FAILED(m_device.CreateDSV(&pDepthStencilTex, &m_depthStencileView)))
			{
				throw std::runtime_error("!Failed to create depth stencil buffer view");
			}
		}

		{
			D3D11_RASTERIZER_DESC descRasterizer{};
			descRasterizer.FillMode = D3D11_FILL_SOLID;
			descRasterizer.CullMode = D3D11_CULL_NONE;
			descRasterizer.DepthClipEnable = true;
			descRasterizer.MultisampleEnable = false;


			if (FAILED(m_device.GetDevice()->CreateRasterizerState(
				&descRasterizer,
				rasterizerState.GetAddressOf()
			)))
			{
				throw std::runtime_error("!Failed to Create Rasterizer State");
			}

		}

		{
			D3D11_SAMPLER_DESC descSampler;
			descSampler.Filter = D3D11_FILTER_ANISOTROPIC;
			descSampler.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			descSampler.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			descSampler.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			descSampler.MipLODBias = 0;
			descSampler.MaxAnisotropy = 16;
			descSampler.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			descSampler.MinLOD = 0;
			descSampler.MaxLOD = D3D11_FLOAT32_MAX;

			ComPtr<ID3D11SamplerState> samplerState;
			if (FAILED(m_device.GetDevice()->CreateSamplerState(
				&descSampler,
				samplerState.GetAddressOf()
			)))
			{
				throw "!Failed to create sampler state";
			}

			m_device.GetDeviceContext()->PSSetSamplers(0, 1, samplerState.GetAddressOf());
		}
	}
	catch (std::runtime_error& e) {
		SnctRuntimeError(e);
		return false;
	}

	return true;
}

void SnctDX11Render::BeforeRender(ISnctDXContext* pCmdList)
{
	SnctDX11Context* pTempCmdList = static_cast<SnctDX11Context*>(pCmdList);

	//　Immadiate　と deferred　の双方をクリア
	// Immadiate context command
	float clearColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	m_device.ClearRTV(&m_backBufferView, clearColor);
	m_device.ClearDSV(&m_depthStencileView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_device.SetRTV(1, &m_backBufferView, &m_depthStencileView);
	m_device.SetViewPort((FLOAT)g_screenWidth, (FLOAT)g_screenHeight, 0.0f, 1.0f);
	m_device.SetRasterizerState(rasterizerState.Get());
	
	// Deferred context command
	pTempCmdList->ClearRTV(&m_backBufferView, clearColor);
	pTempCmdList->ClearDSV(&m_depthStencileView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	pTempCmdList->SetRTV(1, &m_backBufferView, &m_depthStencileView);
	pTempCmdList->SetViewPort((FLOAT)g_screenWidth, (FLOAT)g_screenHeight, 0.0f, 1.0f);
	pTempCmdList->SetRasterizerState(rasterizerState.Get());

}

void SnctDX11Render::AfterRender(ISnctDXContext* pCmdList)
{
	SnctDX11Context* pTempCmdList = static_cast<SnctDX11Context*>(pCmdList);

	try
	{
		ComPtr<ID3D11CommandList> pCommandList;

		// !　ここでDeferred contextで設定したものをCommandListとして登録
		pTempCmdList->RegisterCmdList(pCommandList.ReleaseAndGetAddressOf());

		// Immadiate context で　実行命令
		m_device.ExecuteCmdList(pCommandList.Get());

		//　実質的に運用方法は、Immadiate と変わらない
		//　commandList も存在するが
		//
		//  Deferred から FinishCommandList で受け渡し
		//　Immadiate で Execute する
		//  そのため CommandListは一時オブジェクトで問題ないかも?

		if (FAILED(m_swapChain->Present(1, 0)))
			throw std::runtime_error("!Failed to swap chain present");
	}
	catch (std::runtime_error& e)
	{
		SnctRuntimeError(e);
	}
}

HRESULT SnctDX11Render::CreateCommandList(ISnctDXContext** pCmdList)
{
	return m_device.CreateCmdList(pCmdList);
}
