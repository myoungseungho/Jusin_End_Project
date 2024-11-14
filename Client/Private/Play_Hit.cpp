#include "stdafx.h"
#include "..\Public\Play_Hit.h"

#include "RenderInstance.h"
#include "GameInstance.h"

#include "AttackObject.h"
#include "AttackObject_Grab.h"
#include "AttackObject_CommandGrab.h"
#include "AttackObject_Ranged.h"

#include "AttackObject_Energy.h"

#include "AttackObject_Reflect.h"

#include "UI_Manager.h"
#include "iostream"


#include "BattleInterface.h"


#include "Main_Camera.h"

#include "BoneEffectObject.h"
#include "Effect_Manager.h"	

#include "Effect_Layer.h"

#include "Animation.h"
#include "Main_Camera.h"

CPlay_Hit::CPlay_Hit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCharacter{ pDevice, pContext }
{

}

CPlay_Hit::CPlay_Hit(const CPlay_Hit& Prototype)
	: CCharacter{ Prototype }
{

}

HRESULT CPlay_Hit::Initialize_Prototype()
{
	//m_pFrameEvent = CFrameEvent_Manager::Get_Instance()->Get_pFrameEventMap();
	CFrameEvent_Manager::Get_Instance()->Initalize_NameMap();
	CFrameEvent_Manager::Get_Instance()->LoadFile2("../Bin/FrameEventData/EventData_Hit.txt");

	return S_OK;
}

