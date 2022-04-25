#include "../Interface/ISnct_DXDevice.h"

// A class that manages DirectX11 device 
class SnctDX11Device : public ISnctDxDevice
{
public:
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	// Getter
	SnctDX11Device* Get() { return this; }
	ID3D11Device* GetDevice() { return m_pDevice.Get(); }
	ID3D11DeviceContext* GetDeviceContext() { return m_pDeviceContext.Get(); }

	// Create
	HRESULT CreateDevice(D3D_FEATURE_LEVEL Level)override final;
	HRESULT CreateDeferredContext(ID3D11DeviceContext** ppDeferredContext);
	HRESULT CreateRTV(ISnctDXBuffer* pBuckBuffer, ISnctDXRTV* pRTV) override final;
	HRESULT CreateDSV(ISnctDXBuffer* buffer, ISnctDXDSV* pDSVHandle) override final;

	// Device
	void ExecuteCmdList(ID3D11CommandList* cmdList);


	// Context command
	void SetViewPort(float Width, float Height, float MinDepth, float MaxDepth) ;
	void ClearRTV(ISnctDXRTV* Descriptors, float clearColor[4], UINT NumRects = 1, RECT* pRects = 0);
	void ClearDSV(ISnctDXDSV* Descriptors, UINT Flag, float Depth,
		UINT8 Stencil, UINT NumRects = 0, RECT* pRects = 0) ;
	void SetRTV(UINT NumDescriptors, ISnctDXRTV* Descriptors, ISnctDXDSV* DSHandle = nullptr,
		bool SingleHandleToDescriptorRange = 0);
	void SetRasterizerState(ID3D11RasterizerState* rasterizerState);
private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D11Device> m_pDevice;
	ComPtr<ID3D11DeviceContext> m_pDeviceContext;
};