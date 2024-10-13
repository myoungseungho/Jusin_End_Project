#include "stdafx.h"
#include "..\Public\Main_Camera.h"

#include "GameInstance.h"

CMain_Camera::CMain_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{

}

CMain_Camera::CMain_Camera(const CMain_Camera& Prototype)
	: CCamera{ Prototype }
{

}

HRESULT CMain_Camera::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMain_Camera::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_vecVirtualCamera.reserve(VIRTUAL_CAMERA_END);

	_char* name = "Camera_Normal";
	//처음에는 일반 가상 카메라를 메인 카메라로
	CGameObject* virtualCamera_Normal = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Virtual_Camera_Normal"), &name);
	m_vecVirtualCamera.push_back(static_cast<CCamera*>(virtualCamera_Normal));

	for (size_t i = VIRTUAL_CAMERA_SON_SKILL_1; i < VIRTUAL_CAMERA_END; i++)
	{
		switch (i)
		{
		case VIRTUAL_CAMERA_SON_SKILL_1:
			name = "Camera_Son_Skill_1";
			break;
		case VIRTUAL_CAMERA_SON_SKILL_2:
			name = "Camera_Son_Skill_2";
			break;
		case VIRTUAL_CAMERA_SON_SKILL_3:
			name = "Camera_Son_Skill_3";
			break;
		case VIRTUAL_CAMERA_HIT_SKILL_1:
			name = "Camera_Hit_Skill_1";
			break;
		case VIRTUAL_CAMERA_HIT_SKILL_2:
			name = "Camera_Hit_Skill_2";
			break;
		case VIRTUAL_CAMERA_HIT_SKILL_3:
			name = "Camera_Hit_Skill_3";
			break;
		case VIRTUAL_CAMERA_MINE_SKILL_1:
			name = "Camera_Mine_Skill_1";
			break;
		case VIRTUAL_CAMERA_MINE_SKILL_2:
			name = "Camera_Mine_Skill_2";
			break;
		case VIRTUAL_CAMERA_MINE_SKILL_3:
			name = "Camera_Mine_Skill_3";
			break;
		case VIRTUAL_CAMERA_21_SKILL_1:
			name = "Camera_21_Skill_1";
			break;
		case VIRTUAL_CAMERA_21_SKILL_2:
			name = "Camera_21_Skill_2";
			break;
		case VIRTUAL_CAMERA_21_SKILL_3:
			name = "Camera_21_Skill_3";
			break;
		}

		CGameObject* virtualCamera_Skill = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Virtual_Camera_Skill"), &name);
		m_vecVirtualCamera.push_back(static_cast<CCamera*>(virtualCamera_Skill));
	}

	return S_OK;
}

void CMain_Camera::Priority_Update(_float fTimeDelta)
{
	//여기서 가상카메라의 월드포지션과 회전정보를 갱신한다.
	switch (m_currentMode)
	{
	case Client::CMain_Camera::CAMERA_FREE_MODE:
		Free_Camera(fTimeDelta);
		break;
	case Client::CMain_Camera::CAMERA_DEFAULT_MODE:
		Default_Camera(fTimeDelta);
		break;
	}

	//PrevPlayMode면 Point에 있는 녀석을 순회하는 데이터를 뷰투영에 넣어주고
	//아니라면 현재 선택된 카메라의 Transform 정보를 넣어주기
	Update_Camera(m_vecVirtualCamera[m_currentVirtualMode], m_bPrevPlayMode, fTimeDelta);
}

void CMain_Camera::Add_Point(_float duration, InterpolationType type, const _float4x4* worldMatrixPtr)
{
	m_vecVirtualCamera[m_currentVirtualMode]->Add_Point(duration, type, worldMatrixPtr);
}

const list<CCamera::CameraPoint>& CMain_Camera::Get_ListPoint()
{
	//현재 메인카메라가 사용하고 있는 가상카메라의 listPoints를 가져온다.
	return m_vecVirtualCamera[m_currentVirtualMode]->m_listPoints;
}

void CMain_Camera::Play()
{
	m_bPrevPlayMode = true;
}

void CMain_Camera::Free_Camera(_float fTimeDelta)
{
	//기본 이동 속도
	_float fMoveSpeed = m_vecVirtualCamera[m_currentVirtualMode]->m_fMoveSpeed;

	CTransform* virtual_Transform = static_cast<CTransform*>(m_vecVirtualCamera[m_currentVirtualMode]->Get_Component(TEXT("Com_Transform")));

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
			virtual_Transform->Go_Left(fTimeDelta * fMoveSpeed);
		}

		if (m_pGameInstance->Key_Pressing(DIK_D))
		{
			virtual_Transform->Go_Right(fTimeDelta * fMoveSpeed);
		}

		if (m_pGameInstance->Key_Pressing(DIK_W))
		{
			virtual_Transform->Go_Straight(fTimeDelta * fMoveSpeed);
		}

		if (m_pGameInstance->Key_Pressing(DIK_S))
		{
			virtual_Transform->Go_Backward(fTimeDelta * fMoveSpeed);
		}

		if (m_pGameInstance->Key_Pressing(DIK_Q))
		{
			virtual_Transform->Go_Down(fTimeDelta * fMoveSpeed);
		}

		if (m_pGameInstance->Key_Pressing(DIK_E))
		{
			virtual_Transform->Go_Up(fTimeDelta * fMoveSpeed);
		}

		_long MouseMove = {};

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM_X))
		{
			virtual_Transform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_vecVirtualCamera[m_currentVirtualMode]->m_fMouseSensor * MouseMove * fTimeDelta);
		}

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM_Y))
		{
			virtual_Transform->Turn(virtual_Transform->Get_State(CTransform::STATE_RIGHT), m_vecVirtualCamera[m_currentVirtualMode]->m_fMouseSensor * MouseMove * fTimeDelta);
		}
	}
}

void CMain_Camera::Default_Camera(_float fTimeDelta)
{
}


void CMain_Camera::Update(_float fTimeDelta)
{
}

void CMain_Camera::Late_Update(_float fTimeDelta)
{
}

HRESULT CMain_Camera::Render(_float fTimeDelta)
{
	return S_OK;
}


CMain_Camera* CMain_Camera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMain_Camera* pInstance = new CMain_Camera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMain_Camera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMain_Camera::Clone(void* pArg)
{
	CMain_Camera* pInstance = new CMain_Camera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMain_Camera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMain_Camera::Free()
{
	for (auto& iter : m_vecVirtualCamera)
		Safe_Release(iter);

	__super::Free();

}
