#include "..\Public\Font_Manager.h"
#include "CustomFont.h"

CFont_Manager::CFont_Manager()
{

}

HRESULT CFont_Manager::Add_Font(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _wstring & strFontTag, const _tchar * pFontFilePath)
{
	if (nullptr != Find_Font(strFontTag))
		return E_FAIL;

	CCustomFont*		pFont = CCustomFont::Create(pDevice, pContext, pFontFilePath);
	if (nullptr == pFont)
		return E_FAIL;

	m_Fonts.emplace(strFontTag, pFont);

	return S_OK;
}

HRESULT CFont_Manager::Draw_Font(const _wstring & strFontTag, const _tchar * pText, const _float2 & vPosition, _fvector vFontColor, _float fRadian, _float2 vPivotPos, _float fScale)
{
	CCustomFont*		pFont = Find_Font(strFontTag);
	if (nullptr == pFont)
		return E_FAIL;

	pFont->Draw(pText, vPosition, vFontColor, fRadian, vPivotPos, fScale);	
}

CCustomFont * CFont_Manager::Find_Font(const _wstring & strFontTag)
{
	auto	iter = m_Fonts.find(strFontTag);
	if (iter == m_Fonts.end())
		return nullptr;

	return iter->second;	
}

CFont_Manager * CFont_Manager::Create()
{
	return new CFont_Manager();	
}

void CFont_Manager::Free()
{
	__super::Free();

	for (auto& Pair : m_Fonts)
		Safe_Release(Pair.second);

	m_Fonts.clear();
}

