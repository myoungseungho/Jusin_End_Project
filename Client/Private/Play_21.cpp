#include "stdafx.h"
#include "..\Public\Play_21.h"

#include "RenderInstance.h"
#include "GameInstance.h"

#include "UI_Manager.h"

//#define ANIME_ATTACK_LIGHT1 43
//#define ANIME_ATTACK_LIGHT2 44
//#define ANIME_ATTACK_LIGHT3 47
//
//#define ANIME_ATTACK_MEDIUM 46
//
//#define ANIME_ATTACK_HEAVY 45
//
//#define ANIME_IDLE 0
//#define ANIME_FORWARD_WALK 9
//#define ANIME_BACK_WALK 10
//
//
//#define ANIME_ATTACK_236 66
//#define ANIME_ATTACK_236_Air 67
//
//#define ANIME_ATTACK_236_Air 67
//
//
//#define ANIME_JUMP_UP 6;
//#define ANIME_JUMP_DOWN 7;


#include "iostream"
#include "Animation.h"


#include "AttackObject_Grab.h"
#include "AttackObject_CommandGrab.h"
#include "Main_Camera.h"

#include "BattleInterface.h"

CPlay_21::CPlay_21(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCharacter{ pDevice, pContext }
{

}

CPlay_21::CPlay_21(const CPlay_21& Prototype)
	: CCharacter{ Prototype }
{

}

HRESULT CPlay_21::Initialize_Prototype()
{
	//m_pFrameEvent = CFrameEvent_Manager::Get_Instance()->Get_pFrameEventMap();
	CFrameEvent_Manager::Get_Instance()->Initalize_NameMap();
	CFrameEvent_Manager::Get_Instance()->LoadFile2("../Bin/FrameEventData/EventData_21.txt");



	return S_OK;
}

HRESULT CPlay_21::Initialize(void* pArg)
{

	//Play_21_DESC* pDesc = static_cast<Play_21_DESC*>(pArg);
	//m_strModelName = pDesc->strModelName;

	//m_pFrameEvent = CFrameEvent_Manager::Get_Instance()->Get_pFrameEventMap();

	m_eCharacterIndex = PLAY_21;
	m_iFallAnimationIndex = ANIME_JUMP_DOWN;
	m_iIdleAnimationIndex = ANIME_IDLE;
	m_iJumpAnimationIndex = ANIME_JUMP_UP;
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

	m_iAttack_Air1 = { ANIME_ATTACK_AIR1 };
	m_iAttack_Air2 = { ANIME_ATTACK_AIR2 };
	m_iAttack_Air3 = { ANIME_ATTACK_AIR3 };
	m_iAttack_AirUpper = { ANIME_ATTACK_UPPER_AIR };

	m_iAttack_LightLast = { ANIME_ATTACK_LIGHT3 };
	m_iAttack_Heavy = { ANIME_ATTACK_HEAVY };

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


	m_iNextAnimation.first = ANIME_IDLE;




	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Character_DESC* pDesc = static_cast<Character_DESC*>(pArg);
	m_strName = "S21_" + to_string(m_iPlayerTeam) + to_string(pDesc->ePlayerSlot);
	m_RendererDesc.strName = m_strName;

	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;

	LightDesc.vDirection = _float4(-0.06f, -0.07f, 0.1f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.0f, 1.0f);
	LightDesc.vAmbient = _float4(0.5f, 0.5f, 0.5f, 1.f);
	LightDesc.vSpecular = _float4(0.f, 0.f, 0.f, 1.f);
	LightDesc.pPlayerDirection = &m_iLookDirection;
	LightDesc.strName = m_strName;

	if (FAILED(m_pRenderInstance->Add_Player_Light(m_strName, LightDesc)))
		return E_FAIL;

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-2.f + (m_iPlayerTeam * 2), 0.f, 0.f, 1.f));
	m_tAttackMap.Initalize(this);

	//m_pModelCom->SetUp_Animation(16, true);
	m_pModelCom->SetUp_Animation(ANIME_IDLE, true);
	m_pModelCom->Play_Animation(0.f);




	//MoveCommandPatterns.push_back({ hadoukenPattern, ShowInputBuffer});

	MoveCommandPatterns.push_back({ Command_236Attack, 66 });
	MoveCommandPatterns.push_back({ Command_236Attack_Extra, 66 });

	MoveCommandPatterns.push_back({ Command_LightAttack, ANIME_ATTACK_LIGHT1 });
	MoveCommandPatterns.push_back({ Command_MediumAttack, ANIME_ATTACK_MEDIUM });
	MoveCommandPatterns.push_back({ Command_HeavyAttack, ANIME_ATTACK_HEAVY });


	MoveCommandPatternsFunction.push_back({ Command_236UltimateAttack, bind(&CS21_MeleeAttack::Attack_236Ultimate, &m_tAttackMap) });
	//MoveCommandPatternsFunction.push_back({ Command_236UltimateAttack_Side, bind(&CS21_MeleeAttack::Attack_236Ultimate_Side, &m_tAttackMap) });




	MoveCommandPatternsFunction.push_back({ Command_236Attack,  bind(&CS21_MeleeAttack::Attack_236, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_236Attack_Extra,  bind(&CS21_MeleeAttack::Attack_236, &m_tAttackMap) });

	MoveCommandPatternsFunction.push_back({ Command_214Attack,  bind(&CS21_MeleeAttack::Attack_214, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_214Attack_Extra,  bind(&CS21_MeleeAttack::Attack_214, &m_tAttackMap) });

	MoveCommandPatternsFunction.push_back({ Command_236Special,  bind(&CS21_MeleeAttack::Attack_236Special, &m_tAttackMap) });
	//MoveCommandPatternsFunction.push_back({ Command_236Special_Side,  bind(&CS21_MeleeAttack::Attack_236Special_Side, &m_tAttackMap) });


	MoveCommandPatternsFunction.push_back({ Command_214FinalAttack, bind(&CS21_MeleeAttack::Attack_214Final, &m_tAttackMap) });


	MoveCommandPatternsFunction.push_back({ Command_Crouch_LightAttack, bind(&CS21_MeleeAttack::Attack_Crouch_Light, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_Crouch_MediumAttack, bind(&CS21_MeleeAttack::Attack_Crouch_Medium, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_Crouch_HeavyAttack, bind(&CS21_MeleeAttack::Attack_Crouch_Heavy, &m_tAttackMap) });


	MoveCommandPatternsFunction.push_back({ Command_Crouch_MediumAttack_Extra, bind(&CS21_MeleeAttack::Attack_Crouch_Medium, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_Crouch_HeavyAttack_Extra, bind(&CS21_MeleeAttack::Attack_Crouch_Heavy, &m_tAttackMap) });



	//위에서 부터 확인하므로 간단한 커맨드가 아래로 가야함
	MoveCommandPatternsFunction.push_back({ Command_LightAttack, bind(&CS21_MeleeAttack::Attack_Light, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_MediumAttack, bind(&CS21_MeleeAttack::Attack_Medium, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_HeavyAttack, bind(&CS21_MeleeAttack::Attack_Heavy, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_SpecialAttack, bind(&CS21_MeleeAttack::Attack_Special, &m_tAttackMap) });


	MoveCommandPatternsFunction.push_back({ Command_HeavyAttack_Extra, bind(&CS21_MeleeAttack::Attack_Heavy, &m_tAttackMap) });



	MoveCommandPatternsFunction.push_back({ Command_Grab, bind(&CS21_MeleeAttack::Attack_Grab, &m_tAttackMap) });


	MoveCommandPatternsFunction.push_back({ Command_BackDash, bind(&CS21_MeleeAttack::BackDash, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_Forward, bind(&CS21_MeleeAttack::ForwardDash, &m_tAttackMap) });

	MoveCommandPatternsFunction.push_back({ Command_Crouch_SpecialAttack, bind(&CS21_MeleeAttack::Attack_Crouch_Speical, &m_tAttackMap) });

	m_eCharacterID = CUI_Define::PLAYER_ID::ANDROID21;
	CBattleInterface_Manager::Get_Instance()->Regist_Character(m_iPlayerTeam, this, m_ePlayerSlot);
	if (m_ePlayerSlot != CUI_Define::PLAYER_SLOT::LPLAYER1 && m_ePlayerSlot != CUI_Define::PLAYER_SLOT::RPLAYER1)
	{
		m_bPlaying = false;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(5.f * m_iPlayerTeam, 100.f, 0.f, 1.f));

		//캐릭터 사이즈에 맞게 각자 추가하느라 m_pColliderCom이 없음
		m_pColliderCom->UpdateVector(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	}
	else
		m_bPlaying = true;


	//if (::AllocConsole() == TRUE)
	//{
	//	FILE* nfp[3];
	//	freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
	//	freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
	//	freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
	//	std::ios::sync_with_stdio();
	//}

	return S_OK;
}

void CPlay_21::Player_Update(_float fTimeDelta)
{

	__super::Player_Update(fTimeDelta);


	if (m_bPlaying == false)
		return;


	if (m_pGameInstance->Key_Down(DIK_TAB))
	{
		m_bDebugInputLock = !m_bDebugInputLock;
	}

	if (m_bDebugInputLock)
		return;


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


	Update_LoofAnimationCreate(fTimeDelta);
	Update_PreviousXPosition();

	if (m_bGrabbed)
	{
		Character_Play_Animation(fTimeDelta);
		m_pColliderCom->UpdateVector(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		return;
	}

	//if (m_pGameInstance->Key_Down(DIK_F3))
	//	m_pUI_Manager->UsingChangeCharacher(m_ePlayerSlot);

	//합치기 전 임시 코드.  적 탐지코드임
	//if (m_pEnemy == nullptr)
	//{
	//	_short i = m_pGameInstance->Get_LayerSize(LEVEL_GAMEPLAY, TEXT("Layer_Character"));
	//
	//	for (int i = 0; i < m_pGameInstance->Get_LayerSize(LEVEL_GAMEPLAY, TEXT("Layer_Character")); i++)
	//	{
	//		CGameObject* pObject = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Character"), i);
	//	
	//		if (pObject != this)
	//		{
	//			m_pEnemy = static_cast<CCharacter*>(pObject);
	//		}
	//	
	//	}
	//	
	//}
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
				if (m_bStun == false)
				{
					m_fAccChaseTime = 0.f;
					m_fGravityTime = 0.185f;
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
			Update_NoEventAnimationLoof(fTimeDelta);
		}
		else
			Character_Play_Animation(fTimeDelta);

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




	Move(fTimeDelta);











	m_pColliderCom->UpdateVector(m_pTransformCom->Get_State(CTransform::STATE_POSITION));


	_float fPosX = Get_fPositionX();

	//12(벽) 이상 넘어가지 못하게 
	//if (fPosX > 12)
	//{
	//	Add_Move({ 12.f - fPosX, 0.f });
	//}
	//else if (fPosX < -12)
	//{
	//	Add_Move({ -12.f - fPosX, 0.f });
	//}

	if (Check_bWall())
	{
		//(Get_fPositionX() < -12.f || Get_fPositionX() > 12.f || fabsf(Get_fPositionX() - m_pEnemy->Get_fPositionX()) > 8);
		Move_ForWall();
	}

	Tag_KeyCheck();



	if (m_pGameInstance->Key_Down(DIK_8))
	{
		ShowInputBuffer();
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

	if (m_pGameInstance->Key_Down(DIK_F8))
	{
		Set_AnimationStopWithoutMe(1.f);
	}

	cout << "iHP : " << m_iHP << endl;


	Check_Ground();
}

void CPlay_21::Camera_Update(_float fTimeDelta)
{

}

void CPlay_21::Update(_float fTimeDelta)
{

	__super::Player_Update(fTimeDelta);

	/*



	InputedCommandUpdate(fTimeDelta);
	InputCommand();



	CheckAllCommands();

	if (m_bAnimationLock == false)
	{

		Character_Play_Animation(fTimeDelta);

		//이건 반복재생이 아닌데 모션이 끝난경우 (=움직임 자체가 멈췄을 경우),  추락 등 몇몇 애니메이션 제외
		if (m_bMotionPlaying == false)
		{
			if(m_pModelCom->m_iCurrentAnimationIndex != ANIME_JUMP_DOWN)
				AnimeEndNextMoveCheck();
		}


	}
	else
	{
		m_fAccAnimationLock += fTimeDelta;
		if (m_fAccAnimationLock > m_fMaxAnimationLock)
		{
			m_fAccAnimationLock = 0.f;
			m_bAnimationLock = false;
		}
	}



	Gravity(fTimeDelta);


	if (Check_bCurAnimationisGroundMove() || m_pModelCom->m_iCurrentAnimationIndex ==ANIME_FORWARD_DASH)
	{
		//if ((m_iNextAnimation.first == ANIME_IDLE) || ((m_iNextAnimation.first == ANIME_FORWARD_WALK) || (m_iNextAnimation.first == ANIME_BACK_WALK)))
		if(Check_bCurAnimationisGroundMove(m_iNextAnimation.first))
		{
			Reset_AttackCount();

			_short MoveKey = 0;
			if (m_pGameInstance->Key_Pressing(DIK_W))
			{
				m_pTransformCom->Add_Move({ 0,3,0 });
				Set_Animation(ANIME_JUMP_UP);
			}

			else if (m_pGameInstance->Key_Pressing(DIK_S))
			{
				//if (m_pModelCom->m_iCurrentAnimationIndex != ANIME_CROUCHING)
				if (m_pModelCom->m_iCurrentAnimationIndex != ANIME_FORWARD_DASH)
				{
					m_pModelCom->SetUp_Animation(ANIME_CROUCHING, true);
				}

			}

			else
			{
				if (m_pGameInstance->Key_Pressing(DIK_A))
				{
					MoveKey -= m_iLookDirection;
				}

				else if (m_pGameInstance->Key_Pressing(DIK_D))
				{
					MoveKey += m_iLookDirection;
				}


				if (MoveKey == -1)
				{

					//if (m_pModelCom->m_iCurrentAnimationIndex == ANIME_BACK_DASH)
					//{
					//	m_pModelCom->SetUp_Animation(ANIME_BACK_DASH, false);
					//}
					//else
						m_pModelCom->SetUp_Animation(ANIME_BACK_WALK, false);


					m_iNextAnimation.first = ANIME_IDLE;
					m_iNextAnimation.second = 100.f;

				}
				else if (MoveKey == 1)
				{
					if (m_pModelCom->m_iCurrentAnimationIndex == ANIME_FORWARD_DASH)
					{
						m_pModelCom->SetUp_Animation(ANIME_FORWARD_DASH, true);
					}
					else
						m_pModelCom->SetUp_Animation(ANIME_FORWARD_WALK, false);

					m_iNextAnimation.first = ANIME_IDLE;
					m_iNextAnimation.second = 100.f;
				}
				else
				{
					if (m_pModelCom->m_iCurrentAnimationIndex == ANIME_FORWARD_DASH)
					{
						m_pModelCom->SetUp_Animation(ANIME_FORWARD_DASH_END, false);
					}
					else
						m_pModelCom->SetUp_Animation(ANIME_IDLE, true);

					m_iNextAnimation.first = ANIME_IDLE;
					m_iNextAnimation.second = 100.f;
				}
			}


			//앉기 추가 전에는 이거만 있었음
			//if (m_pGameInstance->Key_Pressing(DIK_A))
			//{
			//	MoveKey -= m_iLookDirection;
			//}
			//
			//else if (m_pGameInstance->Key_Pressing(DIK_D))
			//{
			//	MoveKey += m_iLookDirection;
			//}
			//
			//
			//if (MoveKey == -1)
			//{
			//	m_pModelCom->SetUp_Animation(ANIME_BACK_WALK, false);
			//	m_iNextAnimation.first = ANIME_IDLE;
			//	m_iNextAnimation.second = 100.f;
			//
			//}
			//else if (MoveKey == 1)
			//{
			//	m_pModelCom->SetUp_Animation(ANIME_FORWARD_WALK, false);
			//	m_iNextAnimation.first = ANIME_IDLE;
			//	m_iNextAnimation.second = 100.f;
			//}
			//else
			//{
			//	m_pModelCom->SetUp_Animation(ANIME_IDLE, true);
			//	m_iNextAnimation.first = ANIME_IDLE;
			//	m_iNextAnimation.second = 100.f;
			//}


		}
	}


	if (m_pGameInstance->Key_Down(DIK_8))
	{
		ShowInputBuffer();
	}
	if (m_pGameInstance->Key_Down(DIK_1))
	{
		DebugPositionReset();
	}

	if (m_pGameInstance->Key_Down(DIK_2))
	{
		FlipDirection();
	}
	if (m_pGameInstance->Key_Down(DIK_3))
	{
		m_pModelCom->SetUp_Animation(0,true);
	}


	Check_Ground();
	*/



}

void CPlay_21::Late_Update(_float fTimeDelta)
{
	if(m_bPlaying)
		m_pRenderInstance->Add_RenderObject(CRenderer::RG_PLAYER, this, &m_RendererDesc);

	//#ifdef _DEBUG
	//	m_pRenderInstance->Add_DebugComponent(m_pColliderCom);
	//#endif

	if (m_iPlayerTeam == 2)
	{
		_bool bDebug = true;
	}
}

HRESULT CPlay_21::Render(_float fTimeDelta)
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

		if (FAILED(m_pShaderCom->Begin(1)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}
#ifdef _DEBUG
	m_pColliderCom->Render(fTimeDelta);
#endif // DEBUG
	return S_OK;
}

/*
void CPlay_21::NextMoveCheck()
{

	//지금 당장 공격중에 체크했을때 IDLE이면 변경하지 않고 공격을 마저 진행함

	//점프,이동 중이 아니라는 뜻


	//if (m_pModelCom->m_iCurrentAnimationIndex < m_iNextAnimation.first)
	//{
	//	//Set_Animation(m_iNextAnimation.first);
	//	m_pModelCom->SetUp_Animation(m_iNextAnimation.first,false);
	//	m_iNextAnimation.first = ANIME_IDLE;
	//	m_iNextAnimation.second = 100;
	//
	//}



	//공격중인경우  로 바꾸자  *체크*
	//if (Check_bCurAnimationisGroundMove() == false)
	//{
	//	//다음 애니메이션도 공격이 아니면 Set
	//	//if (Check_bCurAnimationisMove(m_iNextAnimation.first))
	//	//{
	//	//	Set_Animation(m_iNextAnimation.first);
	//	//}
	//
	//	if (m_iNextAnimation.first != ANIME_IDLE)
	//	{
	//		Set_Animation(m_iNextAnimation.first);
	//	}
	//
	//}

	// 체크*
	//if(m_iNextAnimation.first != ANIME_IDLE)
	//{
	//	m_pModelCom->SetUp_Animation(m_iNextAnimation.first, false);
	//
	//	m_iNextAnimation.first = ANIME_IDLE;
	//	m_iNextAnimation.second = 100.f;
	//}


}
*/


/*
void CPlay_21::AttackNextMoveCheck()
{

	//if (m_iNextAnimation.first != ANIME_IDLE)
	//{
	//	Set_Animation(m_iNextAnimation.first);
	//
	//	m_iNextAnimation.first = ANIME_IDLE;
	//	m_iNextAnimation.second = 1000.f;
	//
	//}

	if (m_iNextAnimation.first != ANIME_IDLE)
	{
		Set_Animation(m_iNextAnimation.first);

		m_iNextAnimation.first = ANIME_IDLE;
		m_iNextAnimation.second = 1000.f;

		if (m_fNextAnimationCurrentPosition != 0)
		{
			m_pModelCom->CurrentAnimationPositionJump(m_fNextAnimationCurrentPosition);
			m_fNextAnimationCurrentPosition = 0.f;
		}
	}


}

void CPlay_21::AnimeEndNextMoveCheck()
{

	Set_Animation(m_iNextAnimation.first);

	m_iNextAnimation.first = ANIME_IDLE;
	m_iNextAnimation.second = 1000.f;




}

*/

void CPlay_21::Test_InputCommand()
{
	//if (m_pGameInstance->Key_Down(DIK_U))
	//{
	//	m_pModelCom->SetUp_Animation(ANIME_ATTACK_LIGHT1,false);
	//}
	//
	//if (m_pGameInstance->Key_Down(DIK_I))
	//{
	//	m_pModelCom->SetUp_Animation(ANIME_ATTACK_MEDIUM, false);
	//
	//}
	//
	//
	//if (m_pGameInstance->Key_Down(DIK_K))
	//{
	//	m_pModelCom->SetUp_Animation(ANIME_ATTACK_HEAVY, false);
	//
	//}


}




/*
void CPlay_21::Set_Animation(_uint iAnimationIndex)
{


	//if(iAnimationIndex !=ANIME_IDLE)
	//	m_pModelCom->SetUp_Animation(iAnimationIndex,false);
	//
	//else
	//	m_pModelCom->SetUp_Animation(ANIME_IDLE, true);



	//ver2

	//if (m_pModelCom->m_iCurrentAnimationIndex != ANIME_IDLE)
	//{
	//	m_iNextAnimation.first = iAnimationIndex;
	//	m_iNextAnimation.second = 0.5f;
	//	return;
	//}
	//
	//
	//
	//
	//_uint iDebug = iAnimationIndex;
	//if (iAnimationIndex != 0 && (iAnimationIndex != ANIME_FORWARD_WALK && iAnimationIndex != ANIME_BACK_WALK))
	//{
	//	_bool bDebug = true;
	//}
	//
	//if(iAnimationIndex != ANIME_IDLE)
	//	m_pModelCom->SetUp_Animation(iAnimationIndex, false);



	//if (iAnimationIndex != ANIME_IDLE)
	//	m_pModelCom->SetUp_Animation(iAnimationIndex, true);
	//else
	//	m_pModelCom->SetUp_Animation(iAnimationIndex, false);



	if (iAnimationIndex == ANIME_IDLE)
		m_pModelCom->SetUp_Animation(iAnimationIndex, true);
	else
		m_pModelCom->SetUp_Animation(iAnimationIndex, false);


}
*/

void CPlay_21::KeyTest()
{

}



HRESULT CPlay_21::Ready_Components()
{
	/* Com_Shader */
	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
	//	TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	//	return E_FAIL;
	//
	///* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Play_21"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* Com_Model */
	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_strModelName,
	//	TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
	//	return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC ColliderDesc{};
	if (m_iPlayerTeam == 1)
		ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_BODY;
	else
		ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_BODY;
	ColliderDesc.pMineGameObject = this;
	ColliderDesc.vCenter = { 0.f,0.9f,0.f };
	ColliderDesc.vExtents = { 0.5f,0.8f,1.f };


	//Com_Collider
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	m_pGameInstance->Add_ColliderObject(ColliderDesc.colliderGroup, m_pColliderCom);

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_21OutLine"), TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pOutLineCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlay_21::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

void CPlay_21::Check_Ground()
{

	//_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float fHeight = XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	if (fHeight <= fGroundHeight)
	{
		if (m_pModelCom->m_iCurrentAnimationIndex == ANIME_ATTACK_214)
		{
			if (m_pModelCom->Get_CurrentAnimationPosition() < 55)
			{
				m_pModelCom->CurrentAnimationPositionJump(55.f);
				//m_pModelCom->m_Animations[m_pModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 90.f;
				m_fGravityTime = 0.f;

				Reset_AttackCount();
			}
		}


	}

}

_bool CPlay_21::Check_bCurAnimationisGroundMove(_uint iAnimation)
{
	_uint iModelIndex = iAnimation;

	if (iAnimation == 1000)
		iModelIndex = m_pModelCom->m_iCurrentAnimationIndex;


	//if (iModelIndex == ANIME_IDLE || (iModelIndex == ANIME_FORWARD_WALK || iModelIndex == ANIME_BACK_WALK) )
	//{
	//	return true;
	//}


	//대시 추가하느라 주석
	//if (iModelIndex == ANIME_IDLE || (iModelIndex == ANIME_FORWARD_WALK || iModelIndex == ANIME_BACK_WALK) || iModelIndex == ANIME_CROUCH_START || iModelIndex == ANIME_CROUCHING)
	//{
	//	return true;
	//}


	if (iModelIndex == ANIME_IDLE || iModelIndex == ANIME_FORWARD_WALK || iModelIndex == ANIME_BACK_WALK || iModelIndex == ANIME_CROUCH_START || iModelIndex == ANIME_CROUCHING
		|| iModelIndex == ANIME_FORWARD_DASH)
	{
		return true;
	}

	return false;
}

_bool CPlay_21::Check_bCurAnimationisAttack(_uint iAnimation)
{

	return false;
}

_bool CPlay_21::Check_bCurAnimationisAirAttack(_uint iAnimation)
{
	_uint iModelIndex = iAnimation;

	if (iAnimation == 1000)
		iModelIndex = m_pModelCom->m_iCurrentAnimationIndex;



	if (iModelIndex == ANIME_ATTACK_AIR1 || iModelIndex == ANIME_ATTACK_AIR2 || iModelIndex == ANIME_ATTACK_AIR3)// || iModelIndex == ANIME_ATTACK_SPECIAL_AIR)
	{
		return true;
	}

	return false;
}

void CPlay_21::Reset_AttackCount()
{

	for (size_t i = 0; i < COUNT_END; i++)
		m_bAttackCount[i] = true;

	m_bChaseEnable = true;
	m_bDoubleJumpEnable = true;
}

void CPlay_21::Gravity(_float fTimeDelta)
{

	if (m_pGameInstance->Key_Down(DIK_0))
	{
		_bool bDebug = true;
	}


	__super::Gravity(fTimeDelta);



	//_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//_float fHeight = XMVectorGetY(vPos);

	_ushort iAnimationIndex = m_pModelCom->m_iCurrentAnimationIndex;

	if (iAnimationIndex == ANIME_ATTACK_214 && m_pModelCom->m_fCurrentAnimPosition > 30.f && m_pModelCom->m_fCurrentAnimPosition < 50.f)
	{
		Add_Move({ m_fImpuse.x * fTimeDelta , m_fImpuse.y * fTimeDelta });  //땅에 닿았을때 처리는 현재 GroundCheck 에 있음
	}

	if (iAnimationIndex == ANIME_ATTACK_AIR1 || iAnimationIndex == ANIME_ATTACK_AIR2 || iAnimationIndex == ANIME_ATTACK_AIR3)
	{
		//if (fHeight == 0)
		if (Get_fHeight() == 0)
		{

			//m_pModelCom->SetUp_Animation(m_iIdleAnimationIndex, true);
			Set_Animation(m_iIdleAnimationIndex);


			Set_fGravityTime(0.f);
			//Set_fJumpPower(0.f);
			Set_fImpulse(0.f);
			m_bAriDashEnable = true;
			Set_bAttackGravity(true);
		}

	}

}

void CPlay_21::AttackEvent(_int iAttackEvent, _int AddEvent)
{
	switch (m_pModelCom->m_iCurrentAnimationIndex)
	{
	case Client::CPlay_21::ANIME_ATTACK_LIGHT1:
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
		Desc.fhitCharacter_StunTime = 0.3f;
		Desc.iDamage = 400 * Get_DamageScale();
		Desc.fLifeTime = 0.1f;
		Desc.ihitCharacter_Motion = { HitMotion::HIT_LIGHT };
		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.1f;
		Desc.pOwner = this;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}
	break;
	case Client::CPlay_21::ANIME_ATTACK_LIGHT2:
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
		Desc.fhitCharacter_StunTime = 0.6f;
		Desc.iDamage = 5200 * Get_DamageScale(); //700이었음
		//Desc.iDamage = 700 * Get_DamageScale(); //700이었음
		Desc.fLifeTime = 0.1f;
		Desc.ihitCharacter_Motion = { HitMotion::HIT_LIGHT };
		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.1f;
		Desc.pOwner = this;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}
	break;
	case Client::CPlay_21::ANIME_ATTACK_LIGHT3:
	{

		CAttackObject::ATTACK_DESC Desc{};

		if (m_iPlayerTeam == 1)
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
		else
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
		Desc.ColliderDesc.pMineGameObject = this;
		Desc.ColliderDesc.vCenter = { 0.9f * m_iLookDirection,0.8f,0.f };
		Desc.ColliderDesc.vExtents = { 0.3f,0.5f,0.2f };


		Desc.fhitCharacter_Impus = { 4.f * m_iLookDirection,2.f };
		Desc.fhitCharacter_StunTime = 0.6f;


		Desc.iDamage = 1000 * Get_DamageScale();
		Desc.fLifeTime = 0.1f;
		Desc.ihitCharacter_Motion = { HitMotion::HIT_SPIN_AWAY_LEFTUP };
		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.7f;
		Desc.pOwner = this;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}
	break;
	case Client::CPlay_21::ANIME_ATTACK_MEDIUM:
	{
		CAttackObject::ATTACK_DESC Desc{};
		//Desc.ColliderDesc.width = 0.7;
		//Desc.ColliderDesc.height = 0.8;
		if (m_iPlayerTeam == 1)
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
		else
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
		Desc.ColliderDesc.pMineGameObject = this;
		Desc.ColliderDesc.vExtents = { 0.7f,0.8f,1.f };
		Desc.ColliderDesc.vCenter = { 0.9f * m_iLookDirection,0.8f,0.f };
		//Desc.ColliderDesc.pTransform = m_pTransformCom;
		Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };
		Desc.fhitCharacter_StunTime = 0.5f;
		Desc.iDamage = 700 * Get_DamageScale();;
		Desc.fLifeTime = 0.2f;
		Desc.ihitCharacter_Motion = { HitMotion::HIT_MEDIUM };
		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.1f;
		Desc.pOwner = this;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}
	break;
	case Client::CPlay_21::ANIME_ATTACK_HEAVY:
	{
		CAttackObject::ATTACK_DESC Desc{};
		/*	Desc.ColliderDesc.width = 1.0;
			Desc.ColliderDesc.height = 1.0;
			Desc.ColliderDesc.vCenter = { 1.0f * m_iLookDirection,0.8f,0.f };
			Desc.ColliderDesc.pTransform = m_pTransformCom;*/

		if (m_iPlayerTeam == 1)
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
		else
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
		Desc.ColliderDesc.pMineGameObject = this;
		Desc.ColliderDesc.vExtents = { 1.f,1.f,1.f };
		Desc.ColliderDesc.vCenter = { 1.0f * m_iLookDirection,0.8f,0.f };

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
	break;
	case Client::CPlay_21::ANIME_ATTACK_SPECIAL:
		break;
	case Client::CPlay_21::ANIME_ATTACK_AIR1:
	{

		CAttackObject::ATTACK_DESC Desc{};
		//Desc.ColliderDesc.width = 0.7;
		//Desc.ColliderDesc.height = 0.8;
		if (m_iPlayerTeam == 1)
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
		else
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
		Desc.ColliderDesc.pMineGameObject = this;
		Desc.ColliderDesc.vExtents = { 0.7f,0.8f,1.f };
		Desc.ColliderDesc.vCenter = { 0.9f * m_iLookDirection,0.8f,0.f };

		Desc.fhitCharacter_Impus = { m_fImpuse.x,0 };
		//Desc.fhitCharacter_Impus = { m_fImpuse.x * 0.9f,0 };


		Desc.fhitCharacter_StunTime = 0.5f;
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
	case Client::CPlay_21::ANIME_ATTACK_AIR2:
	{

		CAttackObject::ATTACK_DESC Desc{};
		//Desc.ColliderDesc.width = 0.7;
		//Desc.ColliderDesc.height = 0.8;
		if (m_iPlayerTeam == 1)
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
		else
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
		Desc.ColliderDesc.pMineGameObject = this;
		Desc.ColliderDesc.vExtents = { 0.7f,0.8f,1.f };
		Desc.ColliderDesc.vCenter = { 0.9f * m_iLookDirection,0.8f,0.f };
		//Desc.ColliderDesc.pTransform = m_pTransformCom;
		//Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };
		Desc.fhitCharacter_Impus = { m_fImpuse.x * 0.4f ,0 };

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
	case Client::CPlay_21::ANIME_ATTACK_AIR3:
	{
		CAttackObject::ATTACK_DESC Desc{};

		if (m_iPlayerTeam == 1)
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
		else
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
		Desc.ColliderDesc.pMineGameObject = this;
		Desc.ColliderDesc.vCenter = { 0.9f * m_iLookDirection,0.8f,0.f };
		Desc.ColliderDesc.vExtents = { 0.3f,0.5f,0.2f };


		Desc.fhitCharacter_Impus = { 4.f * m_iLookDirection,2.f };
		Desc.fhitCharacter_StunTime = 0.6f;


		Desc.iDamage = 1000 * Get_DamageScale();
		Desc.fLifeTime = 0.1f;
		if (Get_fHeight() > 0)
		{
			Desc.ihitCharacter_Motion = { HitMotion::HIT_SPIN_AWAY_LEFTUP };
			//Desc.bCameraZoom = false;
		}
		else
			Desc.ihitCharacter_Motion = { HitMotion::HIT_MEDIUM };

		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.7f;
		Desc.pOwner = this;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}
	break;
	case Client::CPlay_21::ANIME_ATTACK_CROUCH_LIGHT:
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
		Desc.fhitCharacter_StunTime = 0.3f;
		Desc.iDamage = 400 * Get_DamageScale();
		Desc.fLifeTime = 0.1f;
		Desc.ihitCharacter_Motion = { HitMotion::HIT_LIGHT };
		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.1f;
		Desc.pOwner = this;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}

	break;
	case Client::CPlay_21::ANIME_ATTACK_CROUCH_MEDUIM:
	{

		CAttackObject::ATTACK_DESC Desc{};
		//Desc.ColliderDesc.width = 0.7;
		//Desc.ColliderDesc.height = 0.8;
		if (m_iPlayerTeam == 1)
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
		else
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
		Desc.ColliderDesc.pMineGameObject = this;

		//오공처럼 쓰던것
		//Desc.ColliderDesc.vCenter = { 0.7f * m_iLookDirection,0.3f,0.f };
		Desc.ColliderDesc.vCenter = { 2.5f * m_iLookDirection,0.3f,0.f };


		Desc.ColliderDesc.vExtents = { 0.7f,0.5f,1.f };
		Desc.fhitCharacter_Impus = { -2.5f * m_iLookDirection,0.3f };

		//Desc.ColliderDesc.pTransform = m_pTransformCom;
		//Desc.fhitCharacter_Impus = { 1.f * m_iLookDirection,0.3f };
		//Desc.fhitCharacter_Impus = { 1.5f * m_iLookDirection,0.3f };
		//Desc.ColliderDesc.vExtents = { 0.7f,0.8f,1.f };


		Desc.fhitCharacter_StunTime = 1.f;
		Desc.iDamage = 700 * Get_DamageScale();
		Desc.fLifeTime = 0.1f;
		Desc.ihitCharacter_Motion = { HitMotion::HIT_CROUCH_MEDIUM };
		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.1f;
		//Desc.bOwnerGravityTimeReset = true;
		Desc.pOwner = this;

		Desc.eAttackType = { ATTACKTYPE_LOW };

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}
	break;

	case Client::CPlay_21::ANIME_ATTACK_CROUCH_HEAVY:
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
		Desc.fhitCharacter_Impus = { 0.2f * m_iLookDirection, 10.f };  //원래 가속도.
		//Desc.fhitCharacter_Impus = { 0.2f * m_iLookDirection, 3.f };  //공중 테스트용 임시

		Desc.fhitCharacter_StunTime = 1.0f;
		Desc.iDamage = 1000 * Get_DamageScale();;
		Desc.fLifeTime = 0.3f;
		Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_UP };
		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.5f;
		Desc.pOwner = this;


		if (Get_fHeight() > 2)
		{
			Desc.bCameraZoom = false;
			Desc.fhitCharacter_Impus = { 0.2f * m_iLookDirection, 4.f };
			Desc.fAnimationLockTime = 0.1f;
			Desc.fLifeTime = 0.1f;
		}

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}
	//공중 어퍼랑 같은 모션임


	break;
	break;
	case Client::CPlay_21::ANIME_ATTACK_CROUCH_SPECIAL:
		break;
	case Client::CPlay_21::ANIME_ATTACK_236:
	{


	}
	break;
	case Client::CPlay_21::ANIME_ATTACK_214:
	{
		//내려찍는중
		if (iAttackEvent == 0)
		{

			if (Get_fHeight() < 1)
				return;


			CAttackObject::ATTACK_DESC Desc{};

			if (m_iPlayerTeam == 1)
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
			else
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
			Desc.ColliderDesc.pMineGameObject = this;
			Desc.ColliderDesc.vCenter = { 0.3f * m_iLookDirection,0.8f,0.f };
			Desc.ColliderDesc.vExtents = { 0.8f,0.7f,0.2f };


			//캐릭터는 Look*2, -15 인데  오히려 좀 끌려오는 느낌으로?
			//Desc.fhitCharacter_Impus = { m_iLookDirection * 2.f, -15.f };

			Desc.fhitCharacter_Impus = { m_iLookDirection * -1.f, -15.f };


			Desc.fhitCharacter_StunTime = 10.f;	//일단잡기마냥 땅에 닿아야 풀리는 느낌 + 추가타로 풀어버리는 느낌


			Desc.iDamage = 100 * Get_DamageScale();
			//Desc.fLifeTime = 0.3f;  //어쩌지 현재높이로부 -13/s로 움직였을때 땅에 닿을때까지의 시간 
			Desc.fLifeTime = Get_fHeight() / 13.f;


			Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_LEFTDOWN };
			Desc.iTeam = m_iPlayerTeam;
			Desc.fAnimationLockTime = 0.1f;
			Desc.pOwner = this;
			Desc.bGroundSmash = true;
			Desc.bCameraZoom = false;
			Desc.iGainAttackStep = 0;

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
		}

		//땅에 닿아서 충격파
		else if (iAttackEvent == 1)
		{
			CAttackObject::ATTACK_DESC Desc{};

			if (m_iPlayerTeam == 1)
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
			else
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
			Desc.ColliderDesc.pMineGameObject = this;
			Desc.ColliderDesc.vCenter = { 0.5f * m_iLookDirection,0.8f,0.f };
			Desc.ColliderDesc.vExtents = { 0.6f,0.5f,0.2f };


			Desc.fhitCharacter_Impus = { m_iLookDirection * 1.5f, 0.2f };
			Desc.fhitCharacter_StunTime = 0.1f;	//일단잡기마냥 땅에 닿아야 풀리는 느낌 + 추가타로 풀어버리는 느낌


			Desc.iDamage = 800 * Get_DamageScale();
			Desc.fLifeTime = 0.3f;
			Desc.ihitCharacter_Motion = { HitMotion::HIT_SPIN_AWAY_LEFTUP };
			Desc.iTeam = m_iPlayerTeam;
			Desc.fAnimationLockTime = 0.7f;
			Desc.pOwner = this;
			Desc.eAttackGrade = GRADE_ULTIMATE;
			Desc.bGroundSmash = false;


			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
		}
	}
	break;
	case Client::CPlay_21::ANIME_ATTACK_236_SPECIAL:
	{

		CAttackObject_CommandGrab::ATTACK_COMMANDGRAB_DESC Desc{};
		if (m_iPlayerTeam == 1)
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
		else
			Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
		Desc.ColliderDesc.pMineGameObject = this;
		Desc.ColliderDesc.vExtents = { 1.4f,2.2f,1.f };
		Desc.ColliderDesc.vCenter = { 0.3f,0.7f,0.f };
		//Desc.ColliderDesc.pTransform = m_pTransformCom;
		//Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };
		Desc.fhitCharacter_StunTime = 1.f;
		Desc.iDamage = 100 * Get_DamageScale();;
		Desc.fLifeTime = 0.2f;
		Desc.ihitCharacter_Motion = { HitMotion::HIT_WALLBOUNCE };
		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.1f;
		Desc.pOwner = this;

		Desc.eAttackType = ATTACKTYPE_COMMANDGRAB;

		Desc.fDistance = { 0.8f * m_iLookDirection,0.5f };
		//Desc.fGrabAnimationPosition = 40.f;
		//Desc.fGrabAnimationPosition = 25.f;
		Desc.iGainAttackStep = 0;
		Desc.bOwnerNextAnimation = true;

		Desc.iGrabAnimationIndex = ANIME_ATTACK_236_SPECIAL;
		Desc.iOnwerNextAnimationIndex = ANIME_ATTACK_236_SPECIAL_SUCCES;
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack_CommandGrab"), TEXT("Layer_AttackObject"), &Desc);

		//236 잡기 컷신
		static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")))->Play(CMain_Camera::VIRTUAL_CAMERA_21_GRAB_SPECIAL, 0, this);
	}
	break;
	case Client::CPlay_21::ANIME_ATTACK_236_SPECIAL_SUCCES:
	{
		if (iAttackEvent == 0)
		{
			CAttackObject::ATTACK_DESC Desc{};
			//Desc.ColliderDesc.width = 1.0;
			//Desc.ColliderDesc.height = 1.3;
			if (m_iPlayerTeam == 1)
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
			else
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
			Desc.ColliderDesc.pMineGameObject = this;
			Desc.ColliderDesc.vExtents = { 1.f,1.3f,1.f };
			Desc.ColliderDesc.vCenter = { 1.0f * m_iLookDirection,0.8f,0.f };
			//Desc.ColliderDesc.pTransform = m_pTransformCom;
			Desc.fhitCharacter_Impus = { 3.f * m_iLookDirection,-20.f };
			Desc.fhitCharacter_StunTime = 1.0f;
			Desc.iDamage = 1200 * Get_DamageScale();;
			Desc.fLifeTime = 0.2f;
			Desc.ihitCharacter_Motion = { HitMotion::HIT_WALLBOUNCE };
			Desc.iTeam = m_iPlayerTeam;
			//Desc.bGroundSmash = true;
			Desc.fAnimationLockTime = 0.5f;
			Desc.pOwner = this;
			Desc.eAttackType = { ATTACKTYPE_MIDDLE };
			//Desc.bCameraZoom = false;
			//Desc.bGrabbedEnd = false;
			//Desc.bCameraZoom = false;

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
		}
		if (iAttackEvent == 1)
		{
			CAttackObject::ATTACK_DESC Desc{};
			//Desc.ColliderDesc.width = 1.0;
			//Desc.ColliderDesc.height = 1.3;
			if (m_iPlayerTeam == 1)
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
			else
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
			Desc.ColliderDesc.pMineGameObject = this;
			Desc.ColliderDesc.vExtents = { 1.f,1.3f,1.f };
			Desc.ColliderDesc.vCenter = { 1.0f * m_iLookDirection,0.8f,0.f };
			//Desc.ColliderDesc.pTransform = m_pTransformCom;
			Desc.fhitCharacter_Impus = { 3.f * m_iLookDirection,-20.f };
			Desc.fhitCharacter_StunTime = 1.0f;
			Desc.iDamage = 100 * Get_DamageScale();;
			Desc.fLifeTime = 0.2f;
			Desc.ihitCharacter_Motion = { HitMotion::HIT_HEAVY_DOWN };
			Desc.iTeam = m_iPlayerTeam;
			Desc.bGroundSmash = true;
			Desc.fAnimationLockTime = 0.f;
			Desc.pOwner = this;
			Desc.eAttackType = { ATTACKTYPE_MIDDLE };

			Desc.bGrabbedEnd = true;
			//Desc.bCameraZoom = false;
			Desc.iGainAttackStep = 0;

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
		}

		
		break;
	case Client::CPlay_21::ANIME_ATTACK_236_ULTIMATE:
		break;
	case Client::CPlay_21::ANIME_FINAL_START:
		break;
	case Client::CPlay_21::ANIME_IDLE:
		break;
	case Client::CPlay_21::ANIME_FORWARD_WALK:
		break;
	case Client::CPlay_21::ANIME_BACK_WALK:
		break;
	case Client::CPlay_21::ANIME_FORWARD_DASH:
		break;
	case Client::CPlay_21::ANIME_BACK_DASH:
		break;
	case Client::CPlay_21::ANIME_FORWARD_DASH_END:
		break;
	case Client::CPlay_21::ANIME_BACK_DASH_END:
		break;
	case Client::CPlay_21::ANIME_CROUCH_START:
		break;
	case Client::CPlay_21::ANIME_CROUCHING:
		break;
	case Client::CPlay_21::ANIME_JUMP_UP:
		break;
	case Client::CPlay_21::ANIME_JUMP_DOWN:
		break;
	case Client::CPlay_21::ANIME_CHASE:
		break;
	case Client::CPlay_21::ANIME_HIT_LIGHT:
		break;
	case Client::CPlay_21::ANIME_HIT_LIGHT_FRONT:
		break;
	case Client::CPlay_21::ANIME_HIT_CROUCH:
		break;
	case Client::CPlay_21::ANIE_HIT_AIR:
		break;
	case Client::CPlay_21::ANIME_HIT_FALL:
		break;
	case Client::CPlay_21::ANIME_HIT_BOUND_DOWN:
		break;
	case Client::CPlay_21::ANIME_HIT_HEAVY_AWAY_LEFT:
		break;
	case Client::CPlay_21::ANIME_HIT_HEAVY_AWAY_UP:
		break;
	case Client::CPlay_21::ANIME_HIT_HEAVY_AWAY_LEFTDOWN:
		break;
	case Client::CPlay_21::ANIME_HIT_HEAVY_AWAY_SPIN_UP:
		break;
	case Client::CPlay_21::ANIME_HIT_HEAVY_AWAY_SPIN_LEFTUP:
		break;
	case Client::CPlay_21::ANIME_HIT_HEAVY_AWAY_SPIN_LEFT:
		break;
	case Client::CPlay_21::ANIME_HIT_WALLBOUNCE:
		break;
	case Client::CPlay_21::ANIME_LAYUP:
		break;
	case Client::CPlay_21::ANIME_BREAK_FALL_GROUND:
		break;
	case Client::CPlay_21::ANIME_BREAK_FALL_AIR:
		break;
	case Client::CPlay_21::ANIME_DIE_STAND:
		break;
	case Client::CPlay_21::ANIME_GUARD_GROUND:
		break;
	case Client::CPlay_21::ANIME_GUARD_CROUCH:
		break;
	case Client::CPlay_21::ANIME_GUARD_AIR:
		break;
	case Client::CPlay_21::ANIME_GRAB_READY:
		break;
	case Client::CPlay_21::ANIME_GRAB:
		switch (iAttackEvent)
		{
			//잡기 시작
		case 0:
		{
			CAttackObject_Grab::ATTACK_Grab_DESC Desc{};
			//Desc.ColliderDesc.width = 1.4f;
			//Desc.ColliderDesc.height = 2.2f;
			if (m_iPlayerTeam == 1)
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
			else
				Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
			Desc.ColliderDesc.pMineGameObject = this;
			Desc.ColliderDesc.vExtents = { 1.4f,2.2f,1.f };
			Desc.ColliderDesc.vCenter = { 0.3f,0.7f,0.f };
			//Desc.ColliderDesc.pTransform = m_pTransformCom;
			//Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };
			Desc.fhitCharacter_StunTime = 1.f;
			Desc.iDamage = 700 * Get_DamageScale();;
			Desc.fLifeTime = 0.2f;
			Desc.ihitCharacter_Motion = { HitMotion::HIT_MEDIUM };
			Desc.iTeam = m_iPlayerTeam;
			Desc.fAnimationLockTime = 0.1f;
			Desc.pOwner = this;

			if (m_bGrab_Air)
				Desc.eAttackType = ATTACKTYPE_GRAB_AIR;
			else
				Desc.eAttackType = ATTACKTYPE_GRAB_GROUND;

			Desc.fDistance = { 1.2f * m_iLookDirection,0.f };
			//Desc.fGrabAnimationPosition = 40.f;
			Desc.fGrabAnimationPosition = 25.f;

			Desc.iOnwerNextAnimationIndex = m_iGrabAnimationIndex;

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack_Grab"), TEXT("Layer_AttackObject"), &Desc);
			m_pGameInstance->Play_Group_Sound(CSound_Manager::SOUND_GROUP_KEY::LIGHT_ATTACK_Goku_SFX, false, 1.f);

		}
		break;

		//난타
		case 1:
		case 3:
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
			Desc.fhitCharacter_StunTime = 1.f;
			Desc.iDamage = 80 * Get_DamageScale();
			Desc.fLifeTime = 0.1f;
			Desc.ihitCharacter_Motion = { HitMotion::HIT_LIGHT };
			Desc.iTeam = m_iPlayerTeam;
			Desc.fAnimationLockTime = 0.f;
			Desc.iGainAttackStep = 0;
			Desc.pOwner = this;
			Desc.bDrawNoneStop = true;

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
			m_pGameInstance->Play_Group_Sound(CSound_Manager::SOUND_GROUP_KEY::LIGHT_ATTACK_Goku_SFX, false, 1.f);
		}
		break;
		case 2:
		case 4:
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
			Desc.fhitCharacter_StunTime = 1.f;
			Desc.iDamage = 80 * Get_DamageScale();
			Desc.fLifeTime = 0.1f;
			Desc.ihitCharacter_Motion = { HitMotion::HIT_LIGHT };
			Desc.iTeam = m_iPlayerTeam;
			Desc.fAnimationLockTime = 0.f;
			Desc.iGainAttackStep = 0;
			Desc.pOwner = this;
			Desc.bDrawNoneStop = true;

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
			m_pGameInstance->Play_Group_Sound(CSound_Manager::SOUND_GROUP_KEY::LIGHT_ATTACK_Goku_SFX, false, 1.f);
		}
		break;

		}
		break;
	default:
		break;
	}

	}
}

void CPlay_21::Play_Sound(_uint SoundName, _bool bisLoof, _float fvolume)
{
	m_pGameInstance->Play_Sound((CSound_Manager::SOUND_KEY_NAME)SoundName, bisLoof, fvolume);
}

void CPlay_21::Play_Group_Sound(_uint groupKey, _bool loop, _float volume)
{
	m_pGameInstance->Play_Group_Sound((CSound_Manager::SOUND_GROUP_KEY)groupKey, loop, volume);
}


CPlay_21* CPlay_21::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlay_21* pInstance = new CPlay_21(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CPlay_21"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlay_21::Clone(void* pArg)
{
	CPlay_21* pInstance = new CPlay_21(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CPlay_21"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlay_21::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