HRESULT CPlay_Hit::Initialize(void* pArg)
{

	m_eCharacterIndex = PLAY_HIT;
	m_iFallAnimationIndex = ANIME_JUMP_DOWN;
	m_iIdleAnimationIndex = ANIME_IDLE;
	m_iStandingMidAttackAnimationIndex = ANIME_ATTACK_MEDIUM;
	m_iChaseAnimationIndex = ANIME_CHASE;
	m_iForwardDashAnimationIndex = ANIME_FORWARD_DASH;

	m_iForwardDashEndAnimationIndex = { ANIME_FORWARD_DASH_END };

	m_iCrouchAnimationIndex = { ANIME_CROUCHING };
	m_iBackWalkAnimationIndex = { ANIME_BACK_WALK };
	m_iForwardWalkAnimationIndex = { ANIME_FORWARD_WALK };
	m_iHit_Away_LeftAnimationIndex = { ANIME_HIT_HEAVY_AWAY_LEFT };
	m_iHit_Away_UpAnimationIndex = { ANIME_HIT_HEAVY_AWAY_UP };
	m_iHit_Away_LeftDownAnimationIndex = { ANIME_HIT_HEAVY_AWAY_LEFTDOWN };

	m_iHit_WallBouce = { ANIME_HIT_WALLBOUNCE };

	m_iHit_Air_LightAnimationIndex = { ANIE_HIT_AIR };
	m_iHit_Air_FallAnimationIndex = { ANIME_HIT_FALL };

	m_iHit_Air_Spin_LeftUp = { ANIME_HIT_HEAVY_AWAY_SPIN_LEFTUP };
	m_iHit_Air_Spin_Up = { ANIME_HIT_HEAVY_AWAY_SPIN_UP };

	m_iAttack_Air1 = { ANIME_ATTACK_AIR1 };
	m_iAttack_Air2 = { ANIME_ATTACK_AIR2 };
	m_iAttack_Air3 = { ANIME_ATTACK_AIR3 };
	m_iAttack_AirUpper = { ANIME_ATTACK_UPPER_AIR };

	m_iAttack_LightLast = { ANIME_ATTACK_LIGHT3 };
	m_iAttack_Heavy = { ANIME_ATTACK_HEAVY };
	m_iAttack_Crouch_Heavy = { ANIME_ATTACK_CROUCH_HEAVY };

	m_iBound_Ground = { ANIME_HIT_BOUND_DOWN };

	m_iGuard_GroundAnimationIndex = { ANIME_GUARD_GROUND };
	m_iGuard_CrouchAnimationIndex = { ANIME_GUARD_CROUCH };
	m_iGuard_AirAnimationIndex = { ANIME_GUARD_AIR };

	m_iGrabReadyAnimationIndex = { ANIME_GRAB_READY };
	m_iGrabAnimationIndex = { ANIME_GRAB };




	m_iJumpAnimationIndex = { ANIME_JUMP_UP };
	m_iHit_Stand_LightAnimationIndex = { ANIME_HIT_LIGHT };
	m_iHit_Stand_MediumAnimationIndex = { ANIME_HIT_LIGHT_FRONT };
	m_iHit_Crouch_AnimationIndex = { ANIME_HIT_CROUCH };
	m_iBreakFall_Ground = { ANIME_BREAK_FALL_GROUND };   //100
	m_iBreakFall_Air = { ANIME_BREAK_FALL_AIR };   //101


	m_iSparkingAnimationIndex = { ANIME_SPARKING };
	m_iDyingStandingAnimationIndex = { ANIME_DIE_STAND };

	m_iReflectAnimationIndex = { ANIME_REFLECT };

	m_iNextAnimation.first = ANIME_IDLE;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-2.f + (m_iPlayerTeam * 2), 0.f, 0.f, 1.f));


	m_tAttackMap.Initalize(this);
	Character_DESC* pDesc = static_cast<Character_DESC*>(pArg);
	m_strName = "HIT_" + to_string(m_iPlayerTeam) + "_" + to_string(pDesc->ePlayerSlot);
	m_RendererDesc.strName = m_strName;
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;

	LightDesc.vDirection = _float4(-0.15f, -0.7f, 0.5f, 0.f);
	LightDesc.vDiffuse = _float4(0.9f, 0.9f, 1.0f, 1.0f);
	LightDesc.vAmbient = _float4(0.5f, 0.5f, 0.5f, 1.f);
	LightDesc.vSpecular = _float4(0.f, 0.f, 0.f, 1.f);
	LightDesc.pPlayerDirection = &m_iLookDirection;
	LightDesc.strName = m_strName;

	if (FAILED(m_pRenderInstance->Add_Player_Light(m_strName, LightDesc)))
		return E_FAIL;
	/*
	빛 각자 생성해주기
	*/


	//m_pModelCom->SetUp_Animation(16, true);
	m_pModelCom->SetUp_Animation(0, true);
	m_pModelCom->Play_Animation(0.f);

	//MoveCommandPatterns.push_back({ hadoukenPattern, ShowInputBuffer});

	MoveCommandPatterns.push_back({ Command_236Attack, 66 });
	MoveCommandPatterns.push_back({ Command_236Attack_Extra, 66 });

	MoveCommandPatterns.push_back({ Command_LightAttack, ANIME_ATTACK_LIGHT1 });
	MoveCommandPatterns.push_back({ Command_MediumAttack, ANIME_ATTACK_MEDIUM });
	MoveCommandPatterns.push_back({ Command_HeavyAttack, ANIME_ATTACK_HEAVY });


	MoveCommandPatternsFunction.push_back({ Command_236UltimateAttack, bind(&CHit_MeleeAttack::Attack_236Ultimate, &m_tAttackMap) });




	MoveCommandPatternsFunction.push_back({ Command_236Attack,  bind(&CHit_MeleeAttack::Attack_236, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_236Attack_Extra,  bind(&CHit_MeleeAttack::Attack_236, &m_tAttackMap) });

	MoveCommandPatternsFunction.push_back({ Command_214Attack,  bind(&CHit_MeleeAttack::Attack_214, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_214Attack_Extra,  bind(&CHit_MeleeAttack::Attack_214, &m_tAttackMap) });

	MoveCommandPatternsFunction.push_back({ Command_236Special,  bind(&CHit_MeleeAttack::Attack_236Special, &m_tAttackMap) });


	MoveCommandPatternsFunction.push_back({ Command_214FinalAttack, bind(&CHit_MeleeAttack::Attack_214Final, &m_tAttackMap) });


	MoveCommandPatternsFunction.push_back({ Command_Crouch_LightAttack, bind(&CHit_MeleeAttack::Attack_Crouch_Light, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_Crouch_MediumAttack, bind(&CHit_MeleeAttack::Attack_Crouch_Medium, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_Crouch_HeavyAttack, bind(&CHit_MeleeAttack::Attack_Crouch_Heavy, &m_tAttackMap) });


	MoveCommandPatternsFunction.push_back({ Command_Crouch_MediumAttack_Extra, bind(&CHit_MeleeAttack::Attack_Crouch_Medium, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_Crouch_HeavyAttack_Extra, bind(&CHit_MeleeAttack::Attack_Crouch_Heavy, &m_tAttackMap) });



	//위에서 부터 확인하므로 간단한 커맨드가 아래로 가야함
	MoveCommandPatternsFunction.push_back({ Command_LightAttack, bind(&CHit_MeleeAttack::Attack_Light, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_MediumAttack, bind(&CHit_MeleeAttack::Attack_Medium, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_HeavyAttack, bind(&CHit_MeleeAttack::Attack_Heavy, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_SpecialAttack, bind(&CHit_MeleeAttack::Attack_Special, &m_tAttackMap) });


	MoveCommandPatternsFunction.push_back({ Command_HeavyAttack_Extra, bind(&CHit_MeleeAttack::Attack_Heavy, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_MediumAttack_Extra, bind(&CHit_MeleeAttack::Attack_Medium, &m_tAttackMap) });


	MoveCommandPatternsFunction.push_back({ Command_Grab, bind(&CHit_MeleeAttack::Attack_Grab, &m_tAttackMap) });




	MoveCommandPatternsFunction_Exactly.push_back({ Command_Up_SpecialAttack, bind(&CHit_MeleeAttack::Attack_Up_Speical, &m_tAttackMap) });
	MoveCommandPatternsFunction_Exactly.push_back({ Command_Crouch_SpecialAttack, bind(&CHit_MeleeAttack::Attack_Crouch_Speical, &m_tAttackMap) });


	//MoveCommandPatternsFunction.push_back({ Command_BackDash, bind(&CHit_MeleeAttack::BackDash, &m_tAttackMap) });
	//MoveCommandPatternsFunction.push_back({ Command_Forward, bind(&CHit_MeleeAttack::ForwardDash, &m_tAttackMap) });

	MoveCommandPatternsFunction_Exactly.push_back({ Command_BackDash, bind(&CHit_MeleeAttack::BackDash, &m_tAttackMap) });
	MoveCommandPatternsFunction_Exactly.push_back({ Command_Forward, bind(&CHit_MeleeAttack::ForwardDash, &m_tAttackMap) });


	MoveCommandPatternsFunction_Exactly.push_back({ Command_Reflect, bind(&CHit_MeleeAttack::Reflect, &m_tAttackMap) });


	m_eCharacterID = CUI_Define::PLAYER_ID::HIT;

	CBattleInterface_Manager::Get_Instance()->Regist_Character(m_iPlayerTeam, this, m_ePlayerSlot);
	if (m_ePlayerSlot != CUI_Define::PLAYER_SLOT::LPLAYER1 && m_ePlayerSlot != CUI_Define::PLAYER_SLOT::RPLAYER1)
	{
		m_bPlaying = false;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(5.f * m_iPlayerTeam, 100.f, 0.f, 1.f));

		//캐릭터 사이즈에 맞게 각자 추가하느라 m_pColliderCom이 없음
		m_pColliderCom->Update(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	}
	else
		m_bPlaying = true;

	return S_OK;
}

void CPlay_Hit::Player_Update(_float fTimeDelta)
{
	__super::Player_Update(fTimeDelta);


	m_pModelCom->m_fCurrentAnimPosition;

	Update_Tag_In(fTimeDelta);


	if (m_bPlaying == false)
		return;


	if (m_pGameInstance->Key_Down(DIK_TAB))
	{
		m_bDebugInputLock = !m_bDebugInputLock;
	}

	if (m_pGameInstance->Key_Down(DIK_F1))
	{
		m_bDebugInputLock = !m_bDebugInputLock;
	}

	if (m_bDebugInputLock)
		return;



	//cout << m_pModelCom->m_fCurrentAnimPosition << endl;





	//사망시 업데이트 처리.  return때문에
	if (m_bDying)
	{
		if (m_bAnimationLock == true)
			Update_AnimationLock(fTimeDelta);
		else
		{
			Character_Play_Animation(fTimeDelta);

			if (Get_fHeight() > 0)
				Update_StunImpus(fTimeDelta);

			Gravity(fTimeDelta);

			if (Check_bWall())
			{
				Move_ForWall();
			}
			_uint iAnimationIndex = m_pModelCom->m_iCurrentAnimationIndex;

			if (m_bMotionPlaying == false)
			{

				if (iAnimationIndex == m_iDyingStandingAnimationIndex || iAnimationIndex == m_iBound_Ground)
				{
					m_fAccDyingTime += fTimeDelta;
					if (m_fAccDyingTime > 2.f)
					{
						Tag_In(m_ePlayerSlot);
					}
				}

			}
			else if (iAnimationIndex == m_iDyingStandingAnimationIndex)
			{
				Stun_Shake();
			}
		}

		return;
	}
	else
		Update_Dying(fTimeDelta);



	Update_CounterPose(fTimeDelta);
	Update_214Pose(fTimeDelta);

	Update_LoofAnimationCreate(fTimeDelta);

	Update_PreviousXPosition();

	if (m_bGrabbed)
	{

		if (m_bAnimationLock && m_fMaxAnimationLock == 0.f)
		{
			m_bAnimationLock = false;
		}

		if (m_bGrabbedGravity && m_bAnimationLock == false)
		{
			if (Get_fHeight() > 0)
			{
				_float fGravity = (-0.7f * (2 * m_fGravityTime - m_fJumpPower) * (2 * m_fGravityTime - m_fJumpPower) + 4) * 0.1;
				//Add_Move({ 0,-fGravity });
				Add_Move({ m_fImpuse.x * fTimeDelta, -fGravity + m_fImpuse.y * fTimeDelta });
			}

			if (m_fGravityTime * 2.f < m_fJumpPower)
			{
				m_fGravityTime += fTimeDelta;

			}

		}
		if (m_bAnimationLock == false)
		{
			Character_Play_Animation(fTimeDelta);
		}
		else
		{
			Update_AnimationLock(fTimeDelta);
		}
		m_pColliderCom->Update(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		return;
	}


	Update_BeReflecting(fTimeDelta);

	pEnemyCheck();

	//방향전환 코드.  적 탐지가 추가된 이후엔  CCharacter로 옮기기
	if (Check_bCurAnimationisGroundMove() || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex)
	{
		CTransform* pEnemyTransform = static_cast<CTransform*>(m_pEnemy->Get_Component(TEXT("Com_Transform")));

		//적 방향의 X값 체크
		_float fX = XMVectorGetX(pEnemyTransform->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION));


		//차이가 좁으면 반전 안함. 둘 다 벽에 붙어있을 때 대비.
		if (fabsf(fX) > 0.05)
		{

			if (fX > 0)
			{
				FlipDirection(1);
			}
			else
			{
				FlipDirection(-1);
			}

		}
	}



	//입력되어있는 커맨드 시간에 따라 휘발
	InputedCommandUpdate(fTimeDelta);

	//Chase == true거나  Stun==ture면 안받음
	if (m_bChase == false && m_bStun == false)   //선입력을 받아야 하는가?
	{
		if (InputCommand())
		{
			CheckAllCommands();
		}

	}
	else   //입력 불가 상태여도 선입력받음.  커맨드 체크는 안하고 입력만 받음
	{
		InputCommand();
	}



	if (m_bAnimationLock == false)
	{

		Sparking_ON(fTimeDelta);

		//추적 관련 코드.
		if (m_iPlayerTeam == 1)
		{
			if (m_pGameInstance->Key_Down(DIK_R))
			{
				Chase_Ready(fTimeDelta);
			}
		}
		else  //Team2
		{
			if (m_pGameInstance->Key_Down(DIK_PGDN))  //PageDown 키
			{
				Chase_Ready(fTimeDelta);
			}
		}


		if (m_bChase)
		{
			Chase2(fTimeDelta);

			if (m_bChase == false)
			{
				//평범하게 체이스가 끝났으면 이렇게.
				//이쪽으로 접근 하지 않는다?
				if (m_bStun == false)
				{
					m_fAccChaseTime = 0.f;

					//상대의 높이가 높으면 더 위로
					//if (m_pModelCom->m_fCurrentAnimPosition > 60)
					//{
					//
					//	m_fGravityTime = 0.05f;
					//}
					//else
					//	m_fGravityTime = 0.185f;


					m_fGravityTime = 0.185;// -(m_pModelCom->m_fCurrentAnimPosition) * 0.01;


					Set_Animation(ANIME_JUMP_DOWN, false);
				}
				else //맞아서 체이스가 끝났으면 
				{
					m_fAccChaseTime = 0.f;
					m_fGravityTime = 0.185f;
				}

			}

		}
		else
		{
			if (m_bChaseStoping)
			{
				m_bChaseStoping = false;
				Set_Animation(m_iFallAnimationIndex);
			}
		}

		if (m_bGrab)
		{
			Chase_Grab(fTimeDelta);
			Character_Attack_Grab(fTimeDelta);
		}




		if (m_bNoEventLoofAnimation)
		{
			Update_NoEventTime(fTimeDelta);

			if (m_bForcedEventLoofAnimation)
			{
				Update_ForcedEventAnimationLoof(fTimeDelta);
			}
			else
			{
				Update_NoEventAnimationLoof(fTimeDelta);
			}
		}
		else
		{



			Character_Play_Animation(fTimeDelta);
			Update_214FinalInvisible();




		}
		//이건 반복재생이 아닌데 모션이 끝난경우 (=움직임 자체가 멈췄을 경우),  추락 등 몇몇 애니메이션 제외
		if (m_bMotionPlaying == false)
		{


			//스턴 추가 전에 있던거
			//if(m_pModelCom->m_iCurrentAnimationIndex != ANIME_JUMP_DOWN)
			//	AnimeEndNextMoveCheck();



			//if (Check_bCurAnimationisAirHit() || Check_bCurAnimationisHitAway() || m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_Spin_LeftUp)
			if (Check_bCurAnimationisAirHit() || Check_bCurAnimationisHitAway())
			{

				if (m_bHitGroundSmashed == false)
					Set_Animation(m_iHit_Air_FallAnimationIndex);

			}
			else if (m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_Spin_LeftUp)
			{
				Set_Animation(m_iHit_Air_LightAnimationIndex);
				m_pModelCom->CurrentAnimationPositionJump(30.f);
			}

			else if (m_pModelCom->m_iCurrentAnimationIndex == m_iHit_WallBouce)
			{
				Set_Animation(m_iHit_Air_FallAnimationIndex);
			}

			else if (m_bStun == false && m_pModelCom->m_iCurrentAnimationIndex != m_iFallAnimationIndex && m_pModelCom->m_iCurrentAnimationIndex != m_iHit_Air_LightAnimationIndex)
				AnimeEndNextMoveCheck();

		}

		//이거맞나?   m_bMotionPlaying 중인데 스턴은 끝났고 Hit Away 중인 경우
		else if (m_bMotionPlaying && Check_bCurAnimationisHitAway() && m_bStun == false)
		{
			Set_Animation(m_iHit_Air_FallAnimationIndex);

		}




		//애니메이션 lock 도중에는 발동 안해야함.   아래에 넣자니 
		if (m_bStun)
		{
			Stun_Shake();
			m_fAccStunTime += fTimeDelta;
			if (m_fAccStunTime > m_fMaxStunTime)
			{
				m_bStun = false;
				m_fAccStunTime = 0.f;

				if (Check_bCurAnimationisHitGround())
				{
					Reset_AttackStep();
				}

			}
			Update_StunImpus(fTimeDelta);
		}
		else
		{





			BreakFall_Air();


			if (m_pModelCom->m_iCurrentAnimationIndex == m_iIdleAnimationIndex && Get_fHeight() == 0)
				m_fImpuse = { 0.f,0.f };



		}
		Gravity(fTimeDelta);

		//여기로 옮기면 2단점프가 안됨
		//AttckCancleJump();
		//Move(fTimeDelta);
	}
	else
	{
		Update_AnimationLock(fTimeDelta);

		//m_fAccAnimationLock += fTimeDelta;
		//if (m_fAccAnimationLock > m_fMaxAnimationLock)
		//{
		//	m_fAccAnimationLock = 0.f;
		//	m_bAnimationLock = false;
		//}
	}



	//중력 처리.    ANimation Lock의 영향을 받아야하나? 위로 옮겨봄
	//Gravity(fTimeDelta);

	//일부 공격 캔슬
	AttckCancleJump();

	Sparking_TimeCount(fTimeDelta);


	//Animation Lock 상태에서도 움직이는 문제 있음

	if (m_bAnimationLock == false)
		Move(fTimeDelta);









	m_pColliderCom->Update(m_pTransformCom->Get_State(CTransform::STATE_POSITION));




	if (Check_bWall())
	{
		Move_ForWall();
	}


	Tag_KeyCheck();


	if (m_pGameInstance->Key_Down(DIK_8))
	{
		ShowInputBuffer();
		m_pModelCom->m_fCurrentAnimPosition;
		_float fHeight = Get_fHeight();
		_bool bDebugf = true;
	}
	if (m_pGameInstance->Key_Down(DIK_1))
	{
		if (m_pGameInstance->Key_Pressing(DIK_A))
		{
			DebugPositionReset(-1);
		}
		else if (m_pGameInstance->Key_Pressing(DIK_D))
		{
			DebugPositionReset(1);
		}
		else
			DebugPositionReset();

	}

	if (m_pGameInstance->Key_Down(DIK_2))
	{
		m_iAttackStepCount = 0;
		m_iDebugComoboDamage = 0;

		m_iHP = 10000;
	}
	if (m_pGameInstance->Key_Down(DIK_3))
	{
		system("cls");
	}

	if (m_pGameInstance->Key_Down(DIK_4))
	{
		Set_bFinalSkillQTE(true);
	}
	if (m_pGameInstance->Key_Down(DIK_INSERT))
	{
		CBattleInterface_Manager::Get_Instance()->Gain_KiGuage(100, m_iPlayerTeam);
	}

}

void CPlay_Hit::Camera_Update(_float fTimeDelta)
{

}

void CPlay_Hit::Update(_float fTimeDelta)
{
	__super::Player_Update(fTimeDelta);

}

void CPlay_Hit::Late_Update(_float fTimeDelta)
{
	if (m_bPlaying || m_bTag_In)
		m_pRenderInstance->Add_RenderObject(CRenderer::RG_PLAYER, this, &m_RendererDesc);


#ifdef _DEBUG
	m_pRenderInstance->Add_DebugComponent(m_pColliderCom);
#endif

}

HRESULT CPlay_Hit::Render(_float fTimeDelta)
{

	if (m_bInvisible == true)
		return S_OK;

	if (m_pModelCom->m_iCurrentAnimationIndex == ANIME_214_FINAL_SUCESS)
	{
		_float fHeight = Get_fHeight();

		if (fHeight > 0.5f)
			m_bInvisible = true;
		else
			m_bInvisible = false;
	}

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		/* 모델이 가지고 있는 머테리얼 중 i번째 메시가 사용해야하는 머테리얼구조체의 aiTextureType_DIFFUSE번째 텍스쳐를 */
		/* m_pShaderCom에 있는 g_DiffuseTexture변수에 던져. */
		if (m_iPlayerTeam == 1)
		{
			if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_p2PTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
				return E_FAIL;
		}
		//m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i);

	   /* 모델이 가지고 있는 뼈들 중에서 현재 렌더링할려고 했던 i번째ㅑ 메시가 사용하는 뼈들을 배열로 만들어서 쉐이더로 던져준다.  */
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pShaderCom->Begin(1)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;

	}




	//corlorChange Test
	//for (size_t i = 0; i < iNumMeshes; i++)
	//{
	//	/* 모델이 가지고 있는 머테리얼 중 i번째 메시가 사용해야하는 머테리얼구조체의 aiTextureType_DIFFUSE번째 텍스쳐를 */
	//	/* m_pShaderCom에 있는 g_DiffuseTexture변수에 던져. */
	//	if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, (aiTextureType)m_iPlayerTeam, "g_DiffuseTexture", i)))
	//		return E_FAIL;
	//	// m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i);
	//
	//	/* 모델이 가지고 있는 뼈들 중에서 현재 렌더링할려고 했던 i번째ㅑ 메시가 사용하는 뼈들을 배열로 만들어서 쉐이더로 던져준다.  */
	//	m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);
	//
	//	if (FAILED(m_pShaderCom->Begin(0)))
	//		return E_FAIL;
	//
	//	if (FAILED(m_pModelCom->Render(i)))
	//		return E_FAIL;
	//}
#ifdef _DEBUG
	m_pColliderCom->Render(fTimeDelta);
#endif // DEBUG

	return S_OK;
}

_bool CPlay_Hit::Update_CounterPose(_float fTimeDelta)
{

	if (m_bCounterPose == false)
		return false;


	//반격 성공시 초기화 하고 업데이트 종료
	if (m_bCounterSucces == true)
	{
		m_bCounterPose = false;
		m_fAccPoseTime = 0.f;
		return false;
	}

	m_fAccPoseTime += fTimeDelta;


	if (m_fAccPoseTime > 0.4f)
	{
		m_bCounterPose = false;
		m_fAccPoseTime = 0.f;

		if (m_pModelCom->m_iCurrentAnimationIndex == ANIME_ATTACK_SPECIAL)
			Set_CurrentAnimationPositionJump(64.999f);

		else if (m_pModelCom->m_iCurrentAnimationIndex == ANIME_ATTACK_SPECIAL_AIR)
			Set_CurrentAnimationPositionJump(49.999f);


	}

	return true;
}

_bool CPlay_Hit::Update_214Pose(_float fTimeDelta)
{
	if (m_b214Posing == false)
		return false;


	_bool bKeyPressing = false;


	//자세 유지중인지 확인
	if (m_iPlayerTeam == 1)
	{
		bKeyPressing = m_pGameInstance->Key_Pressing(DIK_U);
	}
	else if (m_iPlayerTeam == 2)
	{
		bKeyPressing = m_pGameInstance->Key_Pressing(DIK_NUMPAD7);
	}

	//유지중이면 시간더하고 이펙트처리
	if (bKeyPressing == true)
	{

		m_fAccPoseTime += fTimeDelta;
		//이펙트처리도?

	}

	else
	{
		//자세를 시작했으면 최소한 0.2초는 유지해야함 강제로 유지처리
		if (m_fAccPoseTime < 0.2f)
		{
			bKeyPressing = true;
			m_fAccPoseTime += fTimeDelta;
		}
		else
		{
			AttackEvent(400);
			m_b214Posing = false;
		}

	}

}

_bool CPlay_Hit::Get_b236Posing()
{
	return  m_b236Posing;

}

void CPlay_Hit::Set_b236Posing(_bool b236Posing)
{
	m_b236Posing = b236Posing;

}

_bool CPlay_Hit::Get_b236Sepcial()
{
	return m_b236Special;
}

void CPlay_Hit::Set_b236Special(_bool b236Special)
{
	m_b236Special = b236Special;
}




//공용 처리 가능해서 Character로 이사가고 백업.
/*
void CPlay_Hit::AttackNextMoveCheck()
{

	if (m_iNextAnimation.first != m_iIdleAnimationIndex)
	{
		Set_Animation(m_iNextAnimation.first);

		m_iNextAnimation.first = m_iIdleAnimationIndex;
		m_iNextAnimation.second = 1000.f;

		if (m_fNextAnimationCurrentPosition != 0)
		{
			m_pModelCom->CurrentAnimationPositionJump(m_fNextAnimationCurrentPosition);
			m_fNextAnimationCurrentPosition = 0.f;
		}
	}


}

void CPlay_Hit::AnimeEndNextMoveCheck()
{

	Set_Animation(m_iNextAnimation.first);

	m_iNextAnimation.first = ANIME_IDLE;
	m_iNextAnimation.second = 1000.f;


}



void CPlay_Hit::Set_Animation(_uint iAnimationIndex, _bool bloof)
{

	if (iAnimationIndex == m_iIdleAnimationIndex)
		m_pModelCom->SetUp_Animation(iAnimationIndex, true);
	else
		m_pModelCom->SetUp_Animation(iAnimationIndex, bloof);

}

*/




HRESULT CPlay_Hit::Ready_Components()
{
	/* Com_Shader */
	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
	//	TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	//	return E_FAIL;
	//
	///* Com_Model */
	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
	//	return E_FAIL;


	CBounding_AABB::BOUNDING_AABB_DESC ColliderDesc{};
	if (m_iPlayerTeam == 1)
		ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_BODY;
	else
		ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_BODY;
	ColliderDesc.pMineGameObject = this;
	ColliderDesc.vCenter = { 0.f,0.8f,0.f };
	ColliderDesc.vExtents = { 0.5f,0.7f,1.f };


	//Com_Collider
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	m_pGameInstance->Add_ColliderObject(ColliderDesc.colliderGroup, m_pColliderCom);


	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Play_Hit"), TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;



	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_HITOutLine"), TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pOutLineCom))))
		return E_FAIL;


	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_HIT_2P"), TEXT("Com_2PTexture"), reinterpret_cast<CComponent**>(&m_p2PTextureCom))))
		return E_FAIL;




	return S_OK;
}

HRESULT CPlay_Hit::Bind_ShaderResources()
{

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

_bool CPlay_Hit::Check_bCurAnimationisGroundMove(_uint iAnimation)
{
	_uint iModelIndex = iAnimation;

	if (iAnimation == 1000)
		iModelIndex = m_pModelCom->m_iCurrentAnimationIndex;



	if (iModelIndex == ANIME_IDLE || iModelIndex == ANIME_FORWARD_WALK || iModelIndex == ANIME_BACK_WALK || iModelIndex == ANIME_CROUCH_START || iModelIndex == ANIME_CROUCHING
		|| iModelIndex == ANIME_FORWARD_DASH)
	{
		return true;
	}

	return false;
}

_bool CPlay_Hit::Check_bCurAnimationisAttack(_uint iAnimation)
{

	return false;
}

_bool CPlay_Hit::Check_bCurAnimationisAirAttack(_uint iAnimation)
{
	_uint iModelIndex = iAnimation;

	if (iAnimation == 1000)
		iModelIndex = m_pModelCom->m_iCurrentAnimationIndex;



	if (iModelIndex == ANIME_ATTACK_AIR1 || iModelIndex == ANIME_ATTACK_AIR2 || iModelIndex == ANIME_ATTACK_AIR3 || iModelIndex == ANIME_ATTACK_SPECIAL_AIR)
	{
		return true;
	}

	return false;
}

_bool CPlay_Hit::Check_bCurAnimationisHalfGravityStop(_uint iAnimation)
{
	_uint iModelIndex = iAnimation;

	if (iAnimation == 1000)
		iModelIndex = m_pModelCom->m_iCurrentAnimationIndex;



	if (iModelIndex == ANIME_ATTACK_SPECIAL_AIR)
	{
		return true;
	}

	return false;
}

_short CPlay_Hit::Check_bCurAnimationisCanChase()
{
	_uint 	iModelIndex = m_pModelCom->m_iCurrentAnimationIndex;



	//공격중 체이스는 속도 좀 빠르게
	//if(m_bAttackBackEvent && (iModelIndex == ANIME_ATTACK_HEAVY || iModelIndex == ANIME_ATTACK_CROUCH_HEAVY || iModelIndex == ANIME_ATTACK_SPECIAL_AIR || iModelIndex == ANIME_ATTACK_LIGHT3))
	//if (m_bAttackBackEvent && (iModelIndex == ANIME_ATTACK_HEAVY || iModelIndex == ANIME_ATTACK_CROUCH_HEAVY ))
	//{
	//	return 10;
	//}

	if (m_bAttackBackEvent && (iModelIndex == ANIME_ATTACK_HEAVY || iModelIndex == ANIME_ATTACK_CROUCH_HEAVY))
	{
		if (m_b236Special)
			return 0;

		return 10;
	}


	//m_b236Special


	else if (Check_bCurAnimationisGroundMove() || Check_bCurAnimationisAirMove() || iModelIndex == ANIME_ATTACK_LIGHT3)
	{
		return 1;
	}


	return 0;

}

void CPlay_Hit::Reset_AttackCount()
{

	__super::Reset_AttackCount();

	for (size_t i = 0; i < COUNT_END; i++)
		m_bAttackCount[i] = true;


}

void CPlay_Hit::Gravity(_float fTimeDelta)
{
	if (m_pModelCom->m_iCurrentAnimationIndex == ANIME_ATTACK_SPECIAL_AIR)
	{
		return;
	}


	__super::Gravity(fTimeDelta);


	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float fHeight = XMVectorGetY(vPos);
	_ushort iAnimationIndex = m_pModelCom->m_iCurrentAnimationIndex;



	if (iAnimationIndex == ANIME_ATTACK_AIR1 || iAnimationIndex == ANIME_ATTACK_AIR2 || iAnimationIndex == ANIME_ATTACK_AIR3)
	{

		if (fHeight == 0)
		{

			//m_pModelCom->SetUp_Animation(m_iIdleAnimationIndex, true);
			Set_Animation(m_iIdleAnimationIndex);


			Set_fGravityTime(0.f);
			Set_fJumpPower(0.f);
			Set_fImpulse(0.f);
			m_bAriDashEnable = true;
			Set_bAttackGravity(true);
		}

	}



}

void CPlay_Hit::AttackEvent(_int iAttackEvent, _int AddEvent)
{
	//case Client::CPlay_Hit::ANIME_236_SPECIAL_RIGHT:
	//	break;
	//case Client::CPlay_Hit::ANIME_236_SPECIAL_UP:
	//	break;
	//case Client::CPlay_Hit::ANIME_236_SPECIAL_DOWN:
	//	break;

	switch (m_pModelCom->m_iCurrentAnimationIndex)
	{
	case Client::CPlay_Hit::ANIME_ATTACK_LIGHT1:
	{

		CAttackObject::ATTACK_DESC Desc{};

		if (m_iPlayerTeam == 1)
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
		else
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
		Desc.ColliderDesc.pMineGameObject = this;
		Desc.ColliderDesc.vCenter = { 0.9f * m_iLookDirection,0.8f,0.f };
		Desc.ColliderDesc.vExtents = { 0.3f,0.5f,0.2f };



		Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };
		Desc.fhitCharacter_StunTime = 0.4f;
		Desc.iDamage = 400 * Get_DamageScale();
		Desc.fLifeTime = 0.1f;
		Desc.ihitCharacter_Motion = { HitMotion::HIT_LIGHT };
		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.1f;
		Desc.pOwner = this;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}
	break;

	case Client::CPlay_Hit::ANIME_ATTACK_LIGHT2:
	{
		//공격 전 안보임 + 샤샤샥 + 뒤쪽에 구름
		if (iAttackEvent == 0)
		{

			//안보이게,
			m_bInvisible = true;

			//샤샤샥 이펙트,

			//대시구름
			m_pEffect_Manager->Copy_Layer(TEXT("Smoke01"), m_pTransformCom->Get_WorldMatrixPtr());

			//m_pEffect_Manager->Copy_Layer(TEXT("Smoke01_BackZ"), m_pTransformCom->Get_WorldMatrixPtr());
			//m_pEffect_Manager->Copy_Layer(TEXT("Smoke02"), m_pTransformCom->Get_WorldMatrixPtr());
			//m_pEffect_Manager->Copy_Layer(TEXT("Smoke02_Small"), m_pTransformCom->Get_WorldMatrixPtr());
			//m_pEffect_Manager->Copy_Layer(TEXT("Smoke04"), m_pTransformCom->Get_WorldMatrixPtr());


			Character_Make_Effect(TEXT("Moving_Line_Right"));

			//CEffect_Layer* pEffect = m_pEffect_Manager->Get_Instance()->Copy_Layer_AndGet(TEXT("Moving_Line_Right"), m_pTransformCom->Get_WorldMatrixPtr());


			CMain_Camera* main_Camera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")));
			main_Camera->StartCameraShake(0.1f, 0.3f);

		}
		else if (iAttackEvent == 1)
		{


			CMain_Camera* main_Camera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")));
			main_Camera->StartCameraShake(0.1f, 0.3f);

			//적까지의 거리를 탐색하고 사정거리 안에있으면 그 앞까지, 그보다 멀면 사정거리까지만 이동하는 함수 필요
			MoveToEnemy_Ground(2.f);
			m_bInvisible = false;

			Character_Make_Effect(TEXT("Moving_Line_Right"));

			//CEffect_Layer* pEffect = m_pEffect_Manager->Get_Instance()->Copy_Layer_AndGet(TEXT("Moving_Line_Right"),m_pTransformCom->Get_WorldMatrixPtr());

			//공격 생성

			CAttackObject::ATTACK_DESC Desc{};

			if (m_iPlayerTeam == 1)
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
			else
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
			Desc.ColliderDesc.pMineGameObject = this;
			Desc.ColliderDesc.vCenter = { 0.9f * m_iLookDirection,0.8f,0.f };
			Desc.ColliderDesc.vExtents = { 0.3f,0.5f,0.2f };



			Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };
			Desc.fhitCharacter_StunTime = 0.4f;


			Desc.iDamage = 700 * Get_DamageScale();
			//Desc.iDamage = 4200 * Get_DamageScale();
			Desc.fLifeTime = 0.1f;
			Desc.ihitCharacter_Motion = { HitMotion::HIT_LIGHT };
			Desc.iTeam = m_iPlayerTeam;
			Desc.fAnimationLockTime = 0.1f;
			Desc.pOwner = this;

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);


			//m_pEffect_Manager->Copy_Layer(TEXT("Smoke03_Stop"), m_pTransformCom->Get_WorldMatrixPtr());
			Character_Make_Effect(TEXT("Smoke03_Stop"), {}, true);
		}





	}
	break;
	case Client::CPlay_Hit::ANIME_ATTACK_LIGHT3:
	{

		// 순간적으로 안보임, 가로샤샤샥
		if (iAttackEvent == 0)
		{

			//안보이게,
			m_bInvisible = true;

			//샤샤샥 이펙트,
			Character_Make_Effect(TEXT("Moving_Line_Right"));

			//CEffect_Layer* pEffect = m_pEffect_Manager->Get_Instance()->Copy_Layer_AndGet(TEXT("Moving_Line_Right"), m_pTransformCom->Get_WorldMatrixPtr());
			//if (pEffect != nullptr)
			//	pEffect->Set_Copy_Layer_Scaled({ 1.f,0.3f,1.f });

			//대시구름
			m_pEffect_Manager->Copy_Layer(TEXT("Smoke01"), m_pTransformCom->Get_WorldMatrixPtr());

			if (m_bSparking)
			{
				m_iAttackLightLoofCount = 4;
				//m_pModelCom->Set_MaxAnimationUpdate_Time(fValue[0]);
				m_pModelCom->Get_pCurrentAnimation()->m_fTickPerSecond = 240.f;
			}
			else
				m_iAttackLightLoofCount = 2;


		}


		//안보이고있었다면 보이게, 적 앞으로 일정거리 순간이동,  가로샤샤샥  ,공격판정, Grab판정하고? 경직 심하게주기, 또는 그냥 강하게?
		else if (iAttackEvent == 1)
		{
			m_bInvisible = false;


			//루프돌고있으면 방향전환
			if (m_bAttackBackEvent == true)
			{
				FlipDirection();
			}

			MoveToEnemy_Ground(3.f);

			//샤샤샥 이펙트,
			Character_Make_Effect(TEXT("Moving_Line_Right"));

			//CEffect_Layer* pEffect = m_pEffect_Manager->Get_Instance()->Copy_Layer_AndGet(TEXT("Moving_Line_Right"), m_pTransformCom->Get_WorldMatrixPtr());
			//
			//if (pEffect != nullptr)
			//	pEffect->Set_Copy_Layer_Scaled({ 1.f,0.3f,1.f });

			if (m_iAttackLightLoofCount != 0)
			{
				CAttackObject_CommandGrab::ATTACK_COMMANDGRAB_DESC Desc{};
				if (m_iPlayerTeam == 1)
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
				else
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
				Desc.ColliderDesc.pMineGameObject = this;
				//Desc.ColliderDesc.vExtents = { 1.4f,2.2f,1.f };
				Desc.ColliderDesc.vExtents = { 0.8f,0.6f,1.f };
				Desc.ColliderDesc.vCenter = { 0.3f,0.7f,0.f };
				//Desc.ColliderDesc.pTransform = m_pTransformCom;
				//Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };
				Desc.fhitCharacter_StunTime = 20.f;
				Desc.iDamage = 320 * Get_DamageScale(true);;
				Desc.fLifeTime = 0.2f;
				Desc.ihitCharacter_Motion = { HitMotion::HIT_LIGHT };
				Desc.iTeam = m_iPlayerTeam;
				Desc.fAnimationLockTime = 0.1f;
				Desc.pOwner = this;

				Desc.eAttackType = ATTACKTYPE_HIGH;
				//Desc.eAttackGrade = GRADE_ULTIMATE;

				Desc.fDistance = { 0.8f * m_iLookDirection,0.f };
				Desc.fForcedGravityTime = 0.f;
				//Desc.fGrabAnimationPosition = 40.f;
				//Desc.fGrabAnimationPosition = 25.f;
				Desc.iGainAttackStep = 0;
				Desc.iGrabAnimationIndex = ANIME_ATTACK_LIGHT3;
				Desc.bOwnerNextAnimation = false;


				//Desc.iVirtualCameraindex = CMain_Camera::VIRTUAL_CAMERA_SON_ULTIMATE;
				//Desc.ianimationIndex = 1;
				Desc.fCameraShakeDuration = 0.5f;
				Desc.fCameraShakeMagnitude = 0.2f;


				Desc.fForcedGravityTime = 0.f;

				//Desc.bOnwerHitNoneStop = true;


				Desc.bOnwerHitNoneStop = true;
				Desc.bOwnerNextAnimation = false;
				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack_CommandGrab"), TEXT("Layer_AttackObject"), &Desc);

			}

			else if (m_iAttackLightLoofCount == 0)
			{
				CAttackObject::ATTACK_DESC Desc{};

				if (m_iPlayerTeam == 1)
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
				else
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
				Desc.ColliderDesc.pMineGameObject = this;
				Desc.ColliderDesc.vCenter = { 0.9f * m_iLookDirection,0.8f,0.f };
				Desc.ColliderDesc.vExtents = { 0.3f,0.5f,0.2f };



				Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };
				Desc.fhitCharacter_StunTime = 5.f;


				//if (m_iAttackLightLoofCount == 0)
				{
					Desc.iDamage = 450 * Get_DamageScale();
					Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_LEFT };
					Desc.iGainAttackStep = 1;
					Desc.fhitCharacter_Impus = { 20.f * m_iLookDirection,0 };


				}
				//else
				//{
				//	Desc.iDamage = 180 * Get_DamageScale();
				//	Desc.ihitCharacter_Motion = { HitMotion::HIT_LIGHT };
				//	Desc.iGainAttackStep = 0;
				//
				//	Desc.bOnwerHitNoneStop = true;
				//}

				Desc.fLifeTime = 0.1f;



				Desc.iTeam = m_iPlayerTeam;
				Desc.fAnimationLockTime = 0.4f;
				Desc.pOwner = this;
				Desc.bGrabbedEnd = true;


				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);


				//m_pEffect_Manager->Copy_Layer(TEXT("Smoke03_Stop"), m_pTransformCom->Get_WorldMatrixPtr());

			}

		}

		//막혔으면 55로 이동하고 끝내기.  35는 옷이 뚫림
		else if (iAttackEvent == 2)
		{
			if (m_bAttackBackEvent == false)
			{
				//막히면 감속  무한압박 방지용
				m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 40.f;
				Set_CurrentAnimationPositionJump(55.f);


			}
			else if (m_iAttackLightLoofCount == 0)
			{
				Set_CurrentAnimationPositionJump(55.f);
			}
			else
			{
				;
			}

		}

		//,루프타이밍.  안보이게, 달리는척 하다가 적 뒤로이동,  가로샤샤샥, 24로 이동해서 루프돌기. 너무빠르니 20? 어택경직있으니 ㄱㅊ
		else if (iAttackEvent == 3)
		{
			m_bInvisible = true;
			Character_Make_Effect(TEXT("Moving_Line_Right"));

			//CEffect_Layer* pEffect = m_pEffect_Manager->Get_Instance()->Copy_Layer_AndGet(TEXT("Moving_Line_Right"), m_pTransformCom->Get_WorldMatrixPtr());
			//
			//if (pEffect != nullptr)
			//	pEffect->Set_Copy_Layer_Scaled({ 1.f,0.1f,1.f });


			Add_Move({ m_iLookDirection * 3.f,0.f });

			Set_CurrentAnimationPositionJump(24.f);

			m_iAttackLightLoofCount--;

			//적 뒤로 이동을 뭘로 구현하느냐
			//1. Grab판정으로 만들고 애니메이션으로 이동하기
			//2. Add_Move로 순간이동
		}


	}
	break;
	case Client::CPlay_Hit::ANIME_ATTACK_MEDIUM:
	{
		//,안보이게, 가로샤샤샥
		if (iAttackEvent == 0)
		{
			m_bInvisible = true;
			Character_Make_Effect(TEXT("Moving_Line_Right"));

			CMain_Camera* main_Camera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")));
			main_Camera->StartCameraShake(0.1f, 0.3f);
		}

		//,적 앞으로 순간이동, 공격판정.  안보이고있었다면 보이게?
		else if (iAttackEvent == 1)
		{
			CMain_Camera* main_Camera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")));
			main_Camera->StartCameraShake(0.1f, 0.1f);

			m_bInvisible = false;

			MoveToEnemy_Ground(2.f);
			Character_Make_Effect(TEXT("Moving_Line_Right"));


			CAttackObject::ATTACK_DESC Desc{};
			//Desc.ColliderDesc.width = 0.7;
			//Desc.ColliderDesc.height = 0.8;
			if (m_iPlayerTeam == 1)
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
			else
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
			Desc.ColliderDesc.pMineGameObject = this;
			Desc.ColliderDesc.vExtents = { 0.3f,0.2f,1.f };
			Desc.ColliderDesc.vCenter = { 0.9f * m_iLookDirection,0.9f,0.f };
			Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };
			Desc.fhitCharacter_StunTime = 0.4f;
			//Desc.fhitCharacter_StunTime = 0.55f;

			Desc.iDamage = 700 * Get_DamageScale();;
			Desc.fLifeTime = 0.2f;
			Desc.ihitCharacter_Motion = { HitMotion::HIT_MEDIUM };
			Desc.iTeam = m_iPlayerTeam;
			Desc.fAnimationLockTime = 0.1f;
			Desc.pOwner = this;

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
		}
	}
	break;
	case Client::CPlay_Hit::ANIME_ATTACK_HEAVY:
	{


		//,안보임 + 가로샤샤샥
		if (iAttackEvent == 0)
		{
			m_bInvisible = true;

			Character_Make_Effect(TEXT("Moving_Line_Right"));

			CMain_Camera* main_Camera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")));
			main_Camera->StartCameraShake(0.1f, 0.3f);
		}

		else if (iAttackEvent == 1)
		{

			m_bInvisible = false;

			MoveToEnemy_Ground(3.f);
			Character_Make_Effect(TEXT("Moving_Line_Right"));

			CMain_Camera* main_Camera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")));
			main_Camera->StartCameraShake(0.1f, 0.3f);

			{
				CAttackObject_CommandGrab::ATTACK_COMMANDGRAB_DESC Desc{};

				if (m_iPlayerTeam == 1)
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
				else
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
				Desc.ColliderDesc.pMineGameObject = this;
				Desc.ColliderDesc.vExtents = { 0.2f,1.f,1.f };
				Desc.ColliderDesc.vCenter = { 0.8f * m_iLookDirection,0.8f,0.f };

				Desc.fhitCharacter_Impus = { 20.f * m_iLookDirection,0.5f };
				Desc.fhitCharacter_StunTime = 1.0f;
				Desc.iDamage = 0;
				Desc.fLifeTime = 0.2f;
				Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_LEFT };
				Desc.iTeam = m_iPlayerTeam;
				Desc.fAnimationLockTime = 0.7f;

				Desc.fDistance = { 0.8f,0.05f };
				Desc.iGrabAnimationIndex = CPlay_Hit::ANIME_ATTACK_HEAVY;
				Desc.bOwnerNextAnimation = false;

				Desc.iGainAttackStep = 0;
				Desc.iGainHitCount = 0;
				Desc.iGainKiAmount = 0;

				Desc.bGrabbedEnd = true;
				Desc.pOwner = this;
				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack_CommandGrab"), TEXT("Layer_AttackObject"), &Desc);
			}

			if (false)
			{
				CAttackObject::ATTACK_DESC Desc{};

				if (m_iPlayerTeam == 1)
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
				else
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
				Desc.ColliderDesc.pMineGameObject = this;
				Desc.ColliderDesc.vExtents = { 0.2f,1.f,1.f };
				Desc.ColliderDesc.vCenter = { 0.8f * m_iLookDirection,0.8f,0.f };

				Desc.fhitCharacter_Impus = { 20.f * m_iLookDirection,0 };
				Desc.fhitCharacter_StunTime = 1.0f;
				Desc.iDamage = 1000 * Get_DamageScale();;
				Desc.fLifeTime = 0.2f;
				Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_LEFT };
				Desc.iTeam = m_iPlayerTeam;
				Desc.fAnimationLockTime = 0.7f;
				Desc.bGrabbedEnd = true;
				Desc.pOwner = this;
				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
			}

			Character_Make_Effect(TEXT("Ring_Dust_Right"), { 0.7f,0.f });
		}
		//Character_Make_Effect(TEXT("Ring_Dust_Right"), { 0.4f,0.f });



		//Ring_Dust

	}
	break;
	case Client::CPlay_Hit::ANIME_ATTACK_SPECIAL:
	{

		//반격 자세 시작
		if (iAttackEvent == 0)
		{
			m_bCounterPose = true;
		}

		//반격성공. 적 멈춘상태
		else if (iAttackEvent == 1)
		{
			m_bCounterPose = false;
			m_bCounterSucces = false;
			m_bInvisible = false;

			_vector vTargetPos = static_cast<CTransform*>(m_pEnemy->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
			vTargetPos += {1.3f * m_iLookDirection, 0.f, 0, 0};
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTargetPos);

			FlipDirection();

			Character_Make_Effect(TEXT("Moving_Line_Right"));

			CMain_Camera* main_Camera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")));
			main_Camera->StartCameraShake(0.1f, 0.3f);


			//적이 땅에 있으면 반격
			if (XMVectorGetY(vTargetPos) == 0)
			{
				CAttackObject::ATTACK_DESC Desc{};

				if (m_iPlayerTeam == 1)
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
				else
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
				Desc.ColliderDesc.pMineGameObject = this;
				Desc.ColliderDesc.vExtents = { 0.2f,1.f,1.f };
				Desc.ColliderDesc.vCenter = { 0.8f * m_iLookDirection,0.8f,0.f };

				Desc.fhitCharacter_Impus = { 20.f * m_iLookDirection,0 };
				Desc.fhitCharacter_StunTime = 1.0f;
				Desc.iDamage = 1000 * Get_DamageScale();;
				Desc.fLifeTime = 0.2f;
				Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_LEFT };
				Desc.iTeam = m_iPlayerTeam;
				Desc.fAnimationLockTime = 0.7f;
				Desc.bGrabbedEnd = true;
				Desc.pOwner = this;
				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
			}

			//적이 공중에 있으면 점프로 이행 
			else
			{
				Set_Animation(m_iJumpAnimationIndex);
				Set_ForcedGravityTime_LittleUp();
			}

		}


		//지상 반격 성공 후 돌아오는 모션
		else if (iAttackEvent == 2)
		{
			m_bCounterPose = false;
			Set_Animation(ANIME_IDLE);

		}

	}
	break;
	case Client::CPlay_Hit::ANIME_ATTACK_CROUCH_LIGHT:
	{

		if (iAttackEvent == 301)
		{
			if (m_b236Special)
			{
				//120도 느리다?
				//m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 120.f;

				m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 180.f;

			}

		}

		else if (iAttackEvent == 0)
		{
			if (m_b236Special == false)
			{
				CAttackObject::ATTACK_DESC Desc{};

				if (m_iPlayerTeam == 1)
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
				else
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
				Desc.ColliderDesc.pMineGameObject = this;
				Desc.ColliderDesc.vCenter = { 0.9f * m_iLookDirection,0.8f,0.f };
				Desc.ColliderDesc.vExtents = { 0.3f,0.5f,0.2f };



				//Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };
				Desc.fhitCharacter_Impus = { 0.1f * m_iLookDirection,-10.f };

				Desc.fhitCharacter_StunTime = 0.3f;
				Desc.iDamage = 400 * Get_DamageScale();
				Desc.fLifeTime = 0.1f;
				Desc.ihitCharacter_Motion = { HitMotion::HIT_HEAVY_DOWN };//HIT_KNOCK_AWAY_LEFTDOWN //
				Desc.iTeam = m_iPlayerTeam;

				if (m_pEnemy->Get_fHeight() != 0)
					Desc.fAnimationLockTime = 0.25f;
				else
					Desc.fAnimationLockTime = 0.1f;

				Desc.pOwner = this;
				Desc.bOnwerHitNoneStop = true;

				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
			}
			else
			{
				//원거리 공격의 경우 손에 파란이팩트는 동일, 원거리 공격판정
				CAttackObject::ATTACK_DESC Desc{};

				if (m_iPlayerTeam == 1)
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
				else
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
				Desc.ColliderDesc.pMineGameObject = this;
				Desc.ColliderDesc.vExtents = { 0.2f,1.f,1.f };


				//center를 상대 좌표로 할게 아니라 나~상대 까지의 거리로 해야함

				_vector vEnemyPos = m_pEnemy->Get_vPosition();
				_vector vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

				_vector vLength = vEnemyPos - vMyPos;

				Desc.ColliderDesc.vCenter = { XMVectorGetX(vLength),XMVectorGetY(vLength),0.f };


				Desc.fhitCharacter_StunTime = 0.4f;
				Desc.iDamage = 700 * Get_DamageScale();;
				Desc.fLifeTime = 0.2f;
				//Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_UP_GRAVITY };
				//Desc.fhitCharacter_Impus = { 0.5f * m_iLookDirection,8.f };

				//Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_LEFTDOWN };
				Desc.ihitCharacter_Motion = { HitMotion::HIT_HEAVY_DOWN };


				Desc.fhitCharacter_Impus = { 3.f * m_iLookDirection,-20.f };

				Desc.iTeam = m_iPlayerTeam;
				Desc.fAnimationLockTime = 0.1f;
				Desc.bGrabbedEnd = true;
				Desc.pOwner = this;
				Desc.bCameraZoom = false;
				Desc.bOnwerHitNoneStop = true;

				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);

				//m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 40.f;

			}
		}

		else if (iAttackEvent == 101)
		{
			if (m_b236Special == false)
				AttackNextMoveCheck();
		}

		else if (iAttackEvent == 102)
		{
			if (m_b236Special == false)
				AttackNextMoveCheck();
		}
		else if (iAttackEvent == 103)
		{
			if (m_b236Special == false)
				AttackNextMoveCheck();
		}

		else if (iAttackEvent == 201)
		{
			//if (m_b236Special == false)
			//	Set_Animation(ANIME_IDLE);
			//else
			//{
			//	m_b236Special = false;
			//}

			m_b236Special = false;
			Set_Animation(ANIME_IDLE);

		}

	}
	break;
	case Client::CPlay_Hit::ANIME_ATTACK_CROUCH_MEDUIM:
	{
		if (iAttackEvent == 0)
		{
			//너무 크고 z도 안맞음
			//m_pEffect_Manager->Copy_Layer(TEXT("Smoke01_BackZ"), m_pTransformCom->Get_WorldMatrixPtr());
			//Character_Make_Effect(TEXT("Smoke01"), {0.3f,0.f}, true);

			//하나 잘린거 티남, 타이밍 이상함
			//Character_Make_Effect(TEXT("Smoke_Run"), { 0.25f,0.f },true);
			// 
			//뒷받침으로 쓰고있었는데 공격쪽으로 옮김
			//Character_Make_Effect(TEXT("Smoke03_Stop"),{0.f,0.f},true);


			Character_Make_Effect(TEXT("Smoke03_Stop"), { 0.25f,0.f });

			CAttackObject::ATTACK_DESC Desc{};
			if (m_iPlayerTeam == 1)
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
			else
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
			Desc.ColliderDesc.pMineGameObject = this;
			Desc.ColliderDesc.vCenter = { 0.9f * m_iLookDirection,0.8f,0.f };


			Desc.ColliderDesc.vExtents = { 0.7f,0.5f,1.f };
			Desc.ColliderDesc.vCenter = { 0.7f * m_iLookDirection,0.3f,0.f };

			Desc.fhitCharacter_StunTime = 1.f;
			Desc.iDamage = 700 * Get_DamageScale();
			Desc.fLifeTime = 0.1f;
			Desc.ihitCharacter_Motion = { HitMotion::HIT_CROUCH_MEDIUM };
			Desc.iTeam = m_iPlayerTeam;
			Desc.fAnimationLockTime = 0.13f;
			Desc.pOwner = this;

			Desc.eAttackType = { ATTACKTYPE_LOW };

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);


		}

	}
	break;
	case Client::CPlay_Hit::ANIME_ATTACK_CROUCH_HEAVY:
	{
		//어퍼의 경우 속도 60으로 조절. 
		//236특의 경우 빠른 속도 유지
		if (iAttackEvent == 0)
		{
			if (m_b236Special == false)
			{
				//m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 60.f;
				m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 90.f;

			}
			else
			{

			}
		}


		else if (iAttackEvent == 1)
		{
			if (m_b236Special == false)
			{
				CAttackObject::ATTACK_DESC Desc{};
				//Desc.ColliderDesc.width = 2.0;
				//Desc.ColliderDesc.height = 1.3;
				//Desc.ColliderDesc.vCenter = { 0.4f * m_iLookDirection,1.2f,0.f };
				//Desc.ColliderDesc.width = 1.5;
				//Desc.ColliderDesc.height = 1.3;
				if (m_iPlayerTeam == 1)
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
				else
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
				Desc.ColliderDesc.pMineGameObject = this;
				Desc.ColliderDesc.vExtents = { 1.5f,1.3f,1.f };
				Desc.ColliderDesc.vCenter = { 0.2f * m_iLookDirection,1.2f,0.f };


				//Desc.ColliderDesc.pTransform = m_pTransformCom;
				//Desc.fhitCharacter_Impus = { 0.2f * m_iLookDirection, 10.f };  //원래 가속도.
				Desc.fhitCharacter_Impus = { 0.6f * m_iLookDirection, 10.f };  //원래 가속도.

				//Desc.fhitCharacter_Impus = { 0.2f * m_iLookDirection, 3.f };  //공중 테스트용 임시

				Desc.fhitCharacter_StunTime = 1.0f;
				Desc.iDamage = 1000 * Get_DamageScale();;
				Desc.fLifeTime = 0.3f;
				Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_UP };
				Desc.iTeam = m_iPlayerTeam;
				Desc.fAnimationLockTime = 0.5f;
				Desc.pOwner = this;
				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);


				//Character_Make_Effect(TEXT("Ring_Dust"), { m_iLookDirection * 0.7f,0.5f });
				//Character_Make_Effect(TEXT("Ring_Dust"), { m_iLookDirection * 0.4f,0.9f });
				Character_Make_Effect(TEXT("Ring_Dust"), { 0.4f,0.9f });
			}
			else
			{
				//원거리 공격의 경우 손에 파란이팩트는 동일, 원거리 공격판정
				CAttackObject::ATTACK_DESC Desc{};

				if (m_iPlayerTeam == 1)
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
				else
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
				Desc.ColliderDesc.pMineGameObject = this;
				Desc.ColliderDesc.vExtents = { 0.2f,1.f,1.f };


				//center를 상대 좌표로 할게 아니라 나~상대 까지의 거리로 해야함

				_vector vEnemyPos = m_pEnemy->Get_vPosition();
				_vector vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

				_vector vLength = vEnemyPos - vMyPos;

				Desc.ColliderDesc.vCenter = { XMVectorGetX(vLength),XMVectorGetY(vLength),0.f };



				Desc.fhitCharacter_StunTime = 0.4f;
				Desc.iDamage = 700 * Get_DamageScale();;
				Desc.fLifeTime = 0.2f;
				//Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_UP_GRAVITY };
				//Desc.fhitCharacter_Impus = { 0.5f * m_iLookDirection,8.f };

				Desc.ihitCharacter_Motion = { HitMotion::HIT_SPIN_AWAY_LEFTUP };
				Desc.fhitCharacter_Impus = { 0.5f * m_iLookDirection,2.f };

				Desc.iTeam = m_iPlayerTeam;
				Desc.fAnimationLockTime = 0.1f;
				Desc.bGrabbedEnd = true;
				Desc.pOwner = this;
				Desc.bCameraZoom = false;
				Desc.bOnwerHitNoneStop = true;

				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);

				m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 40.f;
			}
		}

		else if (iAttackEvent == 3)
		{

			m_b236Special = false;

		}


	}
	break;
	case Client::CPlay_Hit::ANIME_ATTACK_AIR1:
	{

		CAttackObject::ATTACK_DESC Desc{};
		//Desc.ColliderDesc.width = 0.7;
		//Desc.ColliderDesc.height = 0.8;
		if (m_iPlayerTeam == 1)
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
		else
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
		Desc.ColliderDesc.pMineGameObject = this;

		if (Get_fHeight() > 3)  //공중에선 더 맞추기 쉽도록 보정
		{
			Desc.ColliderDesc.vExtents = { 0.7f,0.8f,1.f };
			Desc.ColliderDesc.vCenter = { 0.9f * m_iLookDirection,0.8f,0.f };
		}
		else
		{
			Desc.ColliderDesc.vExtents = { 0.4f,0.4f,1.f };
			Desc.ColliderDesc.vCenter = { 0.9f * m_iLookDirection,1.0f,0.f };
		}

		Desc.fhitCharacter_Impus = { m_fImpuse.x,0 };

		//if(abs(m_fImpuse.x)<0.3 )
		//	Desc.fhitCharacter_Impus = { m_fImpuse.x*m_iLookDirection,0 };
		//else
		//	Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };


		//Desc.fhitCharacter_Impus = { m_fImpuse.x * 0.9f,0 };


		Desc.fhitCharacter_StunTime = 0.6f;
		Desc.iDamage = 400 * Get_DamageScale();
		Desc.fLifeTime = 0.1f;
		Desc.ihitCharacter_Motion = { HIT_LIGHT };
		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.04f;
		//Desc.bOwnerGravityTimeReset = true;
		Desc.pOwner = this;
		Desc.eAttackType = { ATTACKTYPE_MIDDLE };

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}
	break;
	case Client::CPlay_Hit::ANIME_ATTACK_AIR2:
	{

		CAttackObject::ATTACK_DESC Desc{};
		//Desc.ColliderDesc.width = 0.7;
		//Desc.ColliderDesc.height = 0.8;
		if (m_iPlayerTeam == 1)
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
		else
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;

		Desc.ColliderDesc.pMineGameObject = this;
		if (Get_fHeight() > 3)  //공중에선 더 맞추기 쉽도록 보정
		{
			Desc.ColliderDesc.vExtents = { 0.7f,0.8f,1.f };
			Desc.ColliderDesc.vCenter = { 0.9f * m_iLookDirection,0.8f,0.f };
		}
		else
		{
			Desc.ColliderDesc.vExtents = { 0.6f,0.5f,1.f };
			Desc.ColliderDesc.vCenter = { 0.9f * m_iLookDirection,1.0f,0.f };
		}
		//Desc.ColliderDesc.pTransform = m_pTransformCom;
		//Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };

		Desc.fhitCharacter_Impus = { m_fImpuse.x  ,0 };

		//if (abs(m_fImpuse.x) < 0.3)
		//	Desc.fhitCharacter_Impus = { m_fImpuse.x * m_iLookDirection,0 };
		//else
		//	Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };


		Desc.fhitCharacter_StunTime = 0.4f;

		if (Get_fHeight() > 2)
			Desc.fhitCharacter_StunTime = 0.6f;


		Desc.iDamage = 700 * Get_DamageScale();
		Desc.fLifeTime = 0.1f;
		Desc.ihitCharacter_Motion = { HitMotion::HIT_LIGHT };
		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.04f;
		//Desc.bOwnerGravityTimeReset = true;
		Desc.pOwner = this;
		Desc.eAttackType = { ATTACKTYPE_MIDDLE };

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}
	break;
	case Client::CPlay_Hit::ANIME_ATTACK_AIR3:
	{
		CAttackObject::ATTACK_DESC Desc{};
		//Desc.ColliderDesc.width = 1.0;
		//Desc.ColliderDesc.height = 1.0;
		if (m_iPlayerTeam == 1)
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
		else
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
		Desc.ColliderDesc.pMineGameObject = this;

		if (Get_fHeight() > 3)  //공중에선 더 맞추기 쉽도록 보정
		{
			Desc.ColliderDesc.vExtents = { 1.f,1.f,1.f };
			Desc.ColliderDesc.vCenter = { 1.0f * m_iLookDirection,0.8f,0.f };
		}
		else
		{
			Desc.ColliderDesc.vExtents = { 0.4f,0.8f,1.f };
			Desc.ColliderDesc.vCenter = { 0.7f * m_iLookDirection,0.8f,0.f };
		}
		//Desc.ColliderDesc.pTransform = m_pTransformCom;
		Desc.fhitCharacter_Impus = { 3.f * m_iLookDirection,-20.f };
		Desc.fhitCharacter_StunTime = 0.9f;
		Desc.iDamage = 850 * Get_DamageScale();;
		Desc.fLifeTime = 0.2f;
		//Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_LEFT };
		Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_LEFTDOWN };

		Desc.iTeam = m_iPlayerTeam;


		//높이가 5 이상이면 내려찍히는 판정.  디폴트는 false이므로  else 처리 안함
		//if(Get_fHeight() > 5)
		if (Get_fHeight() > 3)
		{
			Desc.bGroundSmash = true;
			Desc.fAnimationLockTime = 0.7f;
		}
		else
		{
			Desc.bCameraZoom = false;
			Desc.fAnimationLockTime = 0.1f;
		}

		Desc.bGrabbedEnd = true;
		Desc.pOwner = this;
		Desc.eAttackType = { ATTACKTYPE_MIDDLE };

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}
	break;
	case Client::CPlay_Hit::ANIME_ATTACK_UPPER_AIR:
	{
		CAttackObject::ATTACK_DESC Desc{};
		//Desc.ColliderDesc.width = 1.0;
		//Desc.ColliderDesc.height = 1.0;
		if (m_iPlayerTeam == 1)
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
		else
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
		Desc.ColliderDesc.pMineGameObject = this;

		Desc.ColliderDesc.vExtents = { 0.5f,0.7f,1.f };
		Desc.ColliderDesc.vCenter = { 0.7f * m_iLookDirection,0.8f,0.f };

		//Desc.ColliderDesc.pTransform = m_pTransformCom;
		//Desc.fhitCharacter_Impus = { 0.f, 6.f }; //너무 높음
		Desc.fhitCharacter_Impus = { 0.f, 4.f };

		//Desc.fhitCharacter_StunTime = 0.6f;
		Desc.fhitCharacter_StunTime = 0.8f;

		Desc.iDamage = 1000 * Get_DamageScale();;
		Desc.fLifeTime = 0.2f;
		Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_UP };
		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.08f;
		Desc.pOwner = this;

		if (Get_fHeight() > 2)
		{
			Desc.bCameraZoom = false;
			//Desc.ColliderDesc.vCenter = { 1.0f * m_iLookDirection,0.8f,0.f };
			//Desc.ColliderDesc.vCenter = { 1.0f * m_iLookDirection,0.8f,0.f };
			//Desc.fhitCharacter_Impus = { 0.2f * m_iLookDirection, 6.f };
			Desc.fhitCharacter_Impus = { 0.4f * m_iLookDirection, 5.f };

		}
		else
		{
			Desc.fhitCharacter_Impus = { 0.2f * m_iLookDirection, 10.f };  //원래 가속도.
			Desc.bCameraZoom = true;
			Desc.fAnimationLockTime = 0.5f;
		}

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);





		Character_Make_Effect(TEXT("Ring_Dust"), { m_iLookDirection * 1.2f,0.7f });
	}
	break;
	case Client::CPlay_Hit::ANIME_ATTACK_SPECIAL_AIR:
	{
		//반격 자세 시작
		if (iAttackEvent == 0)
		{
			m_bCounterPose = true;
		}

		//반격성공. 적 멈춘상태
		else if (iAttackEvent == 1)
		{
			m_bCounterPose = false;
			m_bCounterSucces = false;
			m_bInvisible = false;

			_vector vTargetPos = static_cast<CTransform*>(m_pEnemy->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
			vTargetPos += {1.3f * m_iLookDirection, 0.1f, 0, 0};
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTargetPos);

			FlipDirection();

			Character_Make_Effect(TEXT("Moving_Line_Right"));

			CMain_Camera* main_Camera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")));
			main_Camera->StartCameraShake(0.1f, 0.3f);



			//점프로 이행 
			Set_Animation(m_iJumpAnimationIndex);
			Set_ForcedGravityTime_LittleUp();


		}


		//반격 실패시 돌아오는 모션
		else if (iAttackEvent == 2)
		{
			m_bCounterPose = false;
			Set_ForcedGravityDown();
			Set_Animation(ANIME_JUMP_DOWN);

		}
	}
	break;
	case Client::CPlay_Hit::ANIME_GRAB:
		break;
	case Client::CPlay_Hit::ANIME_SPARKING:
		break;
	case Client::CPlay_Hit::ANIME_REFLECT:
		break;
	case Client::CPlay_Hit::ANIME_CROUCH_START:
		break;
	case Client::CPlay_Hit::ANIME_CROUCHING:
		break;
	case Client::CPlay_Hit::ANIME_GUARD_GROUND:
		break;
	case Client::CPlay_Hit::ANIME_GUARD_CROUCH:
		break;
	case Client::CPlay_Hit::ANIME_GUARD_AIR:
		break;
	case Client::CPlay_Hit::ANIME_JUMP_UP:
		break;
	case Client::CPlay_Hit::ANIME_JUMP_DOWN:
		break;
	case Client::CPlay_Hit::ANIME_IDLE:
		break;
	case Client::CPlay_Hit::ANIME_FORWARD_WALK:
		break;
	case Client::CPlay_Hit::ANIME_BACK_WALK:
		break;
	case Client::CPlay_Hit::ANIME_FORWARD_DASH:
		break;
	case Client::CPlay_Hit::ANIME_BACK_DASH:
		break;
	case Client::CPlay_Hit::ANIME_FORWARD_DASH_END:
		break;
	case Client::CPlay_Hit::ANIME_BACK_DASH_END:
		break;
	case Client::CPlay_Hit::ANIME_CHASE:
		break;
	case Client::CPlay_Hit::ANIME_GRAB_READY:
		break;
	case Client::CPlay_Hit::ANIME_HIT_LIGHT:
		break;
	case Client::CPlay_Hit::ANIME_HIT_LIGHT_FRONT:
		break;
	case Client::CPlay_Hit::ANIME_HIT_CROUCH:
		break;
	case Client::CPlay_Hit::ANIE_HIT_AIR:
		break;
	case Client::CPlay_Hit::ANIME_HIT_FALL:
		break;
	case Client::CPlay_Hit::ANIME_HIT_BOUND_DOWN:
		break;
	case Client::CPlay_Hit::ANIME_HIT_HEAVY_AWAY_LEFT:
		break;
	case Client::CPlay_Hit::ANIME_HIT_HEAVY_AWAY_UP:
		break;
	case Client::CPlay_Hit::ANIME_HIT_HEAVY_AWAY_LEFTDOWN:
		break;
	case Client::CPlay_Hit::ANIME_HIT_HEAVY_AWAY_SPIN_UP:
		break;
	case Client::CPlay_Hit::ANIME_HIT_HEAVY_AWAY_SPIN_LEFTUP:
		break;
	case Client::CPlay_Hit::ANIME_HIT_HEAVY_AWAY_SPIN_LEFT:
		break;
	case Client::CPlay_Hit::ANIME_HIT_WALLBOUNCE:
		break;
	case Client::CPlay_Hit::ANIME_DIE_STAND:
		break;
	case Client::CPlay_Hit::ANIME_LAYUP:
		break;
	case Client::CPlay_Hit::ANIME_BREAK_FALL_GROUND:
		break;
	case Client::CPlay_Hit::ANIME_BREAK_FALL_AIR:
		break;
	case Client::CPlay_Hit::ANIME_236_POSE:
	{
		if (iAttackEvent == 0)
		{
			m_b236Posing = true;

		}
		else if (iAttackEvent == 1)
		{
			m_b236Posing = false;
		}

	}
	break;
	case Client::CPlay_Hit::ANIME_236_POSE_LIGHT:
	{
		if (iAttackEvent == 0)
		{

			if (m_b236Special == false)
			{
				m_bInvisible = true;
				Character_Make_Effect(TEXT("Moving_Line_Right"));

				CMain_Camera* main_Camera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")));
				main_Camera->StartCameraShake(0.1f, 0.3f);
			}
			else
			{
				//원거리 공격의 경우 스킵 대신 속도 더 빠르게. 기존 61
				m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 121.f;
			}


		}
		else if (iAttackEvent == 1)
		{
			if (m_b236Special == false)
			{
				m_bInvisible = false;
				MoveToEnemy_Ground(7.f);
				Character_Make_Effect(TEXT("Moving_Line_Right"));
				m_b236Posing = false;

				CMain_Camera* main_Camera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")));
				main_Camera->StartCameraShake(0.1f, 0.5f);
				{
					CAttackObject_CommandGrab::ATTACK_COMMANDGRAB_DESC Desc{};

					if (m_iPlayerTeam == 1)
						Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
					else
						Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
					Desc.ColliderDesc.pMineGameObject = this;
					Desc.ColliderDesc.vExtents = { 0.2f,1.f,1.f };
					Desc.ColliderDesc.vCenter = { 0.8f * m_iLookDirection,0.8f,0.f };

					Desc.fhitCharacter_Impus = { 20.f * m_iLookDirection,0.5f };
					Desc.fhitCharacter_StunTime = 1.0f;
					Desc.iDamage = 0;
					Desc.fLifeTime = 0.05f;
					Desc.ihitCharacter_Motion = { HitMotion::HIT_LIGHT };
					Desc.iTeam = m_iPlayerTeam;
					Desc.fAnimationLockTime = 0.7f;

					Desc.fDistance = { 0.8f,0.5f };
					Desc.iGrabAnimationIndex = CPlay_Hit::ANIME_236_POSE_LIGHT;
					Desc.bOwnerNextAnimation = false;

					Desc.iGainAttackStep = 0;
					Desc.iGainHitCount = 0;
					Desc.iGainKiAmount = 0;

					Desc.bGrabbedEnd = true;
					Desc.pOwner = this;

					Desc.fCameraShakeDuration = 0.4f;
					Desc.fCameraShakeMagnitude = 0.1f;

					m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack_CommandGrab"), TEXT("Layer_AttackObject"), &Desc);

				}
				{
					CAttackObject::ATTACK_DESC Desc{};

					if (m_iPlayerTeam == 1)
						Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
					else
						Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
					Desc.ColliderDesc.pMineGameObject = this;
					Desc.ColliderDesc.vExtents = { 0.2f,1.f,1.f };
					Desc.ColliderDesc.vCenter = { 0.8f * m_iLookDirection,0.8f,0.f };

					Desc.fhitCharacter_Impus = { 20.f * m_iLookDirection,0 };
					Desc.fhitCharacter_StunTime = 1.0f;
					Desc.iDamage = 900 * Get_DamageScale();;
					Desc.fLifeTime = 0.05f;
					Desc.ihitCharacter_Motion = { HitMotion::HIT_LIGHT };
					Desc.iTeam = m_iPlayerTeam;
					Desc.fAnimationLockTime = 0.7f;
					Desc.bGrabbedEnd = true;
					Desc.pOwner = this;
					Desc.bCameraZoom = false;

					m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
				}
			}
			else
			{
				//원거리 공격의 경우 손에 파란이팩트는 동일, 원거리 공격판정
				CAttackObject::ATTACK_DESC Desc{};

				if (m_iPlayerTeam == 1)
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
				else
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
				Desc.ColliderDesc.pMineGameObject = this;
				Desc.ColliderDesc.vExtents = { 0.2f,1.f,1.f };


				//center를 상대 좌표로 할게 아니라 나~상대 까지의 거리로 해야함

				_vector vEnemyPos = m_pEnemy->Get_vPosition();
				_vector vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

				_vector vLength = vEnemyPos - vMyPos;

				Desc.ColliderDesc.vCenter = { XMVectorGetX(vLength),XMVectorGetY(vLength),0.f };


				Desc.fhitCharacter_Impus = { 20.f * m_iLookDirection,0 };
				Desc.fhitCharacter_StunTime = 1.0f;
				Desc.iDamage = 700 * Get_DamageScale();;
				Desc.fLifeTime = 0.1f;
				Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_LEFT_NONEBOUNDE };
				Desc.iTeam = m_iPlayerTeam;
				Desc.fAnimationLockTime = 0.1f;
				Desc.bGrabbedEnd = true;
				Desc.pOwner = this;
				Desc.bCameraZoom = false;
				Desc.bOnwerHitNoneStop = true;

				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);

			}
		}
		else if (iAttackEvent == 2)
		{
			//평범하게 사용시 할것없음. 규칙성 유지하느라 틀 만듦
			if (m_b236Special == false)
			{
			}
			else
			{
				m_b236Special = false;
			}
		}
	}
	break;
	case Client::CPlay_Hit::ANIME_236_POSE_MEDIUM:
	{
		//,안보이게 + 샤샤샥
		if (iAttackEvent == 0)
		{
			m_bInvisible = true;
			Character_Make_Effect(TEXT("Moving_Line_Right"));

		}
		else if (iAttackEvent == 1)
		{
			m_bInvisible = false;
			MoveToEnemy_Ground(7.f);
			Character_Make_Effect(TEXT("Moving_Line_Right"));
			m_b236Posing = false;


			{
				CAttackObject_CommandGrab::ATTACK_COMMANDGRAB_DESC Desc{};

				if (m_iPlayerTeam == 1)
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
				else
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
				Desc.ColliderDesc.pMineGameObject = this;
				Desc.ColliderDesc.vExtents = { 0.2f,1.f,1.f };
				Desc.ColliderDesc.vCenter = { 0.8f * m_iLookDirection,0.8f,0.f };

				Desc.fhitCharacter_Impus = { 20.f * m_iLookDirection,0.5f };
				//Desc.fhitCharacter_StunTime = 1.0f;
				Desc.fhitCharacter_StunTime = 0.3f;

				Desc.iDamage = 0;
				Desc.fLifeTime = 0.1f;
				Desc.ihitCharacter_Motion = { HitMotion::HIT_LIGHT };
				Desc.iTeam = m_iPlayerTeam;
				Desc.fAnimationLockTime = 0.7f;

				Desc.fDistance = { 0.8f,0.5f };
				Desc.iGrabAnimationIndex = CPlay_Hit::ANIME_236_POSE_MEDIUM;
				Desc.bOwnerNextAnimation = false;

				Desc.iGainAttackStep = 0;
				Desc.iGainHitCount = 0;
				Desc.iGainKiAmount = 0;

				Desc.bGrabbedEnd = true;
				Desc.pOwner = this;

				Desc.iVirtualCameraindex = CMain_Camera::VIRTUAL_CAMERA_HIT_236_MIDDLE;
				Desc.fCameraShakeDuration = 1.4f;
				Desc.fCameraShakeMagnitude = 0.2f;

				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack_CommandGrab"), TEXT("Layer_AttackObject"), &Desc);

			}
			{
				CAttackObject::ATTACK_DESC Desc{};

				if (m_iPlayerTeam == 1)
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
				else
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
				Desc.ColliderDesc.pMineGameObject = this;
				Desc.ColliderDesc.vExtents = { 0.2f,1.f,1.f };
				Desc.ColliderDesc.vCenter = { 0.8f * m_iLookDirection,0.8f,0.f };

				Desc.fhitCharacter_Impus = { 20.f * m_iLookDirection,-1.f };
				Desc.fhitCharacter_StunTime = 1.0f;
				Desc.iDamage = 900 * Get_DamageScale();;
				Desc.fLifeTime = 0.1f;
				Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_LEFT_NONEBOUNDE };
				Desc.iTeam = m_iPlayerTeam;
				Desc.fAnimationLockTime = 0.7f;
				Desc.bGrabbedEnd = true;
				Desc.pOwner = this;
				Desc.bGroundSmash = true;

				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
			}
		}
	}
	break;
	case Client::CPlay_Hit::ANIME_236_POSE_HEAVY:
	{
		if (iAttackEvent == 0)
		{
			m_bInvisible = true;
			Character_Make_Effect(TEXT("Moving_Line_Right"));


			CMain_Camera* main_Camera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")));
			main_Camera->StartCameraShake(0.1f, 0.3f);
		}
		else if (iAttackEvent == 1)
		{

			CMain_Camera* main_Camera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")));
			main_Camera->Play(CMain_Camera::VIRTUAL_CAMERA_HIT_236_HEAVY, 0, this);
			main_Camera->StartCameraShake(0.8f, 0.3f);

			m_bInvisible = false;
			MoveToEnemy_Ground(5.f);
			Character_Make_Effect(TEXT("Moving_Line_Right"));
			m_b236Posing = false;


			CAttackObject::ATTACK_DESC Desc{};

			if (m_iPlayerTeam == 1)
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
			else
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
			Desc.ColliderDesc.pMineGameObject = this;
			Desc.ColliderDesc.vExtents = { 0.2f,1.f,1.f };
			Desc.ColliderDesc.vCenter = { 0.8f * m_iLookDirection,0.8f,0.f };

			Desc.fhitCharacter_Impus = { 0.5f * m_iLookDirection,3.f };
			//Desc.fhitCharacter_StunTime = 1.0f;
			Desc.fhitCharacter_StunTime = 0.7f;

			Desc.iDamage = 1500 * Get_DamageScale();;
			Desc.fLifeTime = 0.1f;
			Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_UP_GRAVITY };
			Desc.iTeam = m_iPlayerTeam;
			Desc.fAnimationLockTime = 0.7f;
			Desc.pOwner = this;
			Desc.iGainKiAmount = 60;

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
		}
		else if (iAttackEvent == 2)
		{
			if (m_bAttackBackEvent == true)
			{
				//기존 61이었던걸 명중시 더 빠르게
				//m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 181.f;
				//m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 211.f;
				m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 301.f;

			}
		}
	}
	break;


	case Client::CPlay_Hit::ANIME_214_POSE:
	{
		if (iAttackEvent == 0) //Position 20에서 발생
		{
			//딱 20.f에서 멈추는 일 없도록
			Set_CurrentAnimationPositionJump(20.05f);

			m_b214Posing = true;
			m_fAccPoseTime = 0.f;
			m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 0.f;
		}

		//txt로부터 호출할게 아니라 Update214로부터 호출할것
		//처음에 잡기로 고정시켜두고 나중에 추가공격 함
		else if (iAttackEvent == 400)
		{

			//적 위치로부터 AccTime만큼 크기로 잡기판정테스트

			// 실패시 애니메이션 Position 80으로 변경, 성공시 속도 변경.. 실패해도 속도는 바꿔야지  
			//m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 61.f;

			//콜라이더 먼저 되도록 속도 늦춰야됨
			m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 5.f;

			//공격판정 생성
			{



				CAttackObject_CommandGrab::ATTACK_COMMANDGRAB_DESC Desc{};
				if (m_iPlayerTeam == 1)
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
				else
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;


				//_vector vEnemyPos = m_pEnemy->Get_vPosition();
				//_vector vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				//_vector vLength = vEnemyPos - vMyPos;

				//Desc.ColliderDesc.vCenter = { XMVectorGetX(vLength),XMVectorGetY(vLength),0.f };
				Desc.ColliderDesc.vCenter = { 5.f * m_iLookDirection,0.f ,0.f };




				Desc.ColliderDesc.pMineGameObject = this;
				Desc.ColliderDesc.vExtents = { m_fAccPoseTime * 2.f,0.5f,1.f };

				//Desc.ColliderDesc.pTransform = m_pTransformCom;
				//Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };
				Desc.fhitCharacter_StunTime = 10.f;
				Desc.iDamage = 0;
				Desc.fLifeTime = 0.2f;
				Desc.ihitCharacter_Motion = { HitMotion::HIT_LIGHT };
				Desc.iTeam = m_iPlayerTeam;
				Desc.fAnimationLockTime = 0.1f;
				Desc.pOwner = this;


				Desc.iVirtualCameraindex = CMain_Camera::VIRTUAL_CAMERA_HIT_214_MIDDLE;
				Desc.fCameraShakeDuration = 1.4f;
				Desc.fCameraShakeMagnitude = 0.15f;

				Desc.eAttackType = ATTACKTYPE_COMMANDGRAB;

				//Desc.fDistance = { XMVectorGetX(vLength),XMVectorGetY(vLength)};

				///_vector vLength = vEnemyPos - vMyPos;
				//Desc.fDistance = { XMVectorGetX(vLength),0.f };

				_float fLength = m_pEnemy->Get_fPositionX() - Get_fPositionX();
				Desc.fDistance = { fLength,0.f };



				//Desc.fGrabAnimationPosition = 40.f;
				//Desc.fGrabAnimationPosition = 25.f;
				Desc.iGainAttackStep = 0;
				Desc.iGainKiAmount = 0;

				Desc.bOwnerNextAnimation = false;

				Desc.iGrabAnimationIndex = ANIME_214_POSE;


				//Desc.iVirtualCameraindex = CMain_Camera::VIRTUAL_CAMERA_21_GRAB_SPECIAL;

				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack_CommandGrab"), TEXT("Layer_AttackObject"), &Desc);
			}

		}
		else if (iAttackEvent == 1)
		{
			m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 61.f;

			if (m_bAttackBackEvent == false)
			{
				Set_CurrentAnimationPositionJump(80.f);
			}

		}
		else if (iAttackEvent == 2)
		{
			CAttackObject::ATTACK_DESC Desc{};

			if (m_iPlayerTeam == 1)
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
			else
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
			Desc.ColliderDesc.pMineGameObject = this;
			Desc.ColliderDesc.vExtents = { m_fAccPoseTime * 3.f,1.f,1.f };


			//center를 상대 좌표로 할게 아니라 나~상대 까지의 거리로 해야함

			_vector vEnemyPos = m_pEnemy->Get_vPosition();
			_vector vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			_vector vLength = vEnemyPos - vMyPos;

			//Desc.ColliderDesc.vCenter = { XMVectorGetX(vLength), 0.f ,0.f };
			Desc.ColliderDesc.vCenter = { 5.f * m_iLookDirection, 0.f ,0.f };



			Desc.fhitCharacter_StunTime = 0.4f;
			Desc.iDamage = 1334 * Get_DamageScale();;
			Desc.fLifeTime = 0.2f;
			//Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_UP_GRAVITY };
			//Desc.fhitCharacter_Impus = { 0.5f * m_iLookDirection,8.f };

			//Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_LEFTDOWN };
			Desc.ihitCharacter_Motion = { HitMotion::HIT_SPIN_AWAY_LEFTUP };

			//Desc.fhitCharacter_Impus = { 3.f * m_iLookDirection,-20.f };
			Desc.fhitCharacter_Impus = { 4.f * m_iLookDirection,2.f };



			Desc.iTeam = m_iPlayerTeam;
			Desc.fAnimationLockTime = 0.1f;
			Desc.bGrabbedEnd = true;
			Desc.pOwner = this;
			Desc.bCameraZoom = false;
			Desc.bOnwerHitNoneStop = true;

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
		}

		//처음에 잡기로 고정시켜두고 추가공격 함

		//Position 95에서 Idle로 변경은 txt로 함

	}
	break;
	case Client::CPlay_Hit::ANIME_236_SPECIAL:
		break;
	case Client::CPlay_Hit::ANIME_236_ULTIMATE:
	{
		//,20시간정지  + 카메라
		if (iAttackEvent == 0)
		{
			Set_AnimationStopWithoutMe(1.5f);
			Set_AnimationStop(0.6f);
		}


		//,85안보이게 + 샤샤샥
		else if (iAttackEvent == 1)
		{
			m_bInvisible = true;
			Character_Make_Effect(TEXT("Moving_Line_Right"));
		}


		//,100 적 앞으로 이동 + 보이게 + 샤샤샥 + 공격판정
		else if (iAttackEvent == 2)
		{


			m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 90.f;

			//중간있는 버전
			//if(false)
			//{
			//	MoveToEnemy_Ground(12.f);
			//	m_bInvisible = false;
			//	Character_Make_Effect(TEXT("Moving_Line_Right"));
			//
			//	CAttackObject_CommandGrab::ATTACK_COMMANDGRAB_DESC Desc{};
			//	if (m_iPlayerTeam == 1)
			//		Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
			//	else
			//		Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
			//
			//
			//	Desc.ColliderDesc.vCenter = { 0.4f * m_iLookDirection,0.f ,0.f };
			//
			//
			//
			//
			//	Desc.ColliderDesc.pMineGameObject = this;
			//	Desc.ColliderDesc.vExtents = { 2.f,0.5f,1.f };
			//
			//	Desc.fhitCharacter_StunTime = 10.f;
			//	Desc.iDamage = 0;
			//	Desc.fLifeTime = 0.2f;
			//	Desc.ihitCharacter_Motion = { HitMotion::HIT_LIGHT };
			//	Desc.iTeam = m_iPlayerTeam;
			//	Desc.fAnimationLockTime = 0.1f;
			//	Desc.pOwner = this;
			//
			//
			//	Desc.eAttackType = ATTACKTYPE_HIGH;
			//
			//
			//
			//	//_float fLength = m_pEnemy->Get_fPositionX() - Get_fPositionX();
			//	//Desc.fDistance = { fLength,0.f };
			//
			//	_float fPos = Get_fPositionX();
			//
			//	if (fPos > 6 && m_iLookDirection == 1)
			//	{
			//		Desc.fDistance = { -3.f,0.f };
			//	}
			//	else if (fPos < -6 && m_iLookDirection == -1)
			//	{
			//		Desc.fDistance = { 3.f,0.f };
			//
			//	}
			//	else
			//		Desc.fDistance = { 0.9f * m_iLookDirection,0.f };
			//
			//	Desc.iGainAttackStep = 0;
			//	Desc.iGainKiAmount = 0;
			//
			//	Desc.bOwnerNextAnimation = false;
			//
			//	Desc.iGrabAnimationIndex = ANIME_236_ULTIMATE;
			//
			//
			//	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack_CommandGrab"), TEXT("Layer_AttackObject"), &Desc);
			//}

			{



				CAttackObject_CommandGrab::ATTACK_COMMANDGRAB_DESC Desc{};
				if (m_iPlayerTeam == 1)
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
				else
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;



				_vector vEnemyPos = m_pEnemy->Get_vPosition();
				_vector vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

				_vector vLength = vEnemyPos - vMyPos;

				Desc.ColliderDesc.vCenter = { XMVectorGetX(vLength),0.4f,0.f };




				Desc.ColliderDesc.pMineGameObject = this;
				Desc.ColliderDesc.vExtents = { 2.f,1.0f,1.f };

				Desc.fhitCharacter_StunTime = 10.f;
				Desc.iDamage = 0;
				Desc.fLifeTime = 0.2f;
				Desc.ihitCharacter_Motion = { HitMotion::HIT_LIGHT };
				Desc.iTeam = m_iPlayerTeam;
				Desc.fAnimationLockTime = 2.f;
				Desc.bOnwerHitNoneStop = true;
				Desc.pOwner = this;
				Desc.bCameraZoom = false;

				Desc.eAttackType = ATTACKTYPE_HIGH;
				Desc.eAttackGrade = GRADE_ULTIMATE;


				//_float fLength = m_pEnemy->Get_fPositionX() - Get_fPositionX();
				//Desc.fDistance = { fLength,0.f };

				_float fPos = Get_fPositionX();

				if (fPos > 6 && m_iLookDirection == 1)
				{
					Desc.fDistance = { -3.f,0.f };
				}
				else if (fPos < -6 && m_iLookDirection == -1)
				{
					Desc.fDistance = { 3.f,0.f };

				}
				else
					Desc.fDistance = { 0.9f * m_iLookDirection,0.f };

				Desc.iGainAttackStep = 0;
				Desc.iGainKiAmount = 0;

				Desc.bOwnerNextAnimation = false;

				Desc.iGrabAnimationIndex = ANIME_236_ULTIMATE;


				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack_CommandGrab"), TEXT("Layer_AttackObject"), &Desc);
			}

		}

		//,110 정지 + 더 앞으로 이동 + 카메라 + 추가공격 + 이펙트,  애니메이션속도 감속, 120으로 읻오

		//공격 성공했는지 판정 성공시 정지 + 더 앞으로 이동 + 카메라 +애니메이션속도 감속,
		else if (iAttackEvent == 3)
		{
			m_bInvisible = false;

			m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 161.f;

			if (m_bAttackBackEvent == true)
			{

				//중간단계 있는버전
				if (false)
				{
					Add_Move({ 7.f * m_iLookDirection,0.f });
					m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 10.f;
				}

				{
					//Add_Move({ 20.f * m_iLookDirection,0.f });
					Teleport_ToEnemy(7.f, 0.f);
					m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 10.f;
				}
			}
			else
			{
				MoveToEnemy_Ground(12.f);
			}
		}

		//,120  속도1 ?  대신에 추가공격 하자
		else if (iAttackEvent == 4)
		{
			if (m_bAttackBackEvent)
			{
				CAttackObject::ATTACK_DESC Desc{};

				if (m_iPlayerTeam == 1)
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
				else
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
				Desc.ColliderDesc.pMineGameObject = this;
				Desc.ColliderDesc.vExtents = { 20.f,3.f,1.f };


				//center를 상대 좌표로 할게 아니라 나~상대 까지의 거리로 해야함

				_vector vEnemyPos = m_pEnemy->Get_vPosition();
				_vector vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

				_vector vLength = vEnemyPos - vMyPos;

				//Desc.ColliderDesc.vCenter = { XMVectorGetX(vLength), 0.f ,0.f };
				Desc.ColliderDesc.vCenter = { -5.f * m_iLookDirection, 1.f ,0.f };



				Desc.fhitCharacter_StunTime = 0.4f;
				Desc.iDamage = 1334 * Get_DamageScale();;
				Desc.fLifeTime = 0.2f;
				//Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_UP_GRAVITY };
				//Desc.fhitCharacter_Impus = { 0.5f * m_iLookDirection,8.f };

				//Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_LEFTDOWN };
				Desc.ihitCharacter_Motion = { HitMotion::HIT_SPIN_AWAY_LEFTUP };

				//Desc.fhitCharacter_Impus = { 3.f * m_iLookDirection,-20.f };
				Desc.fhitCharacter_Impus = { 1.f * m_iLookDirection,2.f };



				Desc.iTeam = m_iPlayerTeam;
				Desc.fAnimationLockTime = 0.1f;
				Desc.pOwner = this;
				Desc.bCameraZoom = false;
				Desc.bOnwerHitNoneStop = true;
				Desc.bGrabbedEnd = true;

				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
			}
			else
			{
				Set_CurrentAnimationPositionJump(159.9f);

				m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 91.f;
			}
		}

		//123 속도 정상화
		else if (iAttackEvent == 5)
		{

			m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 91.f;
		}

		//155 성공했으면 Idle로
		else if (iAttackEvent == 6)
		{
			if (m_bAttackBackEvent == true)
			{
				Set_Animation(ANIME_IDLE);
			}
		}


		else if (iAttackEvent == 7)
		{
			//if (m_bAttackBackEvent == false)
			//{
			//	Set_Animation(ANIME_IDLE);
			//}
		}
		else if (iAttackEvent == 8)
		{
			if (m_bAttackBackEvent == false)
			{
				Set_Animation(ANIME_IDLE);
			}
		}

	}
	break;
	case Client::CPlay_Hit::ANIME_214_FINAL_START:
	{
		//,상대만 시간정지
		if (iAttackEvent == 0)
		{
			Set_AnimationStopWithoutMe(1.5f);
		}

		//,안보이게,속도빠르게, 샤샤샥
		else if (iAttackEvent == 1)
		{
			m_bInvisible = true;
			Character_Make_Effect(TEXT("Moving_Line_Right"));

			//,0일떄 속도는 61
			m_pModelCom->Get_pCurrentAnimation()->m_fTickPerSecond = 181.f;

		}

		else if (iAttackEvent == 2)
		{

			MoveToEnemy_Ground(12.f);

			m_bInvisible = false;
			Character_Make_Effect(TEXT("Moving_Line_Right"));

			m_pModelCom->Get_pCurrentAnimation()->m_fTickPerSecond = 61.f;

			{
				CAttackObject_CommandGrab::ATTACK_COMMANDGRAB_DESC Desc{};
				if (m_iPlayerTeam == 1)
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
				else
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
				Desc.ColliderDesc.pMineGameObject = this;
				Desc.ColliderDesc.vExtents = { 0.8f,0.6f,1.f };
				Desc.ColliderDesc.vCenter = { 0.3f,0.7f,0.f };
				Desc.fhitCharacter_StunTime = 20.f;
				Desc.iDamage = 500 * Get_DamageScale(true);;
				Desc.fLifeTime = 0.2f;
				Desc.ihitCharacter_Motion = { HitMotion::HIT_LIGHT };
				Desc.iTeam = m_iPlayerTeam;
				Desc.fAnimationLockTime = 0.1f;
				Desc.pOwner = this;

				Desc.eAttackType = ATTACKTYPE_HIGH;
				Desc.eAttackGrade = GRADE_ULTIMATE;

				Desc.fDistance = { 0.8f * m_iLookDirection,0.f };
				Desc.fForcedGravityTime = 0.f;
				Desc.iGainAttackStep = 2;
				Desc.iGrabAnimationIndex = ANIME_214_FINAL_START;
				Desc.iOnwerNextAnimationIndex = ANIME_214_FINAL_SUCESS;


				//Desc.iVirtualCameraindex = CMain_Camera::VIRTUAL_CAMERA_SON_ULTIMATE;
				Desc.ianimationIndex = 1;
				Desc.fCameraShakeDuration = 0.5f;
				Desc.fCameraShakeMagnitude = 0.2f;


				Desc.fForcedGravityTime = 0.f;

				Desc.bOwnerNextAnimation = true;
				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack_CommandGrab"), TEXT("Layer_AttackObject"), &Desc);
			}

		}
		else if (iAttackEvent == 3)
		{

			if (m_bAttackBackEvent == false)
			{
				Set_Animation(ANIME_IDLE);
			}
		}
		else if (iAttackEvent == 4)
		{

			cout << "Event4 , Position : " << m_pModelCom->m_fCurrentAnimPosition << endl;

			CAttackObject::ATTACK_DESC Desc{};

			if (m_iPlayerTeam == 1)
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
			else
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
			Desc.ColliderDesc.pMineGameObject = this;
			Desc.ColliderDesc.vExtents = { 20.f,1.f,1.f };
			Desc.ColliderDesc.vCenter = { 1.0f * m_iLookDirection,0.8f,0.f };

			Desc.fhitCharacter_Impus = { 20.f * m_iLookDirection,0 };
			Desc.fhitCharacter_StunTime = 1.0f;
			Desc.iDamage = 1000 * Get_DamageScale();;
			Desc.fLifeTime = 0.2f;
			Desc.ihitCharacter_Motion = { HitMotion::HIT_MEDIUM };
			Desc.iTeam = m_iPlayerTeam;
			Desc.fAnimationLockTime = 0.7f;
			//Desc.bGrabbedEnd = true;
			//Desc.bOnwerHitNoneStop = true;
			Desc.pOwner = this;
			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);


			m_pModelCom->Get_pCurrentAnimation()->m_fCurrentPosition = 91.f;
		}
		else if (iAttackEvent == 5)
		{

			CAttackObject::ATTACK_DESC Desc{};

			if (m_iPlayerTeam == 1)
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
			else
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
			Desc.ColliderDesc.pMineGameObject = this;
			Desc.ColliderDesc.vExtents = { 20.f,1.f,1.f };
			Desc.ColliderDesc.vCenter = { 1.0f * m_iLookDirection,0.8f,0.f };

			Desc.fhitCharacter_Impus = { 20.f * m_iLookDirection,0 };
			Desc.fhitCharacter_StunTime = 1.0f;
			Desc.iDamage = 1000 * Get_DamageScale();;
			Desc.fLifeTime = 0.2f;
			Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_LEFT };
			Desc.iTeam = m_iPlayerTeam;
			Desc.fAnimationLockTime = 0.f;
			//Desc.bOwnerNextAnimation = true;
			Desc.bGrabbedEnd = true;
			Desc.pOwner = this;
			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);

		}
		else if (iAttackEvent == 6)
		{


			Set_Animation(ANIME_IDLE);
		}
	}
	break;
	case Client::CPlay_Hit::ANIME_214_FINAL_SUCESS:
	{
		//Position0.1
		if (iAttackEvent == 100)
		{
			m_bInvisible = true;
			Character_Make_Effect(TEXT("Moving_Line_Right"));
		}

		//Position 48
		else if (iAttackEvent == 0)
		{
			m_bInvisible = false;

			Character_Make_Effect(TEXT("Moving_Line_Right"));
			Set_CurrentAnimationPositionJump(48.01f);


			{
				CAttackObject::ATTACK_DESC Desc{};
				//Desc.ColliderDesc.width = 0.7;
				//Desc.ColliderDesc.height = 0.8;
				if (m_iPlayerTeam == 1)
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
				else
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
				Desc.ColliderDesc.pMineGameObject = this;
				Desc.ColliderDesc.vExtents = { 10.f,0.2f,1.f };
				Desc.ColliderDesc.vCenter = { 0.9f * m_iLookDirection,0.9f,0.f };
				Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };
				Desc.fhitCharacter_StunTime = 0.4f;
				//Desc.fhitCharacter_StunTime = 0.55f;

				Desc.iDamage = 90 * Get_DamageScale();;
				Desc.fLifeTime = 0.2f;
				Desc.ihitCharacter_Motion = { HitMotion::HIT_MEDIUM };
				Desc.iTeam = m_iPlayerTeam;
				Desc.fAnimationLockTime = 0.1f;
				Desc.pOwner = this;
				Desc.iGainKiAmount = 0;
				Desc.iGainAttackStep = 0;

				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
			}
		}
		//Position 48.1
		else if (iAttackEvent == 101)
		{
			//동시 실행 방지
			if (isNearlyEqual(m_pModelCom->m_fCurrentAnimPosition, 48))
			{
				return;
			}

			else
			{
				m_bInvisible = true;
				Character_Make_Effect(TEXT("Moving_Line_Right"));

				//cout << "Event 101 " << endl;
			}


		}
		//Position 77
		else if (iAttackEvent == 1)
		{
			m_bInvisible = false;
			{
				CAttackObject::ATTACK_DESC Desc{};
				//Desc.ColliderDesc.width = 0.7;
				//Desc.ColliderDesc.height = 0.8;
				if (m_iPlayerTeam == 1)
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
				else
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
				Desc.ColliderDesc.pMineGameObject = this;
				Desc.ColliderDesc.vExtents = { 10.f,0.2f,1.f };
				Desc.ColliderDesc.vCenter = { 0.9f * m_iLookDirection,0.9f,0.f };
				Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };
				Desc.fhitCharacter_StunTime = 0.4f;
				//Desc.fhitCharacter_StunTime = 0.55f;

				Desc.iDamage = 90 * Get_DamageScale();;
				Desc.fLifeTime = 0.2f;
				Desc.ihitCharacter_Motion = { HitMotion::HIT_MEDIUM };
				Desc.iTeam = m_iPlayerTeam;
				Desc.fAnimationLockTime = 0.1f;
				Desc.pOwner = this;
				Desc.iGainKiAmount = 0;
				Desc.iGainAttackStep = 0;

				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
			}
		}
		//Position 77.2
		else if (iAttackEvent == 102)
		{
			//동시 실행 방지
			if (isNearlyEqual(m_pModelCom->m_fCurrentAnimPosition, 77))
			{
				return;
			}

			else
			{
				//m_bInvisible = true;
				//원래 안보이게 해야 했는데 바꾸려는중

				//cout << XMVectorGetZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) << endl;
				//cout << "Event 102 " << endl;
				m_pTransformCom->Add_Move({ 1.f,0.f,-1.5f });
				//cout << XMVectorGetZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) << endl;

				Character_Make_Effect(TEXT("Moving_Line_Right"));
			}


		}
		//Position 96 
		else if (iAttackEvent == 2)
		{
			m_bInvisible = false;

		}

		//Position 100
		else if (iAttackEvent == 3)
		{
			Set_CurrentAnimationPositionJump(105.f);
			//m_pModelCom->Get_pCurrentAnimation()->m_fDuration = 0.20f;


			//너무 느린가
			//m_pModelCom->Set_MaxAnimationUpdate_Time(0.05f);
			//m_pModelCom->Get_pCurrentAnimation()->m_fTickPerSecond = 20.00f;

			//너무 빠른가
			//m_pModelCom->Set_MaxAnimationUpdate_Time(0.01f);
			//m_pModelCom->Get_pCurrentAnimation()->m_fTickPerSecond = 100.00f;


			m_pModelCom->Set_MaxAnimationUpdate_Time(0.02f);
			m_pModelCom->Get_pCurrentAnimation()->m_fTickPerSecond = 50.00f;

			m_bInvisible = true;


		}

		//Position 148
		else if (iAttackEvent == 201)
		{
			//cout << XMVectorGetZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) << endl;
			m_pTransformCom->Add_Move({ -1.f,0.f,1.f });
			//cout << "Event 201 " << endl;
			//cout << XMVectorGetZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) << endl;


			m_bInvisible = false;
			{
				CAttackObject::ATTACK_DESC Desc{};
				//Desc.ColliderDesc.width = 0.7;
				//Desc.ColliderDesc.height = 0.8;
				if (m_iPlayerTeam == 1)
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
				else
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
				Desc.ColliderDesc.pMineGameObject = this;
				Desc.ColliderDesc.vExtents = { 10.f,0.2f,1.f };
				Desc.ColliderDesc.vCenter = { 0.9f * m_iLookDirection,0.9f,0.f };
				Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };
				Desc.fhitCharacter_StunTime = 0.4f;
				//Desc.fhitCharacter_StunTime = 0.55f;

				Desc.iDamage = 90 * Get_DamageScale();;
				Desc.fLifeTime = 0.2f;
				Desc.ihitCharacter_Motion = { HitMotion::HIT_MEDIUM };
				Desc.iTeam = m_iPlayerTeam;
				Desc.fAnimationLockTime = 0.1f;
				Desc.pOwner = this;
				Desc.iGainKiAmount = 0;
				Desc.iGainAttackStep = 0;

				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
			}


			Add_Move({ -3.f * m_iLookDirection,0.f });
			{
				CAttackObject_Energy::ATTACK_RANGED_DESC Desc{};
				if (m_iPlayerTeam == 1)
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Energy_Attack;
				else
					Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Energy_Attack;
				Desc.ColliderDesc.pMineGameObject = this;
				Desc.ColliderDesc.vExtents = { 0.f,0.f,1.f };
				Desc.ColliderDesc.vCenter = { -10.f * m_iLookDirection,1.2f,0.f };
				//Desc.ColliderDesc.pTransform = m_pTransformCom;
				//Desc.fhitCharacter_Impus = { 20.f * m_iLookDirection,0.1f };
				Desc.fhitCharacter_Impus = {  };

				Desc.fhitCharacter_StunTime = 10.f;
				//Desc.iDamage = 120 * Get_DamageScale();

				//if(m_bFinalSkillss3)

				Desc.iDamage = 20 * Get_DamageScale(true);
				Desc.fLifeTime = 7.f;
				Desc.ihitCharacter_Motion = { HitMotion::HIT_NONE };
				Desc.iTeam = m_iPlayerTeam;
				//Desc.bGroundSmash = true;
				Desc.fAnimationLockTime = 0.0f;
				Desc.pOwner = this;
				Desc.eAttackGrade = { GRADE_ULTIMATE };
				Desc.bCameraZoom = false;

				Desc.fStartOffset = { -2.f,0.f };
				Desc.fMoveSpeedNoneDirection = { 20.f,0.f };

				Desc.iAttackCount = 50;
				Desc.iPlayerDirection = m_iLookDirection;		//
				Desc.iGainHitCount = 2;
				Desc.iGainAttackStep = 0;
				//Desc.fAttackDelayTime = 0.04f;  //꽤 자주 풀히트 못함
				//Desc.fAttackDelayTime = 0.03f; //종종 풀히트 못함
				Desc.fAttackDelayTime = 0.03f; //QTE 성공했을때만 못함?

				Desc.eEnergyColor = CAttackObject_Energy::ENERGY_LIGHT_BLUE;

				Desc.fColliderfCY = 1.2f;

				//Desc.bGrabbedEnd = true;
				Desc.bCameraZoom = false;

				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack_Energy"), TEXT("Layer_AttackObject"), &Desc);

			}
			Add_Move({ 3.f * m_iLookDirection,0.f });

		}
		//Position 148.2
		else if (iAttackEvent == 202)
		{
			//동시 실행 방지
			if (isNearlyEqual(m_pModelCom->m_fCurrentAnimPosition, 148))
			{
				return;
			}

			else
			{
				m_bInvisible = true;
				Character_Make_Effect(TEXT("Moving_Line_Right"));
			}


		}
		//162 ~ 255는 Update로 해결

		//255 클로즈업 ,  다음애니메이션 설정, START의 240으로 이동
		else if (iAttackEvent == 4)
		{
			m_bInvisible = false;

			Set_NextAnimation(ANIME_214_FINAL_START, 100.f, 240.f);
		}
	}
	break;
	default:
		break;
	}

	/*





	*/
}

