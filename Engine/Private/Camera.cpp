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
	D3D11_VIEWPORT			ViewPortDesc{};

	_uint		iNumViewports = { 1 };

	//m_pContext->RSGetViewports(&iNumViewports, &ViewPortDesc);

	m_fViewportWidth = 1280;
	m_fViewportHeight = 720;

	return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{
	CAMERA_DESC* pDesc = static_cast<CAMERA_DESC*>(pArg);

	m_vEye = pDesc->vEye;
	m_vAt = pDesc->vAt;

	m_fFovy = pDesc->fFovy;
	m_fNear = pDesc->fNear;
	m_fFar = pDesc->fFar;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&m_vEye), 1.f));
	m_pTransformCom->LookAt(XMVectorSetW(XMLoadFloat3(&m_vAt), 1.f));

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

void CCamera::CameraData_Update(CameraData& data)
{
	//���� ī�޶󿡼� ������� �ޱ�
	_float4x4 worldMatirx = data.WorldMatrix_Float4x4;
	_matrix matrix = Float4x4ToMatrix(worldMatirx);
	//����� �����
	_vector inverse = {};
	_matrix WorldMatrix_Inverse = XMMatrixInverse(&inverse, matrix);

	//���������ο� ����ī�޶��� ������ ����� �־��ֱ�
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_VIEW, WorldMatrix_Inverse);
	//���õ� ����ī�޶��� ���� ��� �ֱ� ���� ���� �־��ֱ�
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(data.fFovy, data.fViewportWidth / data.fViewportHeight, data.fNear, data.fFar));
}

void CCamera::Free()
{
	__super::Free();
}
