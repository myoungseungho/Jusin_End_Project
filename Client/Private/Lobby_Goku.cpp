#include "stdafx.h"
#include "..\Public\Lobby_Goku.h"

#include "RenderInstance.h"
#include "GameInstance.h"

CLobby_Goku::CLobby_Goku(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CLobby_Goku::CLobby_Goku(const CLobby_Goku& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CLobby_Goku::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLobby_Goku::Initialize(void* pArg)
{
	CTransform::TRANSFORM_DESC Desc{};
	Desc.fRotationPerSec = 1.f;
	Desc.fSpeedPerSec = 1.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State_Position(_float3(0.f, 0.f, -37.1f));

	return S_OK;
}

void CLobby_Goku::Camera_Update(_float fTimeDelta)
{

}

void CLobby_Goku::Update(_float fTimeDelta)
{
	// 입력 처리
	_float3 vTargetDir = { 0.f, 0.f, 0.f }; // 목표 방향
	_bool bInput = false;

	// 여러 방향키 입력을 동시에 처리
	if (m_pGameInstance->Key_Pressing(DIK_UP))
	{
		vTargetDir.z += 1.f;
		bInput = true;
	}
	if (m_pGameInstance->Key_Pressing(DIK_DOWN))
	{
		vTargetDir.z -= 1.f;
		bInput = true;
	}
	if (m_pGameInstance->Key_Pressing(DIK_LEFT))
	{
		vTargetDir.x -= 1.f;
		bInput = true;
	}
	if (m_pGameInstance->Key_Pressing(DIK_RIGHT))
	{
		vTargetDir.x += 1.f;
		bInput = true;
	}

	if (bInput)
	{
		// 회전 처리
		RotateTowardsTarget(vTargetDir, fTimeDelta);

		// 이동 처리
		MoveForward(fTimeDelta);
	}
}

void CLobby_Goku::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CLobby_Goku::Render(_float fTimeDelta)
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		/* 모델이 가지고 있는 머테리얼 중 i번째 메시가 사용해야하는 머테리얼구조체의 aiTextureType_DIFFUSE번째 텍스쳐를 */
		/* m_pShaderCom에 있는 g_DiffuseTexture변수에 던져. */
		if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;
		// m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i);

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLobby_Goku::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Model_Lobby_Goku"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLobby_Goku::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CLobby_Goku::RotateTowardsTarget(const _float3& vTargetDir, _float fTimeDelta)
{
	// 상수 정의
	const _float ANGLE_THRESHOLD = 0.1f; // 회전 각도 임계값 (도 단위)
	const _float ROTATION_SPEED = 5.f;    // 회전 속도 (도/초)

	// 목표 방향 벡터를 정규화
	_vector vTargetLook = XMVector3Normalize(XMLoadFloat3(&vTargetDir));

	// 현재 Look 벡터를 가져와서 정규화
	_vector vCurrentLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

	// 현재 Look 벡터와 목표 Look 벡터 사이의 각도 계산 (도 단위)
	_float fAngle = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenNormals(vCurrentLook, vTargetLook)));

	// 회전 축 (Y축 기준)
	_vector vAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	// 현재 Look 벡터와 목표 Look 벡터의 외적을 계산하여 회전 방향 결정
	_float fDotUp = XMVectorGetY(XMVector3Cross(vCurrentLook, vTargetLook));

	// 회전 수행
	if (fAngle > ANGLE_THRESHOLD)
	{
		// 회전 방향 결정: 외적의 Y 성분에 따라 시계 방향 또는 반시계 방향으로 회전
		_vector vRotationAxis = (fDotUp > 0) ? vAxis : XMVectorScale(vAxis, -1.f);

		// 회전 적용 (fTimeDelta * ROTATION_SPEED 만큼 회전)
		m_pTransformCom->Turn(vRotationAxis, fTimeDelta * ROTATION_SPEED);
	}
	else
	{
		// 각도가 임계값 이하이면 정확히 목표 방향으로 설정
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, vTargetLook);

		// 오른쪽 벡터 재계산 및 정규화
		_vector vRight = XMVector3Normalize(XMVector3Cross(m_pTransformCom->Get_State(CTransform::STATE_UP), vTargetLook));
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	}
}

void CLobby_Goku::MoveForward(_float fTimeDelta)
{
	// 상수 정의
	const _float MOVE_SPEED = 5.f; // 이동 속도 (유닛/초)

	// 현재 Look 벡터를 가져와서 정규화
	_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

	// 이동 거리 계산 (속도 * 시간)
	_float3 vMoveDistance;
	XMStoreFloat3(&vMoveDistance, vLook * fTimeDelta * MOVE_SPEED);

	// 현재 위치 가져오기
	_float3 vCurrentPos{};
	XMStoreFloat3(&vCurrentPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	// 새로운 위치 계산
	_float3 vNewPos = _float3(vCurrentPos.x + vMoveDistance.x, vCurrentPos.y + vMoveDistance.y, vCurrentPos.z + vMoveDistance.z);

	// 새로운 위치 설정
	m_pTransformCom->Set_State_Position(vNewPos);
}

CLobby_Goku* CLobby_Goku::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLobby_Goku* pInstance = new CLobby_Goku(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CLobby_Goku"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLobby_Goku::Clone(void* pArg)
{
	CLobby_Goku* pInstance = new CLobby_Goku(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CLobby_Goku"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLobby_Goku::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

	__super::Free();
}
