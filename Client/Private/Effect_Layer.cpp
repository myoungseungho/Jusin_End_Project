#pragma once
#include "stdafx.h"
#include "Effect_Layer.h"
#include "Effect.h"

CEffect_Layer::CEffect_Layer()
{
}

HRESULT CEffect_Layer::Initialize()
{
	return S_OK;
}

void CEffect_Layer::Priority_Update(_float fTimeDelta)
{
}

void CEffect_Layer::Update(_float fTimeDelta)
{
	for (auto& pEffect : m_MixtureEffects)
		pEffect->Update(fTimeDelta);
}

void CEffect_Layer::Late_Update(_float fTimeDelta)
{
	for (auto& pEffect : m_MixtureEffects)
		pEffect->Late_Update(fTimeDelta);
}

HRESULT CEffect_Layer::Render(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CEffect_Layer::Add_Effect(CEffect* pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	m_MixtureEffects.emplace_back(pEffect);

	return S_OK;
}

vector<class CEffect*> CEffect_Layer::Get_Effects()
{
	return m_MixtureEffects;
}

CEffect* CEffect_Layer::Find_Effect(const std::wstring& effectName)
{
	for (CEffect* pEffect : m_MixtureEffects)
	{
		if (pEffect && pEffect->m_EffectName == effectName) // 이름이 일치하는지 확인
		{
			return pEffect;
		}
	}
	return nullptr;
}

HRESULT CEffect_Layer::Play_Effect_Animation(_float fTimeDelta)
{
	if (m_iNumKeyFrames <= 0 || m_fTickPerSecond <= 0.0f)
		return E_FAIL;

	m_fCurrentAnimPosition += fTimeDelta * m_fTickPerSecond;

	if (m_fCurrentAnimPosition > m_fDuration)
		m_fCurrentAnimPosition = m_fDuration;

	for (CEffect* pEffect : m_MixtureEffects)
	{
		if (pEffect)
		{
			pEffect->Play_Animation(m_fCurrentAnimPosition);
		}
	}

	return S_OK;
}

CEffect_Layer* CEffect_Layer::Create()
{
	return new CEffect_Layer();
}

void CEffect_Layer::Free()
{
	__super::Free();

	for (auto& pMixtureEffect : m_MixtureEffects)
		Safe_Release(pMixtureEffect);

	m_MixtureEffects.clear();
}
