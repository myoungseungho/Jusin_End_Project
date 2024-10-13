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

void CCamera::Update_Camera(CCamera* camera)
{
	//여기서는 가상카메라의 셋팅이 들어가야 한다.
	_matrix Inverse_Matrix = static_cast<CTransform*>(camera->Get_Component(TEXT("Com_Transform")))->Get_WorldMatrix_Inverse();
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_VIEW, Inverse_Matrix);

	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(camera->m_fFovy, camera->m_fViewportWidth / camera->m_fViewportHeight, camera->m_fNear, camera->m_fFar));
}

void CCamera::Add_Point(_float duration, InterpolationType type)
{
	CameraPoint cameraPoint{};
	//현재 가상카메라 포지션을 cameraPoint.Position을 넣기
	XMStoreFloat3(&cameraPoint.position, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	//가상카메라의 방향벡터를 노말라이즈해서 각각 넣어주기
	XMStoreFloat3(&cameraPoint.rotationX, XMVector4Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT)));
	XMStoreFloat3(&cameraPoint.rotationY, XMVector4Normalize(m_pTransformCom->Get_State(CTransform::STATE_UP)));
	XMStoreFloat3(&cameraPoint.rotationZ, XMVector4Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK)));

	cameraPoint.duration = duration;
	cameraPoint.interpolationType = type;

	m_listPoints.push_back(cameraPoint);
}

void CCamera::Free()
{
	__super::Free();
}
