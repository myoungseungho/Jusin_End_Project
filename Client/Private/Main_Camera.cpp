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

	_char* name = "";

	for (size_t i = VIRTUAL_CAMERA_NORMAL; i < VIRTUAL_CAMERA_END; i++)
	{
		switch (i)
		{
		case VIRTUAL_CAMERA_NORMAL:
			name = "Camera_Normal";
			break;
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

		CGameObject* virtualCamera_Skill = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Virtual_Camera"), &name);
		m_vecVirtualCamera.push_back(static_cast<CCamera*>(virtualCamera_Skill));
	}

	return S_OK;
}

void CMain_Camera::Priority_Update(_float fTimeDelta)
{
	//���⼭ ����ī�޶��� ���������ǰ� ȸ�������� �����Ѵ�.
	switch (m_currentMode)
	{
	case Client::CMain_Camera::CAMERA_FREE_MODE:
		Free_Camera(fTimeDelta);
		break;
	case Client::CMain_Camera::CAMERA_DEFAULT_MODE:
		Default_Camera(fTimeDelta);
		break;
	}

	// ��鸲 ȿ�� ����
	if (m_bIsShaking)
		ApplyCameraShake(fTimeDelta);

	//PrevPlayMode�� Point�� �ִ� �༮�� ��ȸ�ϴ� �����͸� �������� �־��ְ�
	//�ƴ϶�� ���� ���õ� ī�޶��� Transform ������ �־��ֱ�
	Update_Camera(m_vecVirtualCamera[m_currentVirtualMode], m_bPrevPlayMode, fTimeDelta);
}

void CMain_Camera::Add_Point(_float duration, InterpolationType type, const _float4x4* worldMatrixPtr, _float damping, _bool hasWorldFloat4x4)
{
	m_vecVirtualCamera[m_currentVirtualMode]->Add_Point(duration, type, worldMatrixPtr, damping, hasWorldFloat4x4);
}

void CMain_Camera::Remove_Point(_int currentIndex)
{
	m_vecVirtualCamera[m_currentVirtualMode]->Remove_Point(currentIndex);
}

vector<CameraPoint>& CMain_Camera::Get_VectorPoint()
{
	//���� ����ī�޶� ����ϰ� �ִ� ����ī�޶��� listPoints�� �����´�.
	return m_vecVirtualCamera[m_currentVirtualMode]->m_vecPoints;
}

void CMain_Camera::Play()
{
	m_bPrevPlayMode = true;
}

void CMain_Camera::Move_Point(_int index)
{
	m_vecVirtualCamera[m_currentVirtualMode]->Move_Point(index);
}

void CMain_Camera::Modify_Transform(_int index)
{
	m_vecVirtualCamera[m_currentVirtualMode]->Modify_Transform(index);
}

void CMain_Camera::ApplyCameraData(vector<CameraData>& cameraDataList)
{
	for (const auto& cameraData : cameraDataList)
	{
		// modelID�� skillID�� ����Ͽ� ���� ī�޶��� �ε����� ã��
		int cameraIndex = Get_CameraIndex(cameraData.modelID, cameraData.skillID);
		if (cameraIndex >= 0 && cameraIndex < m_vecVirtualCamera.size()) {
			CCamera* pCurrentCamera = m_vecVirtualCamera[cameraIndex];

			// ���� ����Ʈ �ʱ�ȭ
			pCurrentCamera->m_vecPoints.clear();

			// ���ο� ����Ʈ �Ҵ�
			for (const auto& pointData : cameraData.points)
			{
				CameraPoint point;
				point.position = pointData.position;
				point.rotation = pointData.rotation;
				point.duration = pointData.duration;
				point.interpolationType = static_cast<InterpolationType>(pointData.interpolationType);
				point.damping = pointData.damping;
				point.hasWorldFloat4x4 = pointData.hasWorldFloat4x4;

				// �ش���� Transform���� �����Ʈ���� Ptr�� �־�� �Ѵ�.
				// �� ī�޶� ���ε� ���� Transform�� �������°͵� ������ؾ���
				CGameObject* model = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"));
				CTransform* modelTransform = static_cast<CTransform*>(model->Get_Component(TEXT("Com_Transform")));
				const _float4x4* worldMatrixPtr = modelTransform->Get_WorldMatrixPtr();
				point.pWorldFloat4x4 = worldMatrixPtr;

				pCurrentCamera->m_vecPoints.push_back(point);
			}
		}
	}
}

