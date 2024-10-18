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

	//�ʱ� ��ġ
	m_vEye = _float3(0.f, 10.f, -10.f);
	//�ʱ� ����
	m_vAt = _float3(0.f, 0.f, 0.f);
	//ī�޶� �̸�
	m_Name = *static_cast<_char**>(pArg);
	//ī�޶� ���콺 �ΰ���
	m_fMouseSensor = 0.1f;
	//ī�޶� �̵��ӵ�
	Desc.fSpeedPerSec = 1.f;
	//���� ������ ���ǵ�
	m_fMoveSpeed = Desc.fSpeedPerSec;

	//�þ߰�
	Desc.fFovy = XMConvertToRadians(40.0f);
	//Near
	Desc.fNear = 0.1f;
	//Far
	Desc.fFar = 1000.f;
	//ī�޶� ȸ���ӵ�
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
		return; // ���� ���°� Playing�� �ƴϸ� ������Ʈ���� ����

	// ���� 3���� Point�� ����ٸ� Index�� CurrentPoint�� �������̶�� Stop
	if (m_currentPointIndex >= m_vecPoints.size() - 1)
	{
		// ������ ����Ʈ�� ���������� Play ��� ����
		Stop();
		return;
	}

	CameraPoint currentPoint = m_vecPoints[m_currentPointIndex];
	CameraPoint nextPoint = m_vecPoints[m_currentPointIndex + 1];

	m_elapsedTime += fTimeDelta;

	if (m_elapsedTime >= currentPoint.duration)
	{
		// ���� ����Ʈ�� �̵�
		m_currentPointIndex++;
		m_elapsedTime = 0.0f;

		if (m_currentPointIndex >= m_vecPoints.size() - 1)
		{
			// ������ ����Ʈ�� ���������� Play ��� ����
			Stop();
			return;
		}

		currentPoint = m_vecPoints[m_currentPointIndex];
		nextPoint = m_vecPoints[m_currentPointIndex + 1];
	}

	// ���� ���� ���
	float t = m_elapsedTime / currentPoint.duration;

	// ���� ��Ŀ� ���� t �� ����
	switch (currentPoint.interpolationType)
	{
	case InterpolationType::INTERPOLATION_LINEAR_MODE:
		// t�� �״�� ���
		break;
	case InterpolationType::INTERPOLATION_DAMPING_MODE:
		t = AdjustT_Damping(t, currentPoint.damping);
		break;
	case InterpolationType::INTERPOLATION_SKIP_MODE:
		// Skip ����: ��� ���� ����Ʈ�� �̵�
		t = 1.0f;
		break;
	default:
		break;
	}

	// **1. ���� ������ ����**
	_vector interpolatedPositionLocal;
	if (currentPoint.interpolationType != InterpolationType::INTERPOLATION_SKIP_MODE)
	{
		interpolatedPositionLocal = XMVectorLerp(XMLoadFloat3(&currentPoint.position), XMLoadFloat3(&nextPoint.position), t);
	}
	else
	{
		interpolatedPositionLocal = XMLoadFloat3(&nextPoint.position);
	}

	// **2. ���� ȸ�� ���� (Quaternion Slerp ���)**
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

	// **3. ���� ���� ��� �ε�**
	_matrix modelWorldMatrix = Float4x4ToMatrix(*currentPoint.pWorldFloat4x4);

	// **4. ���� �������� ���� ���������� ��ȯ**
	_vector interpolatedPositionWorld = XMVector3TransformCoord(interpolatedPositionLocal, modelWorldMatrix);

	// **5. ���� ȸ���� ���� ȸ������ ��ȯ**
	// ���� ȸ�� ��� ���� (��ġ ���� ����)
	_matrix modelRotationMatrix = modelWorldMatrix;
	modelRotationMatrix.r[3] = XMVectorSet(0, 0, 0, 1);

	// ���� ȸ�� ��� ����
	_matrix interpolatedRotationMatrixLocal = XMMatrixRotationQuaternion(interpolatedRotationLocal);

	// ���� ȸ�� ��� ���
	_matrix interpolatedRotationMatrixWorld = interpolatedRotationMatrixLocal * modelRotationMatrix;

	// **6. ī�޶��� ���� ��� ����**
	_matrix NewWorldMatrix = interpolatedRotationMatrixWorld;
	NewWorldMatrix.r[3] = interpolatedPositionWorld; // ��ġ ����

	// ���� ��Ʈ�������� Right, Up, Look ���� ����
	_vector right = NewWorldMatrix.r[0];
	_vector up = NewWorldMatrix.r[1];
	_vector look = NewWorldMatrix.r[2];
	_vector position = NewWorldMatrix.r[3];
	m_vBaseCameraPosition = position;

	// ���� ���� ����
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

	//DistanceX�� 20�� ������
	if (distanceX > thresholdDistance)
	{
		// ���� ��� (distanceX�� thresholdDistance �̻��� ������ maxDistance����)
		// t�� ������ ���ѵ�
		float t = (distanceX - thresholdDistance) / (maxDistance - thresholdDistance);
		t = max(0.f, min(t, 1.f)); // [0, 1] ������ Ŭ����

		const float maxYOffset = 7.f; // �ִ� Y �̵� �Ÿ�
		const float maxZOffset = 50.f; // �ִ� Z �̵� �Ÿ�

		// ���� ���� �������� (x=0�� ������)
		_float3 tangent1, tangent2;
		m_pGameInstance->Get_ParallelVectorsInPlane(tangent1, tangent2, m_fFovy);

		// �� ���� ������ �̿��Ͽ� Y�� Z ����
		// ���� ���, tangent1�� �̿��Ͽ� Y��, tangent2�� �̿��Ͽ� Z�� ������ �� �ֽ��ϴ�.
		// ���⼭�� �ܼ��� �� ������ �������Ͽ� Y�� Z�� ����
		targetPosition.y += tangent1.y * maxYOffset * t;
		targetPosition.z += tangent1.z * maxZOffset * t;
		// �Ǵ� �ٸ� ������� ������ �� �ֽ��ϴ�.
	}

	// ī�޶� ��ġ ����
	m_pTransformCom->Set_State_Position(targetPosition);
}

