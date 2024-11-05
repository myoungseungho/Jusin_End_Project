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

	//아이들
	m_pModelCom->SetUp_Animation(1, true, 0.1f);

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
	bool bCurrentKeyUp = m_pGameInstance->Key_Pressing(DIK_UP);
	bool bCurrentKeyDown = m_pGameInstance->Key_Pressing(DIK_DOWN);
	bool bCurrentKeyLeft = m_pGameInstance->Key_Pressing(DIK_LEFT);
	bool bCurrentKeyRight = m_pGameInstance->Key_Pressing(DIK_RIGHT);

	if (bCurrentKeyUp)
	{
		vTargetDir.z += 1.f;
		bInput = true;
	}
	if (bCurrentKeyDown)
	{
		vTargetDir.z -= 1.f;
		bInput = true;
	}
	if (bCurrentKeyLeft)
	{
		vTargetDir.x -= 1.f;
		bInput = true;
	}
	if (bCurrentKeyRight)
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

		// 대쉬 애니메이션 트리거 (키가 처음 눌렸을 때만)
		if ((bCurrentKeyUp && !m_bPrevKeyUp) ||
			(bCurrentKeyDown && !m_bPrevKeyDown) ||
			(bCurrentKeyLeft && !m_bPrevKeyLeft) ||
			(bCurrentKeyRight && !m_bPrevKeyRight))
		{
			m_pModelCom->SetUp_Animation(0, true, 0.1f);
		}
	}
	else
	{
		//애니메이션 설정: 입력이 없을 때 기본 애니메이션 재생
		m_pModelCom->SetUp_Animation(1, true, 0.1f);
	}

	// 현재 프레임의 애니메이션 재생
	m_pModelCom->Play_Animation(fTimeDelta * 3.f);

	// 이전 키 상태 업데이트
	m_bPrevKeyUp = bCurrentKeyUp;
	m_bPrevKeyDown = bCurrentKeyDown;
	m_bPrevKeyLeft = bCurrentKeyLeft;
	m_bPrevKeyRight = bCurrentKeyRight;
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

		//m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i);

		/* 모델이 가지고 있는 뼈들 중에서 현재 렌더링할려고 했던 i번째ㅑ 메시가 사용하는 뼈들을 배열로 만들어서 쉐이더로 던져준다.  */
		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;


		if (FAILED(m_pShaderCom->Begin(3)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLobby_Goku::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
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


XMVECTOR CalculateQuaternionBetweenVectors(XMVECTOR vFrom, XMVECTOR vTo)
{
	// 정규화된 벡터로 가정
	XMVECTOR vFromNorm = XMVector3Normalize(vFrom);
	XMVECTOR vToNorm = XMVector3Normalize(vTo);

	// 두 벡터의 내적 계산
	float dot = XMVectorGetX(XMVector3Dot(vFromNorm, vToNorm));

	// 만약 두 벡터가 거의 반대 방향이라면, 특별히 회전 축을 정해야 합니다.
	if (dot < -0.999999f)
	{
		// 임의의 회전 축을 선택 (예: X축)
		XMVECTOR arbitrary = XMVectorSet(1.f, 0.f, 0.f, 0.f);
		// 벡터가 X축과 평행하면 Y축을 사용
		if (fabsf(XMVectorGetX(XMVector3Dot(vFromNorm, arbitrary))) > 0.999999f)
		{
			arbitrary = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		}
		// 회전 축 계산
		XMVECTOR rotationAxis = XMVector3Normalize(XMVector3Cross(vFromNorm, arbitrary));
		// 180도 회전을 나타내는 쿼터니언
		return XMQuaternionRotationAxis(rotationAxis, XM_PI);
	}
	else if (dot > 0.999999f)
	{
		// 두 벡터가 거의 동일한 경우, 회전이 필요 없음
		return XMQuaternionIdentity();
	}
	else
	{
		// 일반적인 경우, 회전 축과 각도를 계산
		XMVECTOR rotationAxis = XMVector3Normalize(XMVector3Cross(vFromNorm, vToNorm));
		float rotationAngle = acosf(dot);
		return XMQuaternionRotationAxis(rotationAxis, rotationAngle);
	}
}


void CLobby_Goku::RotateTowardsTarget(const _float3& vTargetDir, _float fTimeDelta)
{
	// 회전 속도 (라디안/초)
	const _float ROTATION_SPEED = XM_PI * 5.f; // 180도/초

	// 목표 방향 벡터를 정규화
	_vector vTargetLook = XMVector3Normalize(XMLoadFloat3(&vTargetDir));

	// 현재 Look 벡터를 가져와서 정규화
	_vector vCurrentLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

	// 두 벡터 사이의 회전 쿼터니언 계산
	_vector qRotation = CalculateQuaternionBetweenVectors(vCurrentLook, vTargetLook);

	// 회전 속도에 따른 보간 인자 계산
	_float fRotationAmount = ROTATION_SPEED * fTimeDelta;

	// 회전 보간 (최대 1.0을 넘지 않도록)
	fRotationAmount = min(fRotationAmount, 1.0f);

	// 현재 회전 쿼터니언 가져오기
	_vector qCurrentRotation = XMQuaternionRotationMatrix(m_pTransformCom->Get_WorldMatrix());

	// 보간된 회전 쿼터니언 계산
	_vector qNewRotation = XMQuaternionSlerp(qCurrentRotation, XMQuaternionMultiply(qRotation, qCurrentRotation), fRotationAmount);

	// 회전 행렬로 변환
	_matrix mRotationMatrix = XMMatrixRotationQuaternion(qNewRotation);

	// 월드 행렬의 위치 부분 유지
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float4x4 matrix{};
	XMStoreFloat4x4(&matrix, mRotationMatrix);
	// 새로운 월드 행렬 설정
	m_pTransformCom->Set_WorldMatrix(matrix);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
}

void CLobby_Goku::MoveForward(_float fTimeDelta)
{
	// 상수 정의
	const _float MOVE_SPEED = 15.f; // 이동 속도 (유닛/초)

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
