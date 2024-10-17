#include "stdafx.h"
#include "..\Public\Play_Goku.h"

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


/*

vector<CInput> Command_236Attack =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_NEUTRAL, ATTACK_LIGHT}
};
vector<CInput> Command_236Attack_Extra =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_LIGHT}
};


vector<CInput> Command_214Attack =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_NONE},
	{MOVEKEY_NEUTRAL, ATTACK_LIGHT}
};
vector<CInput> Command_214Attack_Extra =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_LIGHT}
};

vector<CInput> Command_236Special =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_SPECIAL}
};

vector<CInput> Command_236Special_Side =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_DOWN, ATTACK_SPECIAL}
};


vector<CInput> Command_214FinalAttack =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_SPECIAL}
};


vector<CInput> Command_236UltimateAttack =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_GRAB}
};


vector<CInput> Command_236UltimateAttack_Side =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_DOWN, ATTACK_GRAB}
};



vector<CInput> Command_BackDash =
{
	{MOVEKEY_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_NONE}
};

vector<CInput> Command_Forward =
{
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{ MOVEKEY_RIGHT, ATTACK_NONE }

};

vector<CInput> Command_LightAttack ={	{MOVEKEY_NEUTRAL, ATTACK_LIGHT}};
vector<CInput> Command_MediumAttack ={	{MOVEKEY_NEUTRAL, ATTACK_MEDIUM}};
vector<CInput> Command_HeavyAttack ={	{MOVEKEY_NEUTRAL, ATTACK_HEAVY}};
vector<CInput> Command_SpecialAttack ={	{MOVEKEY_NEUTRAL, ATTACK_SPECIAL}};

vector<CInput> Command_HeavyAttack_Extra = { {MOVEKEY_RIGHT, ATTACK_HEAVY} };



vector<CInput> Command_Crouch_LightAttack = { {MOVEKEY_DOWN, ATTACK_LIGHT} };
vector<CInput> Command_Crouch_MediumAttack = { {MOVEKEY_DOWN, ATTACK_MEDIUM} };
vector<CInput> Command_Crouch_HeavyAttack = { {MOVEKEY_DOWN, ATTACK_HEAVY} };
vector<CInput> Command_Crouch_SpecialAttack = { {MOVEKEY_DOWN, ATTACK_SPECIAL} };



vector<CInput> Command_Crouch_MediumAttack_Extra = { {MOVEKEY_DOWN_RIGHT, ATTACK_MEDIUM} };
vector<CInput> Command_Crouch_HeavyAttack_Extra = { {MOVEKEY_DOWN_RIGHT, ATTACK_HEAVY} };

*/



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

	//Play_Goku_DESC* pDesc = static_cast<Play_Goku_DESC*>(pArg);
	//m_strModelName = pDesc->strModelName;

	//m_pFrameEvent = CFrameEvent_Manager::Get_Instance()->Get_pFrameEventMap();

	m_eCharacterIndex = PLAY_GOKU;
	m_iFallAnimationIndex = 7;
	m_iIdleAnimationIndex = 0;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	m_tAttackMap.Initalize(this);

	

	//m_pModelCom->SetUp_Animation(16, true);
	m_pModelCom->SetUp_Animation(0, true);
	m_pModelCom->Play_Animation(0.f);



	CTransform* pCameraTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), m_strTransformTag, 0));
	pCameraTransform->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _vector{ 0.f, 1.f, -5.f });
	pCameraTransform->LookAt(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	

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

	

	//위에서 부터 확인하므로 간단한 커맨드가 아래로 가야함
	MoveCommandPatternsFunction.push_back({ Command_LightAttack, bind(&CGoku_MeleeAttack::Attack_Light, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_MediumAttack, bind(&CGoku_MeleeAttack::Attack_Medium, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_HeavyAttack, bind(&CGoku_MeleeAttack::Attack_Heavy, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_SpecialAttack, bind(&CGoku_MeleeAttack::Attack_Special, &m_tAttackMap) });


	MoveCommandPatternsFunction.push_back({ Command_HeavyAttack_Extra, bind(&CGoku_MeleeAttack::Attack_Heavy, &m_tAttackMap) });


	MoveCommandPatternsFunction.push_back({ Command_BackDash, bind(&CGoku_MeleeAttack::BackDash, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_Forward, bind(&CGoku_MeleeAttack::ForwardDash, &m_tAttackMap) });

	m_iHp = 100;
	m_eCharacterID = GOGU;


	return S_OK;
}

void CPlay_Goku::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CPlay_Goku::Update(_float fTimeDelta)
{
	
	Action_Hit(DIK_0, 0.25f, fTimeDelta);
	Action_AttBuf(DIK_9, m_ePlayerSlot, fTimeDelta);

	if (m_pGameInstance->Key_Down(DIK_F3))
	{
 		m_pUI_Manager->UsingChangeCharacher(m_ePlayerSlot);
	}


	InputedCommandUpdate(fTimeDelta);
	InputCommand();

	//if(m_pGameInstance->Key_Down(DIK_U))
	//	ShowInputBuffer();

	//CheckCommandWithStartCondition()

	//CheckAllCommands();

	//_uint iTest = CheckAllCommands();

	//Set_Animation(CheckAllCommands());


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

/*
void CPlay_Goku::NextMoveCheck()
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

void CPlay_Goku::AttackNextMoveCheck()
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

void CPlay_Goku::AnimeEndNextMoveCheck()
{

	Set_Animation(m_iNextAnimation.first);

	m_iNextAnimation.first = ANIME_IDLE;
	m_iNextAnimation.second = 1000.f;


	

}

void CPlay_Goku::Test_InputCommand()
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

void CPlay_Goku::Set_Animation(_uint iAnimationIndex, _bool bloof)
{

	if (iAnimationIndex == ANIME_IDLE)
		m_pModelCom->SetUp_Animation(iAnimationIndex, true);
	else
		m_pModelCom->SetUp_Animation(iAnimationIndex, bloof);

}


/*
void CPlay_Goku::Set_Animation(_uint iAnimationIndex)
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

void CPlay_Goku::Reset_AttackCount()
{

	for (size_t i = 0; i < COUNT_END; i++)
		m_bAttackCount[i] = true;

	m_iCountGroundSpecial = 0;

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


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
