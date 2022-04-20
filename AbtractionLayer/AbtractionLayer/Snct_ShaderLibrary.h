#pragma once
#include "Snct_DX11.h"
#include "Snct_Windows.h"

enum class DX_SHADER_TYPE
{
	CS,
	VS,
	HS,
	DS,
	GS,
	PS,
};

class SnctShaderLibrary
{
public:
	void CreateShaderFromFile(HashKey key, std::wstring szFileName, DX_SHADER_TYPE type);
	
	ID3DBlob* GetShaderBlob(HashKey key);

protected:

private:
	std::unordered_map<HashKey, ComPtr<ID3DBlob>> m_pShaderBlobMap;
};