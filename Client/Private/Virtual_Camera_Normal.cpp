#include "stdafx.h"
#include "..\Public\Virtual_Camera_Normal.h"

#include "GameInstance.h"

CVirtual_Camera_Normal::CVirtual_Camera_Normal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVirtual_Camera{ pDevice, pContext }
{

}

CVirtual_Camera_Normal::CVirtual_Camera_Normal(const CVirtual_Camera_Normal& Prototype)
	: CVirtual_Camera{ Prototype }
{

}

HRESULT CVirtual_Camera_Normal::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVirtual_Camera_Normal::Initialize(void* pArg)
{
	//초기 위치
	m_Desc.vEye = _float3(0.f, 10.f, -10.f);
	//초기 각도
	m_Desc.vAt = _float3(0.f, 0.f, 0.f);
	//시야각
	m_Desc.fFovy = XMConvertToRadians(60.0f);
	//Near
	m_Desc.fNear = 0.1f;
	//Far
	m_Desc.fFar = 1000.f;
	//카메라 이동속도
	m_Desc.fSpeedPerSec = 10.f;
	//카메라 회전속도
	m_Desc.fRotationPerSec = XMConvertToRadians(90.0f);
	//카메라 마우스 민감도
	m_Desc.fSensor = 0.1f;

	//뷰포트 셋팅
	m_Desc.fViewportHeight = g_iWinSizeX;
	m_Desc.fViewportWidth = g_iWinSizeY;

	if (FAILED(__super::Initialize(&m_Desc)))
		return E_FAIL;

	return S_OK;
}

void CVirtual_Camera_Normal::Priority_Update(_float fTimeDelta)
{

}

void CVirtual_Camera_Normal::Update(_float fTimeDelta)
{
}

void CVirtual_Camera_Normal::Late_Update(_float fTimeDelta)
{
}

HRESULT CVirtual_Camera_Normal::Render(_float fTimeDelta)
{
	return S_OK;
}

CVirtual_Camera_Normal* CVirtual_Camera_Normal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVirtual_Camera_Normal* pInstance = new CVirtual_Camera_Normal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVirtual_Camera_Normal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVirtual_Camera_Normal::Clone(void* pArg)
{
	CVirtual_Camera_Normal* pInstance = new CVirtual_Camera_Normal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVirtual_Camera_Normal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVirtual_Camera_Normal::Free()
{
	__super::Free();

}
