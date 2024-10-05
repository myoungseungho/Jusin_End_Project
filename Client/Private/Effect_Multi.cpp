#pragma once
#include "stdafx.h"
#include "Effect_Multi.h"

CEffect_Multi::CEffect_Multi(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CEffect{ pDevice ,pContext }
{
}

CEffect_Multi::CEffect_Multi(const CGameObject& Prototype)
    : CEffect{ Prototype }
{
}

HRESULT CEffect_Multi::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEffect_Multi::Initialize(void* pArg)
{
    return S_OK;
}

void CEffect_Multi::Priority_Update(_float fTimeDelta)
{
}

void CEffect_Multi::Update(_float fTimeDelta)
{
}

void CEffect_Multi::Late_Update(_float fTimeDelta)
{
}

HRESULT CEffect_Multi::Render()
{
    return S_OK;
}

CEffect_Multi* CEffect_Multi::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Multi* pInstance = new CEffect_Multi(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEffect_Multi"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Multi::Clone(void* pArg)
{
	CEffect_Multi* pInstance = new CEffect_Multi(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CEffect_Multi"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Multi::Free()
{
    __super::Free();
}
