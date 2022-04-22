#pragma once
#include "DX/DX12/Snct_DX12.h"
#include "Snct_Windows.h"

constexpr UINT ThreadNum	= 2;
constexpr UINT CmdListCount	= 2;

class SnctDX12FrameResource {

public:

	 SnctDX12FrameResource(
		 ID3D12Device*			pDevice,
		 ID3D12PipelineState*	pPso,
		 ID3D12DescriptorHeap*	pDsvHeap,
		 D3D12_VIEWPORT*		pViewport,
		 UINT					frameResourceIndex
	 );

	void SetRTV(
		ID3D12GraphicsCommandList*		pCmdList,
		D3D12_CPU_DESCRIPTOR_HANDLE*	pRtvHandle,
		D3D12_CPU_DESCRIPTOR_HANDLE*	pDsvHandle
	);

	void CommandReset	();
	void SwapBarriar	();

	ID3D12CommandList* m_pBatchLists[ThreadNum * 2 + CmdListCount];

	ComPtr<ID3D12CommandAllocator>		m_pCommandAlocators	[CmdListCount];
	ComPtr<ID3D12GraphicsCommandList>	m_pCommandLists		[CmdListCount];

	ComPtr<ID3D12CommandAllocator>		m_pSceneCmdAlocators[ThreadNum];
	ComPtr<ID3D12GraphicsCommandList>	m_pSceneCmdLists	[ThreadNum];

	UINT m_nFenceValue;

private:

	ComPtr<ID3D12PipelineState> m_pPipelineState;




};