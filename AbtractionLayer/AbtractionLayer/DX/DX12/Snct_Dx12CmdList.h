// DirectX12 command list management file...
#pragma once
#include "../ISnct_DxCmdList.h"

// Classes that manage DirectX12 command list
class SnctDx12CmdList : public ISnctDxCommandList
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	ID3D12GraphicsCommandList* Get() { return m_pCmdList.Get(); }
	HRESULT Create(D3D12_COMMAND_LIST_TYPE Type, ID3D12Device* Device, ID3D12CommandAllocator* CmdAllocator);
	void ClearRTV(ISnctDxRTV* Descriptors, UINT NumRects, D3D12_RECT* pRects);
	void ClearDSV(ISnctDxDSV* Descriptors, D3D12_CLEAR_FLAGS Flag, float Depth,
		UINT8 Stencil, UINT NumRects, D3D12_RECT* pRects);
	void Reset(ID3D12CommandAllocator* CmdAllocator, ID3D12PipelineState* PipelineState);
	void SetResourceBarrier(ID3D12Resource* Resource, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After);

	// Override function
	void SetRTV(UINT NumDescriptors, ISnctDxRTV* Descriptors,
		bool SingleHandleToDescriptorRange, ISnctDxDSV* DSHandle)override final;
	void SetViewPort(float Width, float Height, float MinDepth, float MaxDepth) override final;
	void SetScissorRects(float Width, float Height) override final;
	void Close() override final;

private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D12GraphicsCommandList> m_pCmdList;
};

