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
	m_vEye = _float3(0.f, 10.f, -10.f);
	//초기 각도
	m_vAt = _float3(0.f, 0.f, 0.f);
	//카메라 이름
	m_Name = *static_cast<_char**>(pArg);
	//카메라 마우스 민감도
	m_fMouseSensor = 0.1f;
	//카메라 이동속도
	Desc.fSpeedPerSec = 1.f;
	//실제 조정할 스피드
	m_fMoveSpeed = Desc.fSpeedPerSec;

	//시야각
	Desc.fFovy = XMConvertToRadians(40.0f);
	//Near
	Desc.fNear = 0.1f;
	//Far
	Desc.fFar = 1000.f;
	//카메라 회전속도
	Desc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&m_vEye), 1.f));
	m_pTransformCom->LookAt(XMVectorSetW(XMLoadFloat3(&m_vAt), 1.f));

	return S_OK;
}

void CVirtual_Camera::Priority_Update(_float fTimeDelta)
{
	switch (m_currentMode)
	{
	case CAMERA_FREE_MODE:
		Free_Camera(fTimeDelta);
		break;
	case CAMERA_DEFAULT_MODE:
		Default_Camera(fTimeDelta);
		break;
	case CAMERA_CINEMATIC_MODE:
		if (m_currentPlayMode == Playing)
			Play(fTimeDelta);
		break;
	}

	if (m_bIsShaking)
		ApplyCameraShake(fTimeDelta);
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

void CVirtual_Camera::Play(_float fTimeDelta)
{
	if (m_currentPlayMode != CAMERA_PLAY_MODE::Playing)
		return; // 현재 상태가 Playing이 아니면 업데이트하지 않음

	// 만약 3개의 Point가 들었다면 Index가 CurrentPoint가 마지막이라면 Stop
	if (m_currentPointIndex >= m_vecPoints.size() - 1)
	{
		// 마지막 포인트에 도달했으면 Play 모드 종료
		Stop();
		return;
	}

	CameraPoint currentPoint = m_vecPoints[m_currentPointIndex];
	CameraPoint nextPoint = m_vecPoints[m_currentPointIndex + 1];

	m_elapsedTime += fTimeDelta;

	if (m_elapsedTime >= currentPoint.duration)
	{
		// 다음 포인트로 이동
		m_currentPointIndex++;
		m_elapsedTime = 0.0f;

		if (m_currentPointIndex >= m_vecPoints.size() - 1)
		{
			// 마지막 포인트에 도달했으면 Play 모드 종료
			Stop();
			return;
		}

		currentPoint = m_vecPoints[m_currentPointIndex];
		nextPoint = m_vecPoints[m_currentPointIndex + 1];
	}

	// 보간 비율 계산
	float t = m_elapsedTime / currentPoint.duration;

	// 보간 방식에 따른 t 값 조정
	switch (currentPoint.interpolationType)
	{
	case InterpolationType::INTERPOLATION_LINEAR_MODE:
		// t는 그대로 사용
		break;
	case InterpolationType::INTERPOLATION_DAMPING_MODE:
		t = AdjustT_Damping(t, currentPoint.damping);
		break;
	case InterpolationType::INTERPOLATION_SKIP_MODE:
		// Skip 보간: 즉시 다음 포인트로 이동
		t = 1.0f;
		break;
	default:
		break;
	}

	// **1. 로컬 포지션 보간**
	_vector interpolatedPositionLocal;
	if (currentPoint.interpolationType != InterpolationType::INTERPOLATION_SKIP_MODE)
	{
		interpolatedPositionLocal = XMVectorLerp(XMLoadFloat3(&currentPoint.position), XMLoadFloat3(&nextPoint.position), t);
	}
	else
	{
		interpolatedPositionLocal = XMLoadFloat3(&nextPoint.position);
	}

	// **2. 로컬 회전 보간 (Quaternion Slerp 사용)**
	_vector interpolatedRotationLocal;
	if (currentPoint.interpolationType != InterpolationType::INTERPOLATION_SKIP_MODE)
	{
		_vector q1 = XMLoadFloat4(&currentPoint.rotation);
		_vector q2 = XMLoadFloat4(&nextPoint.rotation);
		interpolatedRotationLocal = XMQuaternionSlerp(q1, q2, t);
	}
	else
	{
		interpolatedRotationLocal = XMLoadFloat4(&nextPoint.rotation);
	}

	// **3. 모델의 월드 행렬 로드**
	_matrix modelWorldMatrix = Float4x4ToMatrix(*currentPoint.pWorldFloat4x4);

	// **4. 로컬 포지션을 월드 포지션으로 변환**
	_vector interpolatedPositionWorld = XMVector3TransformCoord(interpolatedPositionLocal, modelWorldMatrix);

	// **5. 로컬 회전을 월드 회전으로 변환**
	// 모델의 회전 행렬 추출 (위치 정보 제거)
	_matrix modelRotationMatrix = modelWorldMatrix;
	modelRotationMatrix.r[3] = XMVectorSet(0, 0, 0, 1);

	// 로컬 회전 행렬 생성
	_matrix interpolatedRotationMatrixLocal = XMMatrixRotationQuaternion(interpolatedRotationLocal);

	// 월드 회전 행렬 계산
	_matrix interpolatedRotationMatrixWorld = interpolatedRotationMatrixLocal * modelRotationMatrix;

	// **6. 카메라의 월드 행렬 생성**
	_matrix NewWorldMatrix = interpolatedRotationMatrixWorld;
	NewWorldMatrix.r[3] = interpolatedPositionWorld; // 위치 설정

	// 월드 매트릭스에서 Right, Up, Look 벡터 추출
	_vector right = NewWorldMatrix.r[0];
	_vector up = NewWorldMatrix.r[1];
	_vector look = NewWorldMatrix.r[2];
	_vector position = NewWorldMatrix.r[3];
	m_vBaseCameraPosition = position;

	// 방향 벡터 설정
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, right);
	m_pTransformCom->Set_State(CTransform::STATE_UP, up);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, look);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, position + m_vShakeOffset);
}

