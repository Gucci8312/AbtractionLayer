// DirectX12 command list management file...
#pragma once
#include "Snct_DX12.h"
#include "../Interface/ISnct_DXContext.h"

// A class that manages DirectX12 command list
class SnctDX12Context : public ISnctDXContext
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	ID3D12GraphicsCommandList* Get() { return m_pCmdList.Get(); }
	HRESULT Create(D3D12_COMMAND_LIST_TYPE Type, ID3D12Device* Device, ID3D12PipelineState* pipelineState, ID3D12CommandAllocator* CmdAllocator);
	void Reset(ID3D12CommandAllocator* CmdAllocator, ID3D12PipelineState* PipelineState);
	void SetResourceBarrier(ISnctDXBuffer* Resource, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After);

	// Override function
	void Close();
	void ClearRTV(ISnctDXRTV* Descriptors, float clearColor[4], UINT NumRects = 0, RECT* pRects = 0)override final;
	void ClearDSV(ISnctDXDSV* Descriptors, UINT Flag, float Depth,
		UINT8 Stencil, UINT NumRects = 0, RECT* pRects = 0) override final;

	// Command list
	void SetRTV(UINT NumDescriptors, ISnctDXRTV* Descriptors, ISnctDXDSV* DSHandle,
		bool SingleHandleToDescriptorRange)override final;
	void SetViewPort(float Width, float Height, float MinDepth, float MaxDepth) override final;
	void SetScissorRects(float Width, float Height);
	void SetPipelineState(ID3D12PipelineState* pPipelineState);
	void SetGraphicsRootSignature(ID3D12RootSignature* pRootsignature);
	void SetVertexBuffer(UINT bufferNum, ISnctDXBuffer* pBuffer, UINT stride, UINT vertexNum) override final;
	void SetIndexBuffer(ISnctDXBuffer* pBuffer, DXGI_FORMAT format, UINT size)	override final;
	void SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology)	override final;
	void DrawIndexedInstanced(UINT indexCount, UINT startIndexLocation, UINT instanceLocation)	override final;

private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D12GraphicsCommandList> m_pCmdList;
};

