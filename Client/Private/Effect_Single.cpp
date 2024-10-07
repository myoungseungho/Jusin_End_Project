#pragma once
#include "stdafx.h"
#include "Effect_Single.h"

CEffect_Single::CEffect_Single(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect{ pDevice ,pContext }
{
}

CEffect_Single::CEffect_Single(const CGameObject& Prototype)
	: CEffect{ Prototype }
{
}

HRESULT CEffect_Single::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Single::Initialize(void* pArg)
{
	return S_OK;
}

void CEffect_Single::Priority_Update(_float fTimeDelta)
{
}

void CEffect_Single::Update(_float fTimeDelta)
{
}

void CEffect_Single::Late_Update(_float fTimeDelta)
{
}

HRESULT CEffect_Single::Render(_float fTimeDelta)
{
	return S_OK;
}

CEffect_Single* CEffect_Single::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Single* pInstance = new CEffect_Single(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEffect_Single"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Single::Clone(void* pArg)
{
	CEffect_Single* pInstance = new CEffect_Single(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CEffect_Single"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Single::Free()
{
	__super::Free();
}
