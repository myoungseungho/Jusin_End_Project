#include "stdafx.h"
#include "..\Public\Player.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "Imgui_Manager.h"
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

	m_pModelCom->SetUp_Animation(16, true);

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
			// 현재 콜라이더의 원래 위치와 크기를 가져오기
			BoundingBox boundingBox = m_pColliderCom->AABB_GetDesc();

			// 현재 위치와 스케일 값을 업데이트된 값으로 설정
			boundingBox.Center = _float3(0.f,0.6f,0.f);
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