_int CMain_Camera::Get_CameraIndex(_int modelID, _int skillID)
{
	_int index = -1;

	if (modelID == 0)
		index = VIRTUAL_CAMERA_NORMAL;

	else if (modelID == 1) { // MODELID_SON
		if (skillID == 0)
			index = VIRTUAL_CAMERA_SON_SKILL_1;
		else if (skillID == 1)
			index = VIRTUAL_CAMERA_SON_SKILL_2;
		else if (skillID == 2)
			index = VIRTUAL_CAMERA_SON_SKILL_3;
	}
	else if (modelID == 2) { // MODELID_HIT
		if (skillID == 0)
			index = VIRTUAL_CAMERA_HIT_SKILL_1;
		else if (skillID == 1)
			index = VIRTUAL_CAMERA_HIT_SKILL_2;
		else if (skillID == 2)
			index = VIRTUAL_CAMERA_HIT_SKILL_3;
	}
	else if (modelID == 3) { // MODELID_HIT
		if (skillID == 0)
			index = VIRTUAL_CAMERA_MINE_SKILL_1;
		else if (skillID == 1)
			index = VIRTUAL_CAMERA_MINE_SKILL_2;
		else if (skillID == 2)
			index = VIRTUAL_CAMERA_MINE_SKILL_3;
	}
	else if (modelID == 4) { // MODELID_HIT
		if (skillID == 0)
			index = VIRTUAL_CAMERA_21_SKILL_1;
		else if (skillID == 1)
			index = VIRTUAL_CAMERA_21_SKILL_2;
		else if (skillID == 2)
			index = VIRTUAL_CAMERA_21_SKILL_3;
	}

	return index;
}

void CMain_Camera::Delete_Points()
{
	m_vecVirtualCamera[m_currentVirtualMode]->Delete_Points();
}

void CMain_Camera::ApplyCameraShake(_float fTimeDelta)
{
	m_fElapsedShakeTime += fTimeDelta;

	if (m_fElapsedShakeTime >= m_fShakeDuration)
	{
		// ��鸲 ����
		StopCameraShake();
		return;
	}

	// ��鸲 ������ ���
	_float progress = m_fElapsedShakeTime / m_fShakeDuration;
	_float damper = 1.0f - progress; // ��鸲 ���� ����

	// ���� ������ ���
	_float offsetX = ((rand() % 1000) / 500.0f - 1.0f) * m_fShakeMagnitude * damper;
	_float offsetY = ((rand() % 1000) / 500.0f - 1.0f) * m_fShakeMagnitude * damper;
	_float offsetZ = ((rand() % 1000) / 500.0f - 1.0f) * m_fShakeMagnitude * damper;

	// ��鸲 ������ ���� ����
	m_vShakeOffset = XMVectorSet(offsetX, offsetY, offsetZ, 0.0f);

	// **����� ���� ��ġ�� ��鸲 ������ ����**
	_vector shakenPosition = m_vBaseCameraPosition + m_vShakeOffset;

	// **ī�޶� ��ġ ������Ʈ**
	CTransform* virtual_Transform = static_cast<CTransform*>(m_vecVirtualCamera[m_currentVirtualMode]->Get_Component(TEXT("Com_Transform")));
	virtual_Transform->Set_State(CTransform::STATE_POSITION, shakenPosition);
}


void CMain_Camera::StartCameraShake(_float fDuration, _float fMagnitude)
{
	m_bIsShaking = true;
	m_fShakeDuration = fDuration;
	m_fShakeMagnitude = fMagnitude;
	m_fElapsedShakeTime = 0.0f;
}

void CMain_Camera::StopCameraShake()
{
	m_bIsShaking = false;
	m_fShakeDuration = 0.0f;
	m_fElapsedShakeTime = 0.0f;
	m_vShakeOffset = XMVectorZero();
}

void CMain_Camera::Free_Camera(_float fTimeDelta)
{
	//�⺻ �̵� �ӵ�
	_float fMoveSpeed = m_vecVirtualCamera[m_currentVirtualMode]->m_fMoveSpeed;

	CTransform* virtual_Transform = static_cast<CTransform*>(m_vecVirtualCamera[m_currentVirtualMode]->Get_Component(TEXT("Com_Transform")));

	// ������ ��ư�� ���ȴ��� Ȯ��
	if (m_pGameInstance->Mouse_Pressing(1))
	{
		// Shift Ű�� ���ȴ��� Ȯ���ϰ�, ���ȴٸ� �̵� �ӵ��� ����
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

	if (m_pGameInstance->Key_Down(DIK_U))
	{
		StartCameraShake(1.f, 1.f);
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
