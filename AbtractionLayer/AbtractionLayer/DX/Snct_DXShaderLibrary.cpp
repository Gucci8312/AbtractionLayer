#include "../Snct_Utility.h"
#include "Snct_DXShaderLibrary.h"

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

//------------------------------------------------------------------------------
/// Create shader Blob data 
/// \param			hash key
/// \param			file path
/// \param			shader type(VS, PS, etc...)
/// \return			none
//------------------------------------------------------------------------------
void SnctShaderLibrary::CreateShaderFromFile(HashKey key, std::wstring szFileName, DX_SHADER_TYPE type)
{
    UINT debugFlag = 0;
	#if defined(_DEBUG)
	debugFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif

	std::string version;

	switch (type)
	{
	case DX_SHADER_TYPE::CS: version = "cs_"; break;
	case DX_SHADER_TYPE::VS: version = "vs_"; break;
	case DX_SHADER_TYPE::HS: version = "hs_"; break;
	case DX_SHADER_TYPE::DS: version = "ds_"; break;
	case DX_SHADER_TYPE::GS: version = "gs_"; break;
	case DX_SHADER_TYPE::PS: version = "ps_"; break;
	}

	version += "5_0";

	try
	{
		ComPtr<ID3DBlob> pBlob;

		if (FAILED(D3DCompileFromFile(
			szFileName.c_str(),
			nullptr,
			nullptr,
			"main",
			version.c_str(),
			debugFlag,
			0,
			pBlob.GetAddressOf(),
			nullptr
		)))
		{
			throw std::runtime_error("!Failed to compile from file");
		}

		m_pShaderBlobMap.emplace(key, pBlob);
	}
	catch (std::runtime_error &e) 
	{
		SnctRuntimeError(e);
	}
}

//------------------------------------------------------------------------------
/// return shader blob data 
/// \param			hash key
/// \return			shader blob data
//------------------------------------------------------------------------------
ID3DBlob* SnctShaderLibrary::GetShaderBlob(HashKey key)
{
	return m_pShaderBlobMap.at(key).Get();
}
