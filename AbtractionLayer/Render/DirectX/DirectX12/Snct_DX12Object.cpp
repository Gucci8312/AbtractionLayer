#include "Snct_DX12Object.h"

bool SnctDX12Object::Create(ISnctDXDevice* pDevice)
{
	SnctDX12Device* pTempDevice = static_cast<SnctDX12Device*>(pDevice);

	SnctDX12Buffer* pTempVBuffer = static_cast<SnctDX12Buffer*>(m_pVBuffer);
	SnctDX12Buffer* pTempIBuffer = static_cast<SnctDX12Buffer*>(m_pIBuffer);

	ComPtr<ID3DBlob> signature;
	
	D3D12_ROOT_PARAMETER	rootParams[2] = {};
	D3D12_DESCRIPTOR_RANGE	camRange{}, objRange{};
			
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

	// sampler	(s0)
	D3D12_STATIC_SAMPLER_DESC descStaticSampler{};
	descStaticSampler.Filter					= D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	descStaticSampler.AddressU					= D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	descStaticSampler.AddressV					= D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	descStaticSampler.AddressW					= D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	descStaticSampler.MipLODBias				= 0.0f;
	descStaticSampler.MaxAnisotropy				= 16;
	descStaticSampler.ComparisonFunc			= D3D12_COMPARISON_FUNC_NEVER;
	descStaticSampler.BorderColor				= D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	descStaticSampler.MinLOD					= 0.0f;
	descStaticSampler.MaxLOD					= D3D12_FLOAT32_MAX;
	descStaticSampler.ShaderRegister			= 0;
	descStaticSampler.RegisterSpace				= 0;
	descStaticSampler.ShaderVisibility			= D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_SIGNATURE_DESC descRootSignature{};
	descRootSignature.Flags						= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	descRootSignature.pParameters				= rootParams;
	descRootSignature.NumParameters				= _countof(rootParams);
	descRootSignature.pStaticSamplers			= &descStaticSampler;
	descRootSignature.NumStaticSamplers			= 1;

	D3D12SerializeRootSignature(
		&descRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&signature,
		nullptr
	);
	
	pTempDevice->GetDevice()->CreateRootSignature(
		0,
		signature->GetBufferPointer(),
		signature->GetBufferSize(),
		IID_PPV_ARGS(m_pRootSignature.GetAddressOf())
	);


	D3D12_GRAPHICS_PIPELINE_STATE_DESC descPipeline{};
	descPipeline.pRootSignature	= m_pRootSignature.Get();

	// vertex
	{
		DX12VertexShader* vertex = pShaderCompiler->LoadShader<DX12VertexShader>(DX12_SHADER_TYPE::VERTEX, L"vertex.hlsl");

		D3D12_SHADER_BYTECODE shaderBytecode{};
		shaderBytecode.pShaderBytecode	= vs->GetBufferPointer();
		shaderBytecode.BytecodeLength	= vs->GetBufferSize();

		descPipeline.InputLayout.pInputElementDescs		= vertex->layout.data();
		descPipeline.InputLayout.NumElements			= (UINT)vertex->layout.size();
		descPipeline.VS									= shaderBytecode;
	}

	// pixe
	{
		DX12PixelShader* pixel = pShaderCompiler->LoadShader<DX12PixelShader>(DX12_SHADER_TYPE::PIXEL, L"pixel.hlsl");

		D3D12_SHADER_BYTECODE shaderBytecode{};
		shaderBytecode.pShaderBytecode	= ps->GetBufferPointer();
		shaderBytecode.BytecodeLength	= ps->GetBufferSize();
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
	descPipeline.RTVFormats[0]					= DXGI_FORMAT_R8G8B8A8_UNORM;
	descPipeline.DSVFormat						= DXGI_FORMAT_D32_FLOAT;
	descPipeline.SampleMask						= UINT_MAX;
	descPipeline.SampleDesc						= {1,0};

	pTempDevice->GetDevice()->CreateGraphicsPipelineState(
		&descPipeline,
		IID_PPV_ARGS(m_pPipelineState.GetAddressOf())
	);

	return false;
}

void SnctDX12Object::Draw(ISnctDXContext* pCmdList)
{
	SnctDX12Context* pTempCmdList = static_cast<SnctDX12Context*>(pCmdList);

	pTempCmdList->SetPipelineState			(m_pPipelineState.Get());
	pTempCmdList->SetIndexBuffer			(m_pIBuffer, DXGI_FORMAT_R32_UINT, m_Indices.size());
	pTempCmdList->SetVertexBuffer			(1, m_pVBuffer, sizeof(Vertex), m_Vertices.size());
	pTempCmdList->SetPrimitiveTopology		(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pTempCmdList->SetViewPort				(g_screenWidth, g_screenHeight, 0.0f, 1.0f);
	pTempCmdList->SetScissorRects			(g_screenWidth, g_screenHeight);
	pTempCmdList->SetGraphicsRootSignature	(m_pRootSignature.Get());
	
}
