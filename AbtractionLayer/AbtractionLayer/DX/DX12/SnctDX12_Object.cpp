#include "SnctDX12_Object.h"
#include "../../Snct_Utility.h"

void SnctDX12Object::Create(ISnctDxDevice* pDevice, HashKey key, Vertices* pVertices, Indices* pIndices)
{
	SnctDX12Device* pTempDevice = dynamic_cast<SnctDX12Device*>(pDevice);
	m_pShaderLibrary = std::make_unique<SnctShaderLibrary>();

	m_pShaderLibrary->CreateShaderFromFile("n_vertex.hlsl", L"../../AbtractionLayer/AbtractionLayer/DX/Shader/n_vertex.hlsl", DX_SHADER_TYPE::VS);
	m_pShaderLibrary->CreateShaderFromFile("n_pixel.hlsl", L"../../AbtractionLayer/AbtractionLayer/DX/Shader/n_pixel.hlsl", DX_SHADER_TYPE::PS);

	// root signature
	try
	{
		D3D12_ROOT_PARAMETER	rootParams[3] = {};
		D3D12_DESCRIPTOR_RANGE	camRange{}, objRange{}, srvRange{};

		// constant (b0)
		camRange.NumDescriptors = 1;
		camRange.BaseShaderRegister = 0;
		camRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		camRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParams[0].DescriptorTable = { 1, &camRange };

		// constant (b1)
		objRange.NumDescriptors = 1;
		objRange.BaseShaderRegister = 1;
		objRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		objRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParams[1].DescriptorTable = { 1, &objRange };

		// texture	(t0)
		srvRange.NumDescriptors = 1;
		srvRange.BaseShaderRegister = 0;
		srvRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		srvRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParams[2].DescriptorTable = { 1, &srvRange };

		// sampler	(s0)
		D3D12_STATIC_SAMPLER_DESC descStaticSampler{};
		descStaticSampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		descStaticSampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		descStaticSampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		descStaticSampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		descStaticSampler.MipLODBias = 0.0f;
		descStaticSampler.MaxAnisotropy = 16;
		descStaticSampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		descStaticSampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		descStaticSampler.MinLOD = 0.0f;
		descStaticSampler.MaxLOD = D3D12_FLOAT32_MAX;
		descStaticSampler.ShaderRegister = 0;
		descStaticSampler.RegisterSpace = 0;
		descStaticSampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		D3D12_ROOT_SIGNATURE_DESC descRootSignature{};
		descRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		descRootSignature.pParameters = rootParams;
		descRootSignature.NumParameters = _countof(rootParams);
		descRootSignature.pStaticSamplers = &descStaticSampler;
		descRootSignature.NumStaticSamplers = 1;

		ComPtr<ID3DBlob> signature;

		if (FAILED(D3D12SerializeRootSignature(
			&descRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1,
			&signature,
			nullptr)
		))
			throw std::runtime_error("!Failed to serialize root signature");

		if (FAILED(pTempDevice->GetDevice()->CreateRootSignature(
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
				shaderBytecode.pShaderBytecode = vertex->GetBufferPointer();
				shaderBytecode.BytecodeLength = vertex->GetBufferSize();

				descPipeline.InputLayout.pInputElementDescs = descInputLayout;
				descPipeline.InputLayout.NumElements = _countof(descInputLayout);
				descPipeline.VS = shaderBytecode;
			}

			// pixe
			{
				ID3DBlob* pixel = m_pShaderLibrary->GetShaderBlob("n_pixel.hlsl");

				D3D12_SHADER_BYTECODE shaderBytecode{};
				shaderBytecode.pShaderBytecode = pixel->GetBufferPointer();
				shaderBytecode.BytecodeLength = pixel->GetBufferSize();
				descPipeline.PS = shaderBytecode;
			}

			// raterizer 
			{
				D3D12_RASTERIZER_DESC descRasterize{};
				descRasterize.FillMode = D3D12_FILL_MODE_SOLID;
				descRasterize.CullMode = D3D12_CULL_MODE_NONE;
				descRasterize.FrontCounterClockwise = false;
				descRasterize.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
				descRasterize.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
				descRasterize.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
				descRasterize.DepthClipEnable = true;
				descRasterize.MultisampleEnable = false;
				descRasterize.AntialiasedLineEnable = false;
				descRasterize.ForcedSampleCount = 0;
				descRasterize.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

				descPipeline.RasterizerState = descRasterize;
			}

			// blend
			{
				D3D12_BLEND_DESC descBlend{};
				descBlend.AlphaToCoverageEnable = false;
				descBlend.IndependentBlendEnable = false;

				for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
				{
					descBlend.RenderTarget[i].BlendEnable = false;
					descBlend.RenderTarget[i].LogicOpEnable = false;
					descBlend.RenderTarget[i].SrcBlend = D3D12_BLEND_ONE;
					descBlend.RenderTarget[i].DestBlend = D3D12_BLEND_ZERO;
					descBlend.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
					descBlend.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_ONE;
					descBlend.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ZERO;
					descBlend.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
					descBlend.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_NOOP;
					descBlend.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				}
				descPipeline.BlendState = descBlend;
			}

			// depth
			{
				D3D12_DEPTH_STENCIL_DESC descDepth;
				descDepth.DepthEnable = true;
				descDepth.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
				descDepth.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
				descDepth.StencilEnable = false;
				descDepth.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
				descDepth.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

				descDepth.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
				descDepth.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
				descDepth.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
				descDepth.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

				descDepth.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
				descDepth.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
				descDepth.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
				descDepth.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

				descPipeline.DepthStencilState = descDepth;
			}

			descPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			descPipeline.NumRenderTargets = 1;
			descPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			descPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
			descPipeline.SampleMask = UINT_MAX;
			descPipeline.SampleDesc = { 1,0 };

			if (FAILED(pTempDevice->GetDevice()->CreateGraphicsPipelineState(
				&descPipeline,
				IID_PPV_ARGS(TEST_CODE_m_pPipelineState.GetAddressOf())
			)))
				throw std::runtime_error("!Failed to create pso");
		}
	}
	catch (std::runtime_error& e)
	{
		SnctRuntimeError(e);
	}
	try {

		TEST_CODE_m_pCameraConstant.resize(2);
		TEST_CODE_m_cameraCBV.resize(2);

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

		pTempDevice->GetDevice()->CreateDescriptorHeap(&descDescriptorHeap, IID_PPV_ARGS(TEST_CODE_m_pCameraHeap.GetAddressOf()));
		UINT nDescSize = pTempDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		for (UINT i = 0; i < 2; ++i)
		{
			if (FAILED(pTempDevice->GetDevice()->CreateCommittedResource(
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

			pTempDevice->GetDevice()->CreateConstantBufferView(&descConstant, cbvCpuHandle);

			D3D12_GPU_DESCRIPTOR_HANDLE cbvGpuHandle{};
			cbvGpuHandle.ptr = TEST_CODE_m_pCameraHeap->GetGPUDescriptorHandleForHeapStart().ptr + static_cast<unsigned long long>(i) * nDescSize;
			TEST_CODE_m_cameraCBV[i] = cbvGpuHandle;
		}
	}
	catch (std::runtime_error& e)
	{
		SnctRuntimeError(e);
	}
	// create vertex & index map
	try
	{
		TEST_CODE_m_pConstantObject.resize(2);
		TEST_CODE_m_objectCBV.resize(2);

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

		m_vertexSize = (UINT)pVertices->size();
		m_indexSize = (UINT)pIndices->size();

		D3D12_RANGE readRange{ 0,0 };

		// vertex
		{
			descResource.Width = (UINT)sizeof(Vertex) * pVertices->size();

			if (FAILED(pTempDevice->GetDevice()->CreateCommittedResource(
				&heapProperties,
				D3D12_HEAP_FLAG_NONE,
				&descResource,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(m_vertexBuffer.GetBufferAddress()))))
				throw "!Failed to crate vertex buffer";

			UINT8* pVertexDataBegin;

			if (FAILED(m_vertexBuffer.GetBuffer()->Map(
				0,
				&readRange,
				reinterpret_cast<void**>(&pVertexDataBegin)
			)))
				throw "!Failed to copy vertex";

			memcpy(pVertexDataBegin, pVertices->data(), sizeof(Vertex) * pVertices->size());
			m_vertexBuffer.GetBuffer()->Unmap(0, nullptr);
		}

		// index
		{
			descResource.Width = (UINT)sizeof(UINT) * pIndices->size();

			if (FAILED(pTempDevice->GetDevice()->CreateCommittedResource(
				&heapProperties,
				D3D12_HEAP_FLAG_NONE,
				&descResource,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(m_indexBuffer.GetBufferAddress())
			)))
				throw std::runtime_error("!Failed to crate index buffer");

			UINT8* pIndexDataBegin;

			if (FAILED(m_indexBuffer.GetBuffer()->Map(
				0,
				&readRange,
				reinterpret_cast<void**>(&pIndexDataBegin)
			)))
				throw std::runtime_error("!Failed to copy index");

			memcpy(pIndexDataBegin, pIndices->data(), sizeof(UINT) * pIndices->size());
			m_indexBuffer.GetBuffer()->Unmap(0, nullptr);
		}

		// constnt 
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

			pTempDevice->GetDevice()->CreateDescriptorHeap(&descDescriptorHeap, IID_PPV_ARGS(TEST_CODE_m_pObjectHeap.GetAddressOf()));
			m_descSize = pTempDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			for (UINT i = 0; i < 2; ++i)
			{
				if (FAILED(pTempDevice->GetDevice()->CreateCommittedResource(
					&heapProperties,
					D3D12_HEAP_FLAG_NONE,
					&descResource,
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(TEST_CODE_m_pConstantObject[i].GetAddressOf())
				)))
					throw std::runtime_error("!Failed to create constant buffer");


				D3D12_CONSTANT_BUFFER_VIEW_DESC descConstant{};
				D3D12_CPU_DESCRIPTOR_HANDLE		cbvCpuHandle{};

				descConstant.BufferLocation = TEST_CODE_m_pConstantObject[i]->GetGPUVirtualAddress();
				descConstant.SizeInBytes = (UINT)((sizeof(XMConstantObject) + 0xff) & ~0xff);
				cbvCpuHandle.ptr = TEST_CODE_m_pObjectHeap->GetCPUDescriptorHandleForHeapStart().ptr + static_cast<unsigned long long>(i) * m_descSize;

				pTempDevice->GetDevice()->CreateConstantBufferView(&descConstant, cbvCpuHandle);

				D3D12_GPU_DESCRIPTOR_HANDLE cbvGpuHandle{};
				cbvGpuHandle.ptr = TEST_CODE_m_pObjectHeap->GetGPUDescriptorHandleForHeapStart().ptr + static_cast<unsigned long long>(i) * m_descSize;
				TEST_CODE_m_objectCBV[i] = cbvGpuHandle;
			}
		}
	}
	catch (std::runtime_error& e)
	{
		SnctRuntimeError(e);
	}
}




void SnctDX12Object::Draw(int nowFrame, ISnctDXContext* pCmdList, XMConstantObject* pObjectData)
{
	// Set object constant buffer

	void* pObjectDataBegin;
	D3D12_RANGE range{ 0,0 };

	TEST_CODE_m_pConstantObject[nowFrame].Get()->Map(
		0,
		&range,
		&pObjectDataBegin
	);

	memcpy(pObjectDataBegin, pObjectData, sizeof(XMConstantObject));
	TEST_CODE_m_pConstantObject[nowFrame].Get()->Unmap(0, nullptr);


	D3D12_VERTEX_BUFFER_VIEW		vertexBufferView{};
	vertexBufferView.BufferLocation = m_vertexBuffer.GetBuffer()->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = sizeof(Vertex);
	vertexBufferView.SizeInBytes = (UINT)sizeof(Vertex) * m_vertexSize;

	D3D12_INDEX_BUFFER_VIEW			indexBufferView{};
	indexBufferView.BufferLocation = m_indexBuffer.GetBuffer()->GetGPUVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	indexBufferView.SizeInBytes = (UINT)sizeof(UINT) * m_indexSize;

	SnctDX12Context* pTempCmdList = static_cast<SnctDX12Context*>(pCmdList);
	// Future : include factory pattern method




	pTempCmdList->Get()->SetGraphicsRootSignature(TEST_CODE_m_pRootSignature.Get());
	pTempCmdList->Get()->SetDescriptorHeaps(1, TEST_CODE_m_pCameraHeap.GetAddressOf());
	pTempCmdList->Get()->SetGraphicsRootDescriptorTable(0, TEST_CODE_m_cameraCBV[nowFrame]);

	pTempCmdList->Get()->SetDescriptorHeaps(1, TEST_CODE_m_pObjectHeap.GetAddressOf());
	pTempCmdList->Get()->SetGraphicsRootDescriptorTable(1, TEST_CODE_m_objectCBV[nowFrame]);

	//pTempCmdList->Get()->SetGraphicsRootConstantBufferView(0, m_12CBView[FrameIndex].Desc.BufferLocation);
	//pTempCmdList->Get()->SetGraphicsRootDescriptorTable(1, m_12Texture.HandleGPU);
	pTempCmdList->Get()->SetPipelineState(TEST_CODE_m_pPipelineState.Get());

	pTempCmdList->Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pTempCmdList->Get()->IASetVertexBuffers(0, 1, &vertexBufferView);
	pTempCmdList->Get()->IASetIndexBuffer(&indexBufferView);

	pTempCmdList->Get()->DrawIndexedInstanced(m_indexSize, 1, 0, 0, 0);
}

void SnctDX12Object::Update()
{
}
