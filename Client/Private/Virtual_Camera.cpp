#include "stdafx.h"
#include "..\Public\Virtual_Camera.h"

#include "GameInstance.h"

CVirtual_Camera::CVirtual_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{

}

CVirtual_Camera::CVirtual_Camera(const CVirtual_Camera& Prototype)
	: CCamera{ Prototype }
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
	CAMERA_DESC Desc{};

	//초기 위치
	Desc.vEye = _float3(0.f, 10.f, -10.f);
	//초기 각도
	Desc.vAt = _float3(0.f, 0.f, 0.f);
	//시야각
	Desc.fFovy = XMConvertToRadians(60.0f);
	//Near
	Desc.fNear = 0.1f;
	//Far
	Desc.fFar = 1000.f;
	//카메라 마우스 민감도
	Desc.fSensor = 0.1f;
	//카메라 이동속도
	Desc.fSpeedPerSec = 1.f;
	//카메라 회전속도
	Desc.fRotationPerSec = XMConvertToRadians(90.0f);
	//카메라 이름
	Desc.cName = *static_cast<_char**>(pArg);

	m_fMoveSpeed = Desc.fSpeedPerSec;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	return S_OK;
}

void CVirtual_Camera::Priority_Update(_float fTimeDelta)
{
	//여기서 가상카메라의 월드포지션과 회전정보를 갱신한다.
	switch (m_currentMode)
	{
	case CAMERA_FREE_MODE:
		Free_Camera(fTimeDelta);
		break;
	case CAMERA_DEFAULT_MODE:
		Default_Camera(fTimeDelta);
		break;
	}
}

void CVirtual_Camera::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CVirtual_Camera::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CVirtual_Camera::Render(_float fTimeDelta)
{
	return S_OK;
}

void CVirtual_Camera::Free_Camera(_float fTimeDelta)
{
	//기본 이동 속도
	_float fMoveSpeed = m_fMoveSpeed;

	// 오른쪽 버튼이 눌렸는지 확인
	if (m_pGameInstance->Mouse_Pressing(1))
	{
		// Shift 키가 눌렸는지 확인하고, 눌렸다면 이동 속도를 증가
		if (m_pGameInstance->Key_Pressing(DIK_LSHIFT))
		{
			fMoveSpeed *= 10.f;
		}

		if (m_pGameInstance->Key_Pressing(DIK_A))
		{
			m_pTransformCom->Go_Left(fTimeDelta * fMoveSpeed);
		}

		if (m_pGameInstance->Key_Pressing(DIK_D))
		{
			m_pTransformCom->Go_Right(fTimeDelta * fMoveSpeed);
		}

		if (m_pGameInstance->Key_Pressing(DIK_W))
		{
			m_pTransformCom->Go_Straight(fTimeDelta * fMoveSpeed);
		}

		if (m_pGameInstance->Key_Pressing(DIK_S))
		{
			m_pTransformCom->Go_Backward(fTimeDelta * fMoveSpeed);
		}

		if (m_pGameInstance->Key_Pressing(DIK_Q))
		{
			m_pTransformCom->Go_Down(fTimeDelta * fMoveSpeed);
		}

		if (m_pGameInstance->Key_Pressing(DIK_E))
		{
			m_pTransformCom->Go_Up(fTimeDelta * fMoveSpeed);
		}

		_long MouseMove = {};

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM_X))
		{
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fMouseSensor * MouseMove * fTimeDelta);
		}

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM_Y))
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), m_fMouseSensor * MouseMove * fTimeDelta);
		}
	}

	if (m_pGameInstance->Key_Down(DIK_U))
	{
		StartCameraShake(1.f, 1.f);
	}
}

void CVirtual_Camera::Default_Camera(_float fTimeDelta)
{
}

CVirtual_Camera* CVirtual_Camera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVirtual_Camera* pInstance = new CVirtual_Camera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVirtual_Camera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVirtual_Camera::Clone(void* pArg)
{
	CVirtual_Camera* pInstance = new CVirtual_Camera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVirtual_Camera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVirtual_Camera::Free()
{
	__super::Free();

}
