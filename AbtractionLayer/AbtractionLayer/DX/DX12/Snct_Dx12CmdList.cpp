#include "Snct_Dx12CmdList.h"

//------------------------------------------------------------------------------
/// Create a command list
/// \param[in]		Command list type
/// \param[in]		Device
/// \param[in]		CommndAllocator
/// \return			HRESULT
//------------------------------------------------------------------------------
HRESULT SnctDX12CmdList::Create(D3D12_COMMAND_LIST_TYPE Type, ID3D12Device* Device, ID3D12CommandAllocator* CmdAllocator)
{
	return Device->CreateCommandList(0, Type,
		CmdAllocator, nullptr, IID_PPV_ARGS(m_pCmdList.ReleaseAndGetAddressOf()));
}


//------------------------------------------------------------------------------
/// Clear render target view
/// \param[in]		Descriptor handle
/// \param[in]		Num
/// \param[in]		Rect
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12CmdList::ClearRTV(ISnctDXRTV* DescriptorHandle, UINT NumRects, RECT* pRects )
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	SnctDX12RTV* TempRTV = static_cast<SnctDX12RTV*>(DescriptorHandle);
	m_pCmdList->ClearRenderTargetView(TempRTV->GetRTV(), clearColor, 0, pRects);
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
void SnctDX12CmdList::ClearDSV(ISnctDXDSV* DescriptorHandle, UINT Flag, float Depth, UINT8 Stencil, UINT NumRects, RECT* pRects )
{
	SnctDX12DSV* TempDSV = static_cast<SnctDX12DSV*>(DescriptorHandle);
	m_pCmdList.Get()->ClearDepthStencilView(*TempDSV->GetpHandle(), static_cast<D3D12_CLEAR_FLAGS>(Flag), Depth, Stencil, NumRects, pRects);
}


//------------------------------------------------------------------------------
/// Reset command list
/// \param[in]		Command allocator
/// \param[in]		Pipeline state
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12CmdList::Reset(ID3D12CommandAllocator* CmdAllocator, ID3D12PipelineState* PipelineState)
{
	m_pCmdList->Reset(CmdAllocator, PipelineState);
}


//------------------------------------------------------------------------------
/// Reset command list
/// \param[in]		Num
/// \param[in]		Descriptor handle
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12CmdList::SetRTV(UINT NumDescriptors, ISnctDXRTV* DescriptorHandle, ISnctDXDSV* DSHandle, bool SingleHandleToDescriptorRange)
{
	SnctDX12RTV* TempRTV = static_cast<SnctDX12RTV*>(DescriptorHandle);
	SnctDX12DSV* TempDSV = static_cast<SnctDX12DSV*>(DSHandle);
	m_pCmdList.Get()->OMSetRenderTargets(NumDescriptors, TempRTV->GetpHandle(), SingleHandleToDescriptorRange, TempDSV->GetpHandle());
}


//------------------------------------------------------------------------------
/// Set view port
/// \param[in]		Screen width
/// \param[in]		Screen height
/// \param[in]		Min depth
/// \param[in]		Max depth
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12CmdList::SetViewPort(float Width, float Height, float MinDepth, float MaxDepth)
{
	// View port settings
	D3D12_VIEWPORT	viewPort = {};
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = static_cast<float>(Width);
	viewPort.Height = static_cast<float>(Height);
	viewPort.MinDepth = MinDepth;
	viewPort.MaxDepth = MaxDepth;

	m_pCmdList->RSSetViewports(1, &viewPort);
}


//------------------------------------------------------------------------------
/// Set scissor rects
/// \param[in]		Screen width
/// \param[in]		Screen height
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12CmdList::SetScissorRects(float Width, float Height)
{
	// Scissor rectangle settings
	D3D12_RECT	scissor = {};
	scissor.left = 0;
	scissor.right = (LONG)Width;
	scissor.top = 0;
	scissor.bottom = (LONG)Height;
	m_pCmdList->RSSetScissorRects(1, &scissor);
}


//------------------------------------------------------------------------------
/// Close command list
/// \param			none
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12CmdList::Close()
{
	m_pCmdList->Close();
}


//------------------------------------------------------------------------------
/// Set resource barrier
/// \param[in]		Resource
/// \param[in]		Screen height
/// \param[in]		Before state
/// \param[in]		After state
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12CmdList::SetResourceBarrier(ISnctDXBuffer* Resource, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After)
{
	SnctDX12Buffer* tempResource = static_cast<SnctDX12Buffer*>(Resource);

	// Resource barrier settings
	D3D12_RESOURCE_BARRIER BarrierDesc;
	ZeroMemory(&BarrierDesc, sizeof(BarrierDesc));
	BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	BarrierDesc.Transition.pResource = tempResource->GetBuffer();
	BarrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	BarrierDesc.Transition.StateBefore = Before;
	BarrierDesc.Transition.StateAfter = After;

	// Set resource barrier
	m_pCmdList->ResourceBarrier(1, &BarrierDesc);
}