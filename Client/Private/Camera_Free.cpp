#include "stdafx.h"
#include "..\Public\Camera_Free.h"

#include "GameInstance.h"
#include "Player.h"
#include "UIManager.h"
#include "UI_MiniMap.h"
#include "Alpha_Enemy.h"
#include "Hyunwoo_Enemy.h"
CCamera_Free::CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{

}

CCamera_Free::CCamera_Free(const CCamera_Free& Prototype)
	: CCamera{ Prototype }
{

}

HRESULT CCamera_Free::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Free::Initialize(void* pArg)
{
	m_fMouseSensor = static_cast<CCamera_Free::CAMERA_FREE_DESC*>(pArg)->fSensor;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVectorSet(0.86f, 0.f, 0.49f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVectorSet(-0.385f, 0.62f, 0.67f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVectorSet(-0.3f, -0.78f, 0.54f, 0.f));

	InitializeCameraPoints();

	return S_OK;
}

void CCamera_Free::Priority_Update(_float fTimeDelta)
{


	__super::Priority_Update(fTimeDelta);
}

void CCamera_Free::Update(_float fTimeDelta)
{
	// 현재 모드에 따른 카메라 업데이트
	switch (m_currentMode)
	{
	case CAMERA_DEFAULT_MODE:
		PlayerCamera();
		break;
	case CAMERA_FREE_MODE:
		FreeCamera(fTimeDelta);
		break;
	case CAMERA_CINEMATIC_MODE:
		CinematicCamera(fTimeDelta);
		break;
	}

	// 흔들림 효과 적용
	if (m_bIsShaking)
	{
		ApplyCameraShake(fTimeDelta);
	}
}


void CCamera_Free::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera_Free::Render(_float fTimeDelta)
{
	return S_OK;
}

void CCamera_Free::StartCameraShake(_float fDuration, _float fMagnitude)
{
	m_bIsShaking = true;
	m_fShakeDuration = fDuration;
	m_fShakeMagnitude = fMagnitude;
	m_fElapsedShakeTime = 0.0f;
}

void CCamera_Free::StopCameraShake()
{
	m_bIsShaking = false;
	m_fShakeDuration = 0.0f;
	m_fElapsedShakeTime = 0.0f;
	m_fShakeMagnitude = 0.0f;
	m_vShakeOffset = XMVectorZero();
}

void CCamera_Free::StartCinematic_Mode(const wstring& cinematicKey, CGameObject* pTargetObject)
{
	// 전달받은 키로 시네마틱을 찾습니다.
	auto it = m_Cinematics.find(cinematicKey);
	if (it != m_Cinematics.end())
	{
		m_pCurrentCinematicPoints = &(it->second);
		m_CurrentCinematicKey = cinematicKey;

		// 대상 객체 설정
		if (pTargetObject != nullptr)
			m_pTargetObject = pTargetObject;
		else
			m_pTargetObject = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, L"Layer_Player");

		// 시네마틱 초기화 변수들 리셋
		m_bCinematicInitialized = false;
		m_CurrentIndex = 0;
		m_fElapsedTime = 0.0f;

		// 모드를 시네마틱 모드로 전환
		m_currentMode = CAMERA_CINEMATIC_MODE;

		//플레이어 HUD랑 Enemy Hud랑 다 꺼야함
		CPlayer* player = static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, L"Layer_Player"));
		player->m_vecHandlers[CPlayer::HUD_HANDLER]->SetActive(false);

		CEnemy* enemy = dynamic_cast<CEnemy*>(m_pTargetObject);
		if (enemy != nullptr)
			enemy->m_vecHandlers[CEnemy::HUD_HANDLER]->SetActive(false);
	}
}
void CCamera_Free::PlayerCamera()
{
	// 플레이어의 현재 위치를 가져옵니다.
	CComponent* playerTransformCom = m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	CTransform* playerTransform = static_cast<CTransform*>(playerTransformCom);
	_vector playerPosition = playerTransform->Get_State(CTransform::STATE_POSITION);

	// 기본적으로 카메라는 플레이어를 따라갑니다.
	_vector defaultCameraOffset = XMVectorSet(4.279f, 11.847f, -8.13f, 0.0f);
	_vector cameraPosition = playerPosition + defaultCameraOffset;

	// 마우스 휠 입력 감지 및 처리
	_byte MouseWheel = m_pGameInstance->Get_DIMouseWheelState();
	float wheelSensitivity = 0.005f; // 휠 감도 조절

	if (MouseWheel != 0) // 휠이 움직였을 때만 처리
	{
		// 카메라의 전방 벡터를 가져옵니다.
		_vector forward = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		// 휠 입력에 따라 카메라 오프셋을 업데이트합니다.
		_vector wheelOffset = forward * (wheelSensitivity * MouseWheel);
		m_vCameraOffset.x += XMVectorGetX(wheelOffset);
		m_vCameraOffset.y += XMVectorGetY(wheelOffset);
		m_vCameraOffset.z += XMVectorGetZ(wheelOffset);
	}

	// _float3를 _vector로 변환합니다.
	_vector vCameraOffset = XMVectorSet(m_vCameraOffset.x, m_vCameraOffset.y, m_vCameraOffset.z, 0.0f);

	// 카메라의 위치를 계산합니다.
	m_vBaseCameraPosition = cameraPosition + vCameraOffset; // 카메라의 기본 위치 저장

	// 카메라의 위치를 새 위치로 설정합니다.
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, cameraPosition + vCameraOffset);

	// **카메라가 플레이어를 바라보도록 설정합니다.**
	m_pTransformCom->LookAt(playerPosition);
}

