#pragma once
#include "../Snct_Render.h"
#include "Snct_DXConstantParameter.h"

class SnctDXRender : public ISnctRender 
{

public:
	SnctDXRender() {
		m_pConstantObject = std::make_unique<XMConstantObject>();
		m_pConstantCamera = std::make_unique<XMConstantCamera>();
	}

	void SetObject(Transform* pTransform) override;
	void SetCamera(Camera* pCamera) override;

protected:

	std::unique_ptr<XMConstantParam>	m_pConstantParam;
	std::unique_ptr<XMConstantCamera>	m_pConstantCamera;
	std::unique_ptr<XMConstantObject>	m_pConstantObject;
	std::unique_ptr<XMConstantMaterial> m_pConstantMaterial;
};