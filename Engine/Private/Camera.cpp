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
		//����ī�޶�
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

//Main_Camera���� ȣ��Ǵ� �� �Լ�
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
	//����ī�޶󿡼� ���õ� ����ī�޶� ������ �ִ� ����Ʈ�� ��ȸ�ϸ鼭 ����ϴ� ���
	//���� ����ī�޶��� Ʈ�������� �̿��ϸ� �ȴ�.
	if (isPrevPlay)
	{
		Prev_Play(camera, fTimeDelta);
	}

	//���⼭�� ����ī�޶��� ������ ���� �Ѵ�.
	_matrix Inverse_Matrix = static_cast<CTransform*>(camera->Get_Component(TEXT("Com_Transform")))->Get_WorldMatrix_Inverse();
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_VIEW, Inverse_Matrix);
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(camera->m_fFovy, camera->m_fViewportWidth / camera->m_fViewportHeight, camera->m_fNear, camera->m_fFar));
}

void CCamera::Prev_Play(CCamera* camera, _float fTimeDelta)
{
	list<CameraPoint> points = camera->m_listPoints;

	// ����Ʈ�� ���ͷ� ��ȯ�Ͽ� �ε����� ���� �����ϰ� ��
	vector<CameraPoint> pointVector(points.begin(), points.end());

	//���� 3���� Point�� ����ٸ� Index�� CurrentPoint�� �������̶�� Stop
	if (m_currentPointIndex >= pointVector.size() - 1)
	{
		// ������ ����Ʈ�� ���������� Play ��� ����
		Prev_Stop();
		return;
	}

	CameraPoint currentPoint = pointVector[m_currentPointIndex];
	CameraPoint nextPoint = pointVector[m_currentPointIndex + 1];

	m_elapsedTime += fTimeDelta;

	if (m_elapsedTime >= currentPoint.duration)
	{
		// ���� ����Ʈ�� �̵�
		m_currentPointIndex++;
		m_elapsedTime = 0.0f;

		if (m_currentPointIndex >= pointVector.size() - 1)
		{
			// ������ ����Ʈ�� ���������� Play ��� ����
			Prev_Stop();
			return;
		}

		currentPoint = pointVector[m_currentPointIndex];
		nextPoint = pointVector[m_currentPointIndex + 1];
	}

	// ���� ���� ���
	float t = m_elapsedTime / currentPoint.duration;

	// ���� ��Ŀ� ���� t �� ����
	switch (currentPoint.interpolationType)
	{
	case InterpolationType::INTERPOLATION_LINEAR_MODE:
		// t�� �״�� ���
		break;
	case InterpolationType::INTERPOLATION_SPLINE_MODE:
		// Spline ���� (����: ������ easing)
		t = t * t * (3.0f - 2.0f * t); // Smoothstep
		break;
	case InterpolationType::INTERPOLATION_SKIP_MODE:
		// Skip ����: ��� ���� ����Ʈ�� �̵�
		t = 1.0f;
		break;
	default:
		break;
	}

	const _float4x4* pWorldFloat4x4 = currentPoint.pWorldFloat4x4;

	// ������ ����
	_float3 interpolatedPosition = {};
	if (currentPoint.interpolationType != InterpolationType::INTERPOLATION_SKIP_MODE)
	{
		interpolatedPosition.x = currentPoint.position.x + (nextPoint.position.x - currentPoint.position.x) * t;
		interpolatedPosition.y = currentPoint.position.y + (nextPoint.position.y - currentPoint.position.y) * t;
		interpolatedPosition.z = currentPoint.position.z + (nextPoint.position.z - currentPoint.position.z) * t;
	}
	else
	{
		interpolatedPosition = nextPoint.position;
	}

	// ȸ�� ���� (Quaternion Slerp ���)
	_vector interpolatedRotation;
	if (currentPoint.interpolationType != InterpolationType::INTERPOLATION_SKIP_MODE)
	{
		// ���� ����Ʈ�� ���� ����Ʈ�� ���ʹϾ�
		_vector q1 = XMVectorSet(currentPoint.rotation.x, currentPoint.rotation.y, currentPoint.rotation.z, currentPoint.rotation.w);
		_vector q2 = XMVectorSet(nextPoint.rotation.x, nextPoint.rotation.y, nextPoint.rotation.z, nextPoint.rotation.w);
		_vector qInterp = XMQuaternionSlerp(q1, q2, t);
		_float4 quat;
		XMStoreFloat4(&quat, qInterp);
		interpolatedRotation = _vector{ quat.x, quat.y, quat.z, quat.w };
	}
	else
	{
		interpolatedRotation = XMLoadFloat4(&nextPoint.rotation);
	}

	// ���� ��Ʈ���� ����
	_matrix NewWorldMatrix = CreateWorldMatrix(interpolatedPosition, interpolatedRotation);

	// ���� ��Ʈ�������� Right, Up, Look ���� ����
	_vector right = NewWorldMatrix.r[0];
	_vector up = NewWorldMatrix.r[1];
	_vector look = NewWorldMatrix.r[2];
	_vector position = NewWorldMatrix.r[3];

	// CTransform ������Ʈ�� ����
	CTransform* cameraTransform = static_cast<CTransform*>(camera->Get_Component(TEXT("Com_Transform")));

	// ���� ���� ����
	cameraTransform->Set_State(CTransform::STATE_RIGHT, right);
	cameraTransform->Set_State(CTransform::STATE_UP, up);
	cameraTransform->Set_State(CTransform::STATE_LOOK, look);

	// ��ġ ����
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

	//�ش� ���� ������� ����
	cameraPoint.pWorldFloat4x4 = pModelFloat4x4;

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

	// ī�޶��� ���� ����(Right, Up, Look) ��ֶ�����
	_vector right = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
	_vector up = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_UP));
	_vector look = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

	_float3 right_f3, up_f3, look_f3;
	XMStoreFloat3(&right_f3, right);
	XMStoreFloat3(&up_f3, up);
	XMStoreFloat3(&look_f3, look);

	// ������ �� ȸ�� ��� ����
	_matrix rotationMatrix = CreateRotationMatrix(right_f3, up_f3, look_f3);

	//ȸ������� ���ʹϾ����� ��ȯ
	_vector quaternion = XMQuaternionRotationMatrix(rotationMatrix);
	_float4 rotation = {};
	XMStoreFloat4(&rotation, quaternion);

	//���ʹϾ� ����
	cameraPoint.rotation = rotation;
	cameraPoint.duration = duration;
	cameraPoint.interpolationType = type;

	m_listPoints.push_back(cameraPoint);
}


void CCamera::Free()
{
	__super::Free();
}