void CCamera_Free::FreeCamera(_float fTimeDelta)
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

void CCamera_Free::ApplyCameraShake(_float fTimeDelta)
{
	m_fElapsedShakeTime += fTimeDelta;

	if (m_fElapsedShakeTime >= m_fShakeDuration)
	{
		// 흔들림 종료
		StopCameraShake();
		return;
	}

	_float progress = m_fElapsedShakeTime / m_fShakeDuration;
	_float damper = 1.0f - progress; // 흔들림 감쇠 비율

	// 랜덤 오프셋 계산
	_float offsetX = ((rand() % 1000) / 500.0f - 1.0f) * m_fShakeMagnitude * damper;
	_float offsetY = ((rand() % 1000) / 500.0f - 1.0f) * m_fShakeMagnitude * damper;
	_float offsetZ = ((rand() % 1000) / 500.0f - 1.0f) * m_fShakeMagnitude * damper;

	// 흔들림 오프셋 벡터 생성
	m_vShakeOffset = XMVectorSet(offsetX, offsetY, offsetZ, 0.0f);

	// 기본 카메라 위치에 흔들림 오프셋을 더합니다.
	_vector shakenPosition = m_vBaseCameraPosition + m_vShakeOffset;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, shakenPosition);
}


void CCamera_Free::CinematicCamera(_float fTimeDelta)
{
	if (m_pCurrentCinematicPoints == nullptr || m_pCurrentCinematicPoints->empty())
	{
		// 시네마틱 포인트가 없는 경우 기본 모드로 전환
		m_currentMode = CAMERA_DEFAULT_MODE;


		return;
	}

	// 초기화가 필요한 경우
	if (!m_bCinematicInitialized)
	{
		// 인덱스 및 시간 초기화
		m_CurrentIndex = 0;
		m_fElapsedTime = 0.0f;

		// 현재 및 다음 카메라 포인트 설정
		m_CurrentCameraPoint = (*m_pCurrentCinematicPoints)[m_CurrentIndex];
		if (m_pCurrentCinematicPoints->size() > 1)
		{
			m_NextCameraPoint = (*m_pCurrentCinematicPoints)[m_CurrentIndex + 1];
		}
		else
		{
			// 포인트가 하나뿐이라면 현재 포인트를 다음 포인트로 설정
			m_NextCameraPoint = m_CurrentCameraPoint;
		}

		// 초기화 완료 표시
		m_bCinematicInitialized = true;
	}

	// 경과 시간을 업데이트합니다.
	m_fElapsedTime += fTimeDelta;

	// 보간 파라미터 t 계산
	_float duration = m_CurrentCameraPoint.duration;
	_float t = m_fElapsedTime / duration;
	t = max(0.0f, min(1.0f, t));

	// 보간된 로컬 위치 계산
	_vector interpolatedLocalPosition;

	switch (m_currentMoveMode)
	{
	case CAMERA_CINEMATIC_MOVE_LIENAR_MODE:
		// 선형 보간 사용
		interpolatedLocalPosition = XMVectorLerp(
			m_CurrentCameraPoint.localPosition,
			m_NextCameraPoint.localPosition,
			t
		);
		break;

	case CAMERA_CINEMATIC_MOVE_SPLINE_MODE:
		// 스플라인 보간 사용 (구현 필요)
		interpolatedLocalPosition = CatmullRomSplinePosition(t);
		break;
	}

	// 대상 객체가 설정되지 않은 경우 기본 모드로 전환
	if (m_pTargetObject == nullptr)
	{
		m_currentMode = CAMERA_DEFAULT_MODE;
		return;
	}

	// 대상 객체의 Transform 컴포넌트를 가져옵니다.
	CTransform* targetTransform = static_cast<CTransform*>(m_pTargetObject->Get_Component(TEXT("Com_Transform")));
	if (targetTransform == nullptr)
	{
		m_currentMode = CAMERA_DEFAULT_MODE;
		return;
	}

	// 대상 객체의 월드 행렬을 가져옵니다.
	_matrix targetWorldMatrix = targetTransform->Get_WorldMatrix();

	// 로컬 좌표를 월드 좌표로 변환합니다.
	_vector worldCameraPosition = XMVector3TransformCoord(interpolatedLocalPosition, targetWorldMatrix);

	// 카메라의 위치를 설정합니다.
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, worldCameraPosition);

	// 카메라의 기본 위치를 저장합니다 (흔들림 적용을 위해)
	m_vBaseCameraPosition = worldCameraPosition;

	// 카메라가 바라볼 지점을 설정합니다 (대상 객체의 위치)
	_vector worldLookAtPosition = targetTransform->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->LookAt(worldLookAtPosition);

	// 현재 구간이 완료되었는지 확인합니다.
	if (t >= 1.0f)
	{
		// 다음 카메라 포인트로 이동
		m_CurrentIndex++;
		if (m_CurrentIndex < m_pCurrentCinematicPoints->size() - 1)
		{
			// 다음 포인트 설정
			m_CurrentCameraPoint = (*m_pCurrentCinematicPoints)[m_CurrentIndex];
			m_NextCameraPoint = (*m_pCurrentCinematicPoints)[m_CurrentIndex + 1];
			m_fElapsedTime = 0.0f; // 경과 시간 초기화
		}
		else
		{
			m_pGameInstance->StopSlowMotion();

			// 마지막 포인트에 도달; 시네마틱 모드 종료
			m_currentMode = CAMERA_DEFAULT_MODE;

			// 초기화 상태 리셋
			m_bCinematicInitialized = false;
			m_pCurrentCinematicPoints = nullptr;
			m_CurrentCinematicKey.clear();

			// 필요한 경우 추가 처리를 수행합니다.

			CPlayer* player = static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, L"Layer_Player"));
			player->m_vecHandlers[CPlayer::HUD_HANDLER]->SetActive(true);

			CEnemy* enemy = dynamic_cast<CEnemy*>(m_pTargetObject);
			if (enemy != nullptr)
				enemy->m_vecHandlers[CEnemy::HUD_HANDLER]->SetActive(true);

			if (dynamic_cast<CHyunwoo_Enemy*>(enemy))
				CUIManager::Get_Instance()->Show_Monster(CUI_MiniMap::BOSS_MINIMAP);

			if (dynamic_cast<CAlpha_Enemy*>(enemy))
				CUIManager::Get_Instance()->Show_Monster(CUI_MiniMap::ALPHA_MINIMAP);

		}
	}
}


