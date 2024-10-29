#include "stdafx.h"
#include "..\Public\AttacKObject.h"

#include "RenderInstance.h"
#include "GameInstance.h"

#include "UI_Define.h"
#include "Character.h"
#include "Main_Camera.h"

CAttackObject::CAttackObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CAttackObject::CAttackObject(const CAttackObject& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CAttackObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAttackObject::Initialize(void* pArg)
{

	if (nullptr == pArg)
		return E_FAIL;


	ATTACK_DESC* pDesc = static_cast<ATTACK_DESC*>(pArg);

	m_ihitCharacter_Motion = pDesc->ihitCharacter_Motion;
	m_eAttackGrade = pDesc->eAttackGrade;
	m_eAttackType = pDesc->eAttackType;

	m_fhitCharacter_Impus = pDesc->fhitCharacter_Impus;
	m_fhitCharacter_StunTime = pDesc->fhitCharacter_StunTime;
	m_fLifeTime = pDesc->fLifeTime;
	m_iTeam = pDesc->iTeam;
	m_iDamage = pDesc->iDamage;


	m_fAnimationLockTime = pDesc->fAnimationLockTime;

	//m_bOwnerGravityTimeReset = pDesc->bOwnerGravityTimeReset;
	m_bGroundSmash = pDesc->bGroundSmash;

	//m_bGain_AttackStep =pDesc->bGainAttackStep;
	m_iGainAttackStep = pDesc->iGainAttackStep;
	m_iGainHitCount = pDesc->iGainHitCount;

	m_pOwner = pDesc->pOwner;

	m_bOwnerNextAnimation = pDesc->bOwnerNextAnimation;
	m_iOnwerNextAnimationIndex = pDesc->iOnwerNextAnimationIndex;

	m_bGrabbedEnd = pDesc->bGrabbedEnd;
	m_bCameraZoom = pDesc->bCameraZoom;

	m_fForcedGravityTime = pDesc->fForcedGravityTime;

	m_iGainKiAmount = pDesc->iGainKiAmount;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->ColliderDesc.pTransform->Get_State(CTransform::STATE_POSITION));

	//m_pOwnerTransform = pDesc->ColliderDesc.pTransform;
	m_pOwnerTransform = static_cast<CTransform*>(m_pOwner->Get_Component(TEXT("Com_Transform")));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pOwnerTransform->Get_State(CTransform::STATE_POSITION));


	if (FAILED(Ready_Components(pDesc)))
		return E_FAIL;

	//m_pColliderCom->Update(m_fOffset);




	return S_OK;
}

void CAttackObject::Priority_Update(_float fTimeDelta)
{

}

void CAttackObject::Update(_float fTimeDelta)
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


	//if(m_bIsActive)
	//{
	//	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pOwnerTransform->Get_State(CTransform::STATE_POSITION));
	//	//m_pColliderCom->Update(m_fOffset);
	//
	//	m_fAccLifeTime += fTimeDelta;
	//
	//	if (m_fAccLifeTime > m_fLifeTime)
	//	{
	//		m_bIsActive = false;
	//	}
	//
	//	//m_pColliderCom->Update(m_pOwnerTransform->Get_WorldMatrix());
	//	m_pColliderCom->UpdateVector(m_pOwnerTransform->Get_State(CTransform::STATE_POSITION));
	//}

}

