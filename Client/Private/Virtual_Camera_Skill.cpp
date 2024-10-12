#include "stdafx.h"
#include "..\Public\Virtual_Camera_Skill.h"

#include "GameInstance.h"

CVirtual_Camera_Skill::CVirtual_Camera_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVirtual_Camera{ pDevice, pContext }
{

}

CVirtual_Camera_Skill::CVirtual_Camera_Skill(const CVirtual_Camera_Skill& Prototype)
	: CVirtual_Camera{ Prototype }
{

}

HRESULT CVirtual_Camera_Skill::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVirtual_Camera_Skill::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CVirtual_Camera_Skill::Priority_Update(_float fTimeDelta)
{
	
}

void CVirtual_Camera_Skill::Update(_float fTimeDelta)
{
}

void CVirtual_Camera_Skill::Late_Update(_float fTimeDelta)
{
}

HRESULT CVirtual_Camera_Skill::Render(_float fTimeDelta)
{
	return S_OK;
}

CVirtual_Camera_Skill* CVirtual_Camera_Skill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVirtual_Camera_Skill* pInstance = new CVirtual_Camera_Skill(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVirtual_Camera_Skill"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVirtual_Camera_Skill::Clone(void* pArg)
{
	CVirtual_Camera_Skill* pInstance = new CVirtual_Camera_Skill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVirtual_Camera_Skill"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVirtual_Camera_Skill::Free()
{
	__super::Free();

}
