#include "stdafx.h"
#include "..\Public\Play_21.h"

#include "RenderInstance.h"
#include "GameInstance.h"



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

	

	//������ ���� Ȯ���ϹǷ� ������ Ŀ�ǵ尡 �Ʒ��� ������
	MoveCommandPatternsFunction.push_back({ Command_LightAttack, bind(&CS21_MeleeAttack::Attack_Light, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_MediumAttack, bind(&CS21_MeleeAttack::Attack_Medium, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_HeavyAttack, bind(&CS21_MeleeAttack::Attack_Heavy, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_SpecialAttack, bind(&CS21_MeleeAttack::Attack_Special, &m_tAttackMap) });


	MoveCommandPatternsFunction.push_back({ Command_HeavyAttack_Extra, bind(&CS21_MeleeAttack::Attack_Heavy, &m_tAttackMap) });


	MoveCommandPatternsFunction.push_back({ Command_BackDash, bind(&CS21_MeleeAttack::BackDash, &m_tAttackMap) });
	MoveCommandPatternsFunction.push_back({ Command_Forward, bind(&CS21_MeleeAttack::ForwardDash, &m_tAttackMap) });


	m_iHp = 100;
	m_eCharacterID = ANDROID21;

	return S_OK;
}

void CPlay_21::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CPlay_21::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	Action_Hit(DIK_F9, 0.25f, fTimeDelta);

	//Action_AttBuf(DIK_9, m_ePlayerSlot, fTimeDelta);



//	InputedCommandUpdate(fTimeDelta);
//	InputCommand();
//
//	//if(m_pGameInstance->Key_Down(DIK_U))
//	//	ShowInputBuffer();
//
//	//CheckCommandWithStartCondition()
//
//	//CheckAllCommands();
//
//	//_uint iTest = CheckAllCommands();
//
//	//Set_Animation(CheckAllCommands());
//
//
//	CheckAllCommands();
//
//	if (m_bAnimationLock == false)
//	{
//
//		Character_Play_Animation(fTimeDelta);
//
//		//�̰� �ݺ������ �ƴѵ� ����� ������� (=������ ��ü�� ������ ���),  �߶� �� ��� �ִϸ��̼� ����
//		if (m_bMotionPlaying == false)
//		{
//			if(m_pModelCom->m_iCurrentAnimationIndex != ANIME_JUMP_DOWN)
//				AnimeEndNextMoveCheck();
//		}
//
//
//	}
//	else
//	{
//		m_fAccAnimationLock += fTimeDelta;
//		if (m_fAccAnimationLock > m_fMaxAnimationLock)
//		{
//			m_fAccAnimationLock = 0.f;
//			m_bAnimationLock = false;
//		}
//	}
//
//	
//
//	Gravity(fTimeDelta);
//
//	
//	if (Check_bCurAnimationisGroundMove() || m_pModelCom->m_iCurrentAnimationIndex ==ANIME_FORWARD_DASH)
//	{
//		//if ((m_iNextAnimation.first == ANIME_IDLE) || ((m_iNextAnimation.first == ANIME_FORWARD_WALK) || (m_iNextAnimation.first == ANIME_BACK_WALK)))
//		if(Check_bCurAnimationisGroundMove(m_iNextAnimation.first))
//		{
//			Reset_AttackCount();
//
//			_short MoveKey = 0;
//			if (m_pGameInstance->Key_Pressing(DIK_W))
//			{
//				m_pTransformCom->Add_Move({ 0,3,0 });
//				Set_Animation(ANIME_JUMP_UP);
//			}
//
//			else if (m_pGameInstance->Key_Pressing(DIK_S))
//			{
//				//if (m_pModelCom->m_iCurrentAnimationIndex != ANIME_CROUCHING)
//				if (m_pModelCom->m_iCurrentAnimationIndex != ANIME_FORWARD_DASH)
//				{
//					m_pModelCom->SetUp_Animation(ANIME_CROUCHING, true);
//				}
//
//			}
//
//			else
//			{
//				if (m_pGameInstance->Key_Pressing(DIK_A))
//				{
//					MoveKey -= m_iLookDirection;
//				}
//
//				else if (m_pGameInstance->Key_Pressing(DIK_D))
//				{
//					MoveKey += m_iLookDirection;
//				}
//
//
//				if (MoveKey == -1)
//				{
//					
//					//if (m_pModelCom->m_iCurrentAnimationIndex == ANIME_BACK_DASH)
//					//{
//					//	m_pModelCom->SetUp_Animation(ANIME_BACK_DASH, false);
//					//}
//					//else
//						m_pModelCom->SetUp_Animation(ANIME_BACK_WALK, false);
//
//
//					m_iNextAnimation.first = ANIME_IDLE;
//					m_iNextAnimation.second = 100.f;
//
//				}
//				else if (MoveKey == 1)
//				{
//					if (m_pModelCom->m_iCurrentAnimationIndex == ANIME_FORWARD_DASH)
//					{
//						m_pModelCom->SetUp_Animation(ANIME_FORWARD_DASH, true);
//					}
//					else
//						m_pModelCom->SetUp_Animation(ANIME_FORWARD_WALK, false);
//
//					m_iNextAnimation.first = ANIME_IDLE;
//					m_iNextAnimation.second = 100.f;
//				}
//				else
//				{
//					if (m_pModelCom->m_iCurrentAnimationIndex == ANIME_FORWARD_DASH)
//					{
//						m_pModelCom->SetUp_Animation(ANIME_FORWARD_DASH_END, false);
//					}
//					else
//						m_pModelCom->SetUp_Animation(ANIME_IDLE, true);
//
//					m_iNextAnimation.first = ANIME_IDLE;
//					m_iNextAnimation.second = 100.f;
//				}
//			}
//
//			
//			//�ɱ� �߰� ������ �̰Ÿ� �־���
//			//if (m_pGameInstance->Key_Pressing(DIK_A))
//			//{
//			//	MoveKey -= m_iLookDirection;
//			//}
//			//
//			//else if (m_pGameInstance->Key_Pressing(DIK_D))
//			//{
//			//	MoveKey += m_iLookDirection;
//			//}
//			//
//			//
//			//if (MoveKey == -1)
//			//{
//			//	m_pModelCom->SetUp_Animation(ANIME_BACK_WALK, false);
//			//	m_iNextAnimation.first = ANIME_IDLE;
//			//	m_iNextAnimation.second = 100.f;
//			//
//			//}
//			//else if (MoveKey == 1)
//			//{
//			//	m_pModelCom->SetUp_Animation(ANIME_FORWARD_WALK, false);
//			//	m_iNextAnimation.first = ANIME_IDLE;
//			//	m_iNextAnimation.second = 100.f;
//			//}
//			//else
//			//{
//			//	m_pModelCom->SetUp_Animation(ANIME_IDLE, true);
//			//	m_iNextAnimation.first = ANIME_IDLE;
//			//	m_iNextAnimation.second = 100.f;
//			//}
//
//			
//		}
//	}
//
//
//	if (m_pGameInstance->Key_Down(DIK_8))
//	{
//		ShowInputBuffer();
//	}
//	if (m_pGameInstance->Key_Down(DIK_1))
//	{
//		DebugPositionReset();
//	}
//
//	if (m_pGameInstance->Key_Down(DIK_2))
//	{
//		FlipDirection();
//	}
//	if (m_pGameInstance->Key_Down(DIK_3))
//	{
//		m_pModelCom->SetUp_Animation(0,true);
//	}
//
//	
//
//	
}

void CPlay_21::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CPlay_21::Render(_float fTimeDelta)
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

	return S_OK;
}

