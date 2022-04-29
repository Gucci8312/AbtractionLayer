#pragma once
#include "../Interface/ISnct_DxContext.h"

// A class that manages DirectX11 device context
class SnctDX11Context : public ISnctDXContext
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	~SnctDX11Context() { m_pContext.Reset(); }

	// Getter
	//ID3D12GraphicsCommandList* Get() { return m_pCmdList.Get(); }
	ID3D11DeviceContext* GetContext() { return m_pContext.Get(); }
	ID3D11DeviceContext** GetContextAddress() { return m_pContext.GetAddressOf(); }

	// Command list
	void SetViewPort(float Width, float Height, float MinDepth, float MaxDepth) override final;
	void ClearRTV(ISnctDXRTV* Descriptors, float clearColor[4], UINT NumRects = 0, RECT* pRects = 0)	override final;
	void SetRasterizerState(ISnctDXRasterizerState* raseterRizeState);
	void ClearDSV(ISnctDXDSV* Descriptors, UINT Flag, float Depth,
		UINT8 Stencil, UINT NumRects = 0, RECT* pRects = 0) override final;
	void SetRTV(UINT NumDescriptors, ISnctDXRTV* Descriptors, ISnctDXDSV* DSHandle = nullptr,
		bool SingleHandleToDescriptorRange = 0)override final;
	void Close(bool RestoreDeferredContextState, ID3D11CommandList** cmdList);
	void SetVertexBuffer(UINT bufferNum, ISnctDXBuffer* pBuffer, UINT stride, UINT num) override final;
	void SetIndexBuffer(ISnctDXBuffer* pBuffer, DXGI_FORMAT format, UINT size) override final;
	void SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology) override final;
	void DrawIndexed(UINT indexCount, UINT startIndexLocation, UINT instanceLocation)override final;
	void VSSetConstantBuffer(UINT startSlot, UINT bufferNum, ISnctDXBuffer* pBuffer);;
	void PSSetConstantBuffer(UINT startSlot, UINT bufferNum, ISnctDXBuffer* pBuffer);;
	void UpdateSubresource(ISnctDXBuffer* pBuffer, UINT dstSubResource, const void* pData, UINT srcRowPitch, UINT srcDepthPitch);
	// device
	void RegisterCmdList(ID3D11CommandList** cmdList);

private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	//ComPtr<ID3D12GraphicsCommandList>	m_pCmdList;
	ComPtr<ID3D11DeviceContext>			m_pContext;
};