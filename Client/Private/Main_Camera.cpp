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

	_char* name = "Camera_Normal";
	//처음에는 일반 가상 카메라를 메인 카메라로
	CGameObject* virtualCamera_Normal = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Virtual_Camera_Normal"), &name);
	m_listVirtualCamera.push_back(static_cast<CCamera*>(virtualCamera_Normal));

	//스킬 버츄얼 카메라
	name = "Camera_Skill_1";
	CGameObject* virtualCamera_Skill = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Virtual_Camera_Skill"), &name);
	m_listVirtualCamera.push_back(static_cast<CCamera*>(virtualCamera_Skill));

	//스킬 버츄얼 카메라2
	name = "Camera_Skill_2";
	CGameObject* virtualCamera_Skill2 = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Virtual_Camera_Skill"), &name);
	m_listVirtualCamera.push_back(static_cast<CCamera*>(virtualCamera_Skill2));

	//처음엔 일반 가상 카메라를 셋팅
	m_current_Virtual_Camara = m_listVirtualCamera.front();

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

	Update_Camera(m_current_Virtual_Camara);
}

void CMain_Camera::Free_Camera(_float fTimeDelta)
{
	//기본 이동 속도
	_float fMoveSpeed = m_current_Virtual_Camara->m_fMoveSpeed;

	CTransform* virtual_Transform = static_cast<CTransform*>(m_current_Virtual_Camara->Get_Component(TEXT("Com_Transform")));

	// 오른쪽 버튼이 눌렸는지 확인
	if (m_pGameInstance->Mouse_Pressing(1))
	{
		// Shift 키가 눌렸는지 확인하고, 눌렸다면 이동 속도를 증가
		if (m_pGameInstance->Key_Pressing(DIK_LSHIFT))
		{
			fMoveSpeed *= 3.f;
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
			virtual_Transform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_current_Virtual_Camara->m_fMouseSensor * MouseMove * fTimeDelta);
		}

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM_Y))
		{
			virtual_Transform->Turn(virtual_Transform->Get_State(CTransform::STATE_RIGHT), m_current_Virtual_Camara->m_fMouseSensor * MouseMove * fTimeDelta);
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
	for (auto& iter : m_listVirtualCamera)
		Safe_Release(iter);

	__super::Free();

}
