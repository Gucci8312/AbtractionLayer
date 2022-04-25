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
			// �f�t�@�[�h�R���e�N�X�g�̍쐬
			m_pDevice.CreateDeferredContext(m_pDeferredContext.GetContextAddress());

			// �^���̂��@Immadiate�ƕς��Ȃ��̂Ł@�����ݒ肪�ł���B�������́A����K�v������B
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

	}
	catch (std::runtime_error& e) {
		SnctRuntimeError(e);
	}

	{
		//m_viewport.Width = (FLOAT)g_screenWidth;
		//m_viewport.Height = (FLOAT)g_screenHeight;
		//m_viewport.MinDepth = 0.0f;
		//m_viewport.MaxDepth = 1.0f;
		//m_viewport.TopLeftX = 0.0f;
		//m_viewport.TopLeftY = 0.0f;

		m_pDevice.SetViewPort((FLOAT)g_screenWidth, (FLOAT)g_screenHeight, 0.0f, 1.0f);
		m_pDeferredContext.SetViewPort((FLOAT)g_screenWidth, (FLOAT)g_screenHeight, 0.0f, 1.0f);
	}

	//m_pShaderLibrary = std::make_unique<SnctShaderLibrary>();
	//m_pShaderLibrary->CreateShaderFromFile("n_vertex", L"n_vertex.hlsl", DX_SHADER_TYPE::VS);
	//m_pShaderLibrary->CreateShaderFromFile("n_pixel", L"n_pixel.hlsl", DX_SHADER_TYPE::PS);
}

void SnctDX11Render::RenderBegin()
{
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

	//�@Immadiate�@�� deferred�@�̑o�����N���A
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

		// !�@������Deferred context�Őݒ肵�����̂�CommandList�Ƃ��ēo�^
		m_pDeferredContext.RegisterCmdList(pCommandList.ReleaseAndGetAddressOf());

		// Immadiate context �Ł@���s����
		m_pDevice.ExecuteCmdList(pCommandList.Get());

		//�@�����I�ɉ^�p���@�́AImmadiate �ƕς��Ȃ�
		//�@commandList �����݂��邪
		//
		//  Deferred ���� FinishCommandList �Ŏ󂯓n��
		//�@Immadiate �� Execute ����
		//  ���̂��� CommandList�͈ꎞ�I�u�W�F�N�g�Ŗ��Ȃ�����?

		if (FAILED(m_pSwapChain->Present(1, 0)))
			throw "!Failed to swap chain present";
	}
	catch (const char* error) {
		assert(false && error);
	}


}

void SnctDX11Render::Draw(HashKey key, SNCT_DRAW_FLAG drawFlag)
{
	//// ���쐬�@�i�ړ��\��j

	//UINT stride = sizeof(Vertex);
	//UINT offset = 0;

	////�@�^�� Immadiate�ƕς��Ȃ��̂Œ��O�̐ݒ肪�\�@
	//m_pDeferredContext.GetContext()->VSSetShader(nullptr/*���_�V�F�[�_�[*/, nullptr, 0);
	//m_pDeferredContext.GetContext()->PSSetShader(nullptr/*�s�N�Z���V�F�[�_�[*/, nullptr, 0);

	//m_pDeferredContext.GetContext()->VSSetConstantBuffers(0, 1, nullptr/*�R���X�^���g�o�b�t�@�[*/);
	//m_pDeferredContext.GetContext()->PSSetConstantBuffers(0, 1, nullptr/*�R���X�^���g�o�b�t�@�[*/);

	//m_pDeferredContext.GetContext()->IASetVertexBuffers(0, 1, nullptr/*vertex buffer*/, &stride, &offset);
	//m_pDeferredContext.GetContext()->IASetInputLayout(nullptr/*layout*/);
	//m_pDeferredContext.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//// 12�Ƌ��ʉ��̂��߁@DrawIndexedInstanced�@���g�p�@Draw�Ȃǂ��g�p��
	//m_pDeferredContext.GetContext()->DrawIndexedInstanced(0/*IndexNum*/, 1, 0, 0, 0);
}

void SnctDX11Render::CreateObject(HashKey key, Vertices* pVertices, Indices* pIndices)
{
}
