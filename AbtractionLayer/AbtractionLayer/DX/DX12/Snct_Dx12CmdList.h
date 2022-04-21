// DirectX12 command list management file...
#pragma once
#include "../ISnct_CmdList.h"
#include "../../Snct_Windows.h"
#include "../Snct_DXResource.h"

// Classes that manage DirectX12 command list
class SnctDx12CmdList : public ISnctCmdList
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	ID3D12GraphicsCommandList* Get() { return m_cmdList.Get(); }
	HRESULT Create(D3D12_COMMAND_LIST_TYPE Type, ID3D12Device* Device, ID3D12CommandAllocator* CmdAllocator);
	void ClearRTV(D3D12_CPU_DESCRIPTOR_HANDLE Descriptors, UINT NumRects, D3D12_RECT* pRects);
	void ClearDSV(D3D12_CPU_DESCRIPTOR_HANDLE Descriptors, D3D12_CLEAR_FLAGS Flag, float Depth,
		unsigned short Stencil, UINT NumRects, D3D12_RECT* pRects);
	void Reset(ID3D12CommandAllocator* CmdAllocator, ID3D12PipelineState* PipelineState);
	void SetRTV(UINT NumDescriptors, ISnctRTV* Descriptors,
		bool SingleHandleToDescriptorRange, D3D12_CPU_DESCRIPTOR_HANDLE* DSHandle);
	void SetViewPort(float Width, float Height, float MinDepth, float MaxDepth) override final;
	void SetScissorRects(float Width, float Height) override final;
	void Close() override final;
	void SetResourceBarrier(ID3D12Resource* Resource, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After);
private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D12GraphicsCommandList> m_cmdList;
};

