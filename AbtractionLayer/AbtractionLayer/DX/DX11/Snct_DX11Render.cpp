#include "../../Snct_Utility.h"
#include "../Snct_DXShaderLibrary.h"
#include "Snct_DX11Render.h"

void SnctDX11Render::Build(HWND hWnd)
{
	try
	{
		ComPtr<IDXGIFactory> factory = nullptr;

		if (FAILED(CreateDXGIFactory(IID_PPV_ARGS(factory.GetAddressOf()))))
		{
			throw std::runtime_error("!Failed to create factory");
		}

		{
			/*	D3D_FEATURE_LEVEL featureLevels[] =
				{
					D3D_FEATURE_LEVEL_11_0,
					D3D_FEATURE_LEVEL_11_1
				};

				if (FAILED(D3D11CreateDevice(
					nullptr,
					D3D_DRIVER_TYPE_HARDWARE,
					nullptr,
					D3D11_CREATE_DEVICE_BGRA_SUPPORT,
					featureLevels,
					ARRAYSIZE(featureLevels),
					D3D11_SDK_VERSION,
					m_pDevice.GetAddressOf(),
					nullptr,
					m_pDevice.GetDeviceContext().GetAddressOf()
				)))*/

			if (FAILED(m_pDevice.Create(D3D_FEATURE_LEVEL_11_1)))
			{
				throw std::runtime_error("!Failed to create device");
			}
		}

		{
			DXGI_SWAP_CHAIN_DESC descSwapChain{};

			descSwapChain.BufferCount = 1;
			descSwapChain.BufferDesc.Width = g_screenWidth;
			descSwapChain.BufferDesc.Height = g_screenHeight;
			descSwapChain.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			descSwapChain.BufferDesc.RefreshRate.Numerator = 60;
			descSwapChain.BufferDesc.RefreshRate.Denominator = 1;
			descSwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			descSwapChain.OutputWindow = hWnd;
			descSwapChain.SampleDesc.Count = 1;
			descSwapChain.SampleDesc.Quality = 0;
			descSwapChain.Windowed = true;

			if (FAILED(factory->CreateSwapChain(
				m_pDevice.GetDevice(),
				&descSwapChain,
				m_pSwapChain.GetAddressOf()
			)))
			{
				throw std::runtime_error("!Failed to create swapchain");
			}
		}

		{
			// デファードコンテクストの作成
			m_pDevice.CreateDeferredContext(m_pDeferredContext.GetAddressOf());

			// 型自体も　Immadiateと変わらないので　同じ設定ができる。もしくは、する必要がある。
		}

		{
			SnctDX11Texture pBackBuffer;

			if (FAILED(m_pSwapChain->GetBuffer(
				0,
				IID_PPV_ARGS(pBackBuffer.SetResourceAddress())
			)))
			{
				throw std::runtime_error("!Failed to get back buffer");
			}
			
			m_pDevice.CreateRTV(&pBackBuffer, &m_pBackBufferView);
		}

		{
			D3D11_DEPTH_STENCIL_DESC descDepthStencil{};
			descDepthStencil.DepthEnable = true;
			descDepthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			descDepthStencil.DepthFunc = D3D11_COMPARISON_LESS;
			descDepthStencil.StencilEnable = FALSE;

			if (FAILED(m_pDevice.GetDevice()->CreateDepthStencilState(
				&descDepthStencil,
				m_pDepthState.GetAddressOf()
			)))
			{
				throw std::runtime_error("!Failed to create depth stancil state");
			}

			ComPtr<ID3D11Texture2D> pDepthStencilTex = nullptr;
			D3D11_TEXTURE2D_DESC	descDepthTex{};
			descDepthTex.Width = g_screenWidth;
			descDepthTex.Height = g_screenHeight;
			descDepthTex.MipLevels = 1;
			descDepthTex.ArraySize = 1;
			descDepthTex.Format = DXGI_FORMAT_R24G8_TYPELESS;
			descDepthTex.SampleDesc = { 1, 0 };
			descDepthTex.Usage = D3D11_USAGE_DEFAULT;
			descDepthTex.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			descDepthTex.CPUAccessFlags = 0;
			descDepthTex.MiscFlags = 0;

			if (FAILED(m_pDevice.GetDevice()->CreateTexture2D(
				&descDepthTex,
				nullptr,
				pDepthStencilTex.GetAddressOf()
			)))
			{
				throw std::runtime_error("!Failed to create depth stencil buffer texture");
			}

			D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView{};
			descDepthStencilView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDepthStencilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDepthStencilView.Flags = 0;

			if (FAILED(m_pDevice.GetDevice()->CreateDepthStencilView(
				pDepthStencilTex.Get(),
				&descDepthStencilView,
				m_pDepthStencileView.GetAddressOf()
			)))
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

			ComPtr<ID3D11RasterizerState> rasterizerState;

			if (FAILED(m_pDevice.GetDevice()->CreateRasterizerState(
				&descRasterizer,
				rasterizerState.GetAddressOf()
			)))
			{
				throw std::runtime_error("!Failed to Create Rasterizer State");
			}

			m_pDevice.GetDeviceContext()->RSSetState(rasterizerState.Get());
			m_pDeferredContext->RSSetState(rasterizerState.Get());
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
			if (FAILED(m_pDevice.GetDevice()->CreateSamplerState(
				&descSampler,
				samplerState.GetAddressOf()
			)))
			{
				throw "!Failed to create sampler state";
			}

			m_pDevice.GetDeviceContext()->PSSetSamplers(0, 1, samplerState.GetAddressOf());
		}

	}
	catch (std::runtime_error& e) {
		SnctRuntimeError(e);
	}

	{
		m_viewport.Width = (FLOAT)g_screenWidth;
		m_viewport.Height = (FLOAT)g_screenHeight;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;

		m_pDevice.GetDeviceContext()->RSSetViewports(1, &m_viewport);
		m_pDeferredContext->RSSetViewports(1, &m_viewport);
	}

	//m_pShaderLibrary = std::make_unique<SnctShaderLibrary>();
	//m_pShaderLibrary->CreateShaderFromFile("n_vertex", L"n_vertex.hlsl", DX_SHADER_TYPE::VS);
	//m_pShaderLibrary->CreateShaderFromFile("n_pixel", L"n_pixel.hlsl", DX_SHADER_TYPE::PS);
}

