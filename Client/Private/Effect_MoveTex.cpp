#pragma once
#include "stdafx.h"
#include "Effect_MoveTex.h"

CEffect_MoveTex::CEffect_MoveTex(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect{ pDevice ,pContext }
{
}

CEffect_MoveTex::CEffect_MoveTex(const CGameObject& Prototype)
	: CEffect{ Prototype }
{
}

HRESULT CEffect_MoveTex::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_MoveTex::Initialize(void* pArg)
{
	return S_OK;
}

void CEffect_MoveTex::Priority_Update(_float fTimeDelta)
{
}

void CEffect_MoveTex::Update(_float fTimeDelta)
{
}

void CEffect_MoveTex::Late_Update(_float fTimeDelta)
{
}

HRESULT CEffect_MoveTex::Render(_float fTimeDelta)
{
	return S_OK;
}

CEffect_MoveTex* CEffect_MoveTex::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_MoveTex* pInstance = new CEffect_MoveTex(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEffect_MoveTex"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_MoveTex::Clone(void* pArg)
{
	CEffect_MoveTex* pInstance = new CEffect_MoveTex(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CEffect_MoveTex"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_MoveTex::Free()
{
	__super::Free();
}
