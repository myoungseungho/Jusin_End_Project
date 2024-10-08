#include "..\Public\Texture.h"
#include "Shader.h"

CTexture::CTexture(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent { pDevice, pContext }
{
}

CTexture::CTexture(const CTexture & Prototype)
	: CComponent { Prototype }
	, m_iNumTextures { Prototype.m_iNumTextures }
	, m_SRVs { Prototype.m_SRVs }
{
	for (auto& pSRV : m_SRVs)
		Safe_AddRef(pSRV);
}

HRESULT CTexture::Initialize_Prototype(const _tchar * pTextureFilePath, _uint iNumTextures)
{
	m_iNumTextures = iNumTextures;

	_tchar			szFullPath[MAX_PATH] = {};
	_tchar			szEXT[MAX_PATH] = {};
	
	for (size_t i = 0; i < iNumTextures; i++)
	{
		ID3D11ShaderResourceView*			pSRV = { nullptr };

		wsprintf(szFullPath, pTextureFilePath, i);

		/* D:\������\144\Framework\Client\Bin\Resources\Textures\Default.dds */
		/* ..\Bin\Resources\Textures\Default.dds */
		_wsplitpath_s(szFullPath, nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);

		HRESULT			hr = { 0 };

		if(false == lstrcmp(szEXT, TEXT(".dds")))
			hr = CreateDDSTextureFromFile(m_pDevice, szFullPath, nullptr, &pSRV);

		else if (false == lstrcmp(szEXT, TEXT(".tga")))
		{

		}

		else
		{
			hr = CreateWICTextureFromFile(m_pDevice, szFullPath, nullptr, &pSRV);
		}

		if (FAILED(hr))
		{
			// HRESULT ���� ����Ͽ� ���� �ľ�
			wprintf(L"CreateWICTextureFromFile failed with HRESULT 0x%08X\n", hr);
			continue;
		}
			

		m_SRVs.emplace_back(pSRV);		
	}
	
	return S_OK;
}

HRESULT CTexture::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CTexture::Bind_ShaderResource(CShader * pShader, const _char * pConstantName, _uint iTextureIndex)
{
	return pShader->Bind_ShaderResourceView(pConstantName, m_SRVs[iTextureIndex]);	
}

_float2 CTexture::Get_TextureSize()
{
	_float2 fTextureSize = { 0.f,0.f };
	ID3D11Resource* pResource = nullptr;

	m_SRVs[0]->GetResource(&pResource);

	if (pResource)
	{
		// Resource�� ID3D11Texture2D�� ĳ�����մϴ�.
		ID3D11Texture2D* pTexture2D = nullptr;
		if (SUCCEEDED(pResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&pTexture2D)))
		{
			// �ؽ�ó�� ��ũ���͸� �����ɴϴ�.
			D3D11_TEXTURE2D_DESC desc;
			pTexture2D->GetDesc(&desc);


			// �ؽ�ó�� ũ�⸦ ����մϴ�.
			fTextureSize.x = desc.Width;
			fTextureSize.y = desc.Height;

			// ��� �� pTexture2D ����
			pTexture2D->Release();
		}

		// ��� �� pResource ����
		pResource->Release();
	}

	return fTextureSize;
}

HRESULT CTexture::Set_SRV(ID3D11ShaderResourceView* pSRV, _int iArray)
{
	m_SRVs[iArray] = pSRV;
	return S_OK;
}

CTexture * CTexture::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pTextureFilePath, _uint iNumTextures)
{
	CTexture*		pInstance = new CTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pTextureFilePath, iNumTextures)))
	{
		wstring Message{};
		Message = TEXT("Failed to Created : CTexture \nFile Path  : ") + wstring(pTextureFilePath);
		MessageBox(nullptr, Message.c_str(), L"System Message", MB_OK);
		Safe_Release(pInstance);
	}


	return pInstance;
}

CComponent * CTexture::Clone(void * pArg)
{
	CTexture*		pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTexture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& pSRV : m_SRVs)
		Safe_Release(pSRV);

	m_SRVs.clear();
}
