#include "stdafx.h"
#include "..\Public\CharaSelectCamera.h"

#include "GameInstance.h"

CCharaSelectCamera::CCharaSelectCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{

}

CCharaSelectCamera::CCharaSelectCamera(const CCharaSelectCamera& Prototype)
	: CCamera{ Prototype }
{

}

HRESULT CCharaSelectCamera::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCharaSelectCamera::Initialize(void* pArg)
{
	m_fMouseSensor = static_cast<CCharaSelectCamera::CAMERA_FREE_DESC*>(pArg)->fSensor;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCharaSelectCamera::Camera_Update(_float fTimeDelta)
{
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_VIEW, m_pTransformCom->Get_WorldMatrix_Inverse());
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(m_fFovy, m_fViewportWidth / m_fViewportHeight, m_fNear, m_fFar));
}


void CCharaSelectCamera::Update(_float fTimeDelta)
{
}

void CCharaSelectCamera::Late_Update(_float fTimeDelta)
{
}

HRESULT CCharaSelectCamera::Render(_float fTimeDelta)
{
	return S_OK;
}

CCharaSelectCamera* CCharaSelectCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCharaSelectCamera* pInstance = new CCharaSelectCamera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCharaSelectCamera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCharaSelectCamera::Clone(void* pArg)
{
	CCharaSelectCamera* pInstance = new CCharaSelectCamera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCharaSelectCamera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCharaSelectCamera::Free()
{
	__super::Free();

}
