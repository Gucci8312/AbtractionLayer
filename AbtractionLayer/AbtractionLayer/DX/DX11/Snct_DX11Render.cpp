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
			if (FAILED(m_pDevice.CreateDevice(D3D_FEATURE_LEVEL_11_1)))
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
			m_pDevice.CreateDeferredContext(m_pDeferredContext.GetContextAddress());

			// 型自体も　Immadiateと変わらないので　同じ設定ができる。もしくは、する必要がある。
		}

		{
			SnctDX11Texture pBackBuffer;
			ID3D11Texture2D* TempRTVBuffer;
			if (FAILED(m_pSwapChain->GetBuffer(
				0,
				IID_PPV_ARGS(&TempRTVBuffer)
			)))
			{
				throw std::runtime_error("!Failed to get back buffer");
			}
			pBackBuffer.SetTextureAddress(TempRTVBuffer);
			if (FAILED(m_pDevice.CreateRTV(&pBackBuffer, &m_pBackBufferView)))
			{
				throw std::runtime_error("!Failed to create render target buffer");
			}
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

			if (FAILED(pDepthStencilTex.Create(m_pDevice.GetDevice(),
				descDepthTex)))
			{
				throw std::runtime_error("!Failed to create depth stencil buffer texture");
			}

			D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView{};
			descDepthStencilView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDepthStencilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDepthStencilView.Flags = 0;

			if (FAILED(m_pDevice.CreateDSV(&pDepthStencilTex, &m_pDepthStencileView)))
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

			m_pDevice.SetRasterizerState(rasterizerState.Get());
			m_pDeferredContext.SetRasterizerState(rasterizerState.Get());
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

		m_pSceneObjects = std::make_unique<SnctDX11Objects>();

		TEST_CODE_CreateCameraConstantBuffer();

	}
	catch (std::runtime_error& e) {
		SnctRuntimeError(e);
	}

	{
		m_pDevice.SetViewPort((FLOAT)g_screenWidth, (FLOAT)g_screenHeight, 0.0f, 1.0f);
		m_pDeferredContext.SetViewPort((FLOAT)g_screenWidth, (FLOAT)g_screenHeight, 0.0f, 1.0f);
	}

	m_pShaderLibrary = std::make_unique<SnctShaderLibrary>();
	m_pShaderLibrary->CreateShaderFromFile("n_vertex", L"../../AbtractionLayer/AbtractionLayer/DX/Shader/n_vertex.hlsl", DX_SHADER_TYPE::VS);
	m_pShaderLibrary->CreateShaderFromFile("n_pixel", L"../../AbtractionLayer/AbtractionLayer/DX/Shader/n_pixel.hlsl", DX_SHADER_TYPE::PS);
}

