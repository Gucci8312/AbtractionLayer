// DirectX12 command list management file...
#pragma once
#include "Snct_DX12.h"
#include "../Interface/ISnct_DxCmdList.h"

// A class that manages DirectX12 command list
class SnctDX12CmdList : public ISnctDXCmdList
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	ID3D12GraphicsCommandList* Get() { return m_pCmdList.Get(); }
	HRESULT Create(D3D12_COMMAND_LIST_TYPE Type, ID3D12Device* Device, ID3D12CommandAllocator* CmdAllocator);
	void Reset(ID3D12CommandAllocator* CmdAllocator, ID3D12PipelineState* PipelineState);
	void SetResourceBarrier(ISnctDXBuffer* Resource, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After);

	// Override function
	void SetRTV(UINT NumDescriptors, ISnctDXRTV* Descriptors,
		bool SingleHandleToDescriptorRange, ISnctDXDSV* DSHandle)override final;
	void SetViewPort(float Width, float Height, float MinDepth, float MaxDepth) override final;
	void SetScissorRects(float Width, float Height) override final;
	void Close() override final;
	void ClearRTV(ISnctDXRTV* Descriptors, UINT NumRects, RECT* pRects = 0)override final;
	void ClearDSV(ISnctDXDSV* Descriptors, DEPTH_CLEAR_FLAGS Flag, float Depth,
		UINT8 Stencil, UINT NumRects, RECT* pRects=0) override final;

private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D12GraphicsCommandList> m_pCmdList;
};

