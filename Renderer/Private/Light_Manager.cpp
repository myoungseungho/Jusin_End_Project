#include "..\Public\Light_Manager.h"
#include "Light.h"

CLight_Manager::CLight_Manager(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

LIGHT_DESC * CLight_Manager::Get_LightDesc(LIGHT_TYPE eLightType, _uint iLightIndex, string strName)
{
	switch (eLightType)
	{
	case LIGHT_BACKGROUND:
	{
		auto	iter = m_Lights.begin();

		for (size_t i = 0; i < iLightIndex; i++)
			++iter;

		return (*iter)->Get_LightDesc();
	}
	case LIGHT_PLAYER:
	{
		auto iter = m_PlayerLights.find(strName);

		if (iter == m_PlayerLights.end()) 
			return nullptr;
		

		return iter->second->Get_LightDesc();
	}
	case LIGHT_EFFECT:
	{
		auto iter = m_EffectLights.find(strName);

		if (iter == m_EffectLights.end())
			return nullptr;

		return iter->second->Get_LightDesc();
	}
	}
}

_int CLight_Manager::Check_EffectLights()
{
	return m_EffectLights.size();
}

HRESULT CLight_Manager::Initialize()
{

	return S_OK;
}

HRESULT CLight_Manager::Add_Light(const LIGHT_DESC & LightDesc)
{
	CLight*			pLight = CLight::Create(LightDesc);
	if (nullptr == pLight)
		return E_FAIL;

	m_Lights.emplace_back(pLight);

	return S_OK;
}

HRESULT CLight_Manager::Add_Player_Light(string strKey, const LIGHT_DESC& LightDesc)
{
	CLight* pLight = CLight::Create(LightDesc);
	if (nullptr == pLight)
		return E_FAIL;

	m_PlayerLights[strKey] = pLight;
	return S_OK;
}

HRESULT CLight_Manager::Render_Lights(LIGHT_TYPE eLightType, CShader * pShader, CVIBuffer_Rect * pVIBuffer, string strName)
{
	switch (eLightType)
	{
	case LIGHT_BACKGROUND:
		for (auto& pLight : m_Lights)
			pLight->Render(pShader, pVIBuffer);
		break;
	case LIGHT_PLAYER:
		m_PlayerLights[strName]->Render(pShader, pVIBuffer, 5);
		break;
	case LIGHT_EFFECT:
		for (auto& pEffectLight : m_EffectLights)
			pEffectLight.second->Render(pShader, pVIBuffer, 6);
		break;
	}

	return S_OK;
}

CLight_Manager * CLight_Manager::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLight_Manager*		pInstance = new CLight_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLight_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLight_Manager::Free()
{
	__super::Free();

	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	m_Lights.clear();

	for (auto& pPlayerLight : m_PlayerLights)
		Safe_Release(pPlayerLight.second);

	m_PlayerLights.clear();

	for (auto& pEffectLight : m_EffectLights)
		Safe_Release(pEffectLight.second);

	m_EffectLights.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
