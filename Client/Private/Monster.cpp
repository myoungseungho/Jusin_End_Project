#include "stdafx.h"
#include "..\Public\Monster.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "Imgui_Manager.h"
#include "Main_Camera.h"
#include "Player.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CMonster::CMonster(const CMonster& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->SetUp_Animation(1, false, 0.1f);

	CMain_Camera* mainCamera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")));
	mainCamera->SetPlayer(CMain_Camera::PLAYER_2P, this);

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{

}

void CMonster::Update(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta);

	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	static _bool m_isJump = false;
	if (m_pGameInstance->Key_Down(DIK_F8))
	{
		m_isJump = !m_isJump;

		if (m_isJump)
		{
			// 현재 콜라이더의 원래 위치와 크기를 가져오기
			BoundingBox boundingBox = m_pColliderCom->AABB_GetDesc();

			// 현재 위치와 스케일 값을 업데이트된 값으로 설정
			boundingBox.Center = _float3(0.f, 0.6f, 0.f);
			boundingBox.Extents = _float3(1.5f, 0.5f, 1.5f);
			m_pColliderCom->AABB_SetDesc(boundingBox);
		}
		else
		{
			// 현재 콜라이더의 원래 위치와 크기를 가져오기
			BoundingBox boundingBox = m_pColliderCom->AABB_GetDesc();

			// 현재 위치와 스케일 값을 업데이트된 값으로 설정
			boundingBox.Center = _float3(0.f, 0.f, 0.f);
			boundingBox.Extents = _float3(1.5f, 1.5f, 1.5f);
			m_pColliderCom->AABB_SetDesc(boundingBox);
		}
	}

	//// 현재 애니메이션 인덱스와 이전 키 상태를 static 변수로 선언
	//static int currentAnimationIndex = 0; // 현재 애니메이션 인덱스 (0~88)
	//static bool previousUpKeyState = false;   // 이전 프레임의 위 방향키 상태
	//static bool previousDownKeyState = false; // 이전 프레임의 아래 방향키 상태
	//const int MAX_ANIMATION_INDEX = 88; // 최대 애니메이션 인덱스

	//// 위 방향키(VK_UP)와 아래 방향키(VK_DOWN) 상태 가져오기
	//SHORT upKeyState = GetAsyncKeyState(VK_UP);
	//SHORT downKeyState = GetAsyncKeyState(VK_DOWN);

	//bool isUpKeyPressed = (upKeyState & 0x8000) != 0;
	//bool isDownKeyPressed = (downKeyState & 0x8000) != 0;

	//// 위 방향키가 새로 눌렸을 때 인덱스 증가
	//if (isUpKeyPressed && !previousUpKeyState)
	//{
	//	currentAnimationIndex++;
	//	if (currentAnimationIndex > MAX_ANIMATION_INDEX)
	//		currentAnimationIndex = 0; // 88에서 증가 시 0으로 순환

	//	m_pModelCom->SetUp_Animation(currentAnimationIndex, true, 0.1f);
	//}

	//// 아래 방향키가 새로 눌렸을 때 인덱스 감소
	//if (isDownKeyPressed && !previousDownKeyState)
	//{
	//	currentAnimationIndex--;
	//	if (currentAnimationIndex < 0)
	//		currentAnimationIndex = MAX_ANIMATION_INDEX; // 0에서 감소 시 88로 순환

	//	m_pModelCom->SetUp_Animation(currentAnimationIndex, true, 0.1f);
	//}

	//// 현재 키 상태를 이전 상태로 저장
	//previousUpKeyState = isUpKeyPressed;
	//previousDownKeyState = isDownKeyPressed;

	if (m_pPlayer == nullptr)
	{
		CPlayer* mainPlayer = static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player")));
		m_pPlayer = mainPlayer;
		return;
	}

	//플레이어는 자기 자신과 몬스터와의 거리를 알아야 한다.
	CTransform* player_Transform = static_cast<CTransform*>(m_pPlayer->Get_Component(TEXT("Com_Transform")));
	_vector playerState = player_Transform->Get_State(CTransform::STATE_POSITION);
	_vector monsterState = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float distance = ComputeDistance(playerState, monsterState);

	_float moveSpeed = 1.f;

	if (distance >= 70)
	{
		_float monsterX = XMVectorGetX(monsterState);
		_float playerX = XMVectorGetX(playerState);

		//플레이어가 몬스터보다 왼쪽이면 더 왼쪽 못감
		if (playerX < monsterX)
		{
			if (m_pGameInstance->Key_Pressing(DIK_UP))
			{
				m_pTransformCom->Move_Position(_float3(0.f, moveSpeed, 0.f));
			}

			if (m_pGameInstance->Key_Pressing(DIK_DOWN))
			{
				m_pTransformCom->Move_Position(_float3(0.f, -moveSpeed, 0.f));
			}

			if (m_pGameInstance->Key_Pressing(DIK_LEFT))
			{
				m_pTransformCom->Move_Position(_float3(-moveSpeed, 0.f, 0.f));
			}
		}
		else if (playerX > monsterX)
		{
			if (m_pGameInstance->Key_Pressing(DIK_UP))
			{
				m_pTransformCom->Move_Position(_float3(0.f, moveSpeed, 0.f));
			}

			if (m_pGameInstance->Key_Pressing(DIK_DOWN))
			{
				m_pTransformCom->Move_Position(_float3(0.f, -moveSpeed, 0.f));
			}

			if (m_pGameInstance->Key_Pressing(DIK_RIGHT))
			{
				m_pTransformCom->Move_Position(_float3(moveSpeed, 0.f, 0.f));
			}
		}
	}
	else
	{
		if (m_pGameInstance->Key_Pressing(DIK_UP))
		{
			m_pTransformCom->Move_Position(_float3(0.f, moveSpeed, 0.f));
		}

		if (m_pGameInstance->Key_Pressing(DIK_DOWN))
		{
			m_pTransformCom->Move_Position(_float3(0.f, -moveSpeed, 0.f));
		}

		if (m_pGameInstance->Key_Pressing(DIK_LEFT))
		{
			m_pTransformCom->Move_Position(_float3(-moveSpeed, 0.f, 0.f));
		}

		if (m_pGameInstance->Key_Pressing(DIK_RIGHT))
		{
			m_pTransformCom->Move_Position(_float3(moveSpeed, 0.f, 0.f));
		}
	}


}