/*
void CPlay_21::NextMoveCheck()
{

	//���� ���� �����߿� üũ������ IDLE�̸� �������� �ʰ� ������ ���� ������

	//����,�̵� ���� �ƴ϶�� ��


	//if (m_pModelCom->m_iCurrentAnimationIndex < m_iNextAnimation.first)
	//{
	//	//Set_Animation(m_iNextAnimation.first);
	//	m_pModelCom->SetUp_Animation(m_iNextAnimation.first,false);
	//	m_iNextAnimation.first = ANIME_IDLE;
	//	m_iNextAnimation.second = 100;
	//
	//}

	

	//�������ΰ��  �� �ٲ���  *üũ*
	//if (Check_bCurAnimationisGroundMove() == false)
	//{
	//	//���� �ִϸ��̼ǵ� ������ �ƴϸ� Set
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

	// üũ*
	//if(m_iNextAnimation.first != ANIME_IDLE)
	//{
	//	m_pModelCom->SetUp_Animation(m_iNextAnimation.first, false);
	//
	//	m_iNextAnimation.first = ANIME_IDLE;
	//	m_iNextAnimation.second = 100.f;
	//}


}
*/

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

void CPlay_21::Set_Animation(_uint iAnimationIndex, _bool bloof)
{

	if (iAnimationIndex == ANIME_IDLE)
		m_pModelCom->SetUp_Animation(iAnimationIndex, true);
	else
		m_pModelCom->SetUp_Animation(iAnimationIndex, bloof);

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

	return S_OK;
}

HRESULT CPlay_21::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

_bool CPlay_21::Check_bCurAnimationisGroundMove(_uint iAnimation)
{
	_uint iModelIndex= iAnimation;

	if(iAnimation == 1000)
		 iModelIndex = m_pModelCom->m_iCurrentAnimationIndex;
	

	//if (iModelIndex == ANIME_IDLE || (iModelIndex == ANIME_FORWARD_WALK || iModelIndex == ANIME_BACK_WALK) )
	//{
	//	return true;
	//}


	//��� �߰��ϴ��� �ּ�
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

_bool CPlay_21::Check_bCurAnimationisAttack(_uint iAnimation)
{

	return false;
}

void CPlay_21::Reset_AttackCount()
{

	for (size_t i = 0; i < COUNT_END; i++)
		m_bAttackCount[i] = true;

	m_iCountGroundSpecial = 0;

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
