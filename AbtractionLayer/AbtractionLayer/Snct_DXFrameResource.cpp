#include "Snct_DXFrameResource.h"

SnctDX12FrameResource::SnctDX12FrameResource(ID3D12Device* pDevice, ID3D12PipelineState* pPso, ID3D12DescriptorHeap* pDsvHeap, D3D12_VIEWPORT* pViewport, UINT frameResourceIndex)
{
	m_nFenceValue		= 0;
	m_pPipelineState	= pPso;

	for (UINT i = 0; i < CmdListCount; ++i)
	{
		pDevice->CreateCommandAllocator	(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_pCommandAlocators[i].GetAddressOf()));
		pDevice->CreateCommandList		(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCommandAlocators[i].Get(), m_pPipelineState.Get(), IID_PPV_ARGS(m_pCommandLists[i].GetAddressOf()));
		m_pCommandLists[i]->Close();
	}

	for (UINT i = 0; i < ThreadNum; ++i)
	{
		pDevice->CreateCommandAllocator	(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_pSceneCmdAlocators[i].GetAddressOf()));
		pDevice->CreateCommandList		(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pSceneCmdAlocators[i].Get(), m_pPipelineState.Get(), IID_PPV_ARGS(m_pSceneCmdLists[i].GetAddressOf()));
		m_pSceneCmdLists[i]->Close();
	}
}

void SnctDX12FrameResource::SetRTV(ID3D12GraphicsCommandList* pCmdList, D3D12_CPU_DESCRIPTOR_HANDLE* pRtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE* pDsvHandle)
{
	pCmdList->OMSetRenderTargets(1, pRtvHandle, false, pDsvHandle);
}

void SnctDX12FrameResource::CommandReset()
{
	for (int i = 0; i < CmdListCount; ++i)
	{
		m_pCommandAlocators[i]->Reset();
		m_pCommandLists[i]->Reset(m_pCommandAlocators[i].Get(), m_pPipelineState.Get());
	}

	for (int i = 0; i < ThreadNum; ++i)
	{
		m_pSceneCmdAlocators[i]->Reset();
		m_pSceneCmdLists[i]->Reset(m_pSceneCmdAlocators[i].Get(), m_pPipelineState.Get());
	}
}

void SnctDX12FrameResource::SwapBarriar()
{
}