void CVirtual_Camera::Set_Camera_Position(_float averageX, _float distanceX, _gvector pos1, _gvector pos2)
{
	const float fixedY = 20.f;
	const float fixedZ = -50.f;

	const float thresholdDistance = 40.f;
	const float maxDistance = 60.f;

	float offsetX = 0.f;
	_float3 targetPosition = _float3(averageX + offsetX, fixedY, fixedZ);

	//DistanceX가 20을 넘으면
	if (distanceX > thresholdDistance)
	{
		// 비율 계산 (distanceX가 thresholdDistance 이상일 때부터 maxDistance까지)
		// t의 비율을 구한뒤
		float t = (distanceX - thresholdDistance) / (maxDistance - thresholdDistance);
		t = max(0.f, min(t, 1.f)); // [0, 1] 범위로 클램프

		const float maxYOffset = 7.f; // 최대 Y 이동 거리
		const float maxZOffset = 50.f; // 최대 Z 이동 거리

		// 평행 벡터 가져오기 (x=0을 만족함)
		_float3 tangent1, tangent2;
		m_pGameInstance->Get_ParallelVectorsInPlane(tangent1, tangent2, m_fFovy);

		// 두 평행 벡터을 이용하여 Y와 Z 조절
		// 예를 들어, tangent1을 이용하여 Y를, tangent2를 이용하여 Z를 조절할 수 있습니다.
		// 여기서는 단순히 두 벡터을 가중합하여 Y와 Z를 조절
		targetPosition.y += tangent1.y * maxYOffset * t;
		targetPosition.z += tangent1.z * maxZOffset * t;
		// 또는 다른 방식으로 조절할 수 있습니다.
	}

	// 카메라 위치 설정
	m_pTransformCom->Set_State_Position(targetPosition);
}