void CPlay_Hit::Play_Sound(_uint SoundName, _bool bisLoof, _float fvolume)
{
	m_pGameInstance->Play_Sound((CSound_Manager::SOUND_KEY_NAME)SoundName, bisLoof, fvolume);
}

void CPlay_Hit::Play_Group_Sound(_uint groupKey, _bool loop, _float volume)
{
	m_pGameInstance->Play_Group_Sound((CSound_Manager::SOUND_GROUP_KEY)groupKey, loop, volume);
}

void CPlay_Hit::Reset_PoseTime()
{
	m_fAccPoseTime = 0.f;
}


void CPlay_Hit::Add_YellowLight()
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
	LightDesc.fLifeTime = 1.f;
	LightDesc.strName = "Explosion";
	if (FAILED(m_pRenderInstance->Add_Effect_Light(LightDesc.strName, LightDesc)))
		return;
}

void CPlay_Hit::Add_BlueLight()
{
	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof(LIGHT_DESC));
	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	LightDesc.fRange = 30.f;

	LightDesc.vDiffuse = _float4(0.9f, 1.1f, 1.7f, 1.0f); // 파란빛 계열로 변경

	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = _float4(1.0f, 0.95f, 0.45f, 1.f);

	LightDesc.fAccTime = 0.f;
	LightDesc.fLifeTime = 2.7f;
	LightDesc.strName = "Ray";
	if (FAILED(m_pRenderInstance->Add_Effect_Light(LightDesc.strName, LightDesc)))
		return;
}


