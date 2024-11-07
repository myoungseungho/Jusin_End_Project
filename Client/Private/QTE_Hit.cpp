#include "stdafx.h"
#include "..\Public\QTE_Hit.h"

#include "RenderInstance.h"
#include "GameInstance.h"
CQTE_Hit::CQTE_Hit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CQTE_Hit::CQTE_Hit(const CQTE_Hit& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CQTE_Hit::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CQTE_Hit::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CQTE_Hit::Camera_Update(_float fTimeDelta)
{

}

void CQTE_Hit::Update(_float fTimeDelta)
{

}

void CQTE_Hit::Late_Update(_float fTimeDelta)
{
	
}

HRESULT CQTE_Hit::Render(_float fTimeDelta)
{
	return S_OK;
}

CQTE_Hit* CQTE_Hit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CQTE_Hit* pInstance = new CQTE_Hit(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CQTE_Hit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CQTE_Hit::Clone(void* pArg)
{
	CQTE_Hit* pInstance = new CQTE_Hit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CQTE_Hit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQTE_Hit::Free()
{
	__super::Free();
}
