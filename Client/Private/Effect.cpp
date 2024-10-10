#pragma once
#include "stdafx.h"
#include "..\Public\Effect.h"
#include "GameInstance.h"

CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice , pContext }
{
}

CEffect::CEffect(const CGameObject& Prototype)
	:CGameObject{ Prototype }
{
}

HRESULT CEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CEffect::Priority_Update(_float fTimeDelta)
{

}

void CEffect::Update(_float fTimeDelta)
{

}

void CEffect::Late_Update(_float fTimeDelta)
{

}

HRESULT CEffect::Render(_float fTimeDelta)
{
	return S_OK;
}

void CEffect::Add_KeyFrame(EFFECT_KEYFRAME NewKeyFrame)
{
	m_EffectKeyFrames.push_back(NewKeyFrame);
}

HRESULT CEffect::Ready_Components(_wstring* pModelName, _wstring* pMaskTextureName, _wstring* pDiffuseTexturueName)
{
	return S_OK;
}

HRESULT CEffect::Bind_ShaderResources()
{
	return S_OK;
}

CEffect* CEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect* pInstance = new CEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect::Clone(void* pArg)
{
	CEffect* pInstance = new CEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect::Free()
{
	__super::Free();
}


