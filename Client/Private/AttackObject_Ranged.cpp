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


	ATTACK_RANGED_DESC* pDesc = static_cast<ATTACK_RANGED_DESC*>(pArg);

	m_fStartOffset = pDesc->fStartOffset;
	m_fRanged_Impus_NoneDirection = pDesc->fRanged_Impus_NoneDirection;
	m_iDirection = pDesc->iDirection;



	_vector vPos = m_pOwner->Get_vPosition();
	_vector vStartOffset = { m_fStartOffset.x, m_fStartOffset.y, 0.f, 0.f };
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos + vStartOffset);


	return S_OK;
}



void CAttackObject_Ranged::Update(_float fTimeDelta)
{

	if (Check_UpdateStop(fTimeDelta))
		return;



	m_fAccLifeTime += fTimeDelta;


	//생존시간 지났거나 맵바깥(땅포함)으로 나갔으면 삭제
	if (m_fAccLifeTime > m_fLifeTime  || Check_MapOut())
	{
		if (m_bEnableDestory)
		{
			Destory();
			m_pGameInstance->Release_Collider(m_pColliderCom);
			m_bEnableDestory = false;
		}
	}
	else
	{
		m_pTransformCom->Add_Move({ m_fRanged_Impus_NoneDirection.x * m_iDirection * fTimeDelta , m_fRanged_Impus_NoneDirection.y * fTimeDelta ,0 });

		m_pColliderCom->UpdateVector(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	}

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

	//디버그용
	if (m_ihitCharacter_Motion == HIT_KNOCK_AWAY_LEFT)
	{
		_bool bDebug = true;
	}

	//원거리 vs 원거리
	if (other->m_ColliderGroup == CCollider_Manager::COLLIDERGROUP::CG_1P_Ranged_Attack || other->m_ColliderGroup == CCollider_Manager::COLLIDERGROUP::CG_2P_Ranged_Attack)
	{

		//같은팀꺼면 생략
		if(m_pColliderCom->m_ColliderGroup != other->m_ColliderGroup)
		{
			//이펙트 처리
			Erase();

			static_cast<CAttackObject_Ranged*>(other->GetMineGameObject())->Erase();
		}

	}

	//아니면 무조건 원거리 vs 몸  이지만 혹시모르니 if
	else if (other->m_ColliderGroup == CCollider_Manager::COLLIDERGROUP::CG_1P_BODY || other->m_ColliderGroup == CCollider_Manager::COLLIDERGROUP::CG_2P_BODY)
	{
		CCharacter* pCharacter = static_cast<CCharacter*>(other->GetMineGameObject());

		AttackColliderResult eResult =
			pCharacter->Set_Hit4(m_ihitCharacter_Motion, m_eAttackGrade, m_eAttackType, m_fhitCharacter_StunTime, m_iDamage, m_fAnimationLockTime, m_pOwner->Get_iDirection(), m_fhitCharacter_Impus);

		if (eResult == RESULT_HIT)
		{
			pCharacter->Set_GroundSmash(m_bGroundSmash);
			m_pOwner->Set_AnimationStop(m_fAnimationLockTime);
			m_pOwner->Gain_KiAmount(m_iGainKiAmount);



			if (m_fForcedGravityTime != 100)   //무시할 기본 값. 0은 쓸 수도 있어서 100으로 함
			{
				pCharacter->Set_ForcveGravityTime(m_fForcedGravityTime);
			}

			if (m_bGrabbedEnd)
				pCharacter->Set_bGrabbed(false);


			m_pOwner->Gain_AttackStep(m_iGainAttackStep);
			m_pOwner->Gain_HitCount(m_iGainHitCount);



			if (m_bOwnerNextAnimation)
			{
				m_pOwner->Set_NextAnimation(m_iOnwerNextAnimationIndex, 1.f);
			}

		}
		else if (eResult == RESULT_GUARD) //가드당해도 충돌은 했으니 시간정지연출
		{
			m_pOwner->Set_AnimationStop(0.08f);
			pCharacter->Set_AnimationStop(0.08f);
		}

		//else if (eResult == RESULT_DRAW)
		//{
		//	m_pOwner->Set_AnimationStop(0.3f);
		//	pCharacter->Set_AnimationStop(0.3f);
		//}

		else if (eResult == RESULT_MISS)
		{
			////잡기는 한번 빗나가면 끝
			//if (m_eAttackType == ATTACKTYPE_GRAB_GROUND || m_eAttackType == ATTACKTYPE_GRAB_AIR)
			//{
			//	if (m_bEnableDestory)
			//	{
			//		Destory();
			//		m_bEnableDestory = false;
			//	}
			//}
			//
			////그 외에는 공격판정 사라지지 않음
			//else
				return;

		}
		else
		{
			eResult;
			_bool bDebug = true;
		}

		if (m_bEnableDestory)
		{
			Destory();
			m_bEnableDestory = false;
		}
	}


	_bool Debug = true;
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

_bool CAttackObject_Ranged::Check_MapOut()
{
	//높이 체크
	if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) < 0.1)
	{
		//지면 폭발 이펙트
		return true;
	}

	//좌우 바깥체크   는 어떻게?
	else if (false)
	{

	}
	else
		return false;

}

void CAttackObject_Ranged::Erase()
{
	if (m_bEnableDestory)
	{
		Destory();
		m_pGameInstance->Release_Collider(m_pColliderCom);
		m_bEnableDestory = false;
	}
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
