#include "../Interface/ISnct_DXDevice.h"

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
	HRESULT Create(D3D_FEATURE_LEVEL Level);
	void CreateDeferredContext(ID3D11DeviceContext** DeferredContext);
	HRESULT CreateRTV(ISnctDXBuffer* buffer, ISnctDXRTV* rtvHandle) override final;
	HRESULT CreateDSV(ISnctDXBuffer* buffer, ISnctDXDSV* rtvHandle) override final;

private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	ComPtr<ID3D11Device> m_pDevice;
	ComPtr<ID3D11DeviceContext> m_pDeviceContext;
};