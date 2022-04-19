#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct XMConstantParam
{
	float deltaTime;
	float param00;
	float param01;
	float param02;
};

struct XMConstantCamera
{
	XMFLOAT4	cameraPosition;
	XMFLOAT4X3	dummy;

	XMFLOAT4X4	view;
	XMFLOAT4X4	projection;
	XMFLOAT4X4	VP;
	XMFLOAT4X4	InversView;
	XMFLOAT4X4	InversProjection;
	XMFLOAT4X4	InversVP;
};

struct XMConstantObject
{
	XMFLOAT4	objectPosition;
	XMFLOAT4X3	dummy;

	XMFLOAT4X4	world;
};

struct XMConstantMaterial
{
	XMFLOAT4	albedo;
	XMFLOAT4	metallic;
	XMFLOAT4	roughness;
	XMFLOAT4	specular;
};