void CAttackObject::Late_Update(_float fTimeDelta)
{

	//if (m_bIsActive)
	//{
	//
	//
	//	
	//
	//
	//	
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pOwnerTransform->Get_State(CTransform::STATE_POSITION));
	//	//m_pColliderCom->Update(m_fOffset);
	//
	//	_bool bisCollsing = false;
	//
	//	//Layer_캐릭터에 대하여
	//	for (int i = 0; i < m_pGameInstance->Get_LayerSize(LEVEL_GAMEPLAY, TEXT("Layer_Character")); i++)
	//	{
	//
	//		CCharacter* pCharacter = static_cast<CCharacter*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Character"), i));
	//
	//
	//
	//		//생성된 공격과 상대방이 다른팀이라면 충돌검사
	//		if (pCharacter->Get_iPlayerTeam() != m_iTeam)
	//		{
	//
	//
	//
	//			//if (m_pColliderCom->isColliding(pCharacter->Get_Component(TEXT("Com_Collider"))))
	//			//{
	//			//	bisCollsing = true;
	//			//	
	//			//	//공격성공시
	//			//	//if(pCharacter->Set_Hit(m_ihitCharacter_Motion, m_fhitCharacter_StunTime, m_iDamage, m_fAnimationLockTime, m_fhitCharacter_Impus))
	//			//	//if (pCharacter->Set_Hit2(m_ihitCharacter_Motion, m_eAttackGrade, m_eAttackType, m_fhitCharacter_StunTime, m_iDamage, m_fAnimationLockTime, m_fhitCharacter_Impus))
	//			//	//{
	//			//	//	pCharacter->Set_GroundSmash(m_bGroundSmash);
	//			//	//	m_pOwner->Set_AnimationStop(m_fAnimationLockTime);
	//			//	//
	//			//	//	if (m_bGain_AttackStep)
	//			//	//	{
	//			//	//		m_pOwner->Gain_AttackStep(1);
	//			//	//	}
	//			//	//
	//			//	//}
	//			//	//else  //가드당했을시 충돌은 했으니
	//			//	//{
	//			//	//	m_pOwner->Set_AnimationStop(0.05f);
	//			//	//	pCharacter->Set_AnimationStop(0.05f);
	//			//	//}
	//
	//
	//			//	AttackColliderResult eResult =
	//			//		pCharacter->Set_Hit3(m_ihitCharacter_Motion, m_eAttackGrade, m_eAttackType, m_fhitCharacter_StunTime, m_iDamage, m_fAnimationLockTime, m_fhitCharacter_Impus);
	//
	//			//		
	//			//	if (eResult == RESULT_HIT)
	//			//	{
	//			//		pCharacter->Set_GroundSmash(m_bGroundSmash);
	//			//		m_pOwner->Set_AnimationStop(m_fAnimationLockTime);
	//
	//			//		//if (m_bGain_AttackStep)
	//			//		{
	//			//			m_pOwner->Gain_AttackStep(m_iGain_AttackStep);
	//			//		}
	//
	//			//		if (m_bOwnerNextAnimation)
	//			//		{
	//			//			m_pOwner->Set_NextAnimation(m_iOnwerNextAnimationIndex,1.f);
	//			//		}
	//
	//			//	}
	//			//	else if (eResult == RESULT_GUARD) //가드당해도 충돌은 했으니 시간정지연출
	//			//	{
	//			//		m_pOwner->Set_AnimationStop(0.08f);
	//			//		pCharacter->Set_AnimationStop(0.08f);
	//			//	}
	//
	//			//	else if (eResult == RESULT_DRAW)
	//			//	{
	//			//		m_pOwner->Set_AnimationStop(0.3f);
	//			//		pCharacter->Set_AnimationStop(0.3f);
	//			//	}
	//
	//			//	else if (eResult == RESULT_MISS)
	//			//	{
	//			//		//잡기는 한번 빗나가면 끝
	//			//		if (m_eAttackType == ATTACKTYPE_GRAB_GROUND || m_eAttackType == ATTACKTYPE_GRAB_AIR)
	//			//			bisCollsing = true;
	//
	//			//		//그 외에는 공격판정 사라지지 않음
	//			//		else
	//			//			bisCollsing = false;
	//
	//			//	}
	//
	//
	//			//}
	//		}
	//	}
	//
	//	if (bisCollsing)
	//	{
	//		m_bIsActive = false;
	//		//if (m_bOwnerGravityTimeReset)
	//		//{
	//		//	m_pOwner->Set_ForcedGravityTime_LittleUp();
	//		//}
	//		Destory();
	//	}
	//


	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CAttackObject::Render(_float fTimeDelta)
{

#ifdef _DEBUG
	m_pColliderCom->Render(fTimeDelta);
#endif // DEBUG


	return S_OK;
}

/*
void CAttacKObject::Set_RemoteDestory()
{
	if (m_bEnableDestory)
	{
		m_pGameInstance->Release_Collider(m_pColliderCom);
		Destory();
		m_bEnableDestory = false;
	}
}
*/


