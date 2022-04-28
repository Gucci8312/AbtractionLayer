#include "Snct_DX12Context.h"

//------------------------------------------------------------------------------
/// Clear render target view
/// \param[in]		Descriptor handle
/// \param[in]		Num
/// \param[in]		Rect
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12Context::ClearRTV(ISnctDXRTV* DescriptorHandle, float clearColor[4], UINT NumRects, RECT* pRects)
{
	SnctDX12RTV* TempRTV = static_cast<SnctDX12RTV*>(DescriptorHandle);
	m_pCmdList->ClearRenderTargetView(TempRTV->GetHandle(), clearColor, NumRects, pRects);
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
void SnctDX12Context::ClearDSV(ISnctDXDSV* DescriptorHandle, UINT Flag, float Depth, UINT8 Stencil, UINT NumRects, RECT* pRects)
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
void SnctDX12Context::Reset( ID3D12PipelineState* PipelineState)
{
	m_pCmdAllocator->Reset();
	m_pCmdList->Reset(m_pCmdAllocator.Get(), PipelineState);
}


//------------------------------------------------------------------------------
/// Reset command list
/// \param[in]		Num
/// \param[in]		Descriptor handle
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12Context::SetRTV(UINT NumDescriptors, ISnctDXRTV* DescriptorHandle, ISnctDXDSV* DSHandle, bool SingleHandleToDescriptorRange)
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
void SnctDX12Context::SetViewPort(float Width, float Height, float MinDepth, float MaxDepth)
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
void SnctDX12Context::SetScissorRects(float Width, float Height)
{
	// Scissor rectangle settings
	D3D12_RECT	scissor = {};
	scissor.left = 0;
	scissor.right = (LONG)Width;
	scissor.top = 0;
	scissor.bottom = (LONG)Height;
	m_pCmdList->RSSetScissorRects(1, &scissor);
}

void SnctDX12Context::SetPipelineState(ID3D12PipelineState* pPipelineState)
{
	m_pCmdList->SetPipelineState(pPipelineState);
}

void SnctDX12Context::SetGraphicsRootSignature(ID3D12RootSignature* pRootsignature)
{
	m_pCmdList->SetGraphicsRootSignature(pRootsignature);
}

void SnctDX12Context::SetVertexBuffer(UINT bufferNum, ISnctDXBuffer* pBuffer, UINT stride, UINT vertexNum)
{
	ID3D12Resource* tempBuffer = static_cast<SnctDX12Buffer*>(pBuffer)->GetBuffer();

	D3D12_VERTEX_BUFFER_VIEW		vertexBufferView{};
	vertexBufferView.BufferLocation = tempBuffer->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = stride;
	vertexBufferView.SizeInBytes = vertexNum * stride;

	m_pCmdList->IASetVertexBuffers(0, 1, &vertexBufferView);
}

void SnctDX12Context::SetIndexBuffer(ISnctDXBuffer* pBuffer, DXGI_FORMAT format, UINT size)
{
	D3D12_INDEX_BUFFER_VIEW			indexBufferView{};
	ID3D12Resource* tempBuffer = static_cast<SnctDX12Buffer*>(pBuffer)->GetBuffer();
	indexBufferView.BufferLocation = tempBuffer->GetGPUVirtualAddress();
	indexBufferView.Format = format;
	indexBufferView.SizeInBytes = size;

	m_pCmdList->IASetIndexBuffer(&indexBufferView);
}

void SnctDX12Context::SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology)
{
	m_pCmdList->IASetPrimitiveTopology(topology);
}

void SnctDX12Context::DrawIndexedInstanced(UINT indexCount, UINT startIndexLocation, UINT instanceLocation)
{
	m_pCmdList->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
}


//------------------------------------------------------------------------------
/// Close command list
/// \param			none
/// \return			none
//------------------------------------------------------------------------------
void SnctDX12Context::Close()
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
void SnctDX12Context::SetResourceBarrier(ISnctDXBuffer* Resource, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After)
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