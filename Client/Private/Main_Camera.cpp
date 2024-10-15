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

	// 흔들림 효과 적용
	if (m_bIsShaking)
		ApplyCameraShake(fTimeDelta);

	//PrevPlayMode면 Point에 있는 녀석을 순회하는 데이터를 뷰투영에 넣어주고
	//아니라면 현재 선택된 카메라의 Transform 정보를 넣어주기
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
	//현재 메인카메라가 사용하고 있는 가상카메라의 listPoints를 가져온다.
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
		// modelID와 skillID를 사용하여 가상 카메라의 인덱스를 찾음
		int cameraIndex = Get_CameraIndex(cameraData.modelID, cameraData.skillID);
		if (cameraIndex >= 0 && cameraIndex < m_vecVirtualCamera.size()) {
			CCamera* pCurrentCamera = m_vecVirtualCamera[cameraIndex];

			// 기존 포인트 초기화
			pCurrentCamera->m_vecPoints.clear();

			// 새로운 포인트 할당
			for (const auto& pointData : cameraData.points)
			{
				CameraPoint point;
				point.position = pointData.position;
				point.rotation = pointData.rotation;
				point.duration = pointData.duration;
				point.interpolationType = static_cast<InterpolationType>(pointData.interpolationType);
				point.damping = pointData.damping;
				point.hasWorldFloat4x4 = pointData.hasWorldFloat4x4;

				// 해당모델의 Transform에서 월드매트리스 Ptr이 있어야 한다.
				// 각 카메라에 매핑된 모델의 Transform을 가져오는것도 만들긴해야함
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
		// 흔들림 종료
		StopCameraShake();
		return;
	}

	// 흔들림 오프셋 계산
	_float progress = m_fElapsedShakeTime / m_fShakeDuration;
	_float damper = 1.0f - progress; // 흔들림 감쇠 비율

	// 랜덤 오프셋 계산
	_float offsetX = ((rand() % 1000) / 500.0f - 1.0f) * m_fShakeMagnitude * damper;
	_float offsetY = ((rand() % 1000) / 500.0f - 1.0f) * m_fShakeMagnitude * damper;
	_float offsetZ = ((rand() % 1000) / 500.0f - 1.0f) * m_fShakeMagnitude * damper;

	// 흔들림 오프셋 벡터 생성
	m_vShakeOffset = XMVectorSet(offsetX, offsetY, offsetZ, 0.0f);

	// **저장된 기준 위치에 흔들림 오프셋 적용**
	_vector shakenPosition = m_vBaseCameraPosition + m_vShakeOffset;

	// **카메라 위치 업데이트**
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
