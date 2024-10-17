#include "stdafx.h"
#include "..\Public\Player.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "Imgui_Manager.h"
#include "Main_Camera.h"
#include "Monster.h"
CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->SetUp_Animation(0, false, 0.1f);

	CMain_Camera* mainCamera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")));
	mainCamera->SetPlayer(CMain_Camera::PLAYER_1P, this);

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{

}

void CPlayer::Update(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta);

	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	static _bool m_isJump = false;
	if (m_pGameInstance->Key_Down(DIK_F8))
	{
		m_isJump = !m_isJump;

		if (m_isJump)
		{
			// ���� �ݶ��̴��� ���� ��ġ�� ũ�⸦ ��������
			BoundingBox boundingBox = m_pColliderCom->AABB_GetDesc();

			// ���� ��ġ�� ������ ���� ������Ʈ�� ������ ����
			boundingBox.Center = _float3(0.f, 0.6f, 0.f);
			boundingBox.Extents = _float3(1.5f, 0.5f, 1.5f);
			m_pColliderCom->AABB_SetDesc(boundingBox);
		}
		else
		{
			// ���� �ݶ��̴��� ���� ��ġ�� ũ�⸦ ��������
			BoundingBox boundingBox = m_pColliderCom->AABB_GetDesc();

			// ���� ��ġ�� ������ ���� ������Ʈ�� ������ ����
			boundingBox.Center = _float3(0.f, 0.f, 0.f);
			boundingBox.Extents = _float3(1.5f, 1.5f, 1.5f);
			m_pColliderCom->AABB_SetDesc(boundingBox);
		}
	}

	//// ���� �ִϸ��̼� �ε����� ���� Ű ���¸� static ������ ����
	//static int currentAnimationIndex = 0; // ���� �ִϸ��̼� �ε��� (0~88)
	//static bool previousUpKeyState = false;   // ���� �������� �� ����Ű ����
	//static bool previousDownKeyState = false; // ���� �������� �Ʒ� ����Ű ����
	//const int MAX_ANIMATION_INDEX = 88; // �ִ� �ִϸ��̼� �ε���

	//// �� ����Ű(VK_UP)�� �Ʒ� ����Ű(VK_DOWN) ���� ��������
	//SHORT upKeyState = GetAsyncKeyState(VK_UP);
	//SHORT downKeyState = GetAsyncKeyState(VK_DOWN);

	//bool isUpKeyPressed = (upKeyState & 0x8000) != 0;
	//bool isDownKeyPressed = (downKeyState & 0x8000) != 0;

	//// �� ����Ű�� ���� ������ �� �ε��� ����
	//if (isUpKeyPressed && !previousUpKeyState)
	//{
	//	currentAnimationIndex++;
	//	if (currentAnimationIndex > MAX_ANIMATION_INDEX)
	//		currentAnimationIndex = 0; // 88���� ���� �� 0���� ��ȯ

	//	m_pModelCom->SetUp_Animation(currentAnimationIndex, true, 0.1f);
	//}

	//// �Ʒ� ����Ű�� ���� ������ �� �ε��� ����
	//if (isDownKeyPressed && !previousDownKeyState)
	//{
	//	currentAnimationIndex--;
	//	if (currentAnimationIndex < 0)
	//		currentAnimationIndex = MAX_ANIMATION_INDEX; // 0���� ���� �� 88�� ��ȯ

	//	m_pModelCom->SetUp_Animation(currentAnimationIndex, true, 0.1f);
	//}

	//// ���� Ű ���¸� ���� ���·� ����
	//previousUpKeyState = isUpKeyPressed;
	//previousDownKeyState = isDownKeyPressed;

	/*if (m_pMonster == nullptr)
	{
		CMonster* mainMonster = static_cast<CMonster*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster")));
		m_pMonster = mainMonster;
		return;
	}*/

	////�÷��̾�� �ڱ� �ڽŰ� ���Ϳ��� �Ÿ��� �˾ƾ� �Ѵ�.
	//CTransform* monster_Transform = static_cast<CTransform*>(m_pMonster->Get_Component(TEXT("Com_Transform")));
	//_vector monsterState = monster_Transform->Get_State(CTransform::STATE_POSITION);
	//_vector playerState = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//_float distance = ComputeDistance(playerState, monsterState);

	//_float moveSpeed = 1.f;

	//if (distance >= 80)
	//{
	//	_float monsterX = XMVectorGetX(monsterState);
	//	_float playerX = XMVectorGetX(playerState);

	//	//�÷��̾ ���ͺ��� �����̸� �� ���� ����
	//	if (playerX < monsterX)
	//	{
	//		if (m_pGameInstance->Key_Pressing(DIK_Y))
	//		{
	//			m_pTransformCom->Move_Position(_float3(0.f, moveSpeed, 0.f));
	//		}

	//		if (m_pGameInstance->Key_Pressing(DIK_H))
	//		{
	//			m_pTransformCom->Move_Position(_float3(0.f, -moveSpeed, 0.f));
	//		}

	//		if (m_pGameInstance->Key_Pressing(DIK_J))
	//		{
	//			m_pTransformCom->Move_Position(_float3(moveSpeed, 0.f, 0.f));
	//		}
	//	}
	//	else if (playerX > monsterX)
	//	{
	//		if (m_pGameInstance->Key_Pressing(DIK_Y))
	//		{
	//			m_pTransformCom->Move_Position(_float3(0.f, moveSpeed, 0.f));
	//		}

	//		if (m_pGameInstance->Key_Pressing(DIK_H))
	//		{
	//			m_pTransformCom->Move_Position(_float3(0.f, -moveSpeed, 0.f));
	//		}

	//		if (m_pGameInstance->Key_Pressing(DIK_G))
	//		{
	//			m_pTransformCom->Move_Position(_float3(-moveSpeed, 0.f, 0.f));
	//		}
	//	}
	//}
	//else
	//{
	//	if (m_pGameInstance->Key_Pressing(DIK_Y))
	//	{
	//		m_pTransformCom->Move_Position(_float3(0.f, moveSpeed, 0.f));
	//	}

	//	if (m_pGameInstance->Key_Pressing(DIK_H))
	//	{
	//		m_pTransformCom->Move_Position(_float3(0.f, -moveSpeed, 0.f));
	//	}

	//	if (m_pGameInstance->Key_Pressing(DIK_G))
	//	{
	//		m_pTransformCom->Move_Position(_float3(-moveSpeed, 0.f, 0.f));
	//	}

	//	if (m_pGameInstance->Key_Pressing(DIK_J))
	//	{
	//		m_pTransformCom->Move_Position(_float3(moveSpeed, 0.f, 0.f));
	//	}
	//}


	if (m_pGameInstance->Key_Down(DIK_1))
	{
		m_pModelCom->SetUp_Animation(1, true, 0.1f);
	}
	if (m_pGameInstance->Key_Down(DIK_2))
	{
		m_pModelCom->SetUp_Animation(2, true, 0.1f);
	}
	if (m_pGameInstance->Key_Down(DIK_3))
	{
		m_pModelCom->SetUp_Animation(4, true, 0.1f);
	}
	if (m_pGameInstance->Key_Down(DIK_4))
	{
		m_pModelCom->SetUp_Animation(3, true, 0.1f);
	}
	if (m_pGameInstance->Key_Down(DIK_5))
	{
		m_pModelCom->SetUp_Animation(0, true, 0.1f);
	}
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

#ifdef _DEBUG
	m_pRenderInstance->Add_DebugComponent(m_pColliderCom);
#endif
}

HRESULT CPlayer::Render(_float fTimeDelta)
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		/* ���� ������ �ִ� ���׸��� �� i��° �޽ð� ����ؾ��ϴ� ���׸�����ü�� aiTextureType_DIFFUSE��° �ؽ��ĸ� */
		/* m_pShaderCom�� �ִ� g_DiffuseTexture������ ����. */
		if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;
		// m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i);

		/* ���� ������ �ִ� ���� �߿��� ���� �������ҷ��� �ߴ� i��°�� �޽ð� ����ϴ� ������ �迭�� ���� ���̴��� �����ش�.  */
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

void CPlayer::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	int a = 3;
}

void CPlayer::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
	int a = 3;
}

void CPlayer::OnCollisionExit(CCollider* other)
{
	int a = 3;
}

HRESULT CPlayer::Ready_Components()
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
	BoundingDesc.colliderGroup = CCollider_Manager::CG_1P_BODY;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc)))
		return E_FAIL;

	m_pGameInstance->Add_ColliderObject(BoundingDesc.colliderGroup, m_pColliderCom);

	return S_OK;
}

HRESULT CPlayer::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pColliderCom);
}
