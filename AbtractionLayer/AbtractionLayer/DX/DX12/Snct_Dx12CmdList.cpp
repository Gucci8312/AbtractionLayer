#include "Snct_Dx12CmdList.h"

//------------------------------------------------------------------------------
/// Create a command list
/// \param[in]		Command list type
/// \param[in]		Device
/// \param[in]		CommndAllocator
/// \return			HRESULT
//------------------------------------------------------------------------------
HRESULT SnctDx12CmdList::Create(D3D12_COMMAND_LIST_TYPE Type, ID3D12Device* Device, ID3D12CommandAllocator* CmdAllocator)
{
	return Device->CreateCommandList(0, Type,
		CmdAllocator, nullptr, IID_PPV_ARGS(m_cmdList.ReleaseAndGetAddressOf()));
}


//------------------------------------------------------------------------------
/// Clear render target view
/// \param[in]		Descriptor handle
/// \param[in]		Num
/// \param[in]		Rect
/// \return			none
//------------------------------------------------------------------------------
void SnctDx12CmdList::ClearRTV(D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle, UINT NumRects, D3D12_RECT* pRects)
{
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_cmdList->ClearRenderTargetView(DescriptorHandle, clearColor, 0, nullptr);
}


//------------------------------------------------------------------------------
/// Clear depth stencil view
/// \param[in]		Descriptor handle
/// \param[in]		Clear flag
/// \param[in]		Depth
/// \param[in]		Stencil
/// \param[in]		Num
/// \param[in]		Rects
/// \return			none
//------------------------------------------------------------------------------
void SnctDx12CmdList::ClearDSV(D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle, D3D12_CLEAR_FLAGS Flag, float Depth, unsigned short Stencil, UINT NumRects, D3D12_RECT* pRects)
{
	m_cmdList.Get()->ClearDepthStencilView(DescriptorHandle, Flag, Depth, Stencil, NumRects, pRects);
}


//------------------------------------------------------------------------------
/// Reset command list
/// \param[in]		Command allocator
/// \param[in]		Pipeline state
/// \return			none
//------------------------------------------------------------------------------
void SnctDx12CmdList::Reset(ID3D12CommandAllocator* CmdAllocator, ID3D12PipelineState* PipelineState)
{
	m_cmdList->Reset(CmdAllocator, PipelineState);
}


//------------------------------------------------------------------------------
/// Reset command list
/// \param[in]		Num
/// \param[in]		Descriptor handle
/// \return			none
//------------------------------------------------------------------------------
void SnctDx12CmdList::SetRTV(UINT NumDescriptors, ISnctRTV* DescriptorHandle, bool SingleHandleToDescriptorRange, D3D12_CPU_DESCRIPTOR_HANDLE* DSHandle)
{
	//D3D12_CPU_DESCRIPTOR_HANDLE* ReceiveHandle={};
	//DescriptorHandle->Get()
	//m_cmdList.Get()->OMSetRenderTargets(NumDescriptors, ReceiveHandle, SingleHandleToDescriptorRange, DSHandle);
}


void SnctDx12CmdList::SetViewPort(float Width, float Height, float MinDepth, float MaxDepth)
{
	// View port settings
	D3D12_VIEWPORT	viewPort = {};
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = static_cast<float>(Width);
	viewPort.Height = static_cast<float>(Height);
	viewPort.MinDepth = MinDepth;
	viewPort.MaxDepth = MaxDepth;

	m_cmdList->RSSetViewports(1, &viewPort);
}


void SnctDx12CmdList::SetScissorRects(float Width, float Height)
{
	// Scissor rectangle settings
	D3D12_RECT	scissor = {};
	scissor.left = 0;
	scissor.right = Width;
	scissor.top = 0;
	scissor.bottom = Height;
	m_cmdList->RSSetScissorRects(1, &scissor);
}


void SnctDx12CmdList::Close()
{
	m_cmdList->Close();
}


void SnctDx12CmdList::SetResourceBarrier(ID3D12Resource* Resource, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After)
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
