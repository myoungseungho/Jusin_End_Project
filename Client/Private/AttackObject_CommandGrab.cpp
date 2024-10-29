#include "stdafx.h"
#include "..\Public\AttackObject_CommandGrab.h"

#include "RenderInstance.h"
#include "GameInstance.h"

#include "Character.h"
#include "Main_Camera.h"

CAttackObject_CommandGrab::CAttackObject_CommandGrab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CAttackObject{ pDevice, pContext }
{

}

CAttackObject_CommandGrab::CAttackObject_CommandGrab(const CAttackObject_CommandGrab& Prototype)
	: CAttackObject{ Prototype }
{

}

HRESULT CAttackObject_CommandGrab::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAttackObject_CommandGrab::Initialize(void* pArg)
{

	if (nullptr == pArg)
		return E_FAIL;

	ATTACK_COMMANDGRAB_DESC* pDesc = static_cast<ATTACK_COMMANDGRAB_DESC*>(pArg);
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	

	m_fDistance = pDesc->fDistance;
	m_iGrabAnimationIndex = pDesc->iGrabAnimationIndex;
	m_fGrabAnimationPosition = pDesc->fGrabAnimationPosition;
	
	m_bForcedHit = pDesc->bForcedHit;
	m_iOnwerDirection = pDesc->iOnwerDirection;

	if (pDesc->iVirtualCameraindex != 200)
	{
		m_iVirtualCameraindex = pDesc->iVirtualCameraindex;
		m_ianimationIndex = pDesc->ianimationIndex;
		m_fCameraShakeDuration = pDesc->fCameraShakeDuration;
		m_fCameraShakeMagnitude = pDesc->fCameraShakeMagnitude;
	}

	if (m_iOnwerDirection == 231)
	{
		m_iOnwerDirection = m_pOwner->Get_iDirection();
	}

	return S_OK;
}

void CAttackObject_CommandGrab::Priority_Update(_float fTimeDelta)
{

}

void CAttackObject_CommandGrab::Update(_float fTimeDelta)
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

void CAttackObject_CommandGrab::Late_Update(_float fTimeDelta)
{

	
		
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CAttackObject_CommandGrab::Render(_float fTimeDelta)
{

#ifdef _DEBUG
	m_pColliderCom->Render(fTimeDelta);
#endif // DEBUG


	return S_OK;
}

/*
void CAttackObject_CommandGrab::Set_RemoteDestory()
{
	if (m_bEnableDestory)
	{
		m_pGameInstance->Release_Collider(m_pColliderCom);
		Destory();
		m_bEnableDestory = false;
	}
}
*/


void CAttackObject_CommandGrab::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{



	//잡기가 끊겼으면 실패
	if (m_pOwner->Get_iAnimationIndex() != m_iGrabAnimationIndex || m_pOwner->Get_bStun())
		return;

	//잡기 vs 사람 
	else if (other->m_ColliderGroup == CCollider_Manager::COLLIDERGROUP::CG_1P_BODY || other->m_ColliderGroup == CCollider_Manager::COLLIDERGROUP::CG_2P_BODY)
	{
		CCharacter* pCharacter = static_cast<CCharacter*>(other->GetMineGameObject());

		AttackColliderResult eResult =
			pCharacter->Set_Hit4(m_ihitCharacter_Motion, m_eAttackGrade, m_eAttackType, m_fhitCharacter_StunTime, m_iDamage, m_fAnimationLockTime, m_iOnwerDirection, m_fhitCharacter_Impus);
			//pCharacter->Set_Hit3(m_ihitCharacter_Motion, m_eAttackGrade, m_eAttackType, m_fhitCharacter_StunTime, m_iDamage, m_fAnimationLockTime, m_fhitCharacter_Impus);

		if (eResult == RESULT_HIT || m_bForcedHit)
		{


			pCharacter->Set_GroundSmash(m_bGroundSmash);
			m_pOwner->Set_AnimationStop(m_fAnimationLockTime);
			m_pOwner->Gain_AttackStep(m_iGainAttackStep);
			m_pOwner->Gain_HitCount(m_iGainHitCount);

			m_pOwner->Set_GrabLoofCount(2);


			if (m_iVirtualCameraindex != 200)
			{
				CMain_Camera* main_Camera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")));
				main_Camera->Play((CMain_Camera::VIRTUAL_CAMERA)m_iVirtualCameraindex, m_ianimationIndex);
				main_Camera->StartCameraShake(m_fCameraShakeDuration, m_fCameraShakeMagnitude);
				
			}

			pCharacter->Set_bGrabbed(true);
			if (m_fForcedGravityTime != 100)   //무시할 기본 값. 0은 쓸 수도 있어서 100으로 함
			{
				pCharacter->Set_ForcveGravityTime(m_fForcedGravityTime);
			}


			if(m_fDistance.x != 100)
			{
				_vector vPos = static_cast<CTransform*>(m_pOwner->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
				vPos += _vector{ m_fDistance.x, m_fDistance.y , 0.f, 0.f };
				static_cast<CTransform*>(pCharacter->Get_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, vPos);
			}


			if (m_bOwnerNextAnimation)
			{
				//m_pOwner->Set_NextAnimation(m_iOnwerNextAnimationIndex, 1.f);

				m_pOwner->Set_Animation(m_iOnwerNextAnimationIndex, false);
				m_pOwner->Set_CurrentAnimationPositionJump(m_fGrabAnimationPosition);

			}



		}
		else if (eResult == RESULT_GUARD) //가드당해도 충돌은 했으니 시간정지연출
		{
			m_pOwner->Set_AnimationStop(0.08f);
			pCharacter->Set_AnimationStop(0.08f);
		}

		else if (eResult == RESULT_DRAW)
		{
			m_pOwner->Set_AnimationStop(0.3f);
			pCharacter->Set_AnimationStop(0.3f);
		}

		else if (eResult == RESULT_MISS)
		{
			//잡기는 한번 빗나가면 끝
			//if (m_eAttackType == ATTACKTYPE_GRAB_GROUND || m_eAttackType == ATTACKTYPE_GRAB_AIR)
			{
				//if (m_bEnableDestory)
				//{
				//	Destory();
				//	m_bEnableDestory = false;
				//}
			}

		}

		//if (m_bEnableDestory)
		//{
		//	Destory();
		//	m_bEnableDestory = false;
		//}
	}




	



	
}

void CAttackObject_CommandGrab::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
	_bool Debug = true;

}

void CAttackObject_CommandGrab::OnCollisionExit(CCollider* other)
{
	_bool Debug = true;

}

void CAttackObject_CommandGrab::CollisingAttack()
{
}

void CAttackObject_CommandGrab::CollisingPlayer()
{
}







CAttackObject_CommandGrab* CAttackObject_CommandGrab::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAttackObject_CommandGrab* pInstance = new CAttackObject_CommandGrab(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CAttackObject_CommandGrab"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAttackObject_CommandGrab::Clone(void* pArg)
{
	CAttackObject_CommandGrab* pInstance = new CAttackObject_CommandGrab(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CAttackObject_CommandGrab"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAttackObject_CommandGrab::Free()
{
	__super::Free();
	Safe_Release(m_pColliderCom);

}