void CAttackObject::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{


	//근접공격 vs 사람 
	if (other->m_ColliderGroup == CCollider_Manager::COLLIDERGROUP::CG_1P_BODY || other->m_ColliderGroup == CCollider_Manager::COLLIDERGROUP::CG_2P_BODY)
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


			if (m_bCameraZoom)
			{
				if (m_bGroundSmash == true)
					//공중에서 바닥으로 내려찍을 때
					Camera_GroundSmash(m_pOwner, pCharacter);
				else if (m_ihitCharacter_Motion == HIT_KNOCK_AWAY_LEFT || m_ihitCharacter_Motion == HIT_SPIN_AWAY_LEFTUP)
					//강공격 맞았을 때 카메라 셋팅
					Camera_Hit_Knock_Away_Left(m_pOwner, pCharacter);
				else if (m_ihitCharacter_Motion == HIT_KNOCK_AWAY_UP)
					//어퍼 맞았을 때
					Camera_Hit_Knock_Away_Up(m_pOwner, pCharacter);




				//아래 위치 조정은 일부러 카메라 안에 넣음

				//어퍼컷/올려차기의 경우  정지시간이 긴 공격들은 위치조정
				if (m_ihitCharacter_Motion == HIT_KNOCK_AWAY_UP && m_fAnimationLockTime > 0.3f)
				{

					_float fCharacterHeight = pCharacter->Get_fHeight();
					_float fOnwerHeight = m_pOwner->Get_fHeight();

					//공중에서는 어퍼연출 없음
					if (fCharacterHeight > 2)
					{

					}

					//피격자의 높이가 공격자보다 낮은경우 좀 위로 조정
					else if (fCharacterHeight < fOnwerHeight + 1.f)
					{
						//둘 사이의 거리가 0.1이라고 할때    0.1+0.2f니까
						pCharacter->Add_Move({ 0, fOnwerHeight - fCharacterHeight + 1.f });

					}

				}
			}
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

		else if (eResult == RESULT_DRAW)
		{
			m_pOwner->Set_AnimationStop(0.3f);
			pCharacter->Set_AnimationStop(0.3f);
		}

		else if (eResult == RESULT_MISS)
		{
			//잡기는 한번 빗나가면 끝
			if (m_eAttackType == ATTACKTYPE_GRAB_GROUND || m_eAttackType == ATTACKTYPE_GRAB_AIR)
			{
				if (m_bEnableDestory)
				{
					Destory();
					m_bEnableDestory = false;
				}
			}

			//그 외에는 공격판정 사라지지 않음
			else
				return;

		}

		if (m_bEnableDestory)
		{
			Destory();
			m_bEnableDestory = false;
		}
	}


	//근접공격 vs 근접공격
	else if (other->m_ColliderGroup == CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack || other->m_ColliderGroup == CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack)
	{
		_bool bDebugA = true;


	}

}

void CAttackObject::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
	_bool Debug = true;

}

void CAttackObject::OnCollisionExit(CCollider* other)
{
	_bool Debug = true;

}

void CAttackObject::CollisingAttack()
{
}

void CAttackObject::CollisingPlayer()
{
}

//강공격 시
void CAttackObject::Camera_Hit_Knock_Away_Left(CCharacter* pOwner, CCharacter* pHitOwner)
{
	CMain_Camera* main_Camera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")));

	CCharacter::Character_INFO_DESC characterDesc = pOwner->Get_PawnDesc();
	CUI_Define::PLAYER_ID PlayerID = characterDesc.ePlayerID;

	switch (PlayerID)
	{
	case Client::CUI_Define::GOKU:
		main_Camera->Play(CMain_Camera::VIRTUAL_CAMERA::VIRTUAL_CAMERA_SON_HEAVY, 0);
		main_Camera->StartCameraShake(0.5f, 0.2f);
		m_pGameInstance->Play_Sound(CSound_Manager::SOUND_KEY_NAME::Goku_Heavy_Attack_SFX, false, 1.f);
		m_pGameInstance->Play_Sound(CSound_Manager::SOUND_KEY_NAME::Goku_Heavy_Attack, false, 1.f);
		break;
	case Client::CUI_Define::ANDROID21:
		main_Camera->Play(CMain_Camera::VIRTUAL_CAMERA::VIRTUAL_CAMERA_21_HEAVY, 0);
		main_Camera->StartCameraShake(0.5f, 0.2f);
		break;
	case Client::CUI_Define::BUU:
		break;
	case Client::CUI_Define::HIT:
		break;
	}
}