void CPlay_Hit::Update_214FinalInvisible()
{
	if (m_pModelCom->m_iCurrentAnimationIndex != CPlay_Hit::ANIME_214_FINAL_SUCESS)
		return;


	_float fCurrentAnimationPosition = m_pModelCom->m_fCurrentAnimPosition;

	if (fCurrentAnimationPosition < 150 || fCurrentAnimationPosition > 254.9)
		return;



	//if (fCurrentAnimationPosition == 164 || fCurrentAnimationPosition == 172 || fCurrentAnimationPosition == 178 ||
	//	fCurrentAnimationPosition == 183 || fCurrentAnimationPosition == 196 || fCurrentAnimationPosition == 202)
	//{
	//	m_bInvisible = false;
	//}

	if (isNearlyEqual(fCurrentAnimationPosition, 155) || isNearlyEqual(fCurrentAnimationPosition, 164) || isNearlyEqual(fCurrentAnimationPosition, 172) || isNearlyEqual(fCurrentAnimationPosition, 178) ||
		isNearlyEqual(fCurrentAnimationPosition, 183) || isNearlyEqual(fCurrentAnimationPosition, 196) || isNearlyEqual(fCurrentAnimationPosition, 202)
		|| isNearlyEqual(fCurrentAnimationPosition, 208) || isNearlyEqual(fCurrentAnimationPosition, 217) || isNearlyEqual(fCurrentAnimationPosition, 225) || isNearlyEqual(fCurrentAnimationPosition, 148))
	{
		m_bInvisible = false;
	}

	else
	{
		m_bInvisible = true;
	}


}