void CMonster::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

#ifdef _DEBUG
	m_pRenderInstance->Add_DebugComponent(m_pColliderCom);
#endif
}

HRESULT CMonster::Render(_float fTimeDelta)
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

		/* 모델이 가지고 있는 뼈들 중에서 현재 렌더링할려고 했던 i번째ㅑ 메시가 사용하는 뼈들을 배열로 만들어서 쉐이더로 던져준다.  */
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

void CMonster::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	int a = 3;
}

void CMonster::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
	int a = 3;
}

void CMonster::OnCollisionExit(CCollider* other)
{
	int a = 3;
}

HRESULT CMonster::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC	BoundingDesc{};

	BoundingDesc.vExtents = _float3(1.5f, 1.5f, 1.5f);
	BoundingDesc.vCenter = _float3(0.f, 0.f, 0.f);
	BoundingDesc.pMineGameObject = this;
	BoundingDesc.colliderGroup = CCollider_Manager::CG_2P_BODY;
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc)))
		return E_FAIL;

	m_pGameInstance->Add_ColliderObject(BoundingDesc.colliderGroup, m_pColliderCom);

	return S_OK;
}

HRESULT CMonster::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CMonster* CMonster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster* pInstance = new CMonster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMonster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster::Clone(void* pArg)
{
	CMonster* pInstance = new CMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMonster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pColliderCom);
}
