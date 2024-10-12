#include "..\Public\Camera.h"
#include "GameInstance.h"
#include "Virtual_Camera.h"
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
	D3D11_VIEWPORT			ViewPortDesc{};

	_uint		iNumViewports = { 1 };

	//m_pContext->RSGetViewports(&iNumViewports, &ViewPortDesc);

	m_fViewportWidth = 1280;
	m_fViewportHeight = 720;

	return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

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

void CCamera::CameraData_Update(CVirtual_Camera* virtual_Camera)
{
	CTransform* transform = static_cast<CTransform*>(virtual_Camera->Get_Component(TEXT("Com_Transform")));
	_matrix matrix = transform->Get_WorldMatrix();

	CVirtual_Camera::VIRTUAL_CAMERA_DESC Desc = virtual_Camera->m_Desc;
	_vector inverse = {};
	_matrix WorldMatrix_Inverse = XMMatrixInverse(&inverse, matrix);

	//파이프라인에 가상카메라의 월드의 역행렬 넣어주기
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_VIEW, WorldMatrix_Inverse);
	//선택된 가상카메라의 투영 행렬 넣기 위한 정보 넣어주기
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(Desc.fFovy, Desc.fViewportWidth / Desc.fViewportHeight, Desc.fNear, Desc.fFar));
}

void CCamera::Free()
{
	__super::Free();
}