void SnctDX11Render::RenderBegin()
{
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

	//　Immadiate　と deferred　の双方をクリア

	m_pDevice.GetDeviceContext()->ClearRenderTargetView(m_pBackBufferView.GetRTV(), clearColor);
	m_pDevice.GetDeviceContext()->ClearDepthStencilView(m_pDepthStencileView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pDevice.GetDeviceContext()->OMSetRenderTargets(1, m_pBackBufferView.GetRTVAddress(), m_pDepthStencileView.Get());
	m_pDevice.GetDeviceContext()->RSSetViewports(1, &m_viewport);

	m_pDeferredContext->ClearRenderTargetView(m_pBackBufferView.GetRTV(), clearColor);
	m_pDeferredContext->ClearDepthStencilView(m_pDepthStencileView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pDeferredContext->OMSetRenderTargets(1, m_pBackBufferView.GetRTVAddress(), m_pDepthStencileView.Get());
	m_pDeferredContext->RSSetViewports(1, &m_viewport);
}

void SnctDX11Render::RenderEnd()
{
	try
	{
		ComPtr<ID3D11CommandList> pCommandList;

		// !　ここでDeferred contextで設定したものをCommandListとして登録
		m_pDeferredContext->FinishCommandList(true, pCommandList.GetAddressOf());

		// Immadiate context で　実行命令
		m_pDevice.GetDeviceContext()->ExecuteCommandList(pCommandList.Get(), false);

		//　実質的に運用方法は、Immadiate と変わらない
		//　commandList も存在するが
		//
		//  Deferred から FinishCommandList で受け渡し
		//　Immadiate で Execute する
		//  そのため CommandListは一時オブジェクトで問題ないかも?

		if (FAILED(m_pSwapChain->Present(1, 0)))
			throw "!Failed to swap chain present";
	}
	catch (const char* error) {
		assert(false && error);
	}


}

void SnctDX11Render::Draw(HashKey key, SNCT_DRAW_FLAG drawFlag)
{
	// 仮作成　（移動予定）

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	//　型は Immadiateと変わらないので直前の設定が可能　
	m_pDeferredContext->VSSetShader(nullptr/*頂点シェーダー*/, nullptr, 0);
	m_pDeferredContext->PSSetShader(nullptr/*ピクセルシェーダー*/, nullptr, 0);

	m_pDeferredContext->VSSetConstantBuffers(0, 1, nullptr/*コンスタントバッファー*/);
	m_pDeferredContext->PSSetConstantBuffers(0, 1, nullptr/*コンスタントバッファー*/);

	m_pDeferredContext->IASetVertexBuffers(0, 1, nullptr/*vertex buffer*/, &stride, &offset);
	m_pDeferredContext->IASetInputLayout(nullptr/*layout*/);
	m_pDeferredContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 12と共通化のため　DrawIndexedInstanced　を使用　Drawなども使用可
	m_pDeferredContext->DrawIndexedInstanced(0/*IndexNum*/, 1, 0, 0, 0);
}

void SnctDX11Render::CreateObject(HashKey key, Vertices* pVertices, Indices* pIndices)
{
}
