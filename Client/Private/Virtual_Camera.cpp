#include "stdafx.h"
#include "..\Public\Virtual_Camera.h"

#include "GameInstance.h"

CVirtual_Camera::CVirtual_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CVirtual_Camera::CVirtual_Camera(const CVirtual_Camera& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CVirtual_Camera::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVirtual_Camera::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CVirtual_Camera::Priority_Update(_float fTimeDelta)
{
	
}

void CVirtual_Camera::Update(_float fTimeDelta)
{
}

void CVirtual_Camera::Late_Update(_float fTimeDelta)
{
}

HRESULT CVirtual_Camera::Render(_float fTimeDelta)
{
	return S_OK;
}


CGameObject* CVirtual_Camera::Clone(void* pArg)
{
	return nullptr;
}

void CVirtual_Camera::Free()
{
	__super::Free();

}