void CCamera_Free::InitializeCameraPoints()
{
	vector<CameraPoint> cinematic1;

	CameraPoint point1;
	point1.localPosition = XMVectorSet(3.f, 7.f, 0.f, 1.f);
	point1.duration = 0.5f;
	cinematic1.push_back(point1);

	CameraPoint point3;
	point3.localPosition = XMVectorSet(-3.f, 8.f, 0.f, 1.f);  // 180도
	point3.duration = 0.5f;
	cinematic1.push_back(point3);

	CameraPoint point6;
	point6.localPosition = XMVectorSet(0.f, 10.f, -3.f, 1.f);  // 270도
	point6.duration = 0.5f;
	cinematic1.push_back(point6);

	CameraPoint point9;
	point9.localPosition = XMVectorSet(2.5f, 6.f, 2.5f, 1.f);
	point9.duration = 0.5f;
	cinematic1.push_back(point9);

	CameraPoint point11;
	point11.localPosition = XMVectorSet(2.5f, 5.f, 2.5f, 1.f);  // 더 좁아짐
	point11.duration = 0.5f;
	cinematic1.push_back(point11);

	// 컨테이너에 추가 (키는 "SkillR_Cinematic" 등으로 설정)
	m_Cinematics[L"SkillR_Cinematic"] = cinematic1;

	// 새로운 Alpha_Cinematic 포인트 설정
	vector<CameraPoint> alphaCinematic;

	CameraPoint alphaPoint0;
	alphaPoint0.localPosition = XMVectorSet(3.0f, 3.f, 0.0f, 1.0f);    // 0도
	alphaPoint0.duration = 2.f; // 원하는 시간으로 조절
	alphaCinematic.push_back(alphaPoint0);

	CameraPoint alphaPoint90;
	alphaPoint90.localPosition = XMVectorSet(0.0f, 3.0f, 3.0f, 1.0f);   // 90도
	alphaPoint90.duration = 2.f;
	alphaCinematic.push_back(alphaPoint90);

	CameraPoint alphaPoint180;
	alphaPoint180.localPosition = XMVectorSet(-3.0f, 3.0f, 0.0f, 1.0f);  // 180도
	alphaPoint180.duration = 2.f;
	alphaCinematic.push_back(alphaPoint180);

	CameraPoint alphaPoint270;
	alphaPoint270.localPosition = XMVectorSet(0.0f, 3.0f, -3.0f, 1.0f);  // 270도
	alphaPoint270.duration = 2.f;
	alphaCinematic.push_back(alphaPoint270);

	// 컨테이너에 추가 (키는 "Alpha_Cinematic"으로 설정)
	m_Cinematics[L"Alpha_Cinematic"] = alphaCinematic;

	// 새로운 Alpha_Cinematic 포인트 설정
	vector<CameraPoint> victoryCinematic;

	CameraPoint VictoryPoint0;
	VictoryPoint0.localPosition = XMVectorSet(3.0f, 3.f, 0.0f, 1.0f);    // 0도
	VictoryPoint0.duration = 3.f; // 원하는 시간으로 조절
	victoryCinematic.push_back(VictoryPoint0);

	CameraPoint VictoryPoint90;
	VictoryPoint90.localPosition = XMVectorSet(0.0f, 3.0f, 3.0f, 1.0f);   // 90도
	VictoryPoint90.duration = 3.f;
	victoryCinematic.push_back(VictoryPoint90);

	CameraPoint VictoryPoint180;
	VictoryPoint180.localPosition = XMVectorSet(-3.0f, 3.0f, 0.0f, 1.0f);  // 180도
	VictoryPoint180.duration = 3.f;
	victoryCinematic.push_back(VictoryPoint180);

	CameraPoint VictoryPoint270;
	VictoryPoint270.localPosition = XMVectorSet(0.0f, 3.0f, -3.0f, 1.0f);  // 270도
	VictoryPoint270.duration = 3.f;
	victoryCinematic.push_back(VictoryPoint270);

	// 컨테이너에 추가 (키는 "Alpha_Cinematic"으로 설정)
	m_Cinematics[L"Victory_Cinematic"] = victoryCinematic;
}