void CVirtual_Camera::Set_Camera_Direction(_float averageX, _gvector pos1, _gvector pos2)
{
	// 카메라의 Look 방향을 고정된 값으로 설정 (예: Z축을 향하도록)
	_vector fixedLook = XMVectorSet(-0.05f, -0.14f, 0.98f, 0.f);
	_vector fixedRight = XMVectorSet(0.9984f, 0.f, 0.054f, 0.f);
	_vector fixedUp = XMVectorSet(-0.00773f, 0.99f, 0.14f, 0.f);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, fixedRight);
	m_pTransformCom->Set_State(CTransform::STATE_UP, fixedUp);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, fixedLook);
}

void CVirtual_Camera::SetPlayer(CMain_Camera::PLAYER_STATE state, CGameObject* pPlayer)
{
	if (state == CMain_Camera::PLAYER_1P)
		m_p1pPlayer = pPlayer;
	else if (state == CMain_Camera::PLAYER_2P)
		m_p2pPlayer = pPlayer;

	Safe_AddRef(pPlayer);
}

void CVirtual_Camera::Start_Play()
{
	if (m_vecPoints.size() == 0)
		return;

	// Stopped 상태에서 Play를 시작하면 초기화
	if (m_currentPlayMode == Stopped) {
		m_currentPointIndex = 0;
		m_elapsedTime = 0.f;
	}

	m_currentMode = CAMERA_CINEMATIC_MODE;
	m_currentPlayMode = Playing;
}

void CVirtual_Camera::Pause()
{
	if (m_currentPlayMode == CAMERA_PLAY_MODE::Playing)
		m_currentPlayMode = CAMERA_PLAY_MODE::Paused;
}


void CVirtual_Camera::Stop()
{
	m_currentMode = CAMERA_FREE_MODE;
	m_currentPlayMode = CAMERA_PLAY_MODE::Stopped;
	m_currentPointIndex = 0;
	m_elapsedTime = 0.f;
	//Move_Point(0);
}

void CVirtual_Camera::Button_Stop()
{
	m_currentMode = CAMERA_FREE_MODE;
	m_currentPlayMode = CAMERA_PLAY_MODE::Stopped;
	m_currentPointIndex = 0;
	m_elapsedTime = 0.f;
	Move_Point(0);
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
			fMoveSpeed *= 50.f;
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

}