void CVirtual_Camera::Set_Camera_Direction(_float averageX, _gvector pos1, _gvector pos2)
{
	// ī�޶��� Look ������ ������ ������ ���� (��: Z���� ���ϵ���)
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

	// Stopped ���¿��� Play�� �����ϸ� �ʱ�ȭ
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
	//�⺻ �̵� �ӵ�
	_float fMoveSpeed = m_fMoveSpeed;

	// ������ ��ư�� ���ȴ��� Ȯ��
	if (m_pGameInstance->Mouse_Pressing(1))
	{
		// Shift Ű�� ���ȴ��� Ȯ���ϰ�, ���ȴٸ� �̵� �ӵ��� ����
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

	//// �÷��̾���� ��ġ�� �����ɴϴ�.
	_vector pos1 = static_cast<CTransform*>(m_p1pPlayer->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
	_vector pos2 = static_cast<CTransform*>(m_p2pPlayer->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

	//// �� �÷��̾� ���� X �Ÿ� ���
	_float distanceX = ComputeDistanceX(pos1, pos2);

	// ��� X ��ġ ���
	_float averageX = (XMVectorGetX(pos1) + XMVectorGetX(pos2)) * 0.5f;

	// ī�޶��� ��ġ ����
	Set_Camera_Position(averageX, distanceX, pos1, pos2);

	//// ī�޶��� ���� ���� ����
	Set_Camera_Direction(averageX, pos1, pos2);
}

_float CVirtual_Camera::ComputeDistanceX(_gvector pos1, _gvector pos2)
{
	// �� �÷��̾� ���� X�� ���� ���
	_float deltaX = XMVectorGetX(XMVectorSubtract(pos2, pos1));
	return abs(deltaX);
}


void CVirtual_Camera::Add_Point(_float duration, InterpolationType type, const _float4x4* pModelFloat4x4, _float damping, _bool hasWorldFloat4x4)
{
	CameraPoint cameraPoint{};

	//�ش� ���� ������� ����

	//����Ʈ ī�޶�� ���� ������ ��� ��������� �����ϰ�
	//������ ī�޶�� ���� ������ ��ȯ�� ������� �����ؾ���
	if (hasWorldFloat4x4 == true)
	{
		_matrix matrix = Float4x4ToMatrix(*pModelFloat4x4);

		// ���� ���� ����� ����� ���
		_vector determinant = XMVectorZero();
		_matrix inverseModelMatrix = XMMatrixInverse(&determinant, matrix);

		// ���� ����ī�޶��� ���� ������ ��������
		_vector worldPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		// ���� �������� ���� ���� ��ǥ�� ��ȯ
		_vector localPosition = XMVector3TransformCoord(worldPosition, inverseModelMatrix);

		// ��ȯ�� ���� �������� CameraPoint ����ü�� ����
		XMStoreFloat3(&cameraPoint.position, localPosition);

		// **ī�޶��� ���� ȸ�� ��� ����**
		_vector right = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
		_vector up = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_UP));
		_vector look = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

		_matrix cameraRotationMatrix = XMMatrixIdentity();
		cameraRotationMatrix.r[0] = right;
		cameraRotationMatrix.r[1] = up;
		cameraRotationMatrix.r[2] = look;
		cameraRotationMatrix.r[3] = XMVectorSet(0, 0, 0, 1); // ��ġ ���� ����

		// **�� ����Ŀ��� ȸ�� �κ� ����**
		_matrix inverseModelRotationMatrix = inverseModelMatrix;
		inverseModelRotationMatrix.r[3] = XMVectorSet(0, 0, 0, 1); // ��ġ ���� ����

		// **ī�޶��� ���� ȸ�� ��� ���**
		_matrix localRotationMatrix = cameraRotationMatrix * inverseModelRotationMatrix;

		// **���� ȸ�� ����� ���ʹϾ����� ��ȯ�Ͽ� ����**
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
		//�׳� ������� ������ ��
		_vector worldPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		XMStoreFloat3(&cameraPoint.position, worldPosition);

		// **ī�޶��� ���� ȸ�� ��� ���� ����**
		_vector right = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
		_vector up = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_UP));
		_vector look = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

		_matrix cameraRotationMatrix = XMMatrixIdentity();
		cameraRotationMatrix.r[0] = right;
		cameraRotationMatrix.r[1] = up;
		cameraRotationMatrix.r[2] = look;
		cameraRotationMatrix.r[3] = XMVectorSet(0, 0, 0, 1); // ��ġ ���� ����

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
		return; // ��ȿ���� ���� �ε����� ��� �ƹ� �۾��� ���� ����
	}

	auto it = m_vecPoints.begin();
	advance(it, currentIndex);
	m_vecPoints.erase(it);
}

