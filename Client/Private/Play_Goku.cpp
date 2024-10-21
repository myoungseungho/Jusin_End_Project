#include "stdafx.h"
#include "..\Public\Play_Goku.h"

#include "RenderInstance.h"
#include "GameInstance.h"


#include "AttackObject.h"

#include "iostream"
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





CPlay_Goku::CPlay_Goku(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCharacter{ pDevice, pContext }
{

}

CPlay_Goku::CPlay_Goku(const CPlay_Goku& Prototype)
	: CCharacter{ Prototype }
{

}

HRESULT CPlay_Goku::Initialize_Prototype()
{
	//m_pFrameEvent = CFrameEvent_Manager::Get_Instance()->Get_pFrameEventMap();
	CFrameEvent_Manager::Get_Instance()->Initalize_NameMap();
	CFrameEvent_Manager::Get_Instance()->LoadFile2("../Bin/FrameEventData/EventData_Goku.txt");



	return S_OK;
}

HRESULT CPlay_Goku::Initialize(void* pArg)
{


	m_eCharacterIndex = PLAY_GOKU;
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

	m_iHit_Air_LightAnimationIndex = { ANIE_HIT_AIR };
	m_iHit_Air_FallAnimationIndex = { ANIME_HIT_FALL };

	m_iHit_Air_Spin_LeftUp = { ANIME_HIT_HEAVY_AWAY_SPIN_LEFTUP };

	m_iAttack_Air1 = { ANIME_ATTACK_AIR1 };
	m_iAttack_Air2 = { ANIME_ATTACK_AIR2 };
	m_iAttack_Air3 = { ANIME_ATTACK_AIR3 };
	m_iAttack_AirUpper = { ANIME_ATTACK_UPPER_AIR };

	m_iBound_Ground = { ANIME_HIT_BOUND_DOWN };

	m_iGuard_GroundAnimationIndex = { ANIME_GUARD_GROUND };
	m_iGuard_CrouchAnimationIndex = {ANIME_GUARD_CROUCH };
	m_iGuard_AirAnimationIndex = { ANIME_GUARD_AIR };


	m_iNextAnimation.first = ANIME_IDLE;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	m_tAttackMap.Initalize(this);

	

	//m_pModelCom->SetUp_Animation(16, true);
	m_pModelCom->SetUp_Animation(0, true);
	m_pModelCom->Play_Animation(0.f);



	//CTransform* pCameraTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), m_strTransformTag, 0));
	//pCameraTransform->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _vector{ 0.f, 1.f, -5.f });
	//pCameraTransform->LookAt(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	

	//MoveCommandPatterns.push_back({ hadoukenPattern, ShowInputBuffer});

	MoveCommandPatterns.push_back({ Command_236Attack, 66 });
	MoveCommandPatterns.push_back({ Command_236Attack_Extra, 66 });

	MoveCommandPatterns.push_back({ Command_LightAttack, ANIME_ATTACK_LIGHT1 });
	MoveCommandPatterns.push_back({ Command_MediumAttack, ANIME_ATTACK_MEDIUM });
	MoveCommandPatterns.push_back({ Command_HeavyAttack, ANIME_ATTACK_HEAVY });
	

	MoveCommandPatternsFunction.push_back({ Command_236UltimateAttack, bind(&CGoku_MeleeAttack::Attack_236Ultimate, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_236UltimateAttack_Side, bind(&CGoku_MeleeAttack::Attack_236Ultimate_Side, &m_tAttackMap) });




	MoveCommandPatternsFunction.push_back({ Command_236Attack,  bind(&CGoku_MeleeAttack::Attack_236, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_236Attack_Extra,  bind(&CGoku_MeleeAttack::Attack_236, &m_tAttackMap) });

	MoveCommandPatternsFunction.push_back({ Command_214Attack,  bind(&CGoku_MeleeAttack::Attack_214, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_214Attack_Extra,  bind(&CGoku_MeleeAttack::Attack_214, &m_tAttackMap) });

	MoveCommandPatternsFunction.push_back({ Command_236Special,  bind(&CGoku_MeleeAttack::Attack_236Special, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_236Special_Side,  bind(&CGoku_MeleeAttack::Attack_236Special_Side, &m_tAttackMap) });
	

	MoveCommandPatternsFunction.push_back({ Command_214FinalAttack, bind(&CGoku_MeleeAttack::Attack_214Final, &m_tAttackMap) });


	MoveCommandPatternsFunction.push_back({ Command_Crouch_LightAttack, bind(&CGoku_MeleeAttack::Attack_Crouch_Light, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_Crouch_MediumAttack, bind(&CGoku_MeleeAttack::Attack_Crouch_Medium, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_Crouch_HeavyAttack, bind(&CGoku_MeleeAttack::Attack_Crouch_Heavy, &m_tAttackMap) });


	MoveCommandPatternsFunction.push_back({ Command_Crouch_MediumAttack_Extra, bind(&CGoku_MeleeAttack::Attack_Crouch_Medium, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_Crouch_HeavyAttack_Extra, bind(&CGoku_MeleeAttack::Attack_Crouch_Heavy, &m_tAttackMap) });

	

	//������ ���� Ȯ���ϹǷ� ������ Ŀ�ǵ尡 �Ʒ��� ������
	MoveCommandPatternsFunction.push_back({ Command_LightAttack, bind(&CGoku_MeleeAttack::Attack_Light, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_MediumAttack, bind(&CGoku_MeleeAttack::Attack_Medium, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_HeavyAttack, bind(&CGoku_MeleeAttack::Attack_Heavy, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_SpecialAttack, bind(&CGoku_MeleeAttack::Attack_Special, &m_tAttackMap) });


	MoveCommandPatternsFunction.push_back({ Command_HeavyAttack_Extra, bind(&CGoku_MeleeAttack::Attack_Heavy, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_MediumAttack_Extra, bind(&CGoku_MeleeAttack::Attack_Medium, &m_tAttackMap) });

	
	//MoveCommandPatternsFunction.push_back({ Command_BackDash, bind(&CGoku_MeleeAttack::BackDash, &m_tAttackMap) });
	//MoveCommandPatternsFunction.push_back({ Command_Forward, bind(&CGoku_MeleeAttack::ForwardDash, &m_tAttackMap) });

	MoveCommandPatternsFunction_Exactly.push_back({ Command_BackDash, bind(&CGoku_MeleeAttack::BackDash, &m_tAttackMap) });
	MoveCommandPatternsFunction_Exactly.push_back({ Command_Forward, bind(&CGoku_MeleeAttack::ForwardDash, &m_tAttackMap) });

	
	if (::AllocConsole() == TRUE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}


	return S_OK;
}

void CPlay_Goku::Priority_Update(_float fTimeDelta)
{

}

void CPlay_Goku::Update(_float fTimeDelta)
{

	//��ġ�� �� �ӽ� �ڵ�.  �� Ž���ڵ���
	if (m_pDebugEnemy == nullptr)
	{
		//_short i = m_pGameInstance->Get_LayerSize(LEVEL_GAMEPLAY, TEXT("Layer_Character"));
	
		for (int i = 0; i < m_pGameInstance->Get_LayerSize(LEVEL_GAMEPLAY, TEXT("Layer_Character")); i++)
		{
			CGameObject* pObject = m_pGameInstance->Get_Object(LEVEL_GAMEPLAY, TEXT("Layer_Character"), i);
	
			if (pObject != this)
			{
				m_pDebugEnemy = static_cast<CCharacter*>(pObject);
			}
	
		}
	}




	//������ȯ �ڵ�.  �� Ž���� �߰��� ���Ŀ�  CCharacter�� �ű��
	if (Check_bCurAnimationisGroundMove() || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex)
	{
		CTransform* pEnemyTransform = static_cast<CTransform*>(m_pDebugEnemy->Get_Component(TEXT("Com_Transform")));
	
		//�� ������ X�� üũ
		_float fX = XMVectorGetX(pEnemyTransform->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	
	
		//���̰� ������ ���� ����. �� �� ���� �پ����� �� ���.
		if (fabsf(fX) > 0.1)
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




	//�ԷµǾ��ִ� Ŀ�ǵ� �ð��� ���� �ֹ�
	InputedCommandUpdate(fTimeDelta);

	//Chase == true�ų�  Stun==ture�� �ȹ���
	if (m_bChase == false && m_bStun == false)   //���Է��� �޾ƾ� �ϴ°�?
	{
		if (InputCommand())
		{
			CheckAllCommands();
		}

	}
	else   //�Է� �Ұ� ���¿��� ���Է¹���.  Ŀ�ǵ� üũ�� ���ϰ� �Է¸� ����
	{
		InputCommand();
	}


	


	if (m_bAnimationLock == false)
	{


		//���� ���� �ڵ�.
		if (m_iPlayerTeam == 1)
		{
			if (m_pGameInstance->Key_Down(DIK_R))
			{
				Chase_Ready(fTimeDelta);
			}
		}
		else  //Team2
		{
			if (m_pGameInstance->Key_Down(DIK_PGDN))  //PageDown Ű
			{
				Chase_Ready(fTimeDelta);
			}
		}

		if (m_bChase)
		{
			Chase2(fTimeDelta);

			if (m_bChase == false)
			{
				m_fAccChaseTime = 0.f;
				//m_fGravityTime = 0.365f;
				//Set_fImpulse( XMVectorGetX(vDir));
				m_fGravityTime = 0.185f;
				//m_pModelCom->SetUp_Animation(ANIME_JUMP_DOWN, false);
				Set_Animation(ANIME_JUMP_DOWN, false);
			}

		}
		

		Character_Play_Animation(fTimeDelta);

		//�̰� �ݺ������ �ƴѵ� ����� ������� (=������ ��ü�� ������ ���),  �߶� �� ��� �ִϸ��̼� ����
		if (m_bMotionPlaying == false)
		{
			
			//���� �߰� ���� �ִ���
			//if(m_pModelCom->m_iCurrentAnimationIndex != ANIME_JUMP_DOWN)
			//	AnimeEndNextMoveCheck();


			if (Check_bCurAnimationisAirHit() || Check_bCurAnimationisHitAway())
			{
				if(m_bHitGroundSmashed == false)
					Set_Animation(m_iHit_Air_FallAnimationIndex);


			}


			else if (m_bStun == false && m_pModelCom->m_iCurrentAnimationIndex != m_iFallAnimationIndex && m_pModelCom->m_iCurrentAnimationIndex != m_iHit_Air_LightAnimationIndex)
				AnimeEndNextMoveCheck();

		}

		//�̰Ÿ³�?   m_bMotionPlaying ���ε� ������ ������ Hit Away ���� ���
		else if (m_bMotionPlaying && Check_bCurAnimationisHitAway() && m_bStun == false)
		{
			Set_Animation(m_iHit_Air_FallAnimationIndex);

		}


		//�ִϸ��̼� lock ���߿��� �ߵ� ���ؾ���.   �Ʒ��� ���ڴ� 
		if (m_bStun)
		{
			Stun_Shake();
			m_fAccStunTime += fTimeDelta;
			if (m_fAccStunTime > m_fMaxStunTime)
			{
				m_bStun = false;
				m_fAccStunTime = 0.f;
			}
			Update_StunImpus(fTimeDelta);
		}
		else
		{

			//if(m_pGameInstance->Key_Pressing())
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

	
	//�߷� ó��.    ANimation Lock�� ������ �޾ƾ��ϳ�? ���� �Űܺ�
	//Gravity(fTimeDelta);

	//�Ϻ� ���� ĵ��
	AttckCancleJump();
	

	/*
	if (Check_bCurAnimationisGroundMove() || m_pModelCom->m_iCurrentAnimationIndex ==m_iForwardDashAnimationIndex)
	{
		if(Check_bCurAnimationisGroundMove(m_iNextAnimation.first))
		{
			Reset_AttackCount();

			_short MoveKey = 0;
			if (m_pGameInstance->Key_Pressing(DIK_W) && m_bJumpLock == false)
			{
				m_pTransformCom->Add_Move({ 0,0.3f,0 });
				
				//Set_fJumpPower(4.f); //�߷�Ver1 ����
				Set_fJumpPower(3.f); //�߷�Ver2 ����


				Set_Animation(m_iJumpAnimationIndex);


				if (m_pGameInstance->Key_Pressing(DIK_A))
				{					
					Set_fImpulse(-5.f);
				}

				else if (m_pGameInstance->Key_Pressing(DIK_D))
				{
					Set_fImpulse(5.f);
				}


			}

			else if (m_pGameInstance->Key_Pressing(DIK_S))
			{
				if (m_pModelCom->m_iCurrentAnimationIndex != m_iForwardDashAnimationIndex)
				{
					m_pModelCom->SetUp_Animation(m_iCrouchAnimationIndex, true);
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
					m_pModelCom->SetUp_Animation(m_iBackWalkAnimationIndex, false);

					m_iNextAnimation.first = m_iIdleAnimationIndex;

					m_iNextAnimation.second = 100.f;

				}
				else if (MoveKey == 1)
				{
					if (m_pModelCom->m_iCurrentAnimationIndex == m_iForwardDashAnimationIndex)
					{
						m_pModelCom->SetUp_Animation(m_iForwardDashAnimationIndex, true);
					}
					else
						m_pModelCom->SetUp_Animation(m_iForwardWalkAnimationIndex, false);

					m_iNextAnimation.first = m_iIdleAnimationIndex;
					m_iNextAnimation.second = 100.f;
				}
				else
				{
					if (m_pModelCom->m_iCurrentAnimationIndex == m_iForwardDashAnimationIndex)
					{
						m_pModelCom->SetUp_Animation(m_iForwardDashEndAnimationIndex, false);
					}
					else
						m_pModelCom->SetUp_Animation(m_iIdleAnimationIndex, true);

					m_iNextAnimation.first = m_iIdleAnimationIndex;
					m_iNextAnimation.second = 100.f;
				}
			}

			
			
		}
	}
	*/



	Move(fTimeDelta);
	Guard_Update();


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
		m_iAttackStepCount = 0;
		m_iDebugComoboDamage = 0;
	}
	if (m_pGameInstance->Key_Down(DIK_3))
	{
		system("cls");
	}

	//Gravity(fTimeDelta);

	m_pColliderCom->Update();


	if (m_iPlayerTeam == 2)
	{
		//m_pModelCom->m_iCurrentAnimationIndex;
		//_bool debuga = true;
		//cout << "Impus  x : " << m_fImpuse.x << "  ,   y : " << m_fImpuse.y << endl;
	}
	
}

void CPlay_Goku::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CPlay_Goku::Render(_float fTimeDelta)
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


	//corlorChange Test
	//for (size_t i = 0; i < iNumMeshes; i++)
	//{
	//	/* ���� ������ �ִ� ���׸��� �� i��° �޽ð� ����ؾ��ϴ� ���׸�����ü�� aiTextureType_DIFFUSE��° �ؽ��ĸ� */
	//	/* m_pShaderCom�� �ִ� g_DiffuseTexture������ ����. */
	//	if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, (aiTextureType)m_iPlayerTeam, "g_DiffuseTexture", i)))
	//		return E_FAIL;
	//	// m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i);
	//
	//	/* ���� ������ �ִ� ���� �߿��� ���� �������ҷ��� �ߴ� i��°�� �޽ð� ����ϴ� ������ �迭�� ���� ���̴��� �����ش�.  */
	//	m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);
	//
	//	if (FAILED(m_pShaderCom->Begin(0)))
	//		return E_FAIL;
	//
	//	if (FAILED(m_pModelCom->Render(i)))
	//		return E_FAIL;
	//}

#ifdef  _DEBUG
	m_pColliderCom->Render();
#endif //  _DEBUG
	return S_OK;
}



//���� ó�� �����ؼ� Character�� �̻簡�� ���.
/*
void CPlay_Goku::AttackNextMoveCheck()
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

void CPlay_Goku::AnimeEndNextMoveCheck()
{

	Set_Animation(m_iNextAnimation.first);

	m_iNextAnimation.first = ANIME_IDLE;
	m_iNextAnimation.second = 1000.f;


}



void CPlay_Goku::Set_Animation(_uint iAnimationIndex, _bool bloof)
{

	if (iAnimationIndex == m_iIdleAnimationIndex)
		m_pModelCom->SetUp_Animation(iAnimationIndex, true);
	else
		m_pModelCom->SetUp_Animation(iAnimationIndex, bloof);

}

*/


void CPlay_Goku::KeyTest()
{
	
}



HRESULT CPlay_Goku::Ready_Components()
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

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Play_Goku"), TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlay_Goku::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

_bool CPlay_Goku::Check_bCurAnimationisGroundMove(_uint iAnimation)
{
	_uint iModelIndex= iAnimation;

	if(iAnimation == 1000)
		 iModelIndex = m_pModelCom->m_iCurrentAnimationIndex;
	


	if (iModelIndex == ANIME_IDLE || iModelIndex == ANIME_FORWARD_WALK || iModelIndex == ANIME_BACK_WALK || iModelIndex == ANIME_CROUCH_START || iModelIndex == ANIME_CROUCHING
		||iModelIndex == ANIME_FORWARD_DASH )
	{
		return true;
	}

	return false;
}

_bool CPlay_Goku::Check_bCurAnimationisAttack(_uint iAnimation)
{

	return false;
}

_bool CPlay_Goku::Check_bCurAnimationisAirAttack(_uint iAnimation)
{
	_uint iModelIndex = iAnimation;

	if (iAnimation == 1000)
		iModelIndex = m_pModelCom->m_iCurrentAnimationIndex;



	if (iModelIndex == ANIME_ATTACK_AIR1 || iModelIndex == ANIME_ATTACK_AIR2 || iModelIndex == ANIME_ATTACK_AIR3 || iModelIndex ==ANIME_ATTACK_SPECIAL_AIR)
	{
		return true;
	}

	return false;
}

void CPlay_Goku::Reset_AttackCount()
{

	for (size_t i = 0; i < COUNT_END; i++)
		m_bAttackCount[i] = true;

	m_iCountGroundSpecial = 0;

}

void CPlay_Goku::Gravity(_float fTimeDelta)
{

	__super::Gravity(fTimeDelta);


	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float fHeight = XMVectorGetY(vPos);
	_ushort iAnimationIndex = m_pModelCom->m_iCurrentAnimationIndex;


	// iAnimationIndex == ANIME_ATTACK_236_SPECIAL_AIR || iAnimationIndex == ANIME_ATTACK_236_SPECIAL_AIR_DOWNSIDE || 


	if (iAnimationIndex == ANIME_ATTACK_236_AIR || iAnimationIndex == ANIME_ATTACK_AIR1 || iAnimationIndex == ANIME_ATTACK_AIR2 || iAnimationIndex == ANIME_ATTACK_AIR3 ||
		iAnimationIndex == ANIME_ATTACK_SPECIAL_AIR)
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

void CPlay_Goku::AttackEvent(_int iAttackEvent, _int AddEvent)
{

	switch (m_pModelCom->m_iCurrentAnimationIndex)
	{
	case Client::CPlay_Goku::ANIME_ATTACK_LIGHT1:
	{		

		CAttacKObject::ATTACK_DESC Desc{};
		Desc.ColliderDesc.fSizeX = 0.7;
		Desc.ColliderDesc.fSizeY = 0.8;
		Desc.ColliderDesc.Offset ={0.9f *m_iLookDirection,0.8f,0.f };
		Desc.ColliderDesc.pTransform = m_pTransformCom;
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

	case Client::CPlay_Goku::ANIME_ATTACK_LIGHT2:
		break;
	case Client::CPlay_Goku::ANIME_ATTACK_LIGHT3:
		break;
	case Client::CPlay_Goku::ANIME_ATTACK_MEDIUM:
	{
		CAttacKObject::ATTACK_DESC Desc{};
		Desc.ColliderDesc.fSizeX = 0.7;
		Desc.ColliderDesc.fSizeY = 0.8;
		Desc.ColliderDesc.Offset = { 0.9f * m_iLookDirection,0.8f,0.f };
		Desc.ColliderDesc.pTransform = m_pTransformCom;
		//Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };
		Desc.fhitCharacter_StunTime = 0.3f;
		Desc.iDamage = 700 * Get_DamageScale();;
		Desc.fLifeTime = 0.2f;
		Desc.ihitCharacter_Motion = { HitMotion::HIT_MEDIUM };
		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.1f;
		Desc.pOwner = this;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}
		break;
	case Client::CPlay_Goku::ANIME_ATTACK_HEAVY:
	{
		CAttacKObject::ATTACK_DESC Desc{};
		Desc.ColliderDesc.fSizeX = 1.0;
		Desc.ColliderDesc.fSizeY = 1.0;
		Desc.ColliderDesc.Offset = { 1.0f * m_iLookDirection,0.8f,0.f };
		Desc.ColliderDesc.pTransform = m_pTransformCom;
		Desc.fhitCharacter_Impus = { 20.f * m_iLookDirection,0 };
		Desc.fhitCharacter_StunTime = 1.0f;
		Desc.iDamage = 1000 * Get_DamageScale();;
		Desc.fLifeTime = 0.2f;
		Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_LEFT };
		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.5f;
		Desc.pOwner = this;
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}
		break;
	case Client::CPlay_Goku::ANIME_ATTACK_SPECIAL:
		break;
	case Client::CPlay_Goku::ANIME_ATTACK_AIR1:
	{

		CAttacKObject::ATTACK_DESC Desc{};
		Desc.ColliderDesc.fSizeX = 0.7;
		Desc.ColliderDesc.fSizeY = 0.8;
		Desc.ColliderDesc.Offset = { 0.9f * m_iLookDirection,0.8f,0.f };
		Desc.ColliderDesc.pTransform = m_pTransformCom;
		//Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };
		Desc.fhitCharacter_Impus = { m_fImpuse.x,0 };

		Desc.fhitCharacter_StunTime = 0.3f;
		Desc.iDamage = 400 * Get_DamageScale();
		Desc.fLifeTime = 0.1f;
		Desc.ihitCharacter_Motion = { HIT_LIGHT };
		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.04f;
		//Desc.bOwnerGravityTimeReset = true;
		Desc.pOwner = this;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}
		break;
	case Client::CPlay_Goku::ANIME_ATTACK_AIR2:
	{

		CAttacKObject::ATTACK_DESC Desc{};
		Desc.ColliderDesc.fSizeX = 0.7;
		Desc.ColliderDesc.fSizeY = 0.8;
		Desc.ColliderDesc.Offset = { 0.9f * m_iLookDirection,0.8f,0.f };
		Desc.ColliderDesc.pTransform = m_pTransformCom;
		//Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };
		Desc.fhitCharacter_Impus = { m_fImpuse.x  ,0 };

		Desc.fhitCharacter_StunTime = 0.3f;
		Desc.iDamage = 700 * Get_DamageScale();
		Desc.fLifeTime = 0.1f;
		Desc.ihitCharacter_Motion = { HitMotion::HIT_LIGHT };
		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.04f;
		//Desc.bOwnerGravityTimeReset = true;

		Desc.pOwner = this;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}
		break;
	case Client::CPlay_Goku::ANIME_ATTACK_AIR3:
	{
		CAttacKObject::ATTACK_DESC Desc{};
		Desc.ColliderDesc.fSizeX = 1.0;
		Desc.ColliderDesc.fSizeY = 1.0;
		Desc.ColliderDesc.Offset = { 1.0f * m_iLookDirection,0.8f,0.f };
		Desc.ColliderDesc.pTransform = m_pTransformCom;
		Desc.fhitCharacter_Impus = { 3.f * m_iLookDirection,-20.f };
		Desc.fhitCharacter_StunTime = 1.0f;
		Desc.iDamage = 850 * Get_DamageScale();;
		Desc.fLifeTime = 0.2f;
		Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_LEFT };
		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.1f;

		//���̰� 5 �̻��̸� ���������� ����.  ����Ʈ�� false�̹Ƿ�  else ó�� ����
		//if(Get_fHeight() > 5)
		if (Get_fHeight() > 3)
		{
			Desc.bGroundSmash = true;
			Desc.fAnimationLockTime = 0.4f;
		}

		Desc.pOwner = this;
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}
		break;
	case Client::CPlay_Goku::ANIME_ATTACK_SPECIAL_AIR:
		break;
	case Client::CPlay_Goku::ANIME_ATTACK_CROUCH_LIGHT:
		break;
	case Client::CPlay_Goku::ANIME_ATTACK_CROUCH_MEDUIM:
	{

		CAttacKObject::ATTACK_DESC Desc{};
		Desc.ColliderDesc.fSizeX = 0.7;
		Desc.ColliderDesc.fSizeY = 0.8;
		Desc.ColliderDesc.Offset = { 0.9f * m_iLookDirection,0.8f,0.f };
		Desc.ColliderDesc.pTransform = m_pTransformCom;
		//Desc.fhitCharacter_Impus = { 1.f * m_iLookDirection,0.3f };
		Desc.fhitCharacter_Impus = { 1.5f * m_iLookDirection,0.3f };
		Desc.fhitCharacter_StunTime = 0.3f;
		Desc.iDamage = 700 * Get_DamageScale();
		Desc.fLifeTime = 0.1f;
		Desc.ihitCharacter_Motion = { HitMotion::HIT_CROUCH_MEDIUM };
		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.1f;
		//Desc.bOwnerGravityTimeReset = true;
		Desc.pOwner = this;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}
		break;
	case Client::CPlay_Goku::ANIME_ATTACK_CROUCH_HEAVY:
	{
		CAttacKObject::ATTACK_DESC Desc{};
		//Desc.ColliderDesc.fSizeX = 2.0;
		//Desc.ColliderDesc.fSizeY = 1.3;
		//Desc.ColliderDesc.Offset = { 0.4f * m_iLookDirection,1.2f,0.f };
		Desc.ColliderDesc.fSizeX = 1.5;
		Desc.ColliderDesc.fSizeY = 1.3;
		Desc.ColliderDesc.Offset = { 0.2f * m_iLookDirection,1.2f,0.f };


		Desc.ColliderDesc.pTransform = m_pTransformCom;
		Desc.fhitCharacter_Impus = { 0.2f * m_iLookDirection, 10.f };  //���� ���ӵ�.
		//Desc.fhitCharacter_Impus = { 0.2f * m_iLookDirection, 3.f };  //���� �׽�Ʈ�� �ӽ�

		Desc.fhitCharacter_StunTime = 1.0f;
		Desc.iDamage = 1000 * Get_DamageScale();;
		Desc.fLifeTime = 0.3f;
		Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_UP };
		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.5f;
		Desc.pOwner = this;
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}
		break;
	case Client::CPlay_Goku::ANIME_ATTACK_UPPER_AIR:
	{
		CAttacKObject::ATTACK_DESC Desc{};
		Desc.ColliderDesc.fSizeX = 1.0;
		Desc.ColliderDesc.fSizeY = 1.0;
		Desc.ColliderDesc.Offset = { 1.0f * m_iLookDirection,0.8f,0.f };
		Desc.ColliderDesc.pTransform = m_pTransformCom;
		Desc.fhitCharacter_Impus = { 0.f, 6.f };
		Desc.fhitCharacter_StunTime = 0.5f;
		Desc.iDamage = 1000 * Get_DamageScale();;
		Desc.fLifeTime = 0.2f;
		Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_UP };
		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.08f;
		Desc.pOwner = this;
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}
		break;
	case Client::CPlay_Goku::ANIME_ATTACK_236:
	{
		CAttacKObject::ATTACK_DESC Desc{};
		Desc.ColliderDesc.fSizeX = 1.0;
		Desc.ColliderDesc.fSizeY = 1.3;
		Desc.ColliderDesc.Offset = { 1.0f * m_iLookDirection,0.8f,0.f };
		Desc.ColliderDesc.pTransform = m_pTransformCom;
		Desc.fhitCharacter_Impus = { 3.f * m_iLookDirection,-20.f };
		Desc.fhitCharacter_StunTime = 1.0f;
		Desc.iDamage = 1200 * Get_DamageScale();;
		Desc.fLifeTime = 0.2f;
		Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_LEFT };
		Desc.iTeam = m_iPlayerTeam;
		Desc.bGroundSmash = true;
		Desc.fAnimationLockTime = 0.5f;
		Desc.pOwner = this;
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}
		break;
	case Client::CPlay_Goku::ANIME_ATTACK_236_AIR:
	{
		CAttacKObject::ATTACK_DESC Desc{};
		Desc.ColliderDesc.fSizeX = 1.0;
		Desc.ColliderDesc.fSizeY = 1.3;
		Desc.ColliderDesc.Offset = { 1.0f * m_iLookDirection,0.8f,0.f };
		Desc.ColliderDesc.pTransform = m_pTransformCom;
		Desc.fhitCharacter_Impus = { 3.f * m_iLookDirection,-20.f };
		Desc.fhitCharacter_StunTime = 1.0f;
		Desc.iDamage = 1200 * Get_DamageScale();;
		Desc.fLifeTime = 0.2f;
		Desc.ihitCharacter_Motion = { HitMotion::HIT_KNOCK_AWAY_LEFT };
		Desc.iTeam = m_iPlayerTeam;
		Desc.bGroundSmash = true;
		Desc.fAnimationLockTime = 0.5f;
		Desc.pOwner = this;
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
	}
		break;
	case Client::CPlay_Goku::ANIME_ATTACK_214:
	{
		if (iAttackEvent == 0)
		{
			CAttacKObject::ATTACK_DESC Desc{};
			Desc.ColliderDesc.fSizeX = 0.7;
			Desc.ColliderDesc.fSizeY = 0.8;
			Desc.ColliderDesc.Offset = { 0.9f * m_iLookDirection,0.8f,0.f };
			Desc.ColliderDesc.pTransform = m_pTransformCom;
			//Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };
			Desc.fhitCharacter_Impus = { m_fImpuse.x,0 };

			Desc.fhitCharacter_StunTime = 0.3f;
			Desc.iDamage = 500 * Get_DamageScale();
			Desc.fLifeTime = 0.1f;
			Desc.ihitCharacter_Motion = { HitMotion::HIT_LIGHT };
			Desc.iTeam = m_iPlayerTeam;
			Desc.fAnimationLockTime = 0.04f;
			//Desc.bOwnerGravityTimeReset = true;
			Desc.pOwner = this;

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
		}
		else
		{
			CAttacKObject::ATTACK_DESC Desc{};
			Desc.ColliderDesc.fSizeX = 0.7;
			Desc.ColliderDesc.fSizeY = 0.8;
			Desc.ColliderDesc.Offset = { 0.9f * m_iLookDirection,0.8f,0.f };
			Desc.ColliderDesc.pTransform = m_pTransformCom;
			//Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0 };
			Desc.fhitCharacter_Impus = { m_fImpuse.x *3.f,0 };

			Desc.fhitCharacter_StunTime = 0.5f;
			Desc.iDamage = 500 * Get_DamageScale();
			Desc.fLifeTime = 0.1f;
			Desc.ihitCharacter_Motion = { HitMotion::HIT_MEDIUM };
			Desc.iTeam = m_iPlayerTeam;
			Desc.fAnimationLockTime = 0.07f;
			//Desc.bOwnerGravityTimeReset = true;
			Desc.pOwner = this;
			Desc.bGainAttackStep = false;		//2Ÿ ������ �����̹Ƿ� step�� �ѹ���.

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
		}
	}

		break;
	case Client::CPlay_Goku::ANIME_ATTACK_236_SPECIAL:
		break;
	case Client::CPlay_Goku::ANIME_ATTACK_236_SPECIAL_UPSIDE:
		break;
	case Client::CPlay_Goku::ANIME_ATTACK_236_SPECIAL_AIR:
		break;
	case Client::CPlay_Goku::ANIME_ATTACK_236_SPECIAL_AIR_DOWNSIDE:
		break;
	case Client::CPlay_Goku::ANIME_FINAL_START:
		break;
	case Client::CPlay_Goku::ANIME_IDLE:
		break;
	case Client::CPlay_Goku::ANIME_FORWARD_WALK:
		break;
	case Client::CPlay_Goku::ANIME_BACK_WALK:
		break;
	case Client::CPlay_Goku::ANIME_FORWARD_DASH:
		break;
	case Client::CPlay_Goku::ANIME_BACK_DASH:
		break;
	case Client::CPlay_Goku::ANIME_FORWARD_DASH_END:
		break;
	case Client::CPlay_Goku::ANIME_BACK_DASH_END:
		break;
	case Client::CPlay_Goku::ANIME_CROUCH_START:
		break;
	case Client::CPlay_Goku::ANIME_CROUCHING:
		break;
	case Client::CPlay_Goku::ANIME_JUMP_UP:
		break;
	case Client::CPlay_Goku::ANIME_JUMP_DOWN:
		break;
	case Client::CPlay_Goku::ANIME_CHASE:
		break;
	default:
		break;
	}

}



CPlay_Goku* CPlay_Goku::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlay_Goku* pInstance = new CPlay_Goku(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CPlay_Goku"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlay_Goku::Clone(void* pArg)
{
	CPlay_Goku* pInstance = new CPlay_Goku(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CPlay_Goku"));
		Safe_Release(pInstance);
	} 

	return pInstance;
}

void CPlay_Goku::Free()
{
	__super::Free();


	//Safe_Release(m_pShaderCom);
	//Safe_Release(m_pModelCom);

	Safe_Release(m_pModelCom_Opening);
	Safe_Release(m_pModelCom_Skill);


}