_bool CPlay_Hit::isNearlyEqual(_float CurValue, _float TargetValue)
{

	return std::fabs(CurValue - TargetValue) <= 0.1;

}

AttackColliderResult CPlay_Hit::Set_Hit4(_uint eAnimation, AttackGrade eAttackGrade, AttackType eAttackType, _float fStunTime, _uint iDamage, _float fStopTime, _short iDirection, _float2 Impus)
{
	if (m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Air || m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Ground || m_pModelCom->m_iCurrentAnimationIndex == m_iBound_Ground)
		return RESULT_MISS;


	if (m_bCounterPose)
	{
		m_bCounterSucces = true;

		//시간정지를 여기서? 아니면 따로?
		Set_AnimationStopWithoutMe(0.5f);
		Set_AnimationStop(0.1f);
		m_bInvisible = true;

		//반격 성공 이펙트 만들기
		Character_Make_Effect(TEXT("Moving_Line_Right"));

		CMain_Camera* main_Camera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")));
		main_Camera->StartCameraShake(0.1f, 0.3f);


		if (m_pModelCom->m_iCurrentAnimationIndex == ANIME_ATTACK_SPECIAL)
		{
			Set_CurrentAnimationPositionJump(24.97f);
		}
		else if (m_pModelCom->m_iCurrentAnimationIndex == ANIME_ATTACK_SPECIAL_AIR)
		{
			Set_CurrentAnimationPositionJump(34.99f);
		}

		return RESULT_MISS;
	}
	//스턴상태, 땅바닥에 꽂혔을때 검사
	else if (m_bHitGroundSmashed && Get_fHeight() == 0)
	{
		if (eAttackGrade != GRADE_ULTIMATE)
			return RESULT_MISS;
	}


	m_bInvisible = false;
	m_b236Posing = false;
	m_b236Special = false;
	m_b214Posing = false;
	m_fAccPoseTime = 0.f;


	//스턴 상태가 아니면 가드 체크
	if (m_bStun == false)
	{

		AttackColliderResult eResult = Guard_Check3(eAttackType);

		//공격자의 방향이 iDirection  피격자는 마주봐야하니 그 반대.


		if (eResult == RESULT_GUARD)
		{
			if (m_pModelCom->m_iCurrentAnimationIndex == m_iCrouchAnimationIndex)
				Set_Animation(m_iGuard_CrouchAnimationIndex);

			else if (m_pModelCom->m_iCurrentAnimationIndex == m_iIdleAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iBackWalkAnimationIndex)
				Set_Animation(m_iGuard_GroundAnimationIndex);

			else if (m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex)
				Set_Animation(m_iGuard_AirAnimationIndex);


			//HitMotion { HIT_LIGHT, HIT_CHASE, HIT_MEDIUM, HIT_HEAVY, HIT_HEAVY_DOWN, HIT_CROUCH_MEDIUM, 
			// HIT_KNOCK_AWAY_LEFT, HIT_KNOCK_AWAY_UP, HIT_KNOCK_AWAY_LEFTDOWN, HIT_SPIN_AWAY_LEFTUP, HIT_WALLBOUNCE, HIT_NONE };


			//가드 이펙트
			switch (eAnimation)
			{
			case Client::HIT_LIGHT:
			case Client::HIT_CHASE:
				m_pEffect_Manager->Copy_Layer(TEXT("Guard01"), m_pTransformCom->Get_WorldMatrixPtr());
				break;


			case Client::HIT_CROUCH_MEDIUM:
			case Client::HIT_MEDIUM:
				m_pEffect_Manager->Copy_Layer(TEXT("Guard02"), m_pTransformCom->Get_WorldMatrixPtr());
				break;

			case Client::HIT_HEAVY:
			case Client::HIT_HEAVY_DOWN:
			case Client::HIT_KNOCK_AWAY_LEFT:
			case Client::HIT_KNOCK_AWAY_UP:
			case Client::HIT_KNOCK_AWAY_LEFTDOWN:
			case Client::HIT_SPIN_AWAY_LEFTUP:
			case Client::HIT_SPIN_AWAY_UP:
				m_pEffect_Manager->Copy_Layer(TEXT("Guard03"), m_pTransformCom->Get_WorldMatrixPtr());
				break;

			case Client::HIT_WALLBOUNCE:
			case Client::HIT_NONE:
			default:
				break;
			}


			Set_CurrentAnimationPositionJump(0.f);

			return RESULT_GUARD;

		}
		else if (eResult != RESULT_HIT)    //회피나 비긴 경우 아니면 일단 속행.   저 경우는 나중에 따로 처리
			return eResult;


	}


	m_bHit = TRUE;
	m_bStun = true;

	//m_fMaxStunTime = fStunTime;

	//누적  콤보수 비례.7배까지,  땅에서는 또 0.9배? 대신 공중에서 1.2배?

	m_fMaxStunTime = fStunTime;// *(30 - CBattleInterface_Manager::Get_Instance()->Get_HitAttackStep(m_iPlayerTeam);

	if (Get_fHeight() > 1)
	{
		m_fMaxStunTime *= 1.1f;
	}

	m_fMaxStunTime *= 1.f - 0.01f * CBattleInterface_Manager::Get_Instance()->Get_HitAttackStep(m_iPlayerTeam);



	m_fAccStunTime = 0.f;

	Set_HitAnimation(eAnimation, Impus);
	Set_AnimationStop(fStopTime);

	Set_bRedHP(true);

	if (iDirection == 1)
		FlipDirection(-1);
	else if (iDirection == -1)
		FlipDirection(1);

	//0인경우는 뒤집지 않음

	m_iHP -= iDamage;


	m_iDebugComoboDamage += iDamage;

	cout << "Dagage : " << iDamage << "  ,  Total : " << m_iDebugComoboDamage << endl;

	if (m_iHP < 0)
	{
		m_iHP = 0;
	}

	return RESULT_HIT;
}