//list�� index ������ ã�ư��� ���� Position�� Rotation���� ī�޶��� Ʈ�������� �����ϸ� �ǰڴ�
void CVirtual_Camera::Move_Point(_int index)
{
	if (index < 0 || index >= m_vecPoints.size()) {
		return;
	}

	const CameraPoint& targetPoint = m_vecPoints[index];  // �ε��� ����

	if (targetPoint.hasWorldFloat4x4 == true)
	{
		// 1. ���� ������
		_float3 localPosition = targetPoint.position;

		// 2. ���� ȸ��
		_float4 localQuaternion = targetPoint.rotation;

		// **3. ���� ���� ��� �ε�**
		_matrix modelWorldMatrix = Float4x4ToMatrix(*targetPoint.pWorldFloat4x4);

		// **4. ���� �������� ���� ���������� ��ȯ**
		_vector interpolatedPositionWorld = XMVector3TransformCoord(XMLoadFloat3(&localPosition), modelWorldMatrix);

		// **5. ���� ȸ���� ���� ȸ������ ��ȯ**
		// ���� ȸ�� ��� ���� (��ġ ���� ����)
		_matrix modelRotationMatrix = modelWorldMatrix;
		modelRotationMatrix.r[3] = XMVectorSet(0, 0, 0, 1);

		// ���� ȸ�� ��� ����
		_matrix interpolatedRotationMatrixLocal = XMMatrixRotationQuaternion(XMLoadFloat4(&localQuaternion));

		// ���� ȸ�� ��� ���
		_matrix interpolatedRotationMatrixWorld = interpolatedRotationMatrixLocal * modelRotationMatrix;

		// **6. ī�޶��� ���� ��� ����**
		_matrix NewWorldMatrix = interpolatedRotationMatrixWorld;
		NewWorldMatrix.r[3] = interpolatedPositionWorld; // ��ġ ����

		// ���� ��Ʈ�������� Right, Up, Look ���� ����
		_vector right = NewWorldMatrix.r[0];
		_vector up = NewWorldMatrix.r[1];
		_vector look = NewWorldMatrix.r[2];
		_vector position = NewWorldMatrix.r[3];

		// CTransform ������Ʈ�� ����
		CTransform* cameraTransform = static_cast<CTransform*>(Get_Component(TEXT("Com_Transform")));

		// ���� ���� ����
		cameraTransform->Set_State(CTransform::STATE_RIGHT, right);
		cameraTransform->Set_State(CTransform::STATE_UP, up);
		cameraTransform->Set_State(CTransform::STATE_LOOK, look);
		// ��ġ ����
		cameraTransform->Set_State(CTransform::STATE_POSITION, position);
	}
	else
	{
		// CTransform ������Ʈ�� ����
		CTransform* cameraTransform = static_cast<CTransform*>(Get_Component(TEXT("Com_Transform")));

		//������� ����
		_float3 worldPosition = targetPoint.position;

		// 2. ���� ȸ��
		_float4 worldQuaternion = targetPoint.rotation;
		// ���� ȸ�� ��� ����
		_matrix interpolatedRotationMatrixWorld = XMMatrixRotationQuaternion(XMLoadFloat4(&worldQuaternion));


		// **6. ī�޶��� ���� ��� ����**
		_matrix NewWorldMatrix = interpolatedRotationMatrixWorld;
		NewWorldMatrix.r[3] = XMVectorSetW(XMLoadFloat3(&worldPosition), 1.f); // ��ġ ����

		// ���� ��Ʈ�������� Right, Up, Look ���� ����
		_vector right = NewWorldMatrix.r[0];
		_vector up = NewWorldMatrix.r[1];
		_vector look = NewWorldMatrix.r[2];
		_vector position = NewWorldMatrix.r[3];

		// ���� ���� ����
		cameraTransform->Set_State(CTransform::STATE_RIGHT, right);
		cameraTransform->Set_State(CTransform::STATE_UP, up);
		cameraTransform->Set_State(CTransform::STATE_LOOK, look);
		// ��ġ ����
		cameraTransform->Set_State(CTransform::STATE_POSITION, position);
	}
}

