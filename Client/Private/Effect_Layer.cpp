#pragma once
#include "stdafx.h"
#include "Effect_Layer.h"
#include "Effect.h"

CEffect_Layer::CEffect_Layer()
{
}

HRESULT CEffect_Layer::Add_Effect(CEffect* pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	m_MixtureEffects.emplace_back(pEffect);

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

void CEffect_Layer::Render(_float fTimeDelta)
{
	for (auto& pEffect: m_MixtureEffects)
		pEffect->Render(fTimeDelta);
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
