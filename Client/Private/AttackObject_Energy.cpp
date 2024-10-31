#include "stdafx.h"
#include "..\Public\AttackObject_Energy.h"

#include "RenderInstance.h"
#include "GameInstance.h"

#include "Character.h"

CAttackObject_Energy::CAttackObject_Energy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CAttackObject{ pDevice, pContext }
{

}

CAttackObject_Energy::CAttackObject_Energy(const CAttackObject_Energy& Prototype)
	: CAttackObject{ Prototype }
{

}

HRESULT CAttackObject_Energy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAttackObject_Energy::Initialize(void* pArg)
{

	if (nullptr == pArg)
		return E_FAIL;


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	ATTACK_RANGED_DESC* pDesc = static_cast<ATTACK_RANGED_DESC*>(pArg);

	m_fStartOffset = pDesc->fStartOffset;
	//m_fRanged_Impus_NoneDirection = pDesc->fRanged_Impus_NoneDirection;
	//m_iDirection = pDesc->iDirection;


	m_eExplositionColor = pDesc->eExplosionColor;

	_vector vPos = m_pOwner->Get_vPosition();
	_vector vStartOffset = { m_fStartOffset.x, m_fStartOffset.y, 0.f, 0.f };
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos + vStartOffset);


	return S_OK;
}



void CAttackObject_Energy::Update(_float fTimeDelta)
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

}

void CAttackObject_Energy::Late_Update(_float fTimeDelta)
{

	


	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CAttackObject_Energy::Render(_float fTimeDelta)
{

#ifdef _DEBUG
	m_pColliderCom->Render(fTimeDelta);
#endif // DEBUG


	return S_OK;
}



void CAttackObject_Energy::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{

	//원거리 vs 원거리
	if (other->m_ColliderGroup == CCollider_Manager::COLLIDERGROUP::CG_1P_Ranged_Attack || other->m_ColliderGroup == CCollider_Manager::COLLIDERGROUP::CG_2P_Ranged_Attack)
	{

		
		//이펙트 처리
		Erase();

		if (m_eExplositionColor != ENERGY_LIGHT_NONE)
		{
			
			if (m_eExplositionColor == ENERGY_LIGHT_YELLOW)
			{
				//Add_YellowLight();
				
				Add_YellowLight(m_pColliderCom->Get_Overlap_Center_Position(other));

			}
		}

		static_cast<CAttackObject_Energy*>(other->GetMineGameObject())->Erase();


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

			if (m_eExplositionColor != ENERGY_LIGHT_NONE)
			{

				if (m_eExplositionColor == ENERGY_LIGHT_YELLOW)
				{
					//Add_YellowLight();
					Add_YellowLight(m_pColliderCom->Get_Overlap_Center_Position(other));
				}
			}

		}
		else if (eResult == RESULT_GUARD) //가드당해도 충돌은 했으니 시간정지연출
		{
			m_pOwner->Set_AnimationStop(0.08f);
			pCharacter->Set_AnimationStop(0.08f);

			if (m_eExplositionColor != ENERGY_LIGHT_NONE)
			{

				if (m_eExplositionColor == ENERGY_LIGHT_YELLOW)
				{
					//Add_YellowLight();
					Add_YellowLight(m_pColliderCom->Get_Overlap_Center_Position(other));
				}
			}
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
		

		if (m_bEnableDestory)
		{
			Destory();
			m_bEnableDestory = false;
		}
	}


	_bool Debug = true;
}

void CAttackObject_Energy::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
	_bool Debug = true;

}

void CAttackObject_Energy::OnCollisionExit(CCollider* other)
{
	_bool Debug = true;

}

void CAttackObject_Energy::CollisingAttack()
{
}

void CAttackObject_Energy::CollisingPlayer()
{
}

_bool CAttackObject_Energy::Check_MapOut()
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

void CAttackObject_Energy::Add_YellowLight(_float3 fPosition)
{
	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof(LIGHT_DESC));
	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vPosition = _float4(fPosition.x, fPosition.y, 0.f, 1.f);
	LightDesc.fRange = 3.f;
	LightDesc.vDiffuse = _float4(1.2f, 1.15f, 0.7f, 1.0f);
	//LightDesc.vDiffuse = _float4(1.0f, 0.f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = _float4(1.0f, 0.95f, 0.45f, 1.f);

	LightDesc.fAccTime = 0.f;
	LightDesc.fLifeTime = 0.5f;
	LightDesc.strName = "Explosion";
	if (FAILED(m_pRenderInstance->Add_Effect_Light(LightDesc.strName, LightDesc)))
		return;
}


void CAttackObject_Energy::Add_YellowLight()
{
	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof(LIGHT_DESC));
	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	LightDesc.fRange = 30.f;
	LightDesc.vDiffuse = _float4(1.2f, 1.15f, 0.7f, 1.0f);
	//LightDesc.vDiffuse = _float4(1.0f, 0.f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = _float4(1.0f, 0.95f, 0.45f, 1.f);

	LightDesc.fAccTime = 0.f;
	LightDesc.fLifeTime = 0.5f;
	LightDesc.strName = "Explosion";
	if (FAILED(m_pRenderInstance->Add_Effect_Light(LightDesc.strName, LightDesc)))
		return;
}


void CAttackObject_Energy::Erase()
{
	if (m_bEnableDestory)
	{
		Destory();
		m_pGameInstance->Release_Collider(m_pColliderCom);
		m_bEnableDestory = false;
	}
}







CAttackObject_Energy* CAttackObject_Energy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAttackObject_Energy* pInstance = new CAttackObject_Energy(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CAttackObject_Energy"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAttackObject_Energy::Clone(void* pArg)
{
	CAttackObject_Energy* pInstance = new CAttackObject_Energy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CAttackObject_Energy"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAttackObject_Energy::Free()
{
	__super::Free();
	Safe_Release(m_pColliderCom);

}