void CVirtual_Camera::Default_Camera(_float fTimeDelta)
{
	if (m_p1pPlayer == nullptr || m_p2pPlayer == nullptr)
		return;

	//// 플레이어들의 위치를 가져옵니다.
	_vector pos1 = static_cast<CTransform*>(m_p1pPlayer->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
	_vector pos2 = static_cast<CTransform*>(m_p2pPlayer->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

	//// 두 플레이어 간의 X 거리 계산
	_float distanceX = ComputeDistanceX(pos1, pos2);

	// 평균 X 위치 계산
	_float averageX = (XMVectorGetX(pos1) + XMVectorGetX(pos2)) * 0.5f;

	// 카메라의 위치 설정
	Set_Camera_Position(averageX, distanceX, pos1, pos2);

	//// 카메라의 방향 벡터 설정
	Set_Camera_Direction(averageX, pos1, pos2);
}

_float CVirtual_Camera::ComputeDistanceX(_gvector pos1, _gvector pos2)
{
	// 두 플레이어 간의 X축 차이 계산
	_float deltaX = XMVectorGetX(XMVectorSubtract(pos2, pos1));
	return abs(deltaX);
}


void CVirtual_Camera::Add_Point(_float duration, InterpolationType type, const _float4x4* pModelFloat4x4, _float damping, _bool hasWorldFloat4x4)
{
	CameraPoint cameraPoint{};

	//해당 모델의 월드행렬 저장

	//디폴트 카메라는 모델의 로컬이 없어서 월드행렬을 저장하고
	//나머지 카메라는 모델의 로컬을 변환한 월드행렬 저장해야함
	if (hasWorldFloat4x4 == true)
	{
		_matrix matrix = Float4x4ToMatrix(*pModelFloat4x4);

		// 모델의 월드 행렬의 역행렬 계산
		_vector determinant = XMVectorZero();
		_matrix inverseModelMatrix = XMMatrixInverse(&determinant, matrix);

		// 현재 가상카메라의 월드 포지션 가져오기
		_vector worldPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		// 월드 포지션을 모델의 로컬 좌표로 변환
		_vector localPosition = XMVector3TransformCoord(worldPosition, inverseModelMatrix);

		// 변환된 로컬 포지션을 CameraPoint 구조체에 저장
		XMStoreFloat3(&cameraPoint.position, localPosition);

		// **카메라의 월드 회전 행렬 생성**
		_vector right = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
		_vector up = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_UP));
		_vector look = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

		_matrix cameraRotationMatrix = XMMatrixIdentity();
		cameraRotationMatrix.r[0] = right;
		cameraRotationMatrix.r[1] = up;
		cameraRotationMatrix.r[2] = look;
		cameraRotationMatrix.r[3] = XMVectorSet(0, 0, 0, 1); // 위치 정보 제거

		// **모델 역행렬에서 회전 부분 추출**
		_matrix inverseModelRotationMatrix = inverseModelMatrix;
		inverseModelRotationMatrix.r[3] = XMVectorSet(0, 0, 0, 1); // 위치 정보 제거

		// **카메라의 로컬 회전 행렬 계산**
		_matrix localRotationMatrix = cameraRotationMatrix * inverseModelRotationMatrix;

		// **로컬 회전 행렬을 쿼터니언으로 변환하여 저장**
		_vector localQuaternion = XMQuaternionRotationMatrix(localRotationMatrix);
		XMStoreFloat4(&cameraPoint.rotation, localQuaternion);

		cameraPoint.duration = duration;
		cameraPoint.interpolationType = type;
		cameraPoint.damping = damping;
		cameraPoint.pWorldFloat4x4 = pModelFloat4x4;
		cameraPoint.hasWorldFloat4x4 = hasWorldFloat4x4;

		m_vecPoints.push_back(cameraPoint);
	}
	else
	{
		//그냥 월드행렬 저장할 것
		_vector worldPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		XMStoreFloat3(&cameraPoint.position, worldPosition);

		// **카메라의 월드 회전 행렬 정보 생성**
		_vector right = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
		_vector up = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_UP));
		_vector look = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

		_matrix cameraRotationMatrix = XMMatrixIdentity();
		cameraRotationMatrix.r[0] = right;
		cameraRotationMatrix.r[1] = up;
		cameraRotationMatrix.r[2] = look;
		cameraRotationMatrix.r[3] = XMVectorSet(0, 0, 0, 1); // 위치 정보 제거

		_vector worldQuaternion = XMQuaternionRotationMatrix(cameraRotationMatrix);
		XMStoreFloat4(&cameraPoint.rotation, worldQuaternion);

		cameraPoint.duration = duration;
		cameraPoint.interpolationType = type;
		cameraPoint.damping = damping;
		cameraPoint.pWorldFloat4x4 = nullptr;
		cameraPoint.hasWorldFloat4x4 = hasWorldFloat4x4;
		m_vecPoints.push_back(cameraPoint);
	}
}

void CVirtual_Camera::Remove_Point(_int currentIndex)
{
	if (currentIndex < 0 || currentIndex >= static_cast<int>(m_vecPoints.size())) {
		return; // 유효하지 않은 인덱스일 경우 아무 작업도 하지 않음
	}

	auto it = m_vecPoints.begin();
	advance(it, currentIndex);
	m_vecPoints.erase(it);
}

