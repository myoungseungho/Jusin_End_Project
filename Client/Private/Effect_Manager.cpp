#pragma once
#include "stdafx.h"
#include "Effect_Manager.h"
#include "Effect_Layer.h"

IMPLEMENT_SINGLETON(CEffect_Manager)

CEffect_Manager::CEffect_Manager()
{
}

HRESULT CEffect_Manager::Initialize(_uint iNumLevels)
{
	return S_OK;
}

void CEffect_Manager::Priority_Update(_float fTimeDelta)
{
}

void CEffect_Manager::Update(_float fTimeDelta)
{
}

void CEffect_Manager::Late_Update(_float fTimeDelta)
{
}

CEffect* CEffect_Manager::Find_EachEffect(const wstring& strEachEffectTag)
{
	auto	iter = m_EachEffects.find(strEachEffectTag);

	if (iter == m_EachEffects.end())
		return nullptr;

	return iter->second;
}

CEffect_Layer* CEffect_Manager::Find_Effect_Layer(const wstring& strEffectLayerTag)
{
	auto	iter = m_FinalEffects.find(strEffectLayerTag);

	if (iter == m_FinalEffects.end())
		return nullptr;

	return iter->second;
}

HRESULT CEffect_Manager::Add_EachEffect(const wstring& strEachEffectTag, CEffect* pEachEffect)
{
	if (nullptr != Find_EachEffect(strEachEffectTag))
		return E_FAIL;

	m_EachEffects.emplace(strEachEffectTag, pEachEffect);

	return S_OK;
}

HRESULT CEffect_Manager::Add_Effect_To_Layer(const wstring& strEachEffectTag, const wstring& strEffectLayerTag, void* pArg)
{
	CEffect* pEachEffect = Find_EachEffect(strEachEffectTag);
	if (nullptr == pEachEffect)
		return E_FAIL;

	CEffect_Layer* pLayer = Find_Effect_Layer(strEffectLayerTag);

	if (nullptr == pLayer)
	{
		CEffect_Layer* pLayer = CEffect_Layer::Create();

		pLayer->Add_Effect(pEachEffect);

		m_FinalEffects.emplace(strEffectLayerTag, pLayer);
	}
	else
		pLayer->Add_Effect(pEachEffect);

	return S_OK;
}

CEffect_Manager* CEffect_Manager::Create(_uint iNumLevels)
{
	CEffect_Manager* pInstance = new CEffect_Manager();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX(TEXT("Failed to Created : CEffect_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Manager::Free()
{
	__super::Free();

	for (auto& Pair : m_EachEffects)
		Safe_Release(Pair.second);

	m_EachEffects.clear();

	for (auto& Pair : m_FinalEffects)
		Safe_Release(Pair.second);

	m_FinalEffects.clear();

}
