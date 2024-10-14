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
	cameraPoint.rotation = rotation;

	cameraPoint.duration = duration;
	cameraPoint.interpolationType = type;

	m_listPoints.push_back(cameraPoint);
}

void CCamera::Remove_Point(_int currentIndex)
{
	if (currentIndex < 0 || currentIndex >= static_cast<int>(m_listPoints.size())) {
		return; // ��ȿ���� ���� �ε����� ��� �ƹ� �۾��� ���� ����
	}

	auto it = m_listPoints.begin();
	advance(it, currentIndex);
	m_listPoints.erase(it);
}

//list�� index ������ ã�ư��� ���� Position�� Rotation���� ī�޶��� Ʈ�������� �����ϸ� �ǰڴ�
void CCamera::Move_Point(_int index)
{
	if (index < 0 || index >= m_listPoints.size()) {
		return;
	}

	auto it = m_listPoints.begin();
	std::advance(it, index);  // index ��ġ�� �ݺ��� �̵�

	const CameraPoint& targetPoint = *it;

}


void CCamera::Free()
{
	__super::Free();
}