void CVirtual_Camera::Modify_Transform(_int index)
{


	CameraPoint& targetPoint = m_vecPoints[index];  // �ε��� ����

	//���� �����Ϸ��� �ϴ� �ε����� Point�� �����ؼ�
	//���� ����ī�޶��� �����ǰ� Rotation�� ���÷� ������ �����ؾ���
	if (targetPoint.pWorldFloat4x4 != nullptr)
	{
		_matrix matrix = Float4x4ToMatrix(*targetPoint.pWorldFloat4x4);

		// ���� ���� ����� ����� ���
		_vector determinant = XMVectorZero();
		_matrix inverseModelMatrix = XMMatrixInverse(&determinant, matrix);

		// ���� ����ī�޶��� ���� ������ ��������
		_vector worldPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		// ���� �������� ���� ���� ��ǥ�� ��ȯ
		_vector localPosition = XMVector3TransformCoord(worldPosition, inverseModelMatrix);

		// ��ȯ�� ���� �������� CameraPoint ����ü�� ����
		XMStoreFloat3(&targetPoint.position, localPosition);

		// **ī�޶��� ���� ȸ�� ��� ����**
		_vector right = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
		_vector up = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_UP));
		_vector look = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

		_matrix cameraRotationMatrix = XMMatrixIdentity();
		cameraRotationMatrix.r[0] = right;
		cameraRotationMatrix.r[1] = up;
		cameraRotationMatrix.r[2] = look;
		cameraRotationMatrix.r[3] = XMVectorSet(0, 0, 0, 1); // ��ġ ���� ����

		// **�� ����Ŀ��� ȸ�� �κ� ����**
		_matrix inverseModelRotationMatrix = inverseModelMatrix;
		inverseModelRotationMatrix.r[3] = XMVectorSet(0, 0, 0, 1); // ��ġ ���� ����

		// **ī�޶��� ���� ȸ�� ��� ���**
		_matrix localRotationMatrix = cameraRotationMatrix * inverseModelRotationMatrix;

		// **���� ȸ�� ����� ���ʹϾ����� ��ȯ�Ͽ� ����**
		_vector localQuaternion = XMQuaternionRotationMatrix(localRotationMatrix);
		_float4 rotation = {};
		XMStoreFloat4(&rotation, localQuaternion);
		targetPoint.rotation = rotation;
	}
	//����Ʈ�� ���� �����ǰ� �����̼Ǹ�
	else
	{
		// ���� ����ī�޶��� ���� ������ ������ �� ����
		_vector worldPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		XMStoreFloat3(&targetPoint.position, worldPosition);

		// **ī�޶��� ���� ȸ�� ��� ����**
		_vector right = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
		_vector up = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_UP));
		_vector look = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

		_matrix cameraRotationMatrix = XMMatrixIdentity();
		cameraRotationMatrix.r[0] = right;
		cameraRotationMatrix.r[1] = up;
		cameraRotationMatrix.r[2] = look;
		cameraRotationMatrix.r[3] = XMVectorSet(0, 0, 0, 1); // ��ġ ���� ����

		_vector worldQuaternion = XMQuaternionRotationMatrix(cameraRotationMatrix);
		XMStoreFloat4(&targetPoint.rotation, worldQuaternion);
	}
}

_float CVirtual_Camera::AdjustT_Damping(_float t, _float damping)
{
	// Damping ����� ���� t ���� ����
	 // damping > 1.0f: �� ������ �����ϰ� ������ ���� (ease-in)
	 // damping < 1.0f: �� ������ �����ϰ� ������ ���� (ease-out)
	 // damping = 1.0f: �⺻ Smoothstep �Լ��� ����
	if (damping == 1.0f)
	{
		return t * t * (3.0f - 2.0f * t); // �⺻ Smoothstep
	}
	else if (damping > 1.0f)
	{
		// Ease-in ȿ�� ��ȭ
		return pow(t, damping) * (3.0f - 2.0f * t);
	}
	else // damping < 1.0f
	{
		// Ease-out ȿ�� ��ȭ
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