void CPlay_Hit::MoveToEnemy_Ground(_float fMaxDistance, _float fOffset)
{
	// 나2
	//적 9
	//거리 7

	_float fEnemyPosX = m_pEnemy->Get_fPositionX();
	_float fMyPosX = Get_fPositionX();

	//오른쪽 보고있는경우
	if (m_iLookDirection == 1)
	{
		//적이 왼쪽에 있는경우엔 제자리
		if (fEnemyPosX <= fMyPosX)
			return;


		//이제 나머지 경우는 적이 오른쪽임.

		_float fDistance = fEnemyPosX - fMyPosX;

		//적이 오른쪽이고, 거리가 최대치보다 먼 경우 최대치까지만 이동
		if (fDistance > fMaxDistance)
		{
			Add_Move({ fMaxDistance,0.f });
		}

		//적이 오른쪽이고, 거리가 최대치보다 작은경우 적의 앞까지 이동
		else //if (fDistance <= fMaxDistance)
		{
			//myPos가 fEnemyPos가 되도록 fDistance만큼 이동해야하는데 Offset이 빠져야한다?
			Add_Move({ fDistance - fOffset,0.f });
		}


	}

	//왼쪽을 보고 있는 경우
	else if (m_iLookDirection == -1)
	{
		//적이 오른에 있는경우엔 제자리
		if (fEnemyPosX >= fMyPosX)
			return;


		//이제 나머지 경우는 적이 왼쪽임.

		_float fDistance = fMyPosX - fEnemyPosX;

		//적이 왼쪽이고, 거리가 최대치보다 먼 경우 최대치까지만 이동
		if (fDistance > fMaxDistance)
		{
			Add_Move({ -fMaxDistance,0.f });
		}

		//적이 오른쪽이고, 거리가 최대치보다 작은경우 적의 앞까지 이동
		else //if (fDistance <= fMaxDistance)
		{
			//myPos가 fEnemyPos가 되도록 fDistance만큼 이동해야하는데 Offset이 빠져야한다?
			Add_Move({ -fDistance + fOffset,0.f });
		}
	}



}



CPlay_Hit* CPlay_Hit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlay_Hit* pInstance = new CPlay_Hit(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CPlay_Hit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlay_Hit::Clone(void* pArg)
{
	CPlay_Hit* pInstance = new CPlay_Hit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CPlay_Hit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlay_Hit::Free()
{
	__super::Free();


	//Safe_Release(m_pShaderCom);
	//Safe_Release(m_pModelCom);

	Safe_Release(m_p2PTextureCom);

}
