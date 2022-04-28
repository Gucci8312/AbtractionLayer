#pragma once
#include "DX11/Snct_DX11.h"
#include "../Snct_Windows.h"
#include "../Snct_Utility.h"

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
	//---------------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------------	
	void CreateShaderFromFile(HashKey key, std::wstring szFileName, DX_SHADER_TYPE type);
	
	//---------------------------------------------------------------------------
	//public variables.
	//---------------------------------------------------------------------------	
	ID3DBlob* GetShaderBlob(HashKey key);

protected:

private:
	//---------------------------------------------------------------------------
	// private variables.
	//---------------------------------------------------------------------------	
	std::unordered_map<HashKey, ComPtr<ID3DBlob>> m_pShaderBlobMap;
};