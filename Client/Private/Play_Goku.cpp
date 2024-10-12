#include "stdafx.h"
#include "..\Public\Play_Goku.h"

#include "RenderInstance.h"
#include "GameInstance.h"


#define ANIME_ATTACK_LIGHT1 43
#define ANIME_ATTACK_LIGHT2 44
#define ANIME_ATTACK_LIGHT3 47

#define ANIME_ATTACK_MEDIUM 46

#define ANIME_ATTACK_HEAVY 45

#define ANIME_IDLE 0


#define ANIME_FOWARD_WALK 9
#define ANIME_BACK_WALK 10


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
vector<CInput> Command_LightAttack ={	{MOVEKEY_NEUTRAL, ATTACK_LIGHT}};
vector<CInput> Command_MediumAttack ={	{MOVEKEY_NEUTRAL, ATTACK_MEDIUM}};
vector<CInput> Command_HeavyAttack ={	{MOVEKEY_NEUTRAL, ATTACK_HEAVY}};
vector<CInput> Command_SpecialAttack ={	{MOVEKEY_NEUTRAL, ATTACK_SPECIAL}};

//vector<CInput> Command_BackWalk = { {MOVEKEY_LEFT, ATTACK_NONE} };
//vector<CInput> Command_ForwardWalk = { {MOVEKEY_RIGHT, ATTACK_NONE} };



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

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	
	

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
	
	//MoveCommandPatterns.push_back({ Command_ForwardWalk, ANIME_FOWARD_WALK });
	//MoveCommandPatterns.push_back({ Command_BackWalk, ANIME_BACK_WALK });


	//MoveCommandPatterns.push_back({ Command_SpecialAttack, ANIME_ATTACK_LIGHT1 });


	//MoveCommandPatternsFunction.push_back({ hadoukenPattern,Test_InputCommand });
	return S_OK;
}

void CPlay_Goku::Priority_Update(_float fTimeDelta)
{

}

void CPlay_Goku::Update(_float fTimeDelta)
{
	

	InputCommand();
	//CheckCommandWithStartCondition()

	//CheckAllCommands();

	//_uint iTest = CheckAllCommands();

	Set_Animation(CheckAllCommands());

	if (m_bAnimationLock == false)
	{

		Character_Play_Animation(fTimeDelta);

		if (m_bMotionPlaying == false)
		{
			if (m_pModelCom->m_iCurrentAnimationIndex != ANIME_IDLE)
			{
				m_pModelCom->SetUp_Animation(ANIME_IDLE, true);
			}
		}

		NextMoveCheck();

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

	


	if(m_iNextAnimation.first != ANIME_IDLE)
	{
		m_iNextAnimation.second -= fTimeDelta;

		if (m_iNextAnimation.second < 0)
		{
			m_iNextAnimation.first = ANIME_IDLE;
			m_iNextAnimation.second = 100;

		}
	}

	//걷기 는 예외처리
	//_ushort iCurrentAniimationIndex = m_pModelCom->m_iCurrentAnimationIndex;
	//if ((iCurrentAniimationIndex == ANIME_FOWARD_WALK) || (iCurrentAniimationIndex == ANIME_BACK_WALK))
	//{
	//	_short MoveKey = 0;
	//
	//	if (m_pGameInstance->Key_Pressing(DIK_A))
	//	{
	//		MoveKey -= m_iLookDirection;
	//	}
	//
	//	else if (m_pGameInstance->Key_Pressing(DIK_D))
	//	{
	//		MoveKey += m_iLookDirection;
	//	}
	//
	//	//if(iCurrentAniimationIndex)
	//
	//	//뒷걸음질 중인데 MoveKey이-1이 아니면 idle로
	//	if ((iCurrentAniimationIndex == ANIME_FOWARD_WALK && MoveKey != 1) || (iCurrentAniimationIndex == ANIME_BACK_WALK && MoveKey != -1))
	//	{
	//		m_pModelCom->SetUp_Animation(0, true);
	//	}
	//	//if(iCurrentAniimationIndex-)
	//
	//}

	_ushort iCurrentAniimationIndex = m_pModelCom->m_iCurrentAnimationIndex;
	if ((iCurrentAniimationIndex == ANIME_IDLE) ||((iCurrentAniimationIndex == ANIME_FOWARD_WALK) || (iCurrentAniimationIndex == ANIME_BACK_WALK)))
	{
		_short MoveKey = 0;

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
			m_pModelCom->SetUp_Animation(ANIME_BACK_WALK,false);
		}
		else if (MoveKey == 1)
		{
			m_pModelCom->SetUp_Animation(ANIME_FOWARD_WALK,false);
		}
		else 
		{
			m_pModelCom->SetUp_Animation(ANIME_IDLE, true);
		}
	}


	//NextMoveCheck();

	if (m_pGameInstance->Key_Down(DIK_8))
	{
		ShowInputBuffer();
	}
	if (m_pGameInstance->Key_Down(DIK_1))
	{
		DebugPositionReset();
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

void CPlay_Goku::NextMoveCheck()
{

	if (m_pModelCom->m_iCurrentAnimationIndex < m_iNextAnimation.first)
	{
		//Set_Animation(m_iNextAnimation.first);
		m_pModelCom->SetUp_Animation(m_iNextAnimation.first,false);
		m_iNextAnimation.first = ANIME_IDLE;
		m_iNextAnimation.second = 100;

	}

}

void CPlay_Goku::Test_InputCommand()
{
	if (m_pGameInstance->Key_Down(DIK_U))
	{
		m_pModelCom->SetUp_Animation(ANIME_ATTACK_LIGHT1,false);
	}

	if (m_pGameInstance->Key_Down(DIK_I))
	{
		m_pModelCom->SetUp_Animation(ANIME_ATTACK_MEDIUM, false);

	}


	if (m_pGameInstance->Key_Down(DIK_K))
	{
		m_pModelCom->SetUp_Animation(ANIME_ATTACK_HEAVY, false);

	}


}

void CPlay_Goku::Set_Animation(_uint iAnimationIndex)
{



	//특정 모션에서만 애니메이션 변경 가능?  이러면 안되는데
	//키 입력 자체는 성공했는데 애니메이션간 연결이 복잡함. 236스킬 도중에 약공격이 나감  Pressing이라 그렇다?
	if (m_pModelCom->m_iCurrentAnimationIndex != ANIME_IDLE)
	{
		m_iNextAnimation.first = iAnimationIndex;
		m_iNextAnimation.second = 0.2f;
		return;
	}




	_uint iDebug = iAnimationIndex;
	if (iAnimationIndex != 0 && (iAnimationIndex !=10 && iAnimationIndex != 9))
	{
		_bool bDebug = true;
	}

	if(iAnimationIndex != ANIME_IDLE)
		m_pModelCom->SetUp_Animation(iAnimationIndex, false);

}

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
	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_strModelName,
	//	TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
	//	return E_FAIL;

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