//list의 index 순으로 찾아가서 나온 Position과 Rotation으로 카메라의 트랜스폼을 셋팅하면 되겠다
void CVirtual_Camera::Move_Point(_int index)
{
	if (index < 0 || index >= m_vecPoints.size()) {
		return;
	}

	const CameraPoint& targetPoint = m_vecPoints[index];  // 인덱스 접근

	if (targetPoint.hasWorldFloat4x4 == true)
	{
		// 1. 로컬 포지션
		_float3 localPosition = targetPoint.position;

		// 2. 로컬 회전
		_float4 localQuaternion = targetPoint.rotation;

		// **3. 모델의 월드 행렬 로드**
		_matrix modelWorldMatrix = Float4x4ToMatrix(*targetPoint.pWorldFloat4x4);

		// **4. 로컬 포지션을 월드 포지션으로 변환**
		_vector interpolatedPositionWorld = XMVector3TransformCoord(XMLoadFloat3(&localPosition), modelWorldMatrix);

		// **5. 로컬 회전을 월드 회전으로 변환**
		// 모델의 회전 행렬 추출 (위치 정보 제거)
		_matrix modelRotationMatrix = modelWorldMatrix;
		modelRotationMatrix.r[3] = XMVectorSet(0, 0, 0, 1);

		// 로컬 회전 행렬 생성
		_matrix interpolatedRotationMatrixLocal = XMMatrixRotationQuaternion(XMLoadFloat4(&localQuaternion));

		// 월드 회전 행렬 계산
		_matrix interpolatedRotationMatrixWorld = interpolatedRotationMatrixLocal * modelRotationMatrix;

		// **6. 카메라의 월드 행렬 생성**
		_matrix NewWorldMatrix = interpolatedRotationMatrixWorld;
		NewWorldMatrix.r[3] = interpolatedPositionWorld; // 위치 설정

		// 월드 매트릭스에서 Right, Up, Look 벡터 추출
		_vector right = NewWorldMatrix.r[0];
		_vector up = NewWorldMatrix.r[1];
		_vector look = NewWorldMatrix.r[2];
		_vector position = NewWorldMatrix.r[3];

		// CTransform 컴포넌트에 설정
		CTransform* cameraTransform = static_cast<CTransform*>(Get_Component(TEXT("Com_Transform")));

		// 방향 벡터 설정
		cameraTransform->Set_State(CTransform::STATE_RIGHT, right);
		cameraTransform->Set_State(CTransform::STATE_UP, up);
		cameraTransform->Set_State(CTransform::STATE_LOOK, look);
		// 위치 설정
		cameraTransform->Set_State(CTransform::STATE_POSITION, position);
	}
	else
	{
		// CTransform 컴포넌트에 설정
		CTransform* cameraTransform = static_cast<CTransform*>(Get_Component(TEXT("Com_Transform")));

		//월드행렬 셋팅
		_float3 worldPosition = targetPoint.position;

		// 2. 월드 회전
		_float4 worldQuaternion = targetPoint.rotation;
		// 월드 회전 행렬 생성
		_matrix interpolatedRotationMatrixWorld = XMMatrixRotationQuaternion(XMLoadFloat4(&worldQuaternion));


		// **6. 카메라의 월드 행렬 생성**
		_matrix NewWorldMatrix = interpolatedRotationMatrixWorld;
		NewWorldMatrix.r[3] = XMVectorSetW(XMLoadFloat3(&worldPosition), 1.f); // 위치 설정

		// 월드 매트릭스에서 Right, Up, Look 벡터 추출
		_vector right = NewWorldMatrix.r[0];
		_vector up = NewWorldMatrix.r[1];
		_vector look = NewWorldMatrix.r[2];
		_vector position = NewWorldMatrix.r[3];

		// 방향 벡터 설정
		cameraTransform->Set_State(CTransform::STATE_RIGHT, right);
		cameraTransform->Set_State(CTransform::STATE_UP, up);
		cameraTransform->Set_State(CTransform::STATE_LOOK, look);
		// 위치 설정
		cameraTransform->Set_State(CTransform::STATE_POSITION, position);
	}
}

