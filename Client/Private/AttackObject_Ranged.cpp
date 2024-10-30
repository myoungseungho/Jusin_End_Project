#include "stdafx.h"
#include "..\Public\AttackObject_Ranged.h"

#include "RenderInstance.h"
#include "GameInstance.h"

#include "Character.h"

CAttackObject_Ranged::CAttackObject_Ranged(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CAttackObject{ pDevice, pContext }
{

}

CAttackObject_Ranged::CAttackObject_Ranged(const CAttackObject_Ranged& Prototype)
	: CAttackObject{ Prototype }
{

}

HRESULT CAttackObject_Ranged::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAttackObject_Ranged::Initialize(void* pArg)
{

	if (nullptr == pArg)
		return E_FAIL;


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;







	return S_OK;
}

void CAttackObject_Ranged::Priority_Update(_float fTimeDelta)
{

}

void CAttackObject_Ranged::Update(_float fTimeDelta)
{


	m_fAccLifeTime += fTimeDelta;

	if (m_fAccLifeTime > m_fLifeTime)
	{
		if (m_bEnableDestory)
		{
			Destory();
			m_pGameInstance->Release_Collider(m_pColliderCom);
			m_bEnableDestory = false;
		}
	}
	else
		m_pColliderCom->UpdateVector(m_pOwnerTransform->Get_State(CTransform::STATE_POSITION));


}

void CAttackObject_Ranged::Late_Update(_float fTimeDelta)
{

	


	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CAttackObject_Ranged::Render(_float fTimeDelta)
{

#ifdef _DEBUG
	m_pColliderCom->Render(fTimeDelta);
#endif // DEBUG


	return S_OK;
}



void CAttackObject_Ranged::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{


}

void CAttackObject_Ranged::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
	_bool Debug = true;

}

void CAttackObject_Ranged::OnCollisionExit(CCollider* other)
{
	_bool Debug = true;

}

void CAttackObject_Ranged::CollisingAttack()
{
}

void CAttackObject_Ranged::CollisingPlayer()
{
}







CAttackObject_Ranged* CAttackObject_Ranged::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAttackObject_Ranged* pInstance = new CAttackObject_Ranged(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CAttackObject_Ranged"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAttackObject_Ranged::Clone(void* pArg)
{
	CAttackObject_Ranged* pInstance = new CAttackObject_Ranged(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CAttackObject_Ranged"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAttackObject_Ranged::Free()
{
	__super::Free();
	Safe_Release(m_pColliderCom);

}
