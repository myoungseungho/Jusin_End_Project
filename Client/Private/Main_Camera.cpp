#include "stdafx.h"
#include "..\Public\Main_Camera.h"

#include "GameInstance.h"
#include "Virtual_Camera.h"

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
	m_fMouseSensor = static_cast<CMain_Camera::CAMERA_FREE_DESC*>(pArg)->fSensor;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//처음에는 일반 가상 카메라를 메인 카메라로
	CGameObject* virtualCamera_Normal = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Virtual_Camera_Normal"));
	m_listVirtualCamera.push_back(static_cast<CVirtual_Camera*>(virtualCamera_Normal));

	//스킬 버츄얼 카메라
	CGameObject* virtualCamera_Skill = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Virtual_Camera_Skill"));
	m_listVirtualCamera.push_back(static_cast<CVirtual_Camera*>(virtualCamera_Skill));

	//처음엔 일반 가상 카메라를 셋팅
	m_current_Virtual_Camara = m_listVirtualCamera.front();
	return S_OK;
}

void CMain_Camera::Priority_Update(_float fTimeDelta)
{
	switch (m_currentMode)
	{
	case Client::CMain_Camera::CAMERA_FREE_MODE:
		FreeCamera(fTimeDelta);
		break;
	case Client::CMain_Camera::CAMERA_DEFAULT_MODE:
		DefaultCamera(fTimeDelta);
		break;
	}

	__super::Priority_Update(fTimeDelta);
}

void CMain_Camera::FreeCamera(_float fTimeDelta)
{
	//기본 이동 속도
	_float fMoveSpeed = 1.0f;

	// 오른쪽 버튼이 눌렸는지 확인
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		// Shift 키가 눌렸는지 확인하고, 눌렸다면 이동 속도를 증가
		if (m_pGameInstance->Get_DIKeyState(DIK_LSHIFT) & 0x80)
		{
			fMoveSpeed = 3.0f; // 원하는 속도로 변경
		}

		if (m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
		{
			m_pTransformCom->Go_Left(fTimeDelta * fMoveSpeed);
		}

		if (m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
		{
			m_pTransformCom->Go_Right(fTimeDelta * fMoveSpeed);
		}

		if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
		{
			m_pTransformCom->Go_Straight(fTimeDelta * fMoveSpeed);
		}

		if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
		{
			m_pTransformCom->Go_Backward(fTimeDelta * fMoveSpeed);
		}

		if (m_pGameInstance->Get_DIKeyState(DIK_Q) & 0x80)
		{
			m_pTransformCom->Go_Down(fTimeDelta * fMoveSpeed);
		}

		if (m_pGameInstance->Get_DIKeyState(DIK_E) & 0x80)
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
}

void CMain_Camera::DefaultCamera(_float fTimeDelta)
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