_vector CCamera_Free::CatmullRomSplinePosition(_float t)
{
	// 포인트 개수 확인
	size_t numPoints = m_pCurrentCinematicPoints->size();

	// 현재 인덱스
	size_t index1 = m_CurrentIndex;
	size_t index2 = m_CurrentIndex + 1;

	// 시작 부분 처리

	size_t index0 = (index1 == 0) ? index1 : index1 - 1;

	// 끝 부분 처리
	size_t index3 = (index2 + 1 < numPoints) ? index2 + 1 : index2;

	// 포인트 가져오기
	_vector p0 = (*m_pCurrentCinematicPoints)[index0].localPosition;
	_vector p1 = (*m_pCurrentCinematicPoints)[index1].localPosition;
	_vector p2 = (*m_pCurrentCinematicPoints)[index2].localPosition;
	_vector p3 = (*m_pCurrentCinematicPoints)[index3].localPosition;

	// 내가 실제 넣어준 m_vecPoint에는 카메라가 실제로 이동하려고 하는 포인트들 밖에 없다.
	// 그래서 만약에 양옆에 포인트가 없을 때는 양 끝에서 한칸씩 떨어진 녀석을 양끝의 포인트로 가정한다.
	// p0, p1, p2, p3이라고 한다면 p0에서는 예컨대 p-1을 p0이라고 두는것, 그리고 p4는 p3으로 두는 것.
	// 
	// 시작 부분에서 index0와 index1이 같다면 p0를 p1로 설정
	if (index1 == 0)
		p0 = p1;

	// 끝 부분에서 index2와 index3이 같다면 p3를 p2로 설정
	if (index3 == index2)
		p3 = p2;

	// 스플라인 보간 계산
	return CatmullRom(p0, p1, p2, p3, t);
}


_vector CCamera_Free::CatmullRom(_vector p0, _vector p1, _vector p2, _vector p3, _float t)
{
	_float t2 = t * t;
	_float t3 = t2 * t;

	return 0.5f * (
		(2.0f * p1) +
		(-p0 + p2) * t +
		(2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 +
		(-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3
		);
}


CCamera_Free* CCamera_Free::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Free* pInstance = new CCamera_Free(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCamera_Free"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Free::Clone(void* pArg)
{
	CCamera_Free* pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCamera_Free"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();
}