//
void CAttackObject::Camera_Hit_Knock_Away_Up(CCharacter* pOwner, CCharacter* pHitOwner)
{
	CMain_Camera* main_Camera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")));

	CCharacter::Character_INFO_DESC characterDesc = pOwner->Get_PawnDesc();
	CUI_Define::PLAYER_ID PlayerID = characterDesc.ePlayerID;

	switch (PlayerID)
	{
	case Client::CUI_Define::GOKU:
		main_Camera->Play(CMain_Camera::VIRTUAL_CAMERA::VIRTUAL_CAMERA_SON_KNOCK_AWAY_UP, 0);
		main_Camera->StartCameraShake(0.5f, 0.2f);
		m_pGameInstance->Play_Sound(CSound_Manager::SOUND_KEY_NAME::Goku_Heavy_Attack_SFX, false, 1.f);
		m_pGameInstance->Play_Sound(CSound_Manager::SOUND_KEY_NAME::Goku_Heavy_Attack, false, 1.f);
		break;
	case Client::CUI_Define::ANDROID21:
		main_Camera->Play(CMain_Camera::VIRTUAL_CAMERA::VIRTUAL_CAMERA_21_KNOCK_AWAY_UP, 0);
		main_Camera->StartCameraShake(0.5f, 0.2f);
		break;
	case Client::CUI_Define::BUU:
		break;
	case Client::CUI_Define::HIT:
		break;
	}
}

void CAttackObject::Camera_GroundSmash(CCharacter* pOwner, CCharacter* pHitOwner)
{
	CMain_Camera* main_Camera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")));

	CCharacter::Character_INFO_DESC characterDesc = pOwner->Get_PawnDesc();
	CUI_Define::PLAYER_ID PlayerID = characterDesc.ePlayerID;

	switch (PlayerID)
	{
	case Client::CUI_Define::GOKU:
		main_Camera->Play(CMain_Camera::VIRTUAL_CAMERA::VIRTUAL_CAMERA_SON_AIR_SMASH, 0);
		m_pGameInstance->Play_Sound(CSound_Manager::SOUND_KEY_NAME::Goku_Heavy_Attack_SFX, false, 1.f);
		m_pGameInstance->Play_Sound(CSound_Manager::SOUND_KEY_NAME::Goku_Heavy_Attack, false, 1.f);
		break;
	case Client::CUI_Define::ANDROID21:
		break;
	case Client::CUI_Define::BUU:
		break;
	case Client::CUI_Define::HIT:
		break;
	}

	main_Camera->StartCameraShake(0.5f, 0.2f);
}




HRESULT CAttackObject::Ready_Components(ATTACK_DESC* pDesc)
{

	//CCollider_Test::COLLIDER_DESC ColliderDesc{};
	//ColliderDesc.pTransform = m_pTransformCom; //pDesc->ColliderDesc.pTransform;
	////ColliderDesc.fSizeX = pDesc->ColliderDesc.fSizeX; 
	////ColliderDesc.fSizeY = pDesc->ColliderDesc.fSizeY;
	//ColliderDesc.fSizeZ = 1;


	//ColliderDesc.Offset = pDesc->ColliderDesc.Offset;

	//CCollider::COLLIDER_DESC ColliderDesc{};
	//ColliderDesc = pDesc->ColliderDesc;
	//ColliderDesc.MineGameObject = this;

	CBounding_AABB::BOUNDING_AABB_DESC ColliderDesc{};
	ColliderDesc = pDesc->ColliderDesc;
	ColliderDesc.pMineGameObject = this;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;


	//m_pColliderCom->Update(m_pOwnerTransform->Get_WorldMatrix());
	m_pColliderCom->UpdateVector(m_pOwnerTransform->Get_State(CTransform::STATE_POSITION));

	m_pGameInstance->Add_ColliderObject(ColliderDesc.colliderGroup, m_pColliderCom);




	return S_OK;
}



CAttackObject* CAttackObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAttackObject* pInstance = new CAttackObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CAttackObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAttackObject::Clone(void* pArg)
{
	CAttackObject* pInstance = new CAttackObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CAttackObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAttackObject::Free()
{
	__super::Free();
	Safe_Release(m_pColliderCom);

}