void CVirtual_Camera::Modify_Transform(_int index)
{


	CameraPoint& targetPoint = m_vecPoints[index];  // 인덱스 접근

	//현재 수정하려고 하는 인덱스의 Point에 접근해서
	//현재 가상카메라의 포지션과 Rotation을 로컬로 내려서 저장해야함
	if (targetPoint.pWorldFloat4x4 != nullptr)
	{
		_matrix matrix = Float4x4ToMatrix(*targetPoint.pWorldFloat4x4);

		// 모델의 월드 행렬의 역행렬 계산
		_vector determinant = XMVectorZero();
		_matrix inverseModelMatrix = XMMatrixInverse(&determinant, matrix);

		// 현재 가상카메라의 월드 포지션 가져오기
		_vector worldPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		// 월드 포지션을 모델의 로컬 좌표로 변환
		_vector localPosition = XMVector3TransformCoord(worldPosition, inverseModelMatrix);

		// 변환된 로컬 포지션을 CameraPoint 구조체에 저장
		XMStoreFloat3(&targetPoint.position, localPosition);

		// **카메라의 월드 회전 행렬 생성**
		_vector right = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
		_vector up = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_UP));
		_vector look = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

		_matrix cameraRotationMatrix = XMMatrixIdentity();
		cameraRotationMatrix.r[0] = right;
		cameraRotationMatrix.r[1] = up;
		cameraRotationMatrix.r[2] = look;
		cameraRotationMatrix.r[3] = XMVectorSet(0, 0, 0, 1); // 위치 정보 제거

		// **모델 역행렬에서 회전 부분 추출**
		_matrix inverseModelRotationMatrix = inverseModelMatrix;
		inverseModelRotationMatrix.r[3] = XMVectorSet(0, 0, 0, 1); // 위치 정보 제거

		// **카메라의 로컬 회전 행렬 계산**
		_matrix localRotationMatrix = cameraRotationMatrix * inverseModelRotationMatrix;

		// **로컬 회전 행렬을 쿼터니언으로 변환하여 저장**
		_vector localQuaternion = XMQuaternionRotationMatrix(localRotationMatrix);
		_float4 rotation = {};
		XMStoreFloat4(&rotation, localQuaternion);
		targetPoint.rotation = rotation;
	}
	//디폴트는 월드 포지션과 로테이션만
	else
	{
		// 현재 가상카메라의 월드 포지션 가져와 서 저장
		_vector worldPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		XMStoreFloat3(&targetPoint.position, worldPosition);

		// **카메라의 월드 회전 행렬 생성**
		_vector right = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
		_vector up = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_UP));
		_vector look = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

		_matrix cameraRotationMatrix = XMMatrixIdentity();
		cameraRotationMatrix.r[0] = right;
		cameraRotationMatrix.r[1] = up;
		cameraRotationMatrix.r[2] = look;
		cameraRotationMatrix.r[3] = XMVectorSet(0, 0, 0, 1); // 위치 정보 제거

		_vector worldQuaternion = XMQuaternionRotationMatrix(cameraRotationMatrix);
		XMStoreFloat4(&targetPoint.rotation, worldQuaternion);
	}
}

_float CVirtual_Camera::AdjustT_Damping(_float t, _float damping)
{
	// Damping 계수에 따라 t 값을 조절
	 // damping > 1.0f: 더 느리게 시작하고 빠르게 끝남 (ease-in)
	 // damping < 1.0f: 더 빠르게 시작하고 느리게 끝남 (ease-out)
	 // damping = 1.0f: 기본 Smoothstep 함수와 동일
	if (damping == 1.0f)
	{
		return t * t * (3.0f - 2.0f * t); // 기본 Smoothstep
	}
	else if (damping > 1.0f)
	{
		// Ease-in 효과 강화
		return pow(t, damping) * (3.0f - 2.0f * t);
	}
	else // damping < 1.0f
	{
		// Ease-out 효과 강화
		return t * t * (3.0f - 2.0f * t) + (1.0f - t) * t * (1.0f - damping);
	}
}

void CVirtual_Camera::Delete_Points()
{
	m_vecPoints.clear();
}

void CVirtual_Camera::ApplyCameraShake(_float fTimeDelta)
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
}

void CVirtual_Camera::StartCameraShake(_float fDuration, _float fMagnitude)
{
	m_vBaseCameraPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_bIsShaking = true;
	m_fShakeDuration = fDuration;
	m_fShakeMagnitude = fMagnitude;
	m_fElapsedShakeTime = 0.0f;
}

void CVirtual_Camera::StopCameraShake()
{
	m_bIsShaking = false;
	m_fShakeDuration = 0.0f;
	m_fElapsedShakeTime = 0.0f;
	m_vShakeOffset = XMVectorZero();
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
	Safe_Release(m_p1pPlayer);
	Safe_Release(m_p2pPlayer);

	__super::Free();

}
