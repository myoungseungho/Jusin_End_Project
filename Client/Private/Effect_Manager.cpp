#pragma once
#include "stdafx.h"
#include "Effect_Manager.h"

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
}
