#include "Snct_DXRender.h"


//------------------------------------------------------------------------------
/// Set object onstant param(world matrix etc...) 
/// \param			object transform (pos, rot, scl)
/// \return			none
//------------------------------------------------------------------------------
void SnctDXRender::SetObject(Transform* pTransform)
{
	XMVECTOR vecPosition;
	XMMATRIX mtxTransform, mtxRotation, mtxScale, mtxWorld;
	
	mtxTransform	= XMMatrixTranslation(pTransform->position.x, pTransform->position.y, pTransform->position.z);
	mtxScale		= XMMatrixScaling(pTransform->scale.x, pTransform->scale.y, pTransform->scale.z);
	mtxRotation		= XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(pTransform->rotation.z),
		XMConvertToRadians(pTransform->rotation.x),
		XMConvertToRadians(pTransform->rotation.y)); // future : quartanion

	mtxWorld	= XMMatrixMultiply(mtxRotation , XMMatrixMultiply(mtxTransform, mtxScale));
	vecPosition	= XMVectorSet(pTransform->position.x, pTransform->position.y, pTransform->position.z, pTransform->position.w);

	XMStoreFloat4	(&m_pConstantObject->objectPosition, vecPosition);
	XMStoreFloat4x4	(&m_pConstantObject->world, XMMatrixTranspose(mtxWorld));
}

//------------------------------------------------------------------------------
/// Set camera onstant param(view matrix etc...) 
/// \param			camera transform (pos, at, up, far, near, hfov)
/// \return			none
//------------------------------------------------------------------------------
void SnctDXRender::SetCamera(Camera* pCamera)
{
	XMVECTOR position;
	XMMATRIX mtxView, mtxProjection, mtxVP;

	position		= XMVectorSet(pCamera->position.x, pCamera->position.y, pCamera->position.z, 1.0f);
	mtxView			= XMMatrixLookAtLH(
		XMVectorSet(pCamera->position.x, pCamera->position.y, pCamera->position.z, 1.0f),
		XMVectorSet(pCamera->at.x, pCamera->at.y, pCamera->at.z, 0.0f),
		XMVectorSet(pCamera->up.x, pCamera->up.y, pCamera->up.z, 0.0f)
	);

	mtxProjection	= XMMatrixPerspectiveFovLH(XMConvertToRadians(pCamera->fFovY), (float)g_screenWidth / (float)g_screenHeight, pCamera->fNear, pCamera->fFar);
	mtxVP			= XMMatrixMultiply(mtxView, mtxProjection);

	XMStoreFloat4	(&m_pConstantCamera->cameraPosition	, position);
	XMStoreFloat4x4	(&m_pConstantCamera->view, XMMatrixTranspose(mtxView));
	XMStoreFloat4x4	(&m_pConstantCamera->projection, XMMatrixTranspose(mtxProjection));
	XMStoreFloat4x4	(&m_pConstantCamera->vp, XMMatrixTranspose(mtxVP));
	XMStoreFloat4x4	(&m_pConstantCamera->inversView, XMMatrixInverse(nullptr, XMMatrixTranspose(mtxView)));
	XMStoreFloat4x4	(&m_pConstantCamera->inversProjection, XMMatrixInverse(nullptr, XMMatrixTranspose(mtxProjection)));
	XMStoreFloat4x4	(&m_pConstantCamera->inversVp, XMMatrixInverse(nullptr, XMMatrixTranspose(mtxVP)));
	
}
