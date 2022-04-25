#pragma once
#include "Snct_DX11.h"
#include "../Interface/ISnct_DxCmdList.h"

class SnctDX11CmdList : public ISnctDXCmdList
{
public:
	~SnctDX11CmdList() { 
		m_pCmdList.Reset(), m_pContext.Reset(); }
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	ID3D12GraphicsCommandList* Get() { return m_pCmdList.Get(); }
	ID3D11DeviceContext* GetContext() { return m_pContext.Get(); }
	ID3D11DeviceContext** GetContextAddress() { return m_pContext.GetAddressOf(); }
	void SetRasterRizerState(ID3D11RasterizerState* raseterRizeState);
	void SetViewPort(float Width, float Height, float MinDepth, float MaxDepth) override final;
	void ClearRTV(ISnctDXRTV* Descriptors, UINT NumRects = 1, RECT* pRects = 0)override final;
	void ClearDSV(ISnctDXDSV* Descriptors, UINT Flag, float Depth,
		UINT8 Stencil, UINT NumRects=0, RECT* pRects = 0) override final;
	void SetRTV(UINT NumDescriptors, ISnctDXRTV* Descriptors, ISnctDXDSV* DSHandle = nullptr,
		bool SingleHandleToDescriptorRange = 0)override final;
	void Close(bool RestoreDeferredContextState, ID3D11CommandList** cmdList);

private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D12GraphicsCommandList>	m_pCmdList;
	ComPtr<ID3D11DeviceContext>			m_pContext;
};