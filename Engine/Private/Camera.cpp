#include "..\Public\Camera.h"
#include "GameInstance.h"
CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CCamera::CCamera(const CCamera& Prototype)
	: CGameObject{ Prototype }
	, m_fViewportWidth{ Prototype.m_fViewportWidth }
	, m_fViewportHeight{ Prototype.m_fViewportHeight }
{
}

HRESULT CCamera::Initialize_Prototype()
{
	m_fViewportWidth = 1280;
	m_fViewportHeight = 720;

	return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{
	if (pArg == nullptr)
	{
		//메인카메라
		if (FAILED(__super::Initialize(pArg)))
			return E_FAIL;
	}
	else
	{
		CAMERA_DESC* pDesc = static_cast<CAMERA_DESC*>(pArg);
		m_vEye = pDesc->vEye;
		m_vAt = pDesc->vAt;

		m_fFovy = pDesc->fFovy;
		m_fNear = pDesc->fNear;
		m_fFar = pDesc->fFar;
		m_fMouseSensor = pDesc->fSensor;

		m_Name = pDesc->cName;

		if (FAILED(__super::Initialize(pArg)))
			return E_FAIL;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&m_vEye), 1.f));
		m_pTransformCom->LookAt(XMVectorSetW(XMLoadFloat3(&m_vAt), 1.f));

	}

	return S_OK;
}

//Main_Camera에서 호출되는 이 함수
void CCamera::Priority_Update(_float fTimeDelta)
{

}

void CCamera::Update(_float fTimeDelta)
{
}

void CCamera::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera::Render(_float fTimeDelta)
{
	return S_OK;
}

void CCamera::Update_Camera(CCamera* camera, _bool isPrevPlay, _float fTimeDelta)
{
	//메인카메라에서 선택된 가상카메라가 가지고 있는 포인트를 순회하면서 재생하는 모드
	//실제 가상카메라의 트랜스폼을 이용하면 된다.
	if (isPrevPlay)
	{
		Prev_Play(camera, fTimeDelta);
	}

	//여기서는 가상카메라의 셋팅이 들어가야 한다.
	_matrix Inverse_Matrix = static_cast<CTransform*>(camera->Get_Component(TEXT("Com_Transform")))->Get_WorldMatrix_Inverse();
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_VIEW, Inverse_Matrix);
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(camera->m_fFovy, camera->m_fViewportWidth / camera->m_fViewportHeight, camera->m_fNear, camera->m_fFar));
}

void CCamera::Prev_Play(CCamera* camera, _float fTimeDelta)
{
	list<CameraPoint> points = camera->m_listPoints;

	// 리스트를 벡터로 변환하여 인덱스로 접근 가능하게 함
	vector<CameraPoint> pointVector(points.begin(), points.end());

	//만약 3개의 Point가 들었다면 Index가 CurrentPoint가 마지막이라면 Stop
	if (m_currentPointIndex >= pointVector.size() - 1)
	{
		// 마지막 포인트에 도달했으면 Play 모드 종료
		Prev_Stop();
		return;
	}

	CameraPoint currentPoint = pointVector[m_currentPointIndex];
	CameraPoint nextPoint = pointVector[m_currentPointIndex + 1];

	m_elapsedTime += fTimeDelta;

	if (m_elapsedTime >= currentPoint.duration)
	{
		// 다음 포인트로 이동
		m_currentPointIndex++;
		m_elapsedTime = 0.0f;

		if (m_currentPointIndex >= pointVector.size() - 1)
		{
			// 마지막 포인트에 도달했으면 Play 모드 종료
			Prev_Stop();
			return;
		}

		currentPoint = pointVector[m_currentPointIndex];
		nextPoint = pointVector[m_currentPointIndex + 1];
	}

	// 보간 비율 계산
	float t = m_elapsedTime / currentPoint.duration;

	// 보간 방식에 따른 t 값 조정
	switch (currentPoint.interpolationType)
	{
	case InterpolationType::INTERPOLATION_LINEAR_MODE:
		// t는 그대로 사용
		break;
	case InterpolationType::INTERPOLATION_SPLINE_MODE:
		// Spline 보간 (예시: 간단한 easing)
		t = t * t * (3.0f - 2.0f * t); // Smoothstep
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

	// CTransform 컴포넌트에 설정
	CTransform* cameraTransform = static_cast<CTransform*>(camera->Get_Component(TEXT("Com_Transform")));

	// 방향 벡터 설정
	cameraTransform->Set_State(CTransform::STATE_RIGHT, right);
	cameraTransform->Set_State(CTransform::STATE_UP, up);
	cameraTransform->Set_State(CTransform::STATE_LOOK, look);
	// 위치 설정
	cameraTransform->Set_State(CTransform::STATE_POSITION, position);
}

void CCamera::Prev_Stop()
{
	m_bPrevPlayMode = false;
	m_currentPointIndex = 0;
}


void CCamera::Add_Point(_float duration, InterpolationType type, const _float4x4* pModelFloat4x4)
{
	CameraPoint cameraPoint{};

	//해당 모델의 월드행렬 저장
	cameraPoint.pWorldFloat4x4 = pModelFloat4x4;

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
	_float4 rotation = {};
	XMStoreFloat4(&rotation, localQuaternion);
	cameraPoint.rotation = rotation;

	cameraPoint.duration = duration;
	cameraPoint.interpolationType = type;

	m_listPoints.push_back(cameraPoint);
}

void CCamera::Remove_Point(_int currentIndex)
{
	if (currentIndex < 0 || currentIndex >= static_cast<int>(m_listPoints.size())) {
		return; // 유효하지 않은 인덱스일 경우 아무 작업도 하지 않음
	}

	auto it = m_listPoints.begin();
	advance(it, currentIndex);
	m_listPoints.erase(it);
}

//list의 index 순으로 찾아가서 나온 Position과 Rotation으로 카메라의 트랜스폼을 셋팅하면 되겠다
void CCamera::Move_Point(_int index)
{
	if (index < 0 || index >= m_listPoints.size()) {
		return;
	}

	auto it = m_listPoints.begin();
	std::advance(it, index);  // index 위치로 반복자 이동

	const CameraPoint& targetPoint = *it;

}


void CCamera::Free()
{
	__super::Free();
}
