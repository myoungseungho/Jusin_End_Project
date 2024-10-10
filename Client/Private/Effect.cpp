#include "stdafx.h"
#include "..\Public\Effect.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "Imgui_Manager.h"
#include "Collider_Manager.h"
CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CEffect::CEffect(const CEffect& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CEffect::Priority_Update(_float fTimeDelta)
{

}

void CEffect::Update(_float fTimeDelta)
{
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_fY -= 0.1;

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_fY += 0.1;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_fX -= 0.1;

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_fX += 0.1;

	_float2 DefaultFloat2 = _float2(-5.f, 5.f);
	Make_Ray(CCollider_Manager::CG_1P_SKILL, DefaultFloat2, _float2(DefaultFloat2.x + m_fX, DefaultFloat2.y + m_fY));
}

void CEffect::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	m_pRenderInstance->Add_DebugComponent(m_pColliderCom);
}

HRESULT CEffect::Render(_float fTimeDelta)
{
	return S_OK;
}

void CEffect::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	int a = 3;
}

void CEffect::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
	int a = 3;
}

void CEffect::OnCollisionExit(CCollider* other)
{
	int a = 3;
}

void CEffect::Make_Ray(CCollider_Manager::COLLIDERGROUP eColliderGroup, _float2 SourcePos, _float2 DestPos)
{
	// 1. 시작점과 끝점 사이의 거리 및 방향 계산
	_float dx = DestPos.x - SourcePos.x;
	_float dy = DestPos.y - SourcePos.y;

	// 두 점 사이의 거리 계산
	_float distance = sqrtf(dx * dx + dy * dy);

	// 방향 벡터 및 정규화
	_float2 direction = { dx / distance, dy / distance };
}

HRESULT CEffect::Ready_Components()
{
	CBounding_AABB::BOUNDING_AABB_DESC	BoundingDesc{};

	BoundingDesc.vExtents = _float3(1.5f, 1.5f, 1.5f);
	BoundingDesc.vCenter = _float3(0.f, 0.f, 0.f);
	BoundingDesc.pMineGameObject = this;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc)))
		return E_FAIL;

	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_1P_SKILL, m_pColliderCom);

	return S_OK;
}

CEffect* CEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect* pInstance = new CEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect::Clone(void* pArg)
{
	CEffect* pInstance = new CEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect::Free()
{
	Safe_Release(m_ColliderCom);

	__super::Free();

}