void SnctDX11Render::RenderBegin()
{
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

	//　Immadiate　と deferred　の双方をクリア
	// Immadiate context command
	m_pDevice.ClearRTV(&m_pBackBufferView, clearColor);
	m_pDevice.ClearDSV(&m_pDepthStencileView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pDevice.SetRTV(1, &m_pBackBufferView, &m_pDepthStencileView);
	m_pDevice.SetViewPort((FLOAT)g_screenWidth, (FLOAT)g_screenHeight, 0.0f, 1.0f);

	// Deferred context command
	m_pDeferredContext.ClearRTV(&m_pBackBufferView, clearColor);
	m_pDeferredContext.ClearDSV(&m_pDepthStencileView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pDeferredContext.SetRTV(1, &m_pBackBufferView, &m_pDepthStencileView);
	m_pDeferredContext.SetViewPort((FLOAT)g_screenWidth, (FLOAT)g_screenHeight, 0.0f, 1.0f);
}

void SnctDX11Render::RenderEnd()
{
	try
	{
		ComPtr<ID3D11CommandList> pCommandList;

		// !　ここでDeferred contextで設定したものをCommandListとして登録
		m_pDeferredContext.RegisterCmdList(pCommandList.ReleaseAndGetAddressOf());

		// Immadiate context で　実行命令
		m_pDevice.ExecuteCmdList(pCommandList.Get());

		//　実質的に運用方法は、Immadiate と変わらない
		//　commandList も存在するが
		//
		//  Deferred から FinishCommandList で受け渡し
		//　Immadiate で Execute する
		//  そのため CommandListは一時オブジェクトで問題ないかも?

		if (FAILED(m_pSwapChain->Present(1, 0)))
			throw std::runtime_error("!Failed to swap chain present");
	}
	catch (std::runtime_error &e)
	{
		SnctRuntimeError(e);
	}
}

void SnctDX11Render::Draw(HashKey key, SNCT_DRAW_FLAG drawFlag)
{
	SnctDX11ObjectBuffer* object = m_pSceneObjects->GetObjectBuffer(key);

	unsigned int strides = sizeof(Vertex);
	unsigned int offsets = 0;

	UpdateObjectBuffer(object->pConstantObject.Get());

	m_pDeferredContext.GetContext()->VSSetConstantBuffers(0, 1, TEST_CODE_m_pCameraConstant.GetAddressOf());
	m_pDeferredContext.GetContext()->PSSetConstantBuffers(0, 1, TEST_CODE_m_pCameraConstant.GetAddressOf());

	m_pDeferredContext.GetContext()->VSSetConstantBuffers(1, 1, object->pConstantObject.GetAddressOf());
	m_pDeferredContext.GetContext()->PSSetConstantBuffers(1, 1, object->pConstantObject.GetAddressOf());

	m_pDeferredContext.GetContext()->IASetVertexBuffers(0, 1, object->pVertexBuffer.GetAddressOf(), &strides, &offsets);
	m_pDeferredContext.GetContext()->IASetIndexBuffer(object->pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	m_pDeferredContext.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pDeferredContext.GetContext()->DrawIndexedInstanced(object->nIndexSize, 100, 0, 0, 0);
}

void SnctDX11Render::CreateObject(HashKey key, Vertices* pVertices, Indices* pIndices)
{
	m_pSceneObjects->AddSceneObject(m_pDevice.GetDevice(), key, pVertices, pIndices);
}

void SnctDX11Render::DrawIndexed(SnctDX11ObjectBuffer* pObject)
{
	// Draw内へ移設

	//unsigned int strides = sizeof(Vertex);
	//unsigned int offsets = 0;

	//UpdateObjectBuffer(pObject->pConstantObject.Get());

	//m_pDeferredContext.GetContext()->VSSetConstantBuffers(0, 1, TEST_CODE_m_pCameraConstant.GetAddressOf());
	//m_pDeferredContext.GetContext()->PSSetConstantBuffers(0, 1, TEST_CODE_m_pCameraConstant.GetAddressOf());

	//m_pDeferredContext.GetContext()->VSSetConstantBuffers(1, 1, pObject->pConstantObject.GetAddressOf());
	//m_pDeferredContext.GetContext()->PSSetConstantBuffers(1, 1, pObject->pConstantObject.GetAddressOf());

	//m_pDeferredContext.GetContext()->IASetVertexBuffers(0, 1, pObject->pVertexBuffer.GetAddressOf(), &strides, &offsets);
	//m_pDeferredContext.GetContext()->IASetIndexBuffer(pObject->pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	//m_pDeferredContext.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//m_pDeferredContext.GetContext()->DrawIndexedInstanced(pObject->nIndexSize, 100, 0, 0, 0);
}

void SnctDX11Render::UpdateCameraBuffer(ID3D11Buffer* pCameraConstant)
{
	m_pDeferredContext.GetContext()->UpdateSubresource(pCameraConstant, 0, nullptr, m_pConstantCamera.get(), 0, 0);
}

void SnctDX11Render::UpdateObjectBuffer(ID3D11Buffer* pObjectConstant)
{
	m_pDeferredContext.GetContext()->UpdateSubresource(pObjectConstant, 0, nullptr, m_pConstantCamera.get(), 0, 0);
}

void SnctDX11Render::TEST_CODE_CreateCameraConstantBuffer()
{
	try 
	{
		D3D11_BUFFER_DESC descConstantBuffer{};

		descConstantBuffer.ByteWidth = (UINT)sizeof(XMConstantObject);
		descConstantBuffer.Usage = D3D11_USAGE_DEFAULT;
		descConstantBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		descConstantBuffer.CPUAccessFlags = 0;
		descConstantBuffer.MiscFlags = 0;
		descConstantBuffer.StructureByteStride = 0;

		if (FAILED(m_pDevice.GetDevice()->CreateBuffer(
			&descConstantBuffer,
			nullptr,
			TEST_CODE_m_pCameraConstant.GetAddressOf()
		)))
			throw std::runtime_error("!Failed to Create Constant Matrix Buffer");
	}
	catch (std::runtime_error& e)
	{
		SnctRuntimeError(e);
	}
}

void SnctDX11Render::TEST_CODE_CreatePipelineState()
{
	//m_pDeferredContext.GetContext()->VSSetShader		();
	//m_pDeferredContext.GetContext()->PSSetShader		();
	//m_pDeferredContext.GetContext()->S		();

}
