#include "stdafx.h"
#include "..\Public\Character.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "iostream"
#include "AttackObject.h"
#include "UI_Manager.h"
#include "Effect_Manager.h"

#include "AttackObject.h"
#include "BattleInterface.h"
#include "Main_Camera.h"

const _float CCharacter::fGroundHeight = 0.f; //0
const _float CCharacter::fJumpPower = 3.f; //0


vector<CInput> CCharacter::Command_236Attack =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_NEUTRAL, ATTACK_LIGHT}
};
vector<CInput> CCharacter::Command_236Attack_Extra =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_LIGHT}
};


vector<CInput> CCharacter::Command_214Attack =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_NONE},
	{MOVEKEY_NEUTRAL, ATTACK_LIGHT}
};
vector<CInput> CCharacter::Command_214Attack_Extra =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_LIGHT}
};

vector<CInput> CCharacter::Command_236Special =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_SPECIAL}
};

vector<CInput> CCharacter::Command_236Special_Side =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_DOWN, ATTACK_SPECIAL}
};

vector<CInput> CCharacter::Command_214Special =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_NONE},
	{MOVEKEY_NEUTRAL, ATTACK_SPECIAL}
};

vector<CInput> CCharacter::Command_214Special_Extra =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_SPECIAL}
};




vector<CInput> CCharacter::Command_214FinalAttack =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_GRAB}
};


vector<CInput> CCharacter::Command_236UltimateAttack =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_GRAB}
};


vector<CInput> CCharacter::Command_236UltimateAttack_Side =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_DOWN, ATTACK_GRAB}
};



vector<CInput> CCharacter::Command_BackDash =
{
	{MOVEKEY_LEFT, ATTACK_NONE},
	{MOVEKEY_NEUTRAL, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_NONE}
};

vector<CInput> CCharacter::Command_Forward =
{
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_NEUTRAL, ATTACK_NONE},
	{ MOVEKEY_RIGHT, ATTACK_NONE }

};

vector<CInput> CCharacter::Command_LightAttack = { {MOVEKEY_NEUTRAL, ATTACK_LIGHT} };
vector<CInput> CCharacter::Command_MediumAttack = { {MOVEKEY_NEUTRAL, ATTACK_MEDIUM} };

vector<CInput> CCharacter::Command_MediumAttack_Extra = { {MOVEKEY_RIGHT, ATTACK_MEDIUM} };

vector<CInput> CCharacter::Command_HeavyAttack = { {MOVEKEY_NEUTRAL, ATTACK_HEAVY} };
vector<CInput> CCharacter::Command_SpecialAttack = { {MOVEKEY_NEUTRAL, ATTACK_SPECIAL} };

vector<CInput> CCharacter::Command_HeavyAttack_Extra = { {MOVEKEY_RIGHT, ATTACK_HEAVY} };

vector<CInput> CCharacter::Command_Grab = { {MOVEKEY_NEUTRAL, ATTACK_GRAB} };


vector<CInput> CCharacter::Command_Crouch_LightAttack = { {MOVEKEY_DOWN, ATTACK_LIGHT} };
vector<CInput> CCharacter::Command_Crouch_MediumAttack = { {MOVEKEY_DOWN, ATTACK_MEDIUM} };
vector<CInput> CCharacter::Command_Crouch_HeavyAttack = { {MOVEKEY_DOWN, ATTACK_HEAVY} };
vector<CInput> CCharacter::Command_Crouch_SpecialAttack = { {MOVEKEY_DOWN, ATTACK_SPECIAL} };

vector<CInput> CCharacter::Command_Crouch_MediumAttack_Extra = { {MOVEKEY_DOWN_RIGHT, ATTACK_MEDIUM} };
vector<CInput> CCharacter::Command_Crouch_HeavyAttack_Extra = { {MOVEKEY_DOWN_RIGHT, ATTACK_HEAVY} };




CCharacter::CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
	, m_pUI_Manager{ CUI_Manager::Get_Instance() }
{
	Safe_AddRef(m_pUI_Manager);
}

CCharacter::CCharacter(const CCharacter& Prototype)
	: CGameObject{ Prototype }
	, m_pFrameEvent{ Prototype.m_pFrameEvent }
	, m_pUI_Manager{ CUI_Manager::Get_Instance() }
	, m_pEffect_Manager{ CEffect_Manager::Get_Instance() }
{
	Safe_AddRef(m_pUI_Manager);
	Safe_AddRef(m_pEffect_Manager);
}

HRESULT CCharacter::Initialize_Prototype()
{
	//m_pFrameEvent = CFrameEvent_Manager::Get_Instance()->Get_pFrameEventMap();
	  //Ã¹ ½ÇÇàÀÏ¶§, ·çÇÁµ¹¾ÒÀ»¶§ ZeroFrameÀ» ¾î¶»°Ô ±¸ºÐÇÏ´Â°¡


	return S_OK;
}

HRESULT CCharacter::Initialize(void* pArg)
{
	Character_DESC* pDesc = static_cast<Character_DESC*>(pArg);
	//m_strModelName = pDesc->strModelName;

	m_pFrameEvent = CFrameEvent_Manager::Get_Instance()->Get_pFrameEventMap();

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Character_DESC* pSlotDesc = static_cast<Character_DESC*>(pArg);
	m_ePlayerSlot = pSlotDesc->ePlayerSlot;
	m_tCharacterDesc.ePlayer_Slot = m_ePlayerSlot;


	m_iPlayerTeam = pDesc->iTeam;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (m_iPlayerTeam == 1)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
		FlipDirection(1);
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(2.f, 0.f, 0.f, 1.f));
		FlipDirection(-1);
	}

	//¸ðµ¨ ·Îµå¸¦ ÇÏÀ§ Å¬·¡½º·Î ¿Å°åÀ¸´Ï °¢ÀÚ Ã³¸®ÇÏ±â 
	//m_pModelCom->SetUp_Animation(0, false);
	//m_pModelCom->Play_Animation(0.f);


	inputBuffer.push_back(CInput(MOVEKEY_NEUTRAL, ATTACK_NONE));


	//CBattleInterface_Manager::Get_Instance()->Regist_Character(m_iPlayerTeam, this, m_ePlayerSlot);
	//if (m_ePlayerSlot != CUI_Define::PLAYER_SLOT::LPLAYER1 && m_ePlayerSlot != CUI_Define::PLAYER_SLOT::RPLAYER1)
	//{
	//	m_bPlaying = false;
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(5.f * m_iPlayerTeam, 100.f, 0.f, 1.f));
	//
	//	//Ä³¸¯ÅÍ »çÀÌÁî¿¡ ¸Â°Ô °¢ÀÚ Ãß°¡ÇÏ´À¶ó m_pColliderComÀÌ ¾øÀ½
	//	//m_pColliderCom->UpdateVector(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	//}
	//else
	//	m_bPlaying = true;

	return S_OK;
}

void CCharacter::Player_Update(_float fTimeDelta)
{
}

void CCharacter::Priority_Update(_float fTimeDelta)
{
	m_tCharacterDesc.bStun = m_bRedHp;

	m_tCharacterDesc.bHit = m_bHit;
	m_tCharacterDesc.bAttBuf = m_bAttBuf;
	m_tCharacterDesc.iHp = m_iHP;

	m_tCharacterDesc.iComboCount = CBattleInterface_Manager::Get_Instance()->Get_HitCount(m_iPlayerTeam);

	if (m_iPrevComboCount < m_tCharacterDesc.iComboCount)
	{
		m_bHit = false;
	}

	m_tCharacterDesc.iSKillCount = m_iSKillCount;
	m_tCharacterDesc.iSKillPoint = m_iSKillPoint;
	m_tCharacterDesc.ePlayer_Slot = m_ePlayerSlot;
	m_tCharacterDesc.ePlayerID = m_eCharacterID;
	m_tCharacterDesc.iTeam = m_iPlayerTeam;

	m_iPrevComboCount = m_tCharacterDesc.iComboCount;

}

void CCharacter::Update(_float fTimeDelta)
{

	if (m_bAnimationLock == false)
	{




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

}

void CCharacter::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CCharacter::Render(_float fTimeDelta)
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		/* ¸ðµ¨ÀÌ °¡Áö°í ÀÖ´Â ¸ÓÅ×¸®¾ó Áß i¹øÂ° ¸Þ½Ã°¡ »ç¿ëÇØ¾ßÇÏ´Â ¸ÓÅ×¸®¾ó±¸Á¶Ã¼ÀÇ aiTextureType_DIFFUSE¹øÂ° ÅØ½ºÃÄ¸¦ */
		/* m_pShaderCom¿¡ ÀÖ´Â g_DiffuseTextureº¯¼ö¿¡ ´øÁ®. */
		if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;
		// m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i);

		/* ¸ðµ¨ÀÌ °¡Áö°í ÀÖ´Â »Àµé Áß¿¡¼­ ÇöÀç ·»´õ¸µÇÒ·Á°í Çß´ø i¹øÂ°¤Á ¸Þ½Ã°¡ »ç¿ëÇÏ´Â »ÀµéÀ» ¹è¿­·Î ¸¸µé¾î¼­ ½¦ÀÌ´õ·Î ´øÁ®ÁØ´Ù.  */
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}



_bool CCharacter::CompareNextAnimation(_uint iAnimationIndex, _float fNextPosition)
{
	_bool bCompare = true;

	if (fNextPosition != 0)
	{
		if (m_fNextAnimationCurrentPosition != fNextPosition)
			bCompare = false;
	}

	if (iAnimationIndex != m_iNextAnimation.first)
		bCompare = false;

	return bCompare;
}

void CCharacter::Set_CurrentAnimationPositionJump(_float fAnimationPosition)
{
	m_pModelCom->CurrentAnimationPositionJump(fAnimationPosition);
}

void CCharacter::ProcessEventsFramesZero(_uint characterIndex, _uint animationIndex)
{

	//¾Ö´Ï¸ÞÀÌ¼Ç º¯°æ½Ã 0¹øÂ° ÇÁ·¹ÀÓÀÌº¥Æ®¸¦ ½ÇÇàÇÏ±â À§ÇÔ

	auto characterIt = m_pFrameEvent->find(m_eCharacterIndex);   //Å×½ºÆ®Áß
	if (characterIt != m_pFrameEvent->end())
	{
		auto& animationMap = characterIt->second;
		auto animationIt = animationMap.find(animationIndex);
		if (animationIt != animationMap.end())
		{
			auto& frameMap = animationIt->second;

			auto frameIt = frameMap[0];

			for (auto event : frameIt)
			{

				CFrameEvent_Manager::Get_Instance()->UseEvent(event, this);
			}

		}
	}



}

void CCharacter::ProcessEventsBetweenFrames2(int characterIndex, int animationIndex, float prevFrame, float currentFrame)
{

	//FrameÀÌ ¸ØÃç¼­ Áßº¹Ã³¸®µÇ´Â°æ¿ì´Â?
   // ÇÁ·¹ÀÓÀÌ 0ÀÎ°æ¿ì¸¦ Á¦¿ÜÇÏ°í ÇÁ·¹ÀÓÀÌ ¸ØÃè´Âµ¥ Á¤È®ÇÏ°Ô ÀÌº¥Æ®°¡ ÀÖÀ» ¼ö ÀÖ³ª?
	   // 0.001ÃÊ´ÜÀ§·Î º¸¸é ºÒ°¡´ÉÇÑ°Ç ¾Æ´Ô

   //ÇØ°á¹æ¹ý
   // 1. ÀÌ ÇÔ¼ö¸¦ play animation ÁÖº¯¿¡ ³Ö¾î¼­, ¾Ö´Ï¸ÞÀÌ¼ÇÀÌ Á¤ÁöÇÑ°æ¿ì ÀÌ ÇÔ¼ö È£Ãâ ÀÚÃ¼¸¦ ¾ÈÇÔ.  << Á¤¼®
   // 2. bool°ªÀ» ¸¸µé¾î¼­ 1È¸¸¸ Ã³¸®

   //ÀÏ´Ü º¸·ù.




   //frameÀÌ 0ÀÎ°æ¿ì´Â ¾Ö´Ï¸ÞÀÌ¼Ç ½ÇÇà ¸»°í, ¾Ö´Ï¸ÞÀÌ¼Ç º¯°æ½Ã 0ÀÇ ÀÌº¥Æ®¸¸ Ã£¾Æ¼­ ÇÏ´Â°É·Î Ã³¸®.   (ProcessEventsFramesZero)
	if (prevFrame == 0)
	{

		//°ú°Å,ÇöÀç ÇÁ·¹ÀÓÀÌ ¸ðµÎ 0À¸·Î ¾Ö´Ï¸ÞÀÌ¼Ç ÀÚÃ¼°¡ Á¤ÁöµÈ °æ¿ì ÇÔ¼ö ³¡
		if (currentFrame == 0)
			return;

		//¾Æ´Ï¸é 0ÇÁ·¹ÀÓ ÀÌº¥Æ® Á¦¿ÜÇÏ°í ½ÇÇàÇÏ±â À§ÇØ  °ú°ÅÇÁ·¹ÀÓ Á¶±Ý Áõ°¡
		else
			prevFrame += 0.001;
	}





	// Ä³¸¯ÅÍ ÀÎµ¦½º Å½»ö

	auto characterIt = m_pFrameEvent->find(m_eCharacterIndex);   //Å×½ºÆ®Áß
	if (characterIt != m_pFrameEvent->end())
	{
		auto& animationMap = characterIt->second;
		auto animationIt = animationMap.find(animationIndex);
		if (animationIt != animationMap.end())
		{
			auto& frameMap = animationIt->second;


			//0ÀÏ¶§¸¦ µû·Î ¸¸µé¾úÀ¸´Ï Á¦¿ÜÇØ¾ßÇÏ³ª?
			for (auto frameIt = frameMap.lower_bound(prevFrame); frameIt != frameMap.end() && frameIt->first <= currentFrame; ++frameIt)
			{
				// ÇØ´ç ÇÁ·¹ÀÓ¿¡¼­ÀÇ ÀÌº¥Æ® ¸®½ºÆ®¸¦ Ãâ·Â
				for (string event : frameIt->second)
				{
					//MSG_BOX(TEXT("ÇÁ·¹ÀÓ " + frameIt->first + "¿¡¼­ ÀÌº¥Æ® ¹ß»ý: " + event));

					_bool bdebug1 = false;

					//m_pSelectedModelCom->m_Animations[m_pSelectedModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 100.f;
					//ÇöÀç 25¿¡¼­ µ¹¾Æ¿ÀÁö ¾Ê´Â ¹®Á¦°¡ ÀÖÀ½...?


					CFrameEvent_Manager::Get_Instance()->UseEvent(event, this);

				}
			}

		}
	}


}

_bool CCharacter::InputCommand()
{



	ButtonInput iAttackkey = ATTACK_NONE;
	DirectionInput iMoveKey = MOVEKEY_NEUTRAL;

	_int DirectionX = 0;
	_int DirectionY = 0;


	if (m_iPlayerTeam == 1)
	{

		if (m_pGameInstance->Key_Pressing(DIK_W))
		{
			DirectionY = 1;
		}

		else if (m_pGameInstance->Key_Pressing(DIK_S))
		{
			DirectionY = -1;
		}

		if (m_pGameInstance->Key_Pressing(DIK_A))
		{
			DirectionX -= m_iLookDirection;
		}

		else if (m_pGameInstance->Key_Pressing(DIK_D))
		{
			DirectionX += m_iLookDirection;
		}

		//if(DirectionX==0 && DirectionY ==0)

		if (DirectionX == -1 && DirectionY == 0)
			iMoveKey = MOVEKEY_LEFT;

		else if (DirectionX == 0 && DirectionY == 1)
			iMoveKey = MOVEKEY_UP;

		else if (DirectionX == -1 && DirectionY == -1)
			iMoveKey = MOVEKEY_DOWN_LEFT;

		else if (DirectionX == 0 && DirectionY == -1)
			iMoveKey = MOVEKEY_DOWN;

		else if (DirectionX == 1 && DirectionY == -1)
			iMoveKey = MOVEKEY_DOWN_RIGHT;

		else if (DirectionX == 1 && DirectionY == 0)
			iMoveKey = MOVEKEY_RIGHT;

		else if (DirectionX == -1 && DirectionY == 1)
			iMoveKey = MOVEKEY_UP_LEFT;

		else if (DirectionX == 1 && DirectionY == 1)
			iMoveKey = MOVEKEY_UP_RIGHT;

		//µÑ ´Ù 0ÀÎ°æ¿ì´Â ±âº»°ªÀÌ¹Ç·Î ÁöÁ¤ÇÏÁö ¾ÊÀ½ ´ë°¢¼± À§´Â ¾²ÀÌ´Â Ä¿¸Çµå°¡ ¾øÀ¸¹Ç·Î ÁöÁ¤ÇÏÁö ¾ÊÀ½



		if (m_pGameInstance->Key_Down(DIK_U))
		{
			iAttackkey = ATTACK_LIGHT;
		}

		if (m_pGameInstance->Key_Down(DIK_I))
		{
			iAttackkey = ATTACK_MEDIUM;

		}
		if (m_pGameInstance->Key_Down(DIK_J))
		{
			iAttackkey = ATTACK_SPECIAL;

		}
		if (m_pGameInstance->Key_Down(DIK_K))
		{
			iAttackkey = ATTACK_HEAVY;

		}


		//if (m_pGameInstance->Key_Pressing(DIK_O))
		if (m_pGameInstance->Key_Down(DIK_O))
		{
			iAttackkey = ATTACK_GRAB;

		}
		// if (m_pGameInstance->Key_Pressing(DIK_Y))
		// {
		//	 iAttackkey = ATTACK_LIGHT;
		//
		// }

		GetUI_Input(DirectionX, DirectionY, iMoveKey, iAttackkey);

	}
	else  //2ÆÀ
	{
		if (m_pGameInstance->Key_Pressing(DIK_UP))
		{
			DirectionY = 1;
		}

		else if (m_pGameInstance->Key_Pressing(DIK_DOWN))
		{
			DirectionY = -1;
		}

		if (m_pGameInstance->Key_Pressing(DIK_LEFT))
		{
			DirectionX -= m_iLookDirection;
		}

		else if (m_pGameInstance->Key_Pressing(DIK_RIGHT))
		{
			DirectionX += m_iLookDirection;
		}

		//if(DirectionX==0 && DirectionY ==0)

		if (DirectionX == -1 && DirectionY == 0)
			iMoveKey = MOVEKEY_LEFT;

		else if (DirectionX == 0 && DirectionY == 1)
			iMoveKey = MOVEKEY_UP;

		else if (DirectionX == -1 && DirectionY == -1)
			iMoveKey = MOVEKEY_DOWN_LEFT;

		else if (DirectionX == 0 && DirectionY == -1)
			iMoveKey = MOVEKEY_DOWN;

		else if (DirectionX == 1 && DirectionY == -1)
			iMoveKey = MOVEKEY_DOWN_RIGHT;

		else if (DirectionX == 1 && DirectionY == 0)
			iMoveKey = MOVEKEY_RIGHT;

		else if (DirectionX == -1 && DirectionY == 1)
			iMoveKey = MOVEKEY_UP_LEFT;

		else if (DirectionX == 1 && DirectionY == 1)
			iMoveKey = MOVEKEY_UP_RIGHT;


		//µÑ ´Ù 0ÀÎ°æ¿ì´Â ±âº»°ªÀÌ¹Ç·Î ÁöÁ¤ÇÏÁö ¾ÊÀ½ ´ë°¢¼± À§´Â ¾²ÀÌ´Â Ä¿¸Çµå°¡ ¾øÀ¸¹Ç·Î ÁöÁ¤ÇÏÁö ¾ÊÀ½



		if (m_pGameInstance->Key_Down(DIK_NUMPAD7))
		{
			iAttackkey = ATTACK_LIGHT;
		}

		if (m_pGameInstance->Key_Down(DIK_NUMPAD8))
		{
			iAttackkey = ATTACK_MEDIUM;

		}
		if (m_pGameInstance->Key_Down(DIK_NUMPAD4))
		{
			iAttackkey = ATTACK_SPECIAL;

		}
		if (m_pGameInstance->Key_Down(DIK_NUMPAD5))
		{
			iAttackkey = ATTACK_HEAVY;

		}


		if (m_pGameInstance->Key_Down(DIK_NUMPAD9))
		{
			iAttackkey = ATTACK_GRAB;

		}
		// if (m_pGameInstance->Key_Pressing(DIK_Y))
		// {
		//	 iAttackkey = ATTACK_LIGHT;
		//
		// }
	}


	_bool bNewKey = false;

	CInput newInput(iMoveKey, iAttackkey);

	if (inputBuffer.size() > 0)
	{
		//¸¶Áö¸· »óÅÂ¿Í Å° ÀÔ·Â »óÅÂ°¡ ¶È°°À¸¸é °»½ÅÇÏÁö ¾ÊÀ½
		if (inputBuffer.back() == newInput)
		{

		}
		else
		{
			UpdateInputBuffer(CInput(iMoveKey, iAttackkey));
			bNewKey = true;
		}
	}
	else
	{
		UpdateInputBuffer(CInput(iMoveKey, iAttackkey));
		bNewKey = true;
	}




	return bNewKey;
}

void CCharacter::InputedCommandUpdate(_float fTimeDelta)
{



	inputBuffer.erase(
		remove_if(inputBuffer.begin(), inputBuffer.end(), [fTimeDelta](CInput& input) {
			input.frameTime += fTimeDelta;  // frameTime¿¡ fTimeDelta ´õÇÏ±â
			return input.frameTime >= 0.35f; // 0.3 ÀÌ»óÀÎ °æ¿ì »èÁ¦
			}),
		inputBuffer.end()
	);



}


_bool CCharacter::Character_Play_Animation(_float fTimeDelta)
{

	_bool bAnimationEnd = false;

	_float fPrePosition = m_pModelCom->m_fCurrentAnimPosition;

	_int iOneFrameTeest = 0;

	if (fPrePosition == 0)
	{

		if (m_pGameInstance->Key_Pressing(DIK_D))
		{
			_bool bDebug = true;
		}

		ProcessEventsFramesZero(m_eCharacterIndex, m_pModelCom->m_iCurrentAnimationIndex);
		fPrePosition += 0.001;

		iOneFrameTeest++;
	}

	if (m_pModelCom->Play_Animation_Lick(fTimeDelta))
	{
		//¸ð¼ÇÀÌ ³¡³µÀ¸¸é, ·çÇÁ¸é    (¾Æ±î±îÁø ·çÇÁ°¡ ¾Æ´Ï¿´´Âµ¥ ÀÌ¹ø¿¡ ·çÇÁ¸é ¾îÂ¼Áö?)
		if (m_pModelCom->m_isLoopAnim)
		{
			fPrePosition = 0.001;
			ProcessEventsFramesZero(m_eCharacterIndex, m_pModelCom->m_iCurrentAnimationIndex);
			iOneFrameTeest++;
		}
		bAnimationEnd = true;
		m_bMotionPlaying = false;
	}
	else
		m_bMotionPlaying = true;


	_float fCurPosition = m_pModelCom->m_fCurrentAnimPosition;


	ProcessEventsBetweenFrames2(0, m_pModelCom->m_iCurrentAnimationIndex, fPrePosition, fCurPosition);

	return bAnimationEnd;
}

_bool CCharacter::Check_bCurAnimationisAirMove(_uint iAnimation)
{
	_uint iModelIndex = iAnimation;

	if (iAnimation == 1000)
		iModelIndex = m_pModelCom->m_iCurrentAnimationIndex;



	//if (iModelIndex == m_iHit_Air_LightAnimationIndex || iModelIndex == m_iHit_Away_LeftAnimationIndex || iModelIndex == m_iHit_Away_UpAnimationIndex || iModelIndex == m_iHit_Air_FallAnimationIndex)
	if (iModelIndex == m_iJumpAnimationIndex || iModelIndex == m_iFallAnimationIndex)
	{
		return true;
	}

	return false;
}

bool CCharacter::CheckCommandSkippingExtras(const vector<CInput>& pattern, int timeWindow)
{
	if (inputBuffer.size() < pattern.size()) return false;

	int patternIndex = 0;

	// ÀÔ·Â ¹öÆÛ¿¡¼­ ÆÐÅÏÀ» Ã£±â ½ÃÀÛ
	for (int i = 0; i < inputBuffer.size(); ++i) {
		const CInput& input = inputBuffer[i];
		const CInput& expected = pattern[patternIndex];

		// ºÒÇÊ¿äÇÑ ÀÔ·ÂÀº °Ç³Ê¶Ü
		if (input.direction == expected.direction && input.button == expected.button) {
			// ÆÐÅÏÀÌ ÀÏÄ¡ÇÏ¸é ´ÙÀ½ ÆÐÅÏÀ¸·Î ³Ñ¾î°¨
			patternIndex++;

			// ¸ðµç ÆÐÅÏÀÌ ÀÏÄ¡ÇÏ¸é ¼º°ø
			if (patternIndex >= pattern.size())
			{
				inputBuffer.clear();
				return true;
			}
		}


	}

	return false;
}

bool CCharacter::CheckCommand_Exactly(const std::vector<CInput>& pattern, int timeWindow)
{
	if (inputBuffer.size() < pattern.size()) return false;  // ÀÔ·Â ¹öÆÛ°¡ ÆÐÅÏº¸´Ù ÂªÀ¸¸é ½ÇÆÐ


	// ÀÔ·Â ¹öÆÛ ÀüÃ¼¸¦ ¼øÈ¸ÇÏ¸é¼­ ÆÐÅÏÀ» Ã£À½
	for (int i = 0; i <= inputBuffer.size() - pattern.size(); ++i)
	{
		bool isPatternMatched = true;

		// ÇöÀç À§Ä¡ºÎÅÍ ÆÐÅÏÀ» ºñ±³
		for (int j = 0; j < pattern.size(); ++j) {
			const CInput& inputCheck = inputBuffer[i + j];
			const CInput& expected = pattern[j];

			// ÆÐÅÏ°ú ÀÏÄ¡ÇÏÁö ¾ÊÀ¸¸é ½ÇÆÐ
			if (inputCheck.direction != expected.direction || inputCheck.button != expected.button) {
				isPatternMatched = false;
				break;
			}


		}

		// ÆÐÅÏÀÌ ÀÏÄ¡ÇßÀ¸¸é true ¹ÝÈ¯
		if (isPatternMatched)
		{
			inputBuffer.clear();
			return true;
		}
	}

	return false;  // ÆÐÅÏÀ» Ã£Áö ¸øÇÏ¸é ½ÇÆÐ
}


_uint CCharacter::CheckAllCommands()
{


	for (const auto& command : MoveCommandPatternsFunction)
	{
		if (CheckCommandSkippingExtras(command.pattern, 0)) {
			command.action();  // ÇØ´ç ÆÐÅÏÀÌ ¸ÅÄªµÇ¸é ÇØ´ç ±â¼ú ½ÇÇà

			return 0;
		}
	}


	//Á¤È®ÇØ¾ßÇÏ¸¸ ÇÏ´Â ÆÐÅÏ µû·Î ±¸ºÐ
	for (const auto& command : MoveCommandPatternsFunction_Exactly)
	{
		if (CheckCommand_Exactly(command.pattern, 0)) {
			command.action();  // ÇØ´ç ÆÐÅÏÀÌ ¸ÅÄªµÇ¸é ÇØ´ç ±â¼ú ½ÇÇà

			return 0;
		}
	}
	return 0;


}

void CCharacter::ShowInputBuffer()
{
	inputBuffer;
	m_fGravityTime;
	m_pModelCom->m_iCurrentAnimationIndex;
	m_pModelCom->m_fCurrentAnimPosition;
	_float fHeight = Get_fHeight();
	_bool bDebug = true;
}

void CCharacter::DebugPositionReset(_short iDirection)
{
	_float fHegiht = Get_fHeight();

	if (iDirection == 0)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { (_float)m_iPlayerTeam,fHegiht,0,1 });
	else if (iDirection == -1)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { -10.5f + (_float)m_iPlayerTeam,fHegiht,0,1 });
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 10.f + (_float)m_iPlayerTeam,fHegiht,0,1 });
	}

}

void CCharacter::FlipDirection(_int iDirection)
{
	if (iDirection == 0)
	{
		m_iLookDirection = -m_iLookDirection;

		m_pTransformCom->Set_Scaled(-1, 1, 1);
	}
	else if (m_iLookDirection != iDirection)
	{

		m_iLookDirection = iDirection;

		m_pTransformCom->Set_Scaled(-1, 1, 1);
	}

	//m_pTransformCom->Set_Scaled(-1, 1, 1);

}

void CCharacter::Create_Effect(_int iEffectIndex)
{
	switch (iEffectIndex)
	{
	case 0:
		//Create_kamehameha();
		break;

	case 1:
		//Create_Smash();
		break;

	default:
		break;
	}
}

_float CCharacter::Get_fHeight()
{
	return 	XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

}

void CCharacter::Set_ForcedGravityDown()
{
	//Áß·Â ÃÖ°íÁ¡
	//if (m_fGravityTime < 0.305)
	//	m_fGravityTime = 0.305f;

	//°øÁß ¾Õ´ë½Ã 
	m_fGravityTime = 0.255f;


}

void CCharacter::Set_ForcedGravityTime_LittleUp()
{
	m_fGravityTime = 0.135f;

	if (m_fJumpPower == 0)
	{
		m_fJumpPower = fJumpPower;
	}
}

void CCharacter::Set_ForcveGravityTime(_float fGravityTime)
{
	m_fGravityTime = fGravityTime;
}

void CCharacter::AttckCancleJump()
{

	if (m_pModelCom->m_iCurrentAnimationIndex == m_iStandingMidAttackAnimationIndex)
	{

		//1ÆÀ
		if (m_iPlayerTeam == 1 && m_pGameInstance->Key_Down(DIK_W))
		{

			//Set_fJumpPower(4.f); //Áß·ÂVer1 ±âÁØ
			Set_fJumpPower(3.f); //Áß·ÂVer2 ±âÁØ


			//if (m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex && m_bDoubleJumpEnable)
			//{
			//	//Set_NextAnimation(m_iJumpAnimationIndex, 0.5f);
			//}
			Set_NextAnimation(m_iJumpAnimationIndex, 0.5f);



			if (m_pGameInstance->Key_Pressing(DIK_A))
			{
				Set_fImpulse(-5.f);
			}

			else if (m_pGameInstance->Key_Pressing(DIK_D))
			{
				Set_fImpulse(5.f);
			}
		}

		//2ÆÀ
		else if (m_iPlayerTeam == 2 && m_pGameInstance->Key_Down(DIK_UP))
		{
			//Set_fJumpPower(4.f); //Áß·ÂVer1 ±âÁØ
			Set_fJumpPower(3.f); //Áß·ÂVer2 ±âÁØ

			//if(m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex && m_bDoubleJumpEnable)
			//{
			//	//Set_NextAnimation(m_iJumpAnimationIndex, 0.5f);
			//}
			Set_NextAnimation(m_iJumpAnimationIndex, 0.5f);

			if (m_pGameInstance->Key_Pressing(DIK_LEFT))
			{
				Set_fImpulse(-5.f);
			}

			else if (m_pGameInstance->Key_Pressing(DIK_RIGHT))
			{
				Set_fImpulse(5.f);
			}
		}

	}


	else if (m_bDoubleJumpEnable && (
		m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex ||
		m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_Air1 || m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_Air2 || m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_AirUpper))
	{

		//1ÆÀ
		if (m_iPlayerTeam == 1 && m_pGameInstance->Key_Down(DIK_W))
		{

			//Set_fJumpPower(4.f); //Áß·ÂVer1 ±âÁØ
			Set_fJumpPower(3.f); //Áß·ÂVer2 ±âÁØ
			//Set_ForcveGravityTime(0.08f); //¹¦ÇÏ°Ô ³·Àºµí
			Set_ForcveGravityTime(0.03f);


			//Set_ForcedGravityTime_LittleUp() //0.135f

			//if (m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex && m_bDoubleJumpEnable)
			//{
			//	//Set_NextAnimation(m_iJumpAnimationIndex, 0.5f);
			//}
			//Set_NextAnimation(m_iJumpAnimationIndex, 0.5f);

			if (m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex)
				m_pModelCom->CurrentAnimationPositionJump(0.f);

			else
				Set_NextAnimation(m_iJumpAnimationIndex, 0.5f);



			if (m_pGameInstance->Key_Pressing(DIK_A))
			{
				Set_fImpulse(-5.f);
			}

			else if (m_pGameInstance->Key_Pressing(DIK_D))
			{
				Set_fImpulse(5.f);
			}
			else
				Set_fImpulse(0.f);


			m_bDoubleJumpEnable = false;

		}

		//2ÆÀ
		else if (m_iPlayerTeam == 2 && m_pGameInstance->Key_Down(DIK_UP))
		{
			//Set_fJumpPower(4.f); //Áß·ÂVer1 ±âÁØ
			Set_fJumpPower(3.f); //Áß·ÂVer2 ±âÁØ
			Set_ForcveGravityTime(0.03f);

			if (m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex)
				m_pModelCom->CurrentAnimationPositionJump(0.f);

			else
				Set_NextAnimation(m_iJumpAnimationIndex, 0.5f);




			Set_ForcveGravityTime(0.03f);

			if (m_pGameInstance->Key_Pressing(DIK_LEFT))
			{
				Set_fImpulse(-5.f);
			}

			else if (m_pGameInstance->Key_Pressing(DIK_RIGHT))
			{
				Set_fImpulse(5.f);
			}
			else
				Set_fImpulse(0.f);


			m_bDoubleJumpEnable = false;

		}


	}

}

/*

void CCharacter::Chase(_float fTimeDelta)
{

	m_pModelCom->SetUp_Animation(m_iChaseAnimationIndex, false);
	m_pModelCom->Play_Animation(0.f);


	CTransform* pTarget = static_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Target"), TEXT("Com_Transform")));
	_vector vTargetPos = pTarget->Get_State(CTransform::STATE_POSITION);

	_vector vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	m_fAccChaseTime += fTimeDelta;

	_vector vDir = XMVector4Normalize(vTargetPos - vMyPos);
	Set_fImpulse(XMVectorGetX(vDir) * 2.f);

	//_float angle = atan2(XMVectorGetY(vDir), XMVectorGetX(vDir));
	_float angle = atan2(XMVectorGetY(vDir), XMVectorGetX(vDir)) * (180.0 / 3.14);

	angle = (angle + 90) * 0.5f;
	//cout << angle << endl;



	if (0 < angle && angle < 90)  //ÀûÀÌ ¿À¸¥ÂÊ¿¡ ÀÖ´Â °æ¿ì
	{
		FlipDirection(1);
	}

	else if (angle > 90)   //ÀûÀÌ ¿ÞÂÊ À§¿¡ ÀÖ´Â °æ¿ì
	{
		//110ÀÇ °æ¿ì 70À¸·Î ¹Ù²ã¾ß ÇÑ´Ù.    ÃÊ°ú°ª 20.   90À¸·ÎºÎÅÍ ÃÊ°ú°ª ¸¸Å­ »©¸é µÊ
		// angle = 90 - (90 - angle);    =  180-angle;

		FlipDirection(-1);
		angle = 180 - angle;
	}
	else if (angle < 0)   //ÀûÀÌ ¿ÞÂÊ¿¡ ¾Æ·¡¿¡ ÀÖ´Â °æ¿ì
	{
		FlipDirection(-1);
		angle = -angle;
	}


	//ÃßÀû ¼Óµµ¸¦ Á¡Á¡ ºü¸£°Ô
	vDir = vDir * m_fAccChaseTime * 0.5f;

	m_pTransformCom->Add_MoveVector(vDir);
	Set_CurrentAnimationPositionJump(angle);
}
*/
void CCharacter::Chase2(_float fTimeDelta)
{




	//µð¹ö±×¿ë ¿¹¿ÜÃ³¸®.  ¸ØÃç¹ö¸®¸é Áö¶ö³²
	if (fTimeDelta > 1)
	{
		return;
	}

	if (m_bStun)
	{
		m_bChase = false;

	}

	m_fAccChaseTime += fTimeDelta;


	//ÁØºñÀÚ¼¼¸é ÀÌ·¸°Ô ÇÑ´Ù.
	if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex)
	{

		if (m_fAccChaseTime > 0.2f)
		{
			m_pModelCom->SetUp_Animation(m_iChaseAnimationIndex, false);
			m_fJumpPower = fJumpPower;

			//if (m_bChaseAttackEnable)
			{
				//°ø°ÝÆÇÁ¤ Å×½ºÆ®
				{

					CAttackObject::ATTACK_DESC Desc{};

					if (m_iPlayerTeam == 1)
						Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_Melee_Attack;
					else
						Desc.ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_Melee_Attack;
					Desc.ColliderDesc.pMineGameObject = this;
					Desc.ColliderDesc.vCenter = { 0.f,0.5f,0.f };
					Desc.ColliderDesc.vExtents = { 1.f,1.f,0.2f };


					Desc.fhitCharacter_Impus = { 0.7f * m_iLookDirection,0.3f };

					Desc.fhitCharacter_StunTime = 0.6f;
					Desc.iDamage = 300 * Get_DamageScale();
					Desc.fLifeTime = 5.f;
					Desc.ihitCharacter_Motion = { HitMotion::HIT_CHASE };

					Desc.iTeam = m_iPlayerTeam;
					Desc.fAnimationLockTime = 0.1f;
					Desc.pOwner = this;

					m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack_Chase"), TEXT("Layer_AttackObject"), &Desc);

					//m_pChaseAttackObejct = static_cast<CAttackObject*>(m_pGameInstance->Add_GameObject_ToLayer_AndGet(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack_Chase"), TEXT("Layer_AttackObject"), &Desc));


					//m_pChaseAttackObejct = { nullptr };
					//Set_RemoteDestory()

				}
				//m_bChaseAttackEnable = false;
			}
		}
		else
		{
			return;
		}
	}

	//µ¹ÁøÁßÀÌ¸é ÀÌ·¸°Ô ÇÑ´Ù
	else if (m_pModelCom->m_iCurrentAnimationIndex == m_iChaseAnimationIndex)
	{
		if (m_fAccChaseTime > 5.f)
		{
			m_bChase = false;
			//if (m_pChaseAttackObejct != nullptr)
			//{
			//	m_pChaseAttackObejct->Set_RemoteDestory();
			//	m_pChaseAttackObejct = nullptr;
			//}
			return;
		}
	}
	else   //µ¹ÁøÁßµµ ÁØºñÀÚ¼¼µµ ¾Æ´Ï¸é
	{
		return;
		AttackNextMoveCheck();

	}


	//CTransform* pTarget = static_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Target"), TEXT("Com_Transform")));
	CTransform* pTarget = static_cast<CTransform*>(m_pEnemy->Get_Component(TEXT("Com_Transform")));

	_vector vTargetPos = pTarget->Get_State(CTransform::STATE_POSITION);

	_vector vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);



	_float vLength = GetVectorLength((vTargetPos - vMyPos));
	//m_pChaseAttackObejct

	if (vLength < 0.5f) //0.3
	{
		m_bChase = false;


		//Å×½ºÆ®
		m_fAccChaseTime = 0.f;
		m_fGravityTime = 0.185f;
		m_pModelCom->SetUp_Animation(m_iFallAnimationIndex, false);


		return;
	}


	m_vChaseDir = XMVector4Normalize(vTargetPos - vMyPos);
	Set_fImpulse(XMVectorGetX(m_vChaseDir) * 2.f);




	//¾Ö´Ï¸ÞÀÌ¼Ç ÀÌ¿ëÀ» À§ÇØ °¢µµ°ªÀ» Æ¯¼ö Ã³¸® ÇÒ ÇÊ¿ä°¡ ÀÖÀ½
	_float angle = atan2(XMVectorGetY(m_vChaseDir), XMVectorGetX(m_vChaseDir)) * (180.0 / 3.14);
	angle = (angle + 90) * 0.5f;

	cout << angle << endl;

	if (0 < angle && angle < 90)  //ÀûÀÌ ¿À¸¥ÂÊ¿¡ ÀÖ´Â °æ¿ì
	{
		//Ä³¸¯ÅÍ º¸´Â ¹æÇâ ¿À¸¥ÂÊÀ¸·Î º¯°æ
		FlipDirection(1);
	}

	else if (angle > 90)   //ÀûÀÌ ¿ÞÂÊ À§¿¡ ÀÖ´Â °æ¿ì 
	{
		//110ÀÇ °æ¿ì 70À¸·Î ¹Ù²ã¾ß ÇÑ´Ù.    ÃÊ°ú°ª 20.   90À¸·ÎºÎÅÍ ÃÊ°ú°ª ¸¸Å­ »©¸é µÊ
		// angle = 90 - (90 - angle);    =  180-angle;

		FlipDirection(-1);
		angle = 180 - angle;
	}
	else if (angle < 0)   //ÀûÀÌ ¿ÞÂÊ¿¡ ¾Æ·¡¿¡ ÀÖ´Â °æ¿ì 
	{
		FlipDirection(-1);
		angle = -angle;
	}


	//ÃßÀû ¼Óµµ¸¦ Á¡Á¡ ºü¸£°Ô
	//m_pTransformCom->Add_MoveVector(m_vChaseDir * m_fAccChaseTime * 0.5f);
	m_pTransformCom->Add_MoveVector(m_vChaseDir * m_fAccChaseTime * m_fAccChaseTime * 0.7f);



	//¾Ö´Ï¸ÞÀÌ¼ÇÀÇ positionÀÌ °¢µµ¸¦ ÀÇ¹ÌÇÔ (1:1Àº ¾Æ´Ï°í Æ¯¼öÃ³¸®µÇ¾îÀÖÀ½)
	Set_CurrentAnimationPositionJump(angle);



	//¹Ýµå½Ã SetÀÌ´ø ½ÃÀý ÄÚµå ¹é¾÷¿ë.
	/*
	//m_bChase °¡ trueÀÏ ¶§¸¸ µé¾î¿Ã°Í



	//µð¹ö±×¿ë ¿¹¿ÜÃ³¸®.  ¸ØÃç¹ö¸®¸é Áö¶ö³²
	if (fTimeDelta > 1)
	{
		return;
	}



	m_fAccChaseTime += fTimeDelta;
	//if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex && m_fAccChaseTime > 0.3f)
	if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex)
	{

		if (m_fAccChaseTime > 0.2f)
		{
			m_pModelCom->SetUp_Animation(m_iChaseAnimationIndex, false);
			m_fJumpPower = fJumpPower;
		}
		else
		{
			return;
		}
	}

	else if (m_fAccChaseTime > 5.f)
	{
		m_bChase = false;
		return;
	}


	//CTransform* pTarget = static_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Target"), TEXT("Com_Transform")));
	CTransform* pTarget = static_cast<CTransform*>(m_pEnemy->Get_Component(TEXT("Com_Transform")));

	_vector vTargetPos = pTarget->Get_State(CTransform::STATE_POSITION);

	_vector vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);



	_float vLength = GetVectorLength((vTargetPos - vMyPos));
	if (vLength < 0.5f) //0.3
	{
		m_bChase = false;



		//Å×½ºÆ®
		m_fAccChaseTime = 0.f;
		m_fGravityTime = 0.185f;
		m_pModelCom->SetUp_Animation(m_iFallAnimationIndex, false);


		return;
	}


	m_vChaseDir = XMVector4Normalize(vTargetPos - vMyPos);
	Set_fImpulse(XMVectorGetX(m_vChaseDir) * 2.f);




	//¾Ö´Ï¸ÞÀÌ¼Ç ÀÌ¿ëÀ» À§ÇØ °¢µµ°ªÀ» Æ¯¼ö Ã³¸® ÇÒ ÇÊ¿ä°¡ ÀÖÀ½
	_float angle = atan2(XMVectorGetY(m_vChaseDir), XMVectorGetX(m_vChaseDir)) * (180.0 / 3.14);
	angle = (angle + 90) * 0.5f;

	cout << angle << endl;

	if (0 < angle && angle < 90)  //ÀûÀÌ ¿À¸¥ÂÊ¿¡ ÀÖ´Â °æ¿ì
	{
		//Ä³¸¯ÅÍ º¸´Â ¹æÇâ ¿À¸¥ÂÊÀ¸·Î º¯°æ
		FlipDirection(1);
	}

	else if (angle > 90)   //ÀûÀÌ ¿ÞÂÊ À§¿¡ ÀÖ´Â °æ¿ì
	{
		//110ÀÇ °æ¿ì 70À¸·Î ¹Ù²ã¾ß ÇÑ´Ù.    ÃÊ°ú°ª 20.   90À¸·ÎºÎÅÍ ÃÊ°ú°ª ¸¸Å­ »©¸é µÊ
		// angle = 90 - (90 - angle);    =  180-angle;

		FlipDirection(-1);
		angle = 180 - angle;
	}
	else if (angle < 0)   //ÀûÀÌ ¿ÞÂÊ¿¡ ¾Æ·¡¿¡ ÀÖ´Â °æ¿ì
	{
		FlipDirection(-1);
		angle = -angle;
	}


	//ÃßÀû ¼Óµµ¸¦ Á¡Á¡ ºü¸£°Ô
	//m_pTransformCom->Add_MoveVector(m_vChaseDir * m_fAccChaseTime * 0.5f);
	m_pTransformCom->Add_MoveVector(m_vChaseDir * m_fAccChaseTime * m_fAccChaseTime );



	//¾Ö´Ï¸ÞÀÌ¼ÇÀÇ positionÀÌ °¢µµ¸¦ ÀÇ¹ÌÇÔ (1:1Àº ¾Æ´Ï°í Æ¯¼öÃ³¸®µÇ¾îÀÖÀ½)
	Set_CurrentAnimationPositionJump(angle);

	*/
}

void CCharacter::Chase_Ready(_float fTimeDelta)
{

	if (m_bChaseEnable == false || m_pModelCom->m_iCurrentAnimationIndex == m_iSparkingAnimationIndex)
		return;


	m_bChaseEnable = false;

	if (m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_LightLast)
	{
		m_bChaseStoping = false;

		Set_NextAnimation(m_iFallAnimationIndex, 5.f);

		m_bChase = true;


	}


	if (Check_bCurAnimationisGroundMove() || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex)
	{

		m_bChaseStoping = false;

		m_pModelCom->SetUp_Animation(m_iFallAnimationIndex, false);

		m_bChase = true;

		if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) < 0.5)
		{
			m_pTransformCom->Add_Move({ 0.f,0.6f,0.f });

		}



	}
	else
	{
		Set_NextAnimation(m_iFallAnimationIndex, 1.f);
		m_bChase = true;
	}








}

void CCharacter::Set_ChaseStoping()
{
	m_bChaseStoping = true;
}

void CCharacter::Chase_Grab(_float fTimeDelta)
{

	//µð¹ö±×¿ë ¿¹¿ÜÃ³¸®.  ¸ØÃç¹ö¸®¸é Áö¶ö³²
	if (fTimeDelta > 1)
	{
		return;
	}

	//Àâ±â ¼º°ø½Ã ÀûÀ» ÃßÀûÇÏÁö ¾ÊÀ½
	if (m_pModelCom->m_fCurrentAnimPosition > 25)
	{




		return;
	}


	//ÁØºñÀÚ¼¼¸é ÀÌ·¸°Ô ÇÑ´Ù.
	if (m_pModelCom->m_iCurrentAnimationIndex == m_iGrabReadyAnimationIndex)
	{
		//ÀÌÆåÆ®»ý¼º 1È¸?
		//if (m_fAccGrabTime == fTimeDelta)
		if (m_fAccGrabTime == 0.f)
		{
			;
		}
		return;
	}

	//ÁØºñÀÚ¼¼µµ Àâ±âÀÚ¼¼µµ ¾Æ´Ñµ¥ ÀÌ ÇÔ¼ö¿¡ µé¾î¿Â °æ¿ì ÃÊ±âÈ­ ÈÄ Á¾·á.
	else if (m_pModelCom->m_iCurrentAnimationIndex != m_iGrabAnimationIndex)
	{
		m_fAccGrabTime = 0.f;
		m_bGrab = false;
		return;
	}
	else if (m_fAccChaseTime > 1.2) //½Ã°£Á¦ÇÑÀ¸·Î Çª´Â°Ô ¾Æ´Ï¶ó  1.2ÃÊ ÀÌ»ó Áö¼ÓµÉ ¼ö ¾øÀ¸´Ï °­Á¦ÇØÁ¦.
	{
		m_fAccGrabTime = 0.f;
		m_bGrab = false;
		return;
	}
	m_fAccGrabTime += fTimeDelta;








	CTransform* pTarget = static_cast<CTransform*>(m_pEnemy->Get_Component(TEXT("Com_Transform")));

	_vector vTargetPos = pTarget->Get_State(CTransform::STATE_POSITION);
	_vector vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


	m_vChaseDir = XMVector4Normalize(vTargetPos - vMyPos);
	Set_fImpulse(XMVectorGetX(m_vChaseDir) * 2.f);

	if (m_bGrab_Air == false)
	{
		m_vChaseDir = XMVectorSetY(m_vChaseDir, 0.f);
	}


	//m_pTransformCom->Add_MoveVector(m_vChaseDir * (15 - m_fAccChaseTime*m_fAccChaseTime*3.f) * fTimeDelta);
	m_pTransformCom->Add_MoveVector(m_vChaseDir * (15 - m_fAccChaseTime * m_fAccChaseTime * 10.f) * fTimeDelta);



}


void CCharacter::Character_Attack_Grab(_float fTimeDelta)
{
	//°ø°ÝÁßÀÎ°æ¿ì
	//if(m_pModelCom->m_fCurrentAnimPosition > 50 || m_pModelCom->m_fCurrentAnimPosition < 270)
	if (m_pModelCom->m_fCurrentAnimPosition > 120 && m_pModelCom->m_fCurrentAnimPosition < 138)
	{

		//¿ì´Ù´Ù È½¼ö ¼Ò¸ðÇÒ¶§±îÁö °ø°Ý.   
		if (m_iGrabLoof > 0)
		{
			cout << m_iGrabLoof << endl;
			m_iGrabLoof--;
			m_pModelCom->CurrentAnimationPositionJump(26.f);
		}
		else //´Ù ›§À¸¸é ÇöÀç »óÅÂ¿¡ µû¶ó ¸¶Áö¸· °ø°Ý
		{
			m_bGrab = false;


			//Àâ±â °ø°ÝÀÌ ºñ°åÀ¸¸é
			if (m_bGrabDraw)
			{
				Add_Move({ -0.4f * m_iLookDirection,0.3f });
				//BreakFall_Air();

				Set_Animation(m_iBreakFall_Air);
				Set_NextAnimation(m_iIdleAnimationIndex, 2.f);
				Set_ForcedGravityDown();

				Reset_AttackStep();
				Set_fImpulse({ -5.f * m_iLookDirection, 2.f });


			}
			else //°ø°Ý ¼º°ø½Ã
			{
				Add_Move({ -0.4f * m_iLookDirection,0.f });
				Set_Animation(m_iAttack_Heavy);

			}

			//else if (m_bGrab_Air)
			//{
			//	Set_Animation(m_iAttack_Air3);
			//}
			//else

		}
	}

}

void CCharacter::Grab_LateDraw()
{
	//Add_Move({ 0.3f * m_iLookDirection,0.3f });
	Add_Move({ 0.f,0.2f });
	Set_GrabLoofCount(1);
	Set_bGrabDraw(true);



	Set_Animation(m_iGrabAnimationIndex, false);
	Set_CurrentAnimationPositionJump(25.f);  	//Ä³¸¯ÅÍ¸¶´Ù ´Ù¸¦¼öµµÀÖÀ½


}


void CCharacter::Move(_float fTimeDelta)
{


	if (Check_bCurAnimationisGroundMove() || m_pModelCom->m_iCurrentAnimationIndex == m_iForwardDashAnimationIndex)
	{
		if (Check_bCurAnimationisGroundMove(m_iNextAnimation.first))
		{



			Reset_AttackCount();

			if (m_iPlayerTeam == 1)
				MoveKey1Team(fTimeDelta);
			else
				MoveKey2Team(fTimeDelta);


		}
	}
}

void CCharacter::MoveKey1Team(_float fTimeDelta)
{
	_short MoveKey = 0;
	if (m_pGameInstance->Key_Pressing(DIK_W) && m_bJumpLock == false)
	{
		m_pTransformCom->Add_Move({ 0,0.3f,0 });

		//Set_fJumpPower(4.f); //Áß·ÂVer1 ±âÁØ
		Set_fJumpPower(3.f); //Áß·ÂVer2 ±âÁØ


		Set_Animation(m_iJumpAnimationIndex);


		if (m_pGameInstance->Key_Pressing(DIK_A))
		{
			Set_fImpulse(-5.f);
		}

		else if (m_pGameInstance->Key_Pressing(DIK_D))
		{
			Set_fImpulse(5.f);
		}

		//Á¡ÇÁ ½ÃÀÛ
		m_pGameInstance->Play_Sound(CSound_Manager::SOUND_KEY_NAME::Goku_Dash_SFX, false, 1.f);
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
				//Æ÷¿öµå ´ë½Ã ²÷°åÀ» ¶§
				m_pGameInstance->Stop_Sound(CSound_Manager::SOUND_KEY_NAME::Goku_DownHook_Dash_SFX);
			}
			else
				m_pModelCom->SetUp_Animation(m_iIdleAnimationIndex, true);

			m_iNextAnimation.first = m_iIdleAnimationIndex;
			m_iNextAnimation.second = 100.f;
		}
	}
}

void CCharacter::MoveKey2Team(_float fTimeDelta)
{
	_short MoveKey = 0;
	if (m_pGameInstance->Key_Pressing(DIK_UP) && m_bJumpLock == false)
	{
		m_pTransformCom->Add_Move({ 0,0.3f,0 });

		//Set_fJumpPower(4.f); //Áß·ÂVer1 ±âÁØ
		Set_fJumpPower(3.f); //Áß·ÂVer2 ±âÁØ


		Set_Animation(m_iJumpAnimationIndex);


		if (m_pGameInstance->Key_Pressing(DIK_LEFT))
		{
			Set_fImpulse(-5.f);
		}

		else if (m_pGameInstance->Key_Pressing(DIK_RIGHT))
		{
			Set_fImpulse(5.f);
		}

		//Á¡ÇÁ ½ÃÀÛ
		m_pGameInstance->Play_Sound(CSound_Manager::SOUND_KEY_NAME::Goku_Dash_SFX, false, 1.f);
	}

	else if (m_pGameInstance->Key_Pressing(DIK_DOWN))
	{
		if (m_pModelCom->m_iCurrentAnimationIndex != m_iForwardDashAnimationIndex)
		{
			m_pModelCom->SetUp_Animation(m_iCrouchAnimationIndex, true);
		}
	}

	else
	{
		if (m_pGameInstance->Key_Pressing(DIK_LEFT))
		{
			MoveKey -= m_iLookDirection;
		}

		else if (m_pGameInstance->Key_Pressing(DIK_RIGHT))
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
				//Æ÷¿öµå ´ë½Ã ²÷°åÀ» ¶§
			}
			else
				m_pModelCom->SetUp_Animation(m_iIdleAnimationIndex, true);

			m_iNextAnimation.first = m_iIdleAnimationIndex;
			m_iNextAnimation.second = 100.f;
		}
	}

}

void CCharacter::Reset_AttackCount()
{
	m_bChaseEnable = true;
	m_bAriDashEnable = true;
}


/*
AttackColliderResult CCharacter::Set_Hit3(_uint eAnimation, AttackGrade eAttackGrade, AttackType eAttackType, _float fStunTime, _uint iDamage, _float fStopTime, _float2 Impus)
{


	if (m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Air || m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Ground || m_pModelCom->m_iCurrentAnimationIndex == m_iBound_Ground)
		return RESULT_MISS;

	//½ºÅÏ»óÅÂ, ¶¥¹Ù´Ú¿¡ ²ÈÇûÀ»¶§ °Ë»ç
	else if (m_bHitGroundSmashed && Get_fHeight() == 0)
	{
		if (eAttackGrade != GRADE_ULTIMATE)
			return RESULT_MISS;
	}

	//½ºÅÏ »óÅÂ°¡ ¾Æ´Ï¸é °¡µå Ã¼Å©
	if (m_bStun == false)
	{

		AttackColliderResult eResult = Guard_Check3(eAttackType);

		if (eResult == RESULT_GUARD)
		{
			if (m_pModelCom->m_iCurrentAnimationIndex == m_iCrouchAnimationIndex)
				Set_Animation(m_iGuard_CrouchAnimationIndex);

			else if (m_pModelCom->m_iCurrentAnimationIndex == m_iIdleAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iBackWalkAnimationIndex)
				Set_Animation(m_iGuard_GroundAnimationIndex);

			else if (m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex)
				Set_Animation(m_iGuard_AirAnimationIndex);

			return RESULT_GUARD;

		}
		else if (eResult != RESULT_HIT)    //È¸ÇÇ³ª ºñ±ä °æ¿ì ¾Æ´Ï¸é ÀÏ´Ü ¼ÓÇà.   Àú °æ¿ì´Â ³ªÁß¿¡ µû·Î Ã³¸®
			return eResult;


	}



	m_bStun = true;

	m_fMaxStunTime = fStunTime;
	m_fAccStunTime = 0.f;

	Set_HitAnimation(eAnimation, Impus);
	Set_AnimationStop(fStopTime);

	Set_bRedHP(true);

	//m_iHP -= iDamage;  // ¿©±â¿¡ ÄÞº¸°è¼ö °öÇÒ°Í
	m_iHP -= iDamage;  // ¿©±â¿¡ ÄÞº¸°è¼ö °öÇÒ°Í


	m_iDebugComoboDamage += iDamage;
	cout << "Dagage : " << iDamage << "  ,  Total : " << m_iDebugComoboDamage << endl;


	if (m_iHP < 0)
	{
		m_iHP = 0;
	}

	return RESULT_HIT;
}
*/
AttackColliderResult CCharacter::Set_Hit4(_uint eAnimation, AttackGrade eAttackGrade, AttackType eAttackType, _float fStunTime, _uint iDamage, _float fStopTime, _short iDirection, _float2 Impus)
{
	if (m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Air || m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Ground || m_pModelCom->m_iCurrentAnimationIndex == m_iBound_Ground)
		return RESULT_MISS;

	//½ºÅÏ»óÅÂ, ¶¥¹Ù´Ú¿¡ ²ÈÇûÀ»¶§ °Ë»ç
	else if (m_bHitGroundSmashed && Get_fHeight() == 0)
	{
		if (eAttackGrade != GRADE_ULTIMATE)
			return RESULT_MISS;
	}

	//½ºÅÏ »óÅÂ°¡ ¾Æ´Ï¸é °¡µå Ã¼Å©
	if (m_bStun == false)
	{

		AttackColliderResult eResult = Guard_Check3(eAttackType);

		//°ø°ÝÀÚÀÇ ¹æÇâÀÌ iDirection  ÇÇ°ÝÀÚ´Â ¸¶ÁÖºÁ¾ßÇÏ´Ï ±× ¹Ý´ë.


		if (eResult == RESULT_GUARD)
		{
			if (m_pModelCom->m_iCurrentAnimationIndex == m_iCrouchAnimationIndex)
				Set_Animation(m_iGuard_CrouchAnimationIndex);

			else if (m_pModelCom->m_iCurrentAnimationIndex == m_iIdleAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iBackWalkAnimationIndex)
				Set_Animation(m_iGuard_GroundAnimationIndex);

			else if (m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex)
				Set_Animation(m_iGuard_AirAnimationIndex);

			Set_CurrentAnimationPositionJump(0.f);

			return RESULT_GUARD;

		}
		else if (eResult != RESULT_HIT)    //È¸ÇÇ³ª ºñ±ä °æ¿ì ¾Æ´Ï¸é ÀÏ´Ü ¼ÓÇà.   Àú °æ¿ì´Â ³ªÁß¿¡ µû·Î Ã³¸®
			return eResult;


	}


	m_bHit = TRUE;
	m_bStun = true;

	m_fMaxStunTime = fStunTime;
	m_fAccStunTime = 0.f;

	Set_HitAnimation(eAnimation, Impus);
	Set_AnimationStop(fStopTime);

	Set_bRedHP(true);

	if (iDirection == 1)
		FlipDirection(-1);
	else if (iDirection == -1)
		FlipDirection(1);

	//0ÀÎ°æ¿ì´Â µÚÁýÁö ¾ÊÀ½


	//m_iHP -= iDamage;  // ¿©±â¿¡ ÄÞº¸°è¼ö °öÇÒ°Í
	m_iHP -= iDamage;  // ¿©±â¿¡ ÄÞº¸°è¼ö °öÇÒ°Í


	m_iDebugComoboDamage += iDamage;
	cout << "Dagage : " << iDamage << "  ,  Total : " << m_iDebugComoboDamage << endl;


	if (m_iHP < 0)
	{
		m_iHP = 0;
	}

	return RESULT_HIT;
}



void CCharacter::Set_HitAnimation(_uint eAnimation, _float2 Impus)
{

	//ÇÏ³ª¶óµµ 0ÀÌ ¾Æ´Ï¸é Àû¿ë
	if (Impus.x != 0 || Impus.y != 0)
	{
		m_fImpuse = Impus;
	}


	switch (eAnimation)
	{
	case Client::HitMotion::HIT_CHASE:  //break ¾øÀ½. °íÀÇÀÓ
	{
		if (Get_fHeight() == 0)
			m_pTransformCom->Add_Move({ 0.f,0.3f,0.f });
	}

	case Client::HitMotion::HIT_LIGHT:
	{
		if (m_pModelCom->m_iCurrentAnimationIndex == m_iCrouchAnimationIndex)
		{
			Set_Animation(m_iHit_Crouch_AnimationIndex, false);
		}
		else if (Get_fHeight() > 0)
		{
			Set_Animation(m_iHit_Air_LightAnimationIndex, false);
			Set_ForcedGravityTime_LittleUp();

		}
		else
			Set_Animation(m_iHit_Stand_LightAnimationIndex, false);
	}
	break;


	case Client::HitMotion::HIT_MEDIUM:
	{
		if (m_pModelCom->m_iCurrentAnimationIndex == m_iCrouchAnimationIndex)
		{
			Set_Animation(m_iHit_Crouch_AnimationIndex, false);
		}
		else if (Get_fHeight() > 0)
		{
			Set_Animation(m_iHit_Air_LightAnimationIndex, false);
			//Set_ForcedGravityTime_LittleUp();
			Set_ForcveGravityTime(0.05f);

		}
		else
			Set_Animation(m_iHit_Stand_LightAnimationIndex, false);
	}
	break;
	case Client::HitMotion::HIT_HEAVY:
		break;
	case Client::HitMotion::HIT_HEAVY_DOWN:
	{

		if (Get_fHeight() > 0)
		{
			Set_Animation(m_iHit_Away_LeftDownAnimationIndex, false);
			Set_ForcveGravityTime(0.3f);
			//Set_CurrentAnimationPositionJump(17.f);
		}
		else
		{
			Add_Move({ 0.f,0.1f });
			Set_Animation(m_iHit_Air_LightAnimationIndex, false);
			Set_ForcedGravityDown();

			Set_CurrentAnimationPositionJump(58.f);
		}


	}
	break;
	case Client::HitMotion::HIT_CROUCH_MEDIUM:
	{
		Set_Animation(m_iHit_Air_FallAnimationIndex);
		m_pTransformCom->Add_Move({ 0.f,0.3f,0.f });
		Set_ForcedGravityTime_LittleUp();
	}
	break;
	case Client::HitMotion::HIT_KNOCK_AWAY_LEFT:
	{
		Set_Animation(m_iHit_Away_LeftAnimationIndex, false);
		//m_pModelCom->CurrentAnimationPositionJump()
		if (Get_fHeight() == 0)
		{
			Add_Move({ 0.f,0.2f });
		}

	}
	break;
	case Client::HitMotion::HIT_KNOCK_AWAY_UP:
	{
		Set_Animation(m_iHit_Away_UpAnimationIndex, false);
		Set_ForcedGravityTime_LittleUp();
	}
	break;

	case Client::HitMotion::HIT_KNOCK_AWAY_LEFTDOWN:
	{
		Set_Animation(m_iHit_Away_LeftDownAnimationIndex, false);
		Set_ForcedGravityTime_LittleUp();
	}
	break;
	case Client::HitMotion::HIT_SPIN_AWAY_LEFTUP:
	{
		Set_Animation(m_iHit_Air_Spin_LeftUp, false);
		m_pTransformCom->Add_Move({ 0.f,0.3f,0.f });
		//Set_ForcedGravityTime_LittleUp();
		Set_ForcveGravityTime(0.f);

	}
	break;

	case Client::HitMotion::HIT_WALLBOUNCE:
	{
		Set_Animation(m_iHit_WallBouce);
	}
	break;
	case Client::HitMotion::HIT_NONE:
	{

	}
	break;

	default:
		break;
	}

}

void CCharacter::Set_AnimationStop(_float fStopTime)
{
	m_bAnimationLock = true;
	m_fMaxAnimationLock = fStopTime;
	m_fAccAnimationLock = 0.f;

	m_pModelCom->Play_Animation(0.f);
}





void CCharacter::Check_StunEnd()
{

}

void CCharacter::Stun_Shake()
{
	m_bStunShakeDirection = !m_bStunShakeDirection;

	m_pTransformCom->Add_Move({ 0.02f - m_bStunShakeDirection * (0.04f),0,0 });

}

void CCharacter::Update_AnimationLock(_float fTimeDelta)
{
	m_fAccAnimationLock += fTimeDelta;
	if (m_fAccAnimationLock > m_fMaxAnimationLock)
	{
		m_bAnimationLock = false;
		m_fAccAnimationLock = 0.f;
	}

}

void CCharacter::Update_StunImpus(_float fTimeDelta)
{

	//°øÁß¿¡¼­ ¸Â°í ³«ÇÏÁß¿¡´Â Áß·Â ºñ½ÁÇÏ°Ô Àû¿ë
	//if (m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_LightAnimationIndex)
	if (m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_LightAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_FallAnimationIndex)
	{
		;
	}

	//¶¥¿¡¼­ ¾àÇÏ°Ô ¸Â¾ÒÀ»°æ¿ì °¡¼Óµµ ÃÖ´ë°ªÀ» Á¦ÇÑÇÔ
	//else if (m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Stand_LightAnimationIndex)
	else if (Check_bCurAnimationisHitGround())
	{
		//À½¼öÀÏ¶§ Á¦ÇÑÀÌ ¾ÈµÊ
		if (m_fImpuse.x > 0.5f)
		{
			m_fImpuse.x = 0.5f;
		}
		else if (m_fImpuse.x < -0.5f)
		{
			m_fImpuse.x = -0.5f;
		}



		m_pTransformCom->Add_Move({ m_fImpuse.x * fTimeDelta, m_fImpuse.y * fTimeDelta, 0 });

	}

	//±× ¿Ü¿¡ ¸Â°í¼­ °­ÇÏ°Ô ³¯¶ó°¡´Â Áß¿¡´Â Àü¿ë °¡¼Óµµ¸¦ ¹ÞÀ½
	else
	{
		m_pTransformCom->Add_Move({ m_fImpuse.x * fTimeDelta, m_fImpuse.y * fTimeDelta, 0 });

		//º®¿¡ Æ¨±æ ¼ö ÀÖ´Â °ø°Ý
		if (m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Away_LeftAnimationIndex)
		{

			//°Å¸®°¡ ¸Ö¾îÁ®¼­ »ý±ä °¡»óÀÇ º®
			//if ( fabsf(Get_fPositionX() - m_pEnemy->Get_fPositionX()) > 8)
			if (Get_fPositionX() < -12.f || Get_fPositionX() > 12.f || fabsf(Get_fPositionX() - m_pEnemy->Get_fPositionX()) > 8)
			{
				Set_Animation(m_iHit_WallBouce);

				CMain_Camera* mainCamera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")));
				mainCamera->StartCameraShake(0.5f, 0.2f);

				Set_AnimationStop(0.2f);

				//playingAnimation == false ¿¡¼­ Ã³¸®?
				//Set_NextAnimation(m_iHit_Air_FallAnimationIndex,1.f);

				//¸Â´Â¼ø°£ º¸´Â ¹æÇâÀÌ °»½ÅµÈ´Ù¸é LookDirectionÀ» ½áµµ µÉÅÙµ¥
				Set_fImpulse({ m_iLookDirection * 2.f,0.5f });

				//Set_ForcedGravityTime_LittleUp();
				Set_ForcveGravityTime(0.f);


				//»õ·Î¿î ½ºÅÏ
				m_fMaxStunTime = 1.f;
				m_fAccStunTime = 0.f;


			}

		}
	}


}

void CCharacter::Set_BreakFall_Ground()
{
	Set_Animation(m_iBreakFall_Ground, 2.f);
	Set_NextAnimation(m_iIdleAnimationIndex, 2.f);

	Set_bRedHP(false);
	Reset_AttackStep();


	DirectionInput iMoveKey = inputBuffer.back().direction;

	if (iMoveKey == MOVEKEY_UP || iMoveKey == MOVEKEY_UP_LEFT)
	{
		Set_fImpulse({ -3.f * m_iLookDirection,1.f });
		Set_ForcveGravityTime(0.f);
	}
	else if (iMoveKey == MOVEKEY_RIGHT)
	{
		//Set_fImpulse({ 0.f , 0.3f });
		Set_fImpulse({ 0.f , 0.1f });

		Set_ForcedGravityTime_LittleUp();
	}


	else //if (iMoveKey == MOVEKEY_LEFT)
	{
		Set_fImpulse({ -5.f * m_iLookDirection, 0.f });
		Set_ForcedGravityTime_LittleUp();
	}





}

void CCharacter::BreakFall_Air()
{
	if (m_bStun)
		return;

	//¶¥¹Ù´Ú¿¡¼­ ÁúÁú ²ø¸®´Â ¸ð¼Çµµ °øÁßÇÇ°ÝÀÌ¶ó Á¶°Ç Ãß°¡ÇØ¾ßÇÔ
	if ((m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_Spin_LeftUp || Check_bCurAnimationisAirHit()) && m_bHitGroundSmashed == false)
	{
		InputCommand();

		CInput InputKey = inputBuffer.back();

		if (InputKey.button != ATTACK_NONE)
		{
			Set_Animation(m_iBreakFall_Air);
			Set_NextAnimation(m_iIdleAnimationIndex, 2.f);
			Set_ForcedGravityDown();

			Reset_AttackStep();


			if (InputKey.direction == MOVEKEY_UP)
			{
				Set_fImpulse({ 0.f, 5.f });
			}
			else if (InputKey.direction == MOVEKEY_DOWN)
			{
				Set_fImpulse({ 0.f, -15.f });
			}

			else if (InputKey.direction == MOVEKEY_LEFT || InputKey.direction == MOVEKEY_UP_LEFT)
			{
				Set_fImpulse({ -10.f * m_iLookDirection, 1.f });
			}

			else //(InputKey.direction == MOVEKEY_RIGHT || InputKey.direction == MOVEKEY_UP_RIGHT)
			{
				Set_fImpulse({ 10.f * m_iLookDirection, 1.f });
			}
			Set_bRedHP(false);


		}

	}
}

void CCharacter::Gain_AttackStep(_ushort iStep)
{
	//{ m_iAttackStepCount += iStep; };

	if (m_iPlayerTeam == 1)
		CBattleInterface_Manager::Get_Instance()->Gain_HitAttackStep(iStep, 2);
	else
		CBattleInterface_Manager::Get_Instance()->Gain_HitAttackStep(iStep, 1);
}

void CCharacter::Gain_HitCount(_ushort iHit)
{
	if (m_iPlayerTeam == 1)
		CBattleInterface_Manager::Get_Instance()->Gain_HitCount(iHit, 2);

	else
		CBattleInterface_Manager::Get_Instance()->Gain_HitCount(iHit, 1);

}

_float CCharacter::Get_DamageScale()
{


	//±øÀ¸·Î ´õÇÏ´Â°Ç ½¬¿îµ¥ 1È÷Æ®´ç 1ÀÌ ¾Æ´Ñµ¥ µû·Î ´õÇØµµ µÇ³ª  //½ºÆÄÅ·µµ ÀÖ´Âµ¥ ¹Þ´ÂÂÊ¿¡¼­ ´õÇÏ´Â°Ô ¾Æ´Ï¶ó ¶§¸®´ÂÂÊ¿¡ ´õÇØ¾ßÇÏ´Â°Å ¾Æ´Ô?
	//µ©°¨ºñÀ²
	//Step Count	0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17 +
	//Next Hit		0%	10%	20% 30% 40% 50% 60% 70% 70% 70% 70% 75% 75% 75% 80% 80% 80% 85%
	//µ¥¹ÌÁöºñÀ²    1.0 0.9 0.8 0              0.3   

	_uint iAttackStepCount;
	if (m_iPlayerTeam == 1)
		iAttackStepCount = CBattleInterface_Manager::Get_Instance()->Get_HitAttackStep(2);
	else
		iAttackStepCount = CBattleInterface_Manager::Get_Instance()->Get_HitAttackStep(1);


	_float fDamageScale;// = 1.f;

	if (iAttackStepCount <= 7)
	{
		fDamageScale = 1.0f - iAttackStepCount * 0.1f;
	}

	else if (iAttackStepCount <= 10)
	{
		fDamageScale = 0.3f;
	}

	else if (iAttackStepCount <= 13)
	{
		fDamageScale = 0.25f;
	}

	else if (iAttackStepCount <= 16)
	{
		fDamageScale = 0.2f;
	}
	else
	{
		fDamageScale = 0.15f;
	}


	if (m_bSparking)
	{
		//fDamageScale += 0.2f;   //ÇÕ¿¬»ê. ³Ê¹« Å«°¡?  15%->35%
		fDamageScale *= 1.2f;	  //°ö¿¬»ê .  15%->16%   ³Ê¹« ÀÛÀº°¡ ½ÍÁö¸¸ ¿øÀÛ¹Ý¿µ.
	}


	//return fDamageScale;
	return fDamageScale * 0.7f;
}

void CCharacter::Set_GroundSmash(_bool bSmash)
{
	m_bHitGroundSmashed = bSmash;
}

/*
void CCharacter::Guard_Update()
{

	if(Check_bCurAnimationisGroundMove() || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex)
	{
		if (m_iPlayerTeam == 1)
		{
			if (m_iLookDirection == 1 && m_pGameInstance->Key_Pressing(DIK_A))
			{
				m_bGuard = true;
			}

			else if (m_iLookDirection == -1 && m_pGameInstance->Key_Pressing(DIK_D))
			{
				m_bGuard = true;
			}
			else
				m_bGuard = false;
		}

		else
		{
			if (m_iLookDirection == 1 && m_pGameInstance->Key_Pressing(DIK_LEFT))
			{
				m_bGuard = true;
			}

			else if (m_iLookDirection == -1 && m_pGameInstance->Key_Pressing(DIK_RIGHT))
			{
				m_bGuard = true;
			}
			else
				m_bGuard = false;
		}
	}
	else
	{
		m_bGuard = false;
	}


}
*/

void CCharacter::Set_GrabLoofCount(_ushort iLoofCount)
{
	m_iGrabLoof = iLoofCount;
}

AttackColliderResult CCharacter::Guard_Check3(AttackType eAttackType)
{
	//°¡µå Áß¿¡´Â ¾î¶² °ø°Ý µé¾î¿Íµµ ¹«Á¶°Ç °¡µå ¼º°ø
	if (Check_bCurAnimationisGuard())
	{
		return RESULT_GUARD;
	}


	if (eAttackType == ATTACKTYPE_GRAB_GROUND || eAttackType == ATTACKTYPE_GRAB_AIR || eAttackType == ATTACKTYPE_COMMANDGRAB)
	{
		return CompareGrabType3(eAttackType);
	}

	// °ø°ÝÀÌ³ª Çàµ¿À» ÇÏÁö ¾Ê°í ÀÖÀ»¶§¸¸ °¡µå Ã¼Å©.  ¹º°¡ ÇÏ°íÀÖ¾úÀ¸¸é ¹«Á¶°Ç °¡µå½ÇÆÐ.
	if (Check_bCurAnimationisGroundMove() || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex)
	{




		if (m_iPlayerTeam == 1)
		{
			//º¸°í ÀÖ´Â ¹æÇâÀÇ ¹Ý´ë¸¦ ´©¸£°í ÀÖÀ» ½Ã,   »óÁßÇÏ´Ü Ã¼Å© (Àâ±â Æ÷ÇÔ)
			if (m_iLookDirection == 1 && m_pGameInstance->Key_Pressing(DIK_A))
			{
				return CompareGuardType3(eAttackType);

			}

			else if (m_iLookDirection == -1 && m_pGameInstance->Key_Pressing(DIK_D))
			{
				return CompareGuardType3(eAttackType);
			}

			//¾È´­·¶À¸¸é °¡µå½ÇÆÐ
			else
				return RESULT_HIT;
		}

		else
		{
			//º¸°í ÀÖ´Â ¹æÇâÀÇ ¹Ý´ë¸¦ ´©¸£°í ÀÖÀ» ½Ã,   »óÁßÇÏ´Ü Ã¼Å© (Àâ±â Æ÷ÇÔ)
			if (m_iLookDirection == 1 && m_pGameInstance->Key_Pressing(DIK_LEFT))
			{
				return CompareGuardType3(eAttackType);
			}

			else if (m_iLookDirection == -1 && m_pGameInstance->Key_Pressing(DIK_RIGHT))
			{
				return CompareGuardType3(eAttackType);
			}

			//¾È´­·¶À¸¸é °¡µå½ÇÆÐ
			else
				return RESULT_HIT;
		}
	}
	else
	{
		return RESULT_HIT;
	}
}

_bool CCharacter::CompareGuardType(AttackType eAttackType)
{
	//»ó´ÜÀº ¹«Á¶°Ç °¡µå °¡´É
	if (eAttackType == ATTACKTYPE_HIGH)
		return true;

	//Áß´ÜÀº ¾É¾ÆÀÖÀ¸¸é °¡µå ½ÇÆÐ
	else if (eAttackType == ATTACKTYPE_MIDDLE)
	{
		if (m_pModelCom->m_iCurrentAnimationIndex == m_iCrouchAnimationIndex)
			return false;

		else
			return true;
	}

	//ÇÏ´ÜÀº ¼­ÀÖÀ¸¸é °¡µå ½ÇÆÐ.  °È´ÂÁßµµ Æ÷ÇÔµÇ´Âµ¥  ¹Ù²ã¾ßÇÑ´Ù.      ¾É¾ÆÀÖ°Å³ª °øÁßÀÌ¸é ¼º°ø?
	else if (eAttackType == ATTACKTYPE_LOW)
	{
		//if (m_pModelCom->m_iCurrentAnimationIndex == m_iIdleAnimationIndex)
		//	return false;
		//
		//else
		//	return true;

		if (m_pModelCom->m_iCurrentAnimationIndex == m_iCrouchAnimationIndex || Get_fHeight() > 0)
			return true;

		else
			return false;



	}

	//Àâ±â´Â Àâ±â¹öÆ°À¸·Î »ó¼â °¡´É
	else if (eAttackType == ATTACKTYPE_GRAB_GROUND)
	{

		//ÇÇ°ÝµÈ »ó´ëÀÇ ³ôÀÌ°¡ 0ÀÌ ¾Æ´Ï°Å³ª Àâ±â ¹öÆ°À¸·Î »ó¼âÇßÀ¸¸é ½ÇÆÐ.
		//if (Get_fHeight != 0 || m_pGameInstance->Key_Pressing(DIK_O))
		//	return false;

		//»ó´ëÀÇ ³ôÀÌ°¡ 0ÀÌ ¾Æ´Ï¸é ½ÇÆÐ
		if (Get_fHeight() != 0)
			return false;

		//Àâ±â ¹öÆ°À¸·Î »ó¼âÇßÀ¸¸é ½ÇÆÐ.
		else if (m_iPlayerTeam == 1 && m_pGameInstance->Key_Pressing(DIK_O))
			return false;

		else if (m_iPlayerTeam == 2 && m_pGameInstance->Key_Pressing(DIK_NUMPAD9))
			return false;

		else
			return true;


		//´ëÄ¡»óÅÂ°¡ ¾Æ´Ñ ÇÇ°ÝÁß¿¡´Â ³ôÀÌ ±¸ºÐ ¾øÀÌ ¸Â´Â°Ô Á¤»óÀÓ

	}

	//°øÁß¿¡¼­´Â  °øÁß¿¡ ÀÖ´Â »ó´ëÇÑÅ×¸¸ ÅëÇÔ.
	else if (eAttackType == ATTACKTYPE_GRAB_AIR)
	{
		//ÇÇ°ÝµÈ »ó´ëÀÇ ³ôÀÌ°¡ 0ÀÌ°Å³ª Àâ±â ¹öÆ°À¸·Î »ó¼âÇßÀ¸¸é ½ÇÆÐ.
		//if (Get_fHeight == 0 || m_pGameInstance->Key_Pressing(DIK_O))


		//»ó´ë°¡ °øÁßÀÌ ¾Æ´Ï¸é ½ÇÆÐ
		if (Get_fHeight() == 0)
			return false;

		//»ó´ë°¡ Àâ±â ¹öÆ°À¸·Î »ó¼âÇßÀ¸¸é ½ÇÆÐ.
		else if (m_iPlayerTeam == 1 && m_pGameInstance->Key_Pressing(DIK_O))
			return false;

		else if (m_iPlayerTeam == 2 && m_pGameInstance->Key_Pressing(DIK_NUMPAD9))
			return false;

		else
			return true;
	}

	//±â¼ú¿¡ ÀÇÇÑ Àâ±â´Â  Á¡ÇÁ·Î¸¸ °¡´É.
	else if (eAttackType == ATTACKTYPE_COMMANDGRAB)
	{
		//³ôÀÌÁ¶°Ç ´ë½Å ¾Ö´Ï¸ÞÀÌ¼ÇÀ¸·Î ÇØ¾ßÇÏ³ª? 
		if (Get_fHeight() > 0)
			return false;
		else
			return true;
	}



}

AttackColliderResult CCharacter::CompareGuardType3(AttackType eAttackType)
{
	//»ó´ÜÀº ¹«Á¶°Ç °¡µå °¡´É
	if (eAttackType == ATTACKTYPE_HIGH)
		return RESULT_GUARD;

	//Áß´ÜÀº ¾É¾ÆÀÖÀ¸¸é °¡µå ½ÇÆÐ
	else if (eAttackType == ATTACKTYPE_MIDDLE)
	{
		if (m_pModelCom->m_iCurrentAnimationIndex == m_iCrouchAnimationIndex)
			return RESULT_HIT;

		else
			return RESULT_GUARD;
	}

	//ÇÏ´ÜÀº ¼­ÀÖÀ¸¸é °¡µå ½ÇÆÐ.  °È´ÂÁßµµ Æ÷ÇÔµÇ´Âµ¥  ¹Ù²ã¾ßÇÑ´Ù.      ¾É¾ÆÀÖ°Å³ª °øÁßÀÌ¸é ¼º°ø?
	else if (eAttackType == ATTACKTYPE_LOW)
	{
		//if (m_pModelCom->m_iCurrentAnimationIndex == m_iIdleAnimationIndex)
		//	return false;
		//
		//else
		//	return true;

		if (m_pModelCom->m_iCurrentAnimationIndex == m_iCrouchAnimationIndex || Get_fHeight() > 0)
			return RESULT_GUARD;

		else
			return RESULT_HIT;



	}





}

AttackColliderResult CCharacter::CompareGrabType3(AttackType eAttackType)
{
	//Àâ±â´Â Àâ±â¹öÆ°À¸·Î »ó¼â °¡´É
	if (eAttackType == ATTACKTYPE_GRAB_GROUND)
	{

		//ÇÇ°ÝµÈ »ó´ëÀÇ ³ôÀÌ°¡ 0ÀÌ ¾Æ´Ï°Å³ª Àâ±â ¹öÆ°À¸·Î »ó¼âÇßÀ¸¸é ½ÇÆÐ.
		//if (Get_fHeight != 0 || m_pGameInstance->Key_Pressing(DIK_O))
		//	return false;

		//»ó´ëÀÇ ³ôÀÌ°¡ 0ÀÌ ¾Æ´Ï¸é ½ÇÆÐ
		if (Get_fHeight() != 0)
			return RESULT_MISS;

		//Àâ±â ¹öÆ°À¸·Î »ó¼âÇßÀ¸¸é ½ÇÆÐ.
		else if (m_iPlayerTeam == 1 && m_pGameInstance->Key_Pressing(DIK_O) &&(m_pModelCom->m_iCurrentAnimationIndex == m_iGrabReadyAnimationIndex || Check_bCurAnimationisGroundMove()))
			return RESULT_DRAW;

		else if (m_iPlayerTeam == 2 && m_pGameInstance->Key_Pressing(DIK_NUMPAD9) && (m_pModelCom->m_iCurrentAnimationIndex == m_iGrabReadyAnimationIndex || Check_bCurAnimationisGroundMove()))
			return RESULT_DRAW;

		else
			return RESULT_HIT;


		//´ëÄ¡»óÅÂ°¡ ¾Æ´Ñ ÇÇ°ÝÁß¿¡´Â ³ôÀÌ ±¸ºÐ ¾øÀÌ ¸Â´Â°Ô Á¤»óÀÓ

	}

	//°øÁß¿¡¼­´Â  °øÁß¿¡ ÀÖ´Â »ó´ëÇÑÅ×¸¸ ÅëÇÔ.
	else if (eAttackType == ATTACKTYPE_GRAB_AIR)
	{
		//ÇÇ°ÝµÈ »ó´ëÀÇ ³ôÀÌ°¡ 0ÀÌ°Å³ª Àâ±â ¹öÆ°À¸·Î »ó¼âÇßÀ¸¸é ½ÇÆÐ.
		//if (Get_fHeight == 0 || m_pGameInstance->Key_Pressing(DIK_O))


		//»ó´ë°¡ °øÁßÀÌ ¾Æ´Ï¸é ½ÇÆÐ
		if (Get_fHeight() == 0)
			return RESULT_MISS;

		//»ó´ë°¡ Àâ±â ¹öÆ°À¸·Î »ó¼âÇßÀ¸¸é ½ÇÆÐ.
		else if (m_iPlayerTeam == 1 && m_pGameInstance->Key_Pressing(DIK_O) && (m_pModelCom->m_iCurrentAnimationIndex == m_iGrabReadyAnimationIndex || Check_bCurAnimationisAirMove()))
			return RESULT_DRAW;

		else if (m_iPlayerTeam == 2 && m_pGameInstance->Key_Pressing(DIK_NUMPAD9) && (m_pModelCom->m_iCurrentAnimationIndex == m_iGrabReadyAnimationIndex || Check_bCurAnimationisAirMove()))
			return RESULT_DRAW;

		else
			return RESULT_HIT;
	}

	//±â¼ú¿¡ ÀÇÇÑ Àâ±â´Â  Á¡ÇÁ·Î¸¸ °¡´É.
	else if (eAttackType == ATTACKTYPE_COMMANDGRAB)
	{
		//³ôÀÌÁ¶°Ç ´ë½Å ¾Ö´Ï¸ÞÀÌ¼ÇÀ¸·Î ÇØ¾ßÇÏ³ª? 
		if (Get_fHeight() > 0)
			return RESULT_MISS;
		else
		{
			//¶¥¿¡ ²ø¸®´ÂÁß¸¸ ¾Æ´Ï¸é µÊ
			if (m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_LightAnimationIndex && m_pModelCom->m_fCurrentAnimPosition > 55.f && m_bHitGroundSmashed)
				return RESULT_MISS;
		}
		return RESULT_HIT;
	}
}

void CCharacter::Teleport_ToEnemy(_float OffsetX, _float OffsetY)
{

	_vector vTargetPos = static_cast<CTransform*>(m_pEnemy->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

	vTargetPos += {OffsetX* m_iLookDirection, OffsetY, 0, 0};

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTargetPos);


}

void CCharacter::Set_Grab(_bool bAir)
{
	m_bGrab = true;
	m_bGrab_Air = bAir;
	m_bGrabDraw = false;
}

void CCharacter::Set_ChaseStop()
{
	m_bChase = false;

	m_fAccChaseTime = 0.f;


	m_fGravityTime = 0.185 - m_pModelCom->m_fCurrentAnimPosition * 0.002f;



}

_ushort CCharacter::Get_BreakFall_AirAnimationIndex()
{
	return m_iBreakFall_Air;
}

_ushort CCharacter::Get_JumpAirAnimationIndex()
{
	return m_iJumpAnimationIndex;
}

_bool CCharacter::Get_bStun()
{
	return m_bStun;
}



void CCharacter::Update_PreviousXPosition()
{
	m_fPreviousX = XMVectorGetX(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
}

_float CCharacter::Get_fCalculatePreviousXPosition()
{
	return  m_fPreviousX - XMVectorGetX(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
}

_float CCharacter::Get_fAbsCalculatePreviousXPosition()
{
	return  abs(m_fPreviousX - XMVectorGetX(m_pTransformCom->Get_State(CTransform::STATE_POSITION)));
}



void CCharacter::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{

	//Àâ±âÁß¿¡´Â °ãÃÄµµ µÊ
	if (m_bGrabbed || static_cast<CCharacter*>(other->GetMineGameObject())->Get_bGrabbed() || m_bPlaying == false)
		return;


	_bool debugA = true;
	if (m_iPlayerTeam == 1 && other->m_ColliderGroup == CCollider_Manager::COLLIDERGROUP::CG_2P_BODY)
	{
		//CTransform* pTransofrm = static_cast<CTransform*>(other->GetMineGameObject()->Get_Component(TEXT("Com_Transform")));
		//pTransofrm->Add_Move({ 0.1f,0.f,0.f });


		CCharacter* pCharacter = static_cast<CCharacter*>(other->GetMineGameObject());

		//1. ÀÏ´Ü stunÀÎ³ð ÀÖÀ¸¸é ±×³ðÀÌ ¹Ð·Á³²
		//if (pCharacter->Get_bStun() == true)
		//{
		//	pCharacter->G
		//}



		_float fMyabsPos = fabsf(Get_fPositionX());
		_float fCharacterabsPos = fabsf(pCharacter->Get_fPositionX());

		if (fMyabsPos > 11.f || fCharacterabsPos > 11.f)
		{
			if (fMyabsPos < fCharacterabsPos)
			{
				//¾ó¸¶³ª ¹Ð¸®´Â°¡? °ãÄ£¸¸Å­? 
				m_pTransformCom->Add_Move({ -m_iLookDirection * m_pColliderCom->Get_Overlap_X(other) ,0.f,0.f });
			}

		}

		else if (m_bStun == true || Check_bCurAnimationisAirHit())
		{
			m_pTransformCom->Add_Move({ -m_iLookDirection * m_pColliderCom->Get_Overlap_X(other),0.f,0.f });
		}

		//µÑ ´Ù stun»óÅÂ°¡ ¾Æ´Ï°í, ¶¥¿¡ÀÖÀ¸¸é
		else //if (pCharacter->Get_fHeight() == 0 && Get_fHeight() == 0)
		{



			//´©±¸ ÇÏ³ª º®¿¡ÀÖÀ¸¸é º® ¿ì¼±, Àý´ë°ªÀÌ ÀÛÀº »ç¶÷ÀÌ ¹Ð¸².



			//ÀÌµ¿·®ÀÌ ÀûÀºÂÊÀÌ ¹Ð·Á³² .   ¾î´ÀÂÊÀ¸·Î? °ãÄ£ºÐ·®¸¸Å­?  ¾ç¼ö¹æÇâ? À½¼ö¹æÇâ?  º¸°íÀÖ´Â°÷ÀÇ µÚ °ÚÁö ¹¹
			//¹Ð·Á³­ÂÊÀº ÀÌ ÀÌµ¿·®À» ±â¹ÝÀ¸·Î ´Ù½Ã ¹Ð¾î³»Áö ¾Ê°Ô ÀÌµ¿·®À» ¾÷µ¥ÀÌÆ®ÇØÁà¾ßÇÔ
			_float CompareMoveX = pCharacter->Get_fAbsCalculatePreviousXPosition() - Get_fAbsCalculatePreviousXPosition();

			if (CompareMoveX > 0)
			{
				//m_pTransformCom->Add_Move({ m_iLookDirection * -0.05f,0.f,0.f });
				m_pTransformCom->Add_Move({ -m_iLookDirection * m_pColliderCom->Get_Overlap_X(other),0.f,0.f });
				Update_PreviousXPosition();

				pCharacter->Update_PreviousXPosition();
			}
			else if (CompareMoveX == 0)
			{
				//CTransform* pTransofrm = static_cast<CTransform*>(pCharacter->Get_Component(TEXT("Com_Transform")));
				//pTransofrm->Add_Move({ pCharacter->Get_iDirection() * -0.1f,0.f,0.f });
				//pCharacter->Update_PreviousXPosition();



			}



		}

	}
	else if (m_iPlayerTeam == 2 && other->m_ColliderGroup == CCollider_Manager::COLLIDERGROUP::CG_1P_BODY)
	{
		//CTransform* pTransofrm = static_cast<CTransform*>(other->GetMineGameObject()->Get_Component(TEXT("Com_Transform")));
		//pTransofrm->Add_Move({ 0.1f,0.f,0.f });


		CCharacter* pCharacter = static_cast<CCharacter*>(other->GetMineGameObject());
		//°¡¼Óµµ°¡ ³ôÀºÂÊ´ë·Î ÀÌµ¿?  °øÁß vs ¶¥ÀÌ¸é ¶¥ÀÌ ¿ì¼±?

		//1.³ôÀÌºñ±³.   °øÁß vs ¶¥ÀÎ °æ¿ì ¶¥ÀÌ ¿ì¼±.    °øÁß¿¡ ÀÖ´Â Ä³¸¯ÀÌ ¹Ð·Á³²
		//2. µÑ ´Ù ¶¥ÀÎ°æ¿ì ¼Óµµ³ôÀºÂÊ?  ´Þ¸®±â°¡ ¼Óµµ·Î Ã³¸®ÇÏÁø ¾ÊÀ»ÅÙµ¥  ÁøÂ¥ ÀÌÀü xÁÂÇ¥ ÇÊ¿äÇØ?
		_float fMyabsPos = fabsf(Get_fPositionX());
		_float fCharacterabsPos = fabsf(pCharacter->Get_fPositionX());

		if (fMyabsPos > 11.f || fCharacterabsPos > 11.f)
		{
			if (fMyabsPos < fCharacterabsPos)
			{
				//¾ó¸¶³ª ¹Ð¸®´Â°¡? °ãÄ£¸¸Å­? 
				m_pTransformCom->Add_Move({ -m_iLookDirection * m_pColliderCom->Get_Overlap_X(other) ,0.f,0.f });
			}

		}

		else if (m_bStun == true || Check_bCurAnimationisAirHit())
		{
			m_pTransformCom->Add_Move({ -m_iLookDirection * m_pColliderCom->Get_Overlap_X(other),0.f,0.f });
		}



		//µÑ ´Ù stun»óÅÂ°¡ ¾Æ´Ï°í, ¶¥¿¡ÀÖÀ¸¸é
		else //if (pCharacter->Get_fHeight() == 0 && Get_fHeight() == 0)
		{


			//´©±¸ ÇÏ³ª º®¿¡ÀÖÀ¸¸é º® ¿ì¼±, Àý´ë°ªÀÌ ÀÛÀº »ç¶÷ÀÌ ¹Ð¸².

			_float fMyabsPos = fabsf(Get_fPositionX());
			_float fCharacterabsPos = fabsf(pCharacter->Get_fPositionX());

			if (fMyabsPos > 11.f || fCharacterabsPos > 11.f)
			{
				if (fMyabsPos < fCharacterabsPos)
				{
					//¾ó¸¶³ª ¹Ð¸®´Â°¡? °ãÄ£¸¸Å­? 
					m_pTransformCom->Add_Move({ -m_iLookDirection * m_pColliderCom->Get_Overlap_X(other)  ,0.f,0.f });
				}

			}

			//ÀÌµ¿·®ÀÌ ÀûÀºÂÊÀÌ ¹Ð·Á³² .   ¾î´ÀÂÊÀ¸·Î? °ãÄ£ºÐ·®¸¸Å­?  ¾ç¼ö¹æÇâ? À½¼ö¹æÇâ?  º¸°íÀÖ´Â°÷ÀÇ µÚ °ÚÁö ¹¹
			//¹Ð·Á³­ÂÊÀº ÀÌ ÀÌµ¿·®À» ±â¹ÝÀ¸·Î ´Ù½Ã ¹Ð¾î³»Áö ¾Ê°Ô ÀÌµ¿·®À» ¾÷µ¥ÀÌÆ®ÇØÁà¾ßÇÔ


			_float CompareMoveX = pCharacter->Get_fAbsCalculatePreviousXPosition() - Get_fAbsCalculatePreviousXPosition();

			if (CompareMoveX > 0)
			{
				//m_pTransformCom->Add_Move({ m_iLookDirection * -0.05f,0.f,0.f });
				m_pTransformCom->Add_Move({ -m_iLookDirection * m_pColliderCom->Get_Overlap_X(other),0.f,0.f });
				Update_PreviousXPosition();

				pCharacter->Update_PreviousXPosition();
			}
			else if (CompareMoveX == 0)
			{
				//CTransform* pTransofrm = static_cast<CTransform*>(pCharacter->Get_Component(TEXT("Com_Transform")));
				//pTransofrm->Add_Move({ pCharacter->Get_iDirection() * -0.1f,0.f,0.f });
				//pCharacter->Update_PreviousXPosition();



			}



		}
	}
}


void CCharacter::OnCollisionStay(CCollider* other, _float fTimeDelta)
{

	//Àâ±âÁß¿¡´Â °ãÃÄµµ µÊ
	if (m_bGrabbed || static_cast<CCharacter*>(other->GetMineGameObject())->Get_bGrabbed() || m_bPlaying == false)
		return;


	_bool debugA = true;
	if (m_iPlayerTeam == 1 && other->m_ColliderGroup == CCollider_Manager::COLLIDERGROUP::CG_2P_BODY)
	{
		//CTransform* pTransofrm = static_cast<CTransform*>(other->GetMineGameObject()->Get_Component(TEXT("Com_Transform")));
		//pTransofrm->Add_Move({ 0.1f,0.f,0.f });


		CCharacter* pCharacter = static_cast<CCharacter*>(other->GetMineGameObject());

		//1. ÀÏ´Ü stunÀÎ³ð ÀÖÀ¸¸é ±×³ðÀÌ ¹Ð·Á³²
		//if (pCharacter->Get_bStun() == true)
		//{
		//	pCharacter->G
		//}



		_float fMyabsPos = fabsf(Get_fPositionX());
		_float fCharacterabsPos = fabsf(pCharacter->Get_fPositionX());

		if (fMyabsPos > 11.f || fCharacterabsPos > 11.f)
		{
			if (fMyabsPos < fCharacterabsPos)
			{
				//¾ó¸¶³ª ¹Ð¸®´Â°¡? °ãÄ£¸¸Å­? 
				m_pTransformCom->Add_Move({ -m_iLookDirection * m_pColliderCom->Get_Overlap_X(other) ,0.f,0.f });
			}

		}

		else if (m_bStun == true || Check_bCurAnimationisAirHit())
		{
			m_pTransformCom->Add_Move({ -m_iLookDirection * m_pColliderCom->Get_Overlap_X(other),0.f,0.f });
		}

		//µÑ ´Ù stun»óÅÂ°¡ ¾Æ´Ï°í, ¶¥¿¡ÀÖÀ¸¸é
		else //if (pCharacter->Get_fHeight() == 0 && Get_fHeight() == 0)
		{



			//´©±¸ ÇÏ³ª º®¿¡ÀÖÀ¸¸é º® ¿ì¼±, Àý´ë°ªÀÌ ÀÛÀº »ç¶÷ÀÌ ¹Ð¸².



			//ÀÌµ¿·®ÀÌ ÀûÀºÂÊÀÌ ¹Ð·Á³² .   ¾î´ÀÂÊÀ¸·Î? °ãÄ£ºÐ·®¸¸Å­?  ¾ç¼ö¹æÇâ? À½¼ö¹æÇâ?  º¸°íÀÖ´Â°÷ÀÇ µÚ °ÚÁö ¹¹
			//¹Ð·Á³­ÂÊÀº ÀÌ ÀÌµ¿·®À» ±â¹ÝÀ¸·Î ´Ù½Ã ¹Ð¾î³»Áö ¾Ê°Ô ÀÌµ¿·®À» ¾÷µ¥ÀÌÆ®ÇØÁà¾ßÇÔ
			_float CompareMoveX = pCharacter->Get_fAbsCalculatePreviousXPosition() - Get_fAbsCalculatePreviousXPosition();

			if (CompareMoveX > 0)
			{
				//m_pTransformCom->Add_Move({ m_iLookDirection * -0.05f,0.f,0.f });
				m_pTransformCom->Add_Move({ -m_iLookDirection * m_pColliderCom->Get_Overlap_X(other),0.f,0.f });
				Update_PreviousXPosition();

				pCharacter->Update_PreviousXPosition();
			}
			else if (CompareMoveX == 0)
			{
				//CTransform* pTransofrm = static_cast<CTransform*>(pCharacter->Get_Component(TEXT("Com_Transform")));
				//pTransofrm->Add_Move({ pCharacter->Get_iDirection() * -0.1f,0.f,0.f });
				//pCharacter->Update_PreviousXPosition();



			}



		}

	}
	else if (m_iPlayerTeam == 2 && other->m_ColliderGroup == CCollider_Manager::COLLIDERGROUP::CG_1P_BODY)
	{
		//CTransform* pTransofrm = static_cast<CTransform*>(other->GetMineGameObject()->Get_Component(TEXT("Com_Transform")));
		//pTransofrm->Add_Move({ 0.1f,0.f,0.f });


		CCharacter* pCharacter = static_cast<CCharacter*>(other->GetMineGameObject());
		//°¡¼Óµµ°¡ ³ôÀºÂÊ´ë·Î ÀÌµ¿?  °øÁß vs ¶¥ÀÌ¸é ¶¥ÀÌ ¿ì¼±?

		//1.³ôÀÌºñ±³.   °øÁß vs ¶¥ÀÎ °æ¿ì ¶¥ÀÌ ¿ì¼±.    °øÁß¿¡ ÀÖ´Â Ä³¸¯ÀÌ ¹Ð·Á³²
		//2. µÑ ´Ù ¶¥ÀÎ°æ¿ì ¼Óµµ³ôÀºÂÊ?  ´Þ¸®±â°¡ ¼Óµµ·Î Ã³¸®ÇÏÁø ¾ÊÀ»ÅÙµ¥  ÁøÂ¥ ÀÌÀü xÁÂÇ¥ ÇÊ¿äÇØ?
		_float fMyabsPos = fabsf(Get_fPositionX());
		_float fCharacterabsPos = fabsf(pCharacter->Get_fPositionX());

		if (fMyabsPos > 11.f || fCharacterabsPos > 11.f)
		{
			if (fMyabsPos < fCharacterabsPos)
			{
				//¾ó¸¶³ª ¹Ð¸®´Â°¡? °ãÄ£¸¸Å­? 
				m_pTransformCom->Add_Move({ -m_iLookDirection * m_pColliderCom->Get_Overlap_X(other) ,0.f,0.f });
			}

		}

		else if (m_bStun == true || Check_bCurAnimationisAirHit())
		{
			m_pTransformCom->Add_Move({ -m_iLookDirection * m_pColliderCom->Get_Overlap_X(other),0.f,0.f });
		}



		//µÑ ´Ù stun»óÅÂ°¡ ¾Æ´Ï°í, ¶¥¿¡ÀÖÀ¸¸é
		else //if (pCharacter->Get_fHeight() == 0 && Get_fHeight() == 0)
		{


			//´©±¸ ÇÏ³ª º®¿¡ÀÖÀ¸¸é º® ¿ì¼±, Àý´ë°ªÀÌ ÀÛÀº »ç¶÷ÀÌ ¹Ð¸².

			_float fMyabsPos = fabsf(Get_fPositionX());
			_float fCharacterabsPos = fabsf(pCharacter->Get_fPositionX());

			if (fMyabsPos > 11.f || fCharacterabsPos > 11.f)
			{
				if (fMyabsPos < fCharacterabsPos)
				{
					//¾ó¸¶³ª ¹Ð¸®´Â°¡? °ãÄ£¸¸Å­? 
					m_pTransformCom->Add_Move({ -m_iLookDirection * m_pColliderCom->Get_Overlap_X(other)  ,0.f,0.f });
				}

			}

			//ÀÌµ¿·®ÀÌ ÀûÀºÂÊÀÌ ¹Ð·Á³² .   ¾î´ÀÂÊÀ¸·Î? °ãÄ£ºÐ·®¸¸Å­?  ¾ç¼ö¹æÇâ? À½¼ö¹æÇâ?  º¸°íÀÖ´Â°÷ÀÇ µÚ °ÚÁö ¹¹
			//¹Ð·Á³­ÂÊÀº ÀÌ ÀÌµ¿·®À» ±â¹ÝÀ¸·Î ´Ù½Ã ¹Ð¾î³»Áö ¾Ê°Ô ÀÌµ¿·®À» ¾÷µ¥ÀÌÆ®ÇØÁà¾ßÇÔ


			_float CompareMoveX = pCharacter->Get_fAbsCalculatePreviousXPosition() - Get_fAbsCalculatePreviousXPosition();

			if (CompareMoveX > 0)
			{
				//m_pTransformCom->Add_Move({ m_iLookDirection * -0.05f,0.f,0.f });
				m_pTransformCom->Add_Move({ -m_iLookDirection * m_pColliderCom->Get_Overlap_X(other),0.f,0.f });
				Update_PreviousXPosition();

				pCharacter->Update_PreviousXPosition();
			}
			else if (CompareMoveX == 0)
			{
				//CTransform* pTransofrm = static_cast<CTransform*>(pCharacter->Get_Component(TEXT("Com_Transform")));
				//pTransofrm->Add_Move({ pCharacter->Get_iDirection() * -0.1f,0.f,0.f });
				//pCharacter->Update_PreviousXPosition();



			}



		}
	}

}

void CCharacter::OnCollisionExit(CCollider* other)
{
	_bool debugA = true;

}

_float CCharacter::Get_fPositionX()
{
	return 	XMVectorGetX(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
}


void CCharacter::Set_bGrabbed(_bool bGrabbed)
{
	m_bGrabbed = bGrabbed;
}

_bool CCharacter::Get_bGrabbed()
{
	return m_bGrabbed;
}






void CCharacter::Set_bRedHP(_bool bRedHP)
{
	if (bRedHP == true)
	{
		//m_pUI_Manager->Set_Hp(m_iHP);
		m_tCharacterDesc.iHp = m_iHP;

	}

	m_bRedHp = bRedHP;
	m_tCharacterDesc.bStun = m_bRedHp;
}

void CCharacter::Set_GrabAnimation()
{
	Set_Animation(m_iGrabAnimationIndex);
}

void CCharacter::Add_Move(_float2 fMovement)
{
	m_pTransformCom->Add_Move({ fMovement.x, fMovement.y,0 });
}

void CCharacter::Sparking_ON(_float fTimeDelta)
{

	//½ºÆÄÅ·ÀÌ ²¨Á®ÀÖ´Â°æ¿ì ÄÑ´ÂÁö È®ÀÎ
	if (m_bSparking == false)
	{
		if (m_iPlayerTeam == 1)
		{
			if (m_pGameInstance->Key_Down(DIK_R) && m_pGameInstance->Key_Pressing(DIK_O) && CBattleInterface_Manager::Get_Instance()->Get_bSparkingEnable(m_iPlayerTeam))
			{
				if (Check_bCurAnimationisGroundMove() || Check_bCurAnimationisAirMove() || m_pModelCom->m_iCurrentAnimationIndex == m_iGrabReadyAnimationIndex)
				{
					Set_Animation(m_iSparkingAnimationIndex);
					Set_NextAnimation(m_iIdleAnimationIndex, 3.f);
					CBattleInterface_Manager::Get_Instance()->Set_bSparkingEnable(false, m_iPlayerTeam);
					m_bSparking = true;

					//ÀÎ¿ø¼ö Á¶°Ç¹®
					m_fMaxSparkingTime = 10.f;

				}


				//Set_NextAnimation(m_iSparkingAnimationIndex, 100.f);
				//CBattleInterface_Manager::Get_Instance()->Set_bSparkingEnable(false, m_iPlayerTeam);
			}

		}
		else
		{
			if (m_pGameInstance->Key_Down(DIK_PGDN) && m_pGameInstance->Key_Pressing(DIK_NUMPAD9) && CBattleInterface_Manager::Get_Instance()->Get_bSparkingEnable(m_iPlayerTeam))
			{
				if (Check_bCurAnimationisGroundMove() || Check_bCurAnimationisAirMove() || m_pModelCom->m_iCurrentAnimationIndex == m_iGrabReadyAnimationIndex)

				{
					Set_Animation(m_iSparkingAnimationIndex);
					Set_NextAnimation(m_iIdleAnimationIndex, 3.f);

					CBattleInterface_Manager::Get_Instance()->Set_bSparkingEnable(false, m_iPlayerTeam);
					m_bSparking = true;

					//ÀÎ¿ø¼ö Á¶°Ç¹®
					m_fMaxSparkingTime = 10.f;

				}
			}
		}
	}



	////ÀÌ¹Ì ÄÑÁ®ÀÖ´Â°æ¿ì Å¸ÀÌ¸Ó Ã¼Å©
	//else
	//{
	//	m_fAccSparkingTime += fTimeDelta;
	//	if (m_fAccSparkingTime > m_fMaxSparkingTime)
	//	{
	//		m_bSparking = false;
	//
	//	}
	//
	//}


}

void CCharacter::Sparking_TimeCount(_float fTimeDelta)
{
	//ÀÌ¹Ì ÄÑÁ®ÀÖ´Â°æ¿ì Å¸ÀÌ¸Ó Ã¼Å©
	if (m_bSparking)
	{
		m_fAccSparkingTime += fTimeDelta;
		if (m_fAccSparkingTime > m_fMaxSparkingTime)
		{
			m_bSparking = false;

		}

	}
}

void CCharacter::Gain_KiAmount(_ushort iKiAmount)
{
	CBattleInterface_Manager::Get_Instance()->Gain_KiGuage(iKiAmount, m_iPlayerTeam);
}

_bool CCharacter::Get_bCharacterDead()
{
	return m_bCharacterDead;
}

void CCharacter::Tag_KeyCheck()
{
	if (m_iPlayerTeam == 1)
	{
		if (m_pGameInstance->Key_Down(DIK_F3) && m_pModelCom->m_iCurrentAnimationIndex == m_iIdleAnimationIndex)
<<<<<<< HEAD
=======
		{
			m_pUI_Manager->UsingChangeCharacher(static_cast<CUI_Define::PLAYER_SLOT>(0));
>>>>>>> ì›ê²©/ìµœì§„ì˜
			Tag_In(0);
		}
	}
	else if (m_iPlayerTeam == 2)
	{
		if (m_pGameInstance->Key_Down(DIK_F4) && m_pModelCom->m_iCurrentAnimationIndex == m_iIdleAnimationIndex)
		{
			m_pUI_Manager->UsingChangeCharacher(static_cast<CUI_Define::PLAYER_SLOT>(3));
			Tag_In(0);
		}
	}
}

void CCharacter::Tag_In(_ubyte iTagSlot)
{
	m_bPlaying = false;

	//¼ø°£ÀÌµ¿ ÀÌÆåÆ®
	m_bTag_In = true;



	//»ó´ë Ä³¸¯ÅÍµéÀÇ Enemy¸¦ º¯°æÇÔ


	//ÀÌ°Ç PLAYER_SLOT
	//0->1   1->0,   2->3, 3->2  

	//interface Àü¿ë slotÀ¸·Î ¹Ù²Ù·Á¸é
	//0->1 1->0,   2->1   3->0

	//Â¦¼ö¸é +1  È¦¼ö¸é -1
	if (m_ePlayerSlot % 2 == 0)
	{
		CBattleInterface_Manager::Get_Instance()->Tag_CharacterAIO(m_iPlayerTeam, 1, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	}
	else
		CBattleInterface_Manager::Get_Instance()->Tag_CharacterAIO(m_iPlayerTeam, 0, m_pTransformCom->Get_State(CTransform::STATE_POSITION));



	m_pTransformCom->Set_State_Position({ -100.f,-100.f,0.f });
	m_pColliderCom->UpdateVector(m_pTransformCom->Get_State(CTransform::STATE_POSITION));


}

void CCharacter::RegisterEnemy(CCharacter* pEnemy)
{
	m_pEnemy = pEnemy;
}

void CCharacter::pEnemyCheck()
{
	if (m_pEnemy == nullptr)
	{
		m_pEnemy = CBattleInterface_Manager::Get_Instance()->EnemyInitalize(m_iPlayerTeam);
	}

}


void CCharacter::Tag_Out(_vector vPosition)
{
	//ÀÏ´ÜÀº ÀÓ½Ã·Î
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	m_bPlaying = true;

	m_bTag_In = false;


	Chase_Ready(0.f);

}

void CCharacter::Set_bGrabDraw(_bool bGrabDraw)
{
	m_bGrabDraw = bGrabDraw;
}



void CCharacter::Reset_AttackStep()
{
	CBattleInterface_Manager::Get_Instance()->Reset_HitCount(m_iPlayerTeam);
	CBattleInterface_Manager::Get_Instance()->Reset_HitAttackStep(m_iPlayerTeam);

	Set_bRedHP(false);
}




_uint* CCharacter::Get_pAnimationIndex()
{
	return &(m_pModelCom->m_iCurrentAnimationIndex);
}

_short CCharacter::Get_iAnimationIndex()
{
	return m_pModelCom->m_iCurrentAnimationIndex;
}



_bool CCharacter::Check_bCurAnimationisAirHit(_uint iAnimation)
{
	_uint iModelIndex = iAnimation;

	if (iAnimation == 1000)
		iModelIndex = m_pModelCom->m_iCurrentAnimationIndex;



	//if (iModelIndex == m_iHit_Air_LightAnimationIndex || iModelIndex == m_iHit_Away_LeftAnimationIndex || iModelIndex == m_iHit_Away_UpAnimationIndex || iModelIndex == m_iHit_Air_FallAnimationIndex)
	if (iModelIndex == m_iHit_Air_LightAnimationIndex || iModelIndex == m_iHit_Air_FallAnimationIndex)
	{
		return true;
	}

	return false;
}

_bool CCharacter::Check_bCurAnimationisHitAway(_uint iAnimation)
{
	_uint iModelIndex = iAnimation;

	if (iAnimation == 1000)
		iModelIndex = m_pModelCom->m_iCurrentAnimationIndex;



	if (iModelIndex == m_iHit_Away_LeftAnimationIndex || iModelIndex == m_iHit_Away_UpAnimationIndex || iModelIndex == m_iHit_Away_LeftDownAnimationIndex)
	{
		return true;
	}

	return false;
}

_bool CCharacter::Check_bCurAnimationisHitGround(_uint iAnimation)
{
	_uint iModelIndex = iAnimation;

	if (iAnimation == 1000)
		iModelIndex = m_pModelCom->m_iCurrentAnimationIndex;



	if (iModelIndex == m_iHit_Stand_LightAnimationIndex || iModelIndex == m_iHit_Stand_MediumAnimationIndex || iModelIndex == m_iHit_Crouch_AnimationIndex)
	{
		return true;
	}

	return false;
}

_bool CCharacter::Check_bCurAnimationisGuard(_uint iAnimation)
{
	//_uint iModelIndex = iAnimation;
	//if (iAnimation == 1000)

	_uint iModelIndex = iModelIndex = m_pModelCom->m_iCurrentAnimationIndex;


	if (iModelIndex == m_iGuard_AirAnimationIndex || iModelIndex == m_iGuard_CrouchAnimationIndex || iModelIndex == m_iGuard_GroundAnimationIndex)
	{
		return true;
	}

	return false;
}

_bool CCharacter::Check_bCurAnimationisGrab(_uint iAnimation)
{
	//_uint iModelIndex = iAnimation;
	//if (iAnimation == 1000)

	_uint iModelIndex = iModelIndex = m_pModelCom->m_iCurrentAnimationIndex;


	if (iModelIndex == m_iGrabAnimationIndex || iModelIndex == m_iGrabReadyAnimationIndex)
	{
		return true;
	}

	return false;
}


void CCharacter::Set_NextAnimation(_uint iAnimationIndex, _float fLifeTime, _float fAnimationPosition)
{
	m_iNextAnimation.first = iAnimationIndex;
	m_iNextAnimation.second = fLifeTime;


	m_fNextAnimationCurrentPosition = fAnimationPosition;

}

void CCharacter::AttackNextMoveCheck()
{
	if (m_iNextAnimation.first != m_iIdleAnimationIndex)
	{


		if (m_bNextAnimationGravityEvent) 	//Set_Animationº¸´Ù À§¿¡ ÀÖ¾î¾ßÇÔ
		{
			m_bNextAnimationGravityEvent = false;
			Set_ForcedGravityTime_LittleUp();
		}

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

void CCharacter::AnimeEndNextMoveCheck()
{

	Set_Animation(m_iNextAnimation.first);

	m_iNextAnimation.first = m_iIdleAnimationIndex;
	m_iNextAnimation.second = 1000.f;



}

void CCharacter::Set_Animation(_uint iAnimationIndex, _bool bloof)
{



	if (iAnimationIndex == m_iIdleAnimationIndex)
		m_pModelCom->SetUp_Animation(iAnimationIndex, true);
	else
		m_pModelCom->SetUp_Animation(iAnimationIndex, bloof);

	if (iAnimationIndex == m_iHit_Air_LightAnimationIndex || iAnimationIndex == m_iHit_Stand_LightAnimationIndex || iAnimationIndex == m_iHit_Stand_MediumAnimationIndex)
	{
		m_pModelCom->CurrentAnimationPositionJump(0.f);
	}

}

void CCharacter::Gravity(_float fTimeDelta)
{



	if (m_bChase == true)
	{
		return;
	}

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float fHeight = XMVectorGetY(vPos);




	if (fHeight > 0)
	{

		if (m_iPlayerTeam == 2)
		{
			_float fGravity = (-0.7f * (2 * m_fGravityTime - m_fJumpPower) * (2 * m_fGravityTime - m_fJumpPower) + 4) * 0.1;

			_bool bDebug = true;
		}


		// IDLEÀÌ¸é °øÁß ÇÏ°­¸ð¼ÇÀ¸·Î º¯°æ


		if (m_bStun)
		{

		}


		else if (Check_bCurAnimationisGroundMove())
			m_pModelCom->SetUp_Animation(m_iFallAnimationIndex, false);


		// if (Check_bCurAnimationisGroundMove())
		//m_pModelCom->SetUp_Animation(m_iFallAnimationIndex, false);

		//Áß·Â ver1 ÀÏ¶§´Â Ç×»ó ´õÇØ¾ß ÀÚ¿¬½º·¯¿îµ¥ 2¿¡¼­´Â ¾Æ´Ô
		//if (m_fGravityTime < m_fJumpPower)
		//{
		//	m_fGravityTime += fTimeDelta;
		//}


		//ver1  ±¦ÂúÀºµ¥ ³Ê¹« µÕ½ÇµÕ½ÇÇÔ  Á¡ÇÁ·Â4 º£ÀÌ½º.
		//_float fGravity = ( - 0.3f * (m_fGravityTime - m_fJumpPower) * (m_fGravityTime - m_fJumpPower) + 2) *0.03f;

		//ver  Á¡ÇÁ¿¡ ¼Óµµ°¨ÀÌ Á» ´À²¸Áö°í ÀüÃ¼ÀûÀ¸·Î ³·À½, Á¡ÇÁ·Â 3 º£ÀÌ½º.
		//_float fGravity = (-0.7f * (m_fGravityTime - m_fJumpPower) * (m_fGravityTime - m_fJumpPower) + 4) * 0.03f;

		//Ver3  Á¡ÇÁ·Â 3
		//_float fGravity = (-0.7f * (m_fGravityTime - m_fJumpPower) * (m_fGravityTime - m_fJumpPower) + 4) * 0.05f;


		//1~3Â¥¸® ÀüºÎ ½Ç¼ö·Î Gravity µÎ¹ø¾¿ È£ÃâÇÔ.  

		//Ver4 Á¡ÇÁ·Â3.   ¸¶Áö¸· °ªÀ» *0.1 ´ë½Å *0.08ÇØµµ ÀÚ¿¬½º·¯¿ò
		//_float fGravity = (-0.7f * (m_fGravityTime - m_fJumpPower) * (m_fGravityTime - m_fJumpPower) + 4) * 0.1;
		_float fGravity = (-0.7f * (2 * m_fGravityTime - m_fJumpPower) * (2 * m_fGravityTime - m_fJumpPower) + 4) * 0.1;



		//Á¡ÇÁ½ÃÀÛ, Á¡ÇÁÁß¿¡ Áß·Â µû¸§ 
		//if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex)

		//°øÁß ¾à°ø°ÝÀ¸·Î ¸Â´ÂÁß Ãß°¡
		//if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_LightAnimationIndex)

		//°øÁß ±âº»°ø°ÝÁß Ãß°¡( ±âÅºÁ¦¿Ü)
		//if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex ||
		//	m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_Air1 || m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_Air2 || m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_Air3 ||
		//	m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_LightAnimationIndex)
		if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex ||
			m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_Air1 || m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_Air2 || m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_Air3 ||
			m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_AirUpper || m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Ground ||
			m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_Spin_LeftUp || m_pModelCom->m_iCurrentAnimationIndex == m_iHit_WallBouce ||
			Check_bCurAnimationisAirHit() || Check_bCurAnimationisHitAway() || m_pModelCom->m_iCurrentAnimationIndex == m_iGuard_AirAnimationIndex
			|| Check_bCurAnimationisHalfGravityStop())
		{




			//½º¸Å½Ã ´çÇßÀ¸¸é ½Ã°£ ´õÇÏÁö ¾ÊÀ½.   °øÁß ¾Æ·¡°­ Áß¿¡µµ ´õÇÏÁö ¾ÊÀ½
			if (Check_bCurAnimationisHitAway() || m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_AirUpper || (m_bAttackGravity == false && Check_bCurAnimationisHalfGravityStop()))
			{
				;
			}

			else if (m_fGravityTime * 2.f < m_fJumpPower)
			{
				m_fGravityTime += fTimeDelta;

				//ÀÏ½ÃÁ¤ÁöÇØ¼­ ¿©±â¼­ ³Ñ¾î°¡¸é °ªÀÌ È® ³ô¾ÆÁú ¼ö ÀÖÀ½.  ¿¹¿ÜÃ³¸®ÇØ¼­ ½Ã°£°ª ÇÑ¹ø ´õ?
			}
			else if (m_fGravityTime * 2.f > m_fJumpPower)
			{
				m_fGravityTime = m_fJumpPower * 0.5f;
			}


			//ÀÏºÎ °ø°ÝÀÇ °æ¿ì  Gravity °¡ false¸é Áß·Â Á¤¿ë ¾ÈÇÔ
			if (m_bAttackGravity == false && Check_bCurAnimationisHalfGravityStop())
			{
				;
			}

			else if (m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Ground || m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_Spin_LeftUp || m_pModelCom->m_iCurrentAnimationIndex == m_iHit_WallBouce)
			{
				m_pTransformCom->Add_Move({ m_fImpuse.x * fTimeDelta,-fGravity + m_fImpuse.y * fTimeDelta,0 });
			}
			else if (Check_bCurAnimationisHitAway() == false && m_pModelCom->m_iCurrentAnimationIndex != m_iAttack_AirUpper)
				m_pTransformCom->Add_Move({ m_fImpuse.x * fTimeDelta,-fGravity,0 });


		}
		else
		{

			//Áß·ÂVer2 Àü¿ë Ã³¸®.  ¿Ã¶ó°¡´Ù°¡ °ø°Ý¶§¹®¿¡ ¸ØÃè´Âµ¥  °ø°Ý ³¡³ª°í ´Ù½Ã ¿Ã¶ó°¡´Â°Å ÀÌ»óÇØ¼­ Ã³¸®
			//if (fGravity < 0 && m_fGravityTime < m_fJumpPower)


			if (m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Air)
			{
				m_pTransformCom->Add_Move({ m_fImpuse.x * fTimeDelta, m_fImpuse.y * fTimeDelta,0.f });
			}



			if (m_bAttackGravity == true)
			{

				if (fGravity < 0 && m_fGravityTime * 2 < m_fJumpPower)
				{
					m_fGravityTime += fTimeDelta;
				}

			}
			//°¡¼Ó¸¸ ¹Þ°í Áß·ÂÀº ³ÀµÎ´Â ÄÚµå. ¸ðµç ¸ð¼Ç¿¡ °¡¼Óµµ Àû¿ëÇÒ²¨ ¾Æ´Ï¸é ±²ÀåÈ÷ ÀÌ»óÇÏ°Ô º¸ÀÓ.
			//m_pTransformCom->Add_Move({ m_fImpuse * fTimeDelta,0,0 });


		}



	}




	//ÀÌºÎºÐÀº Gravity¶û ´Ù¸£°Ô ºÐ¸®ÇØµµµÉÁöµµ 

	//	else if (fHeight <0)   //¿Ö else if ¿´´ÂÁö ±â¾ïÀÌ ¾È³². ½Ç¼ö¿´À»Áöµµ
	//if (fHeight <0)
	if (fHeight < 0)
	{
		//m_pEffect_Manager->Copy_Layer(TEXT("Smoke01"), m_pTransformCom->Get_WorldMatrixPtr());
		//m_pEffect_Manager->Copy_Layer(TEXT("Smoke01_BackZ"), m_pTransformCom->Get_WorldMatrixPtr());
		//m_pEffect_Manager->Copy_Layer(TEXT("Smoke02"), m_pTransformCom->Get_WorldMatrixPtr());
		//m_pEffect_Manager->Copy_Layer(TEXT("Smoke02_Small"), m_pTransformCom->Get_WorldMatrixPtr());
		//m_pEffect_Manager->Copy_Layer(TEXT("Smoke04"), m_pTransformCom->Get_WorldMatrixPtr());

		//m_pEffect_Manager->Copy_Layer(TEXT("Smoke05"), m_pTransformCom->Get_WorldMatrixPtr());
		//m_pEffect_Manager->Copy_Layer(TEXT("Aura01"), m_pTransformCom->Get_WorldMatrixPtr());

		//m_pEffect_Manager->Copy_Layer(TEXT("BurstU-1"), m_pTransformCom->Get_WorldMatrixPtr());

		m_pEffect_Manager->Copy_Layer(TEXT("Smoke03_Stop"), m_pTransformCom->Get_WorldMatrixPtr());

		//if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || Check_bCurAnimationisAirAttack())
		if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || Check_bCurAnimationisAirAttack() || m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Air)
		{
			m_pModelCom->SetUp_Animation(m_iIdleAnimationIndex, true);

			Set_fGravityTime(0.f);
			//Set_fJumpPower(0.f);
			Set_fImpulse(0.f);

			Set_NextAnimation(m_iIdleAnimationIndex, 2.f);
			m_bAriDashEnable = true;
			m_bChaseEnable = true;
			Set_bAttackGravity(true);

			if (m_bJumpLock == false)
			{
				m_bJumpLock = true;
			}

			//»ç»ÓÈ÷ Á¡ÇÁÇØ¼­ ¶¥ ÂøÁö
			m_pGameInstance->Play_Sound(CSound_Manager::SOUND_KEY_NAME::Goku_Dash_SFX, false, 1.f);
		}
		else if (Check_bCurAnimationisAirHit() || Check_bCurAnimationisHitAway())
		{
			//m_pModelCom->SetUp_Animation(m_iIdleAnimationIndex, true);

			if (m_bHitGroundSmashed)
			{
				Set_Animation(m_iHit_Air_LightAnimationIndex);
				m_pModelCom->CurrentAnimationPositionJump(55.f);
				Set_NextAnimation(m_iBound_Ground, 5.f);

				//m_bHitGroundSmashed = false;
				//m_pTransformCom->Add_Move({ -1 * fTimeDelta * m_iLookDirection, 0, 0 });
			}
			else
			{
				//m_pModelCom->SetUp_Animation(m_iBound_Ground, false);

				//¹é¾÷
				//Set_Animation(m_iBreakFall_Ground, 2.f);
				//Set_fImpulse({ 5.f,0.f });
				Set_BreakFall_Ground();

				Set_NextAnimation(m_iIdleAnimationIndex, 2.f);
			}


			if (m_pModelCom->m_iCurrentAnimationIndex != m_iBreakFall_Ground)
			{
				Set_fGravityTime(0.f);
				Set_fImpulse(0.f);
			}


			m_bAriDashEnable = true;
			Set_bAttackGravity(true);

			if (m_bJumpLock == false)
			{
				m_bJumpLock = true;
			}

			//ÇÇ°ÝµÇ¼­ ¶¥¿¡ ²Ã¾Æ¹ÚÈú¶§ »ç¿îµå
			m_pGameInstance->Play_Sound(CSound_Manager::SOUND_KEY_NAME::Goku_Dash_SFX, false, 1.f);
		}
		if (m_bHitGroundSmashed == false)
		{
			m_bStun = false;
			m_fAccStunTime = 0.f;
		}
		//m_pTransformCom->Add_Move({ 0,-fHeight,0 });

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { XMVectorGetX(vPos),0.f,XMVectorGetZ(vPos),1.f });

	}

	else if (fHeight == 0)
	{
		if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex)
		{
			m_pModelCom->SetUp_Animation(m_iIdleAnimationIndex, true);

			Set_fGravityTime(0.f);
			//Set_fJumpPower(0.f);
			Set_fImpulse(0.f);
			m_bAriDashEnable = true;
			Set_bAttackGravity(true);
		}

		//else if (m_bHitGroundSmashed == true && m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_LightAnimationIndex)
		//{
		//	if (m_bMotionPlaying == false)
		//	{
		//		Set_Animation(m_iBreakFall_Ground, 2.f);
		//		Set_NextAnimation(m_iIdleAnimationIndex, 2.f);
		//		m_bHitGroundSmashed = false;
		//	}
		//
		//}
		//else if (m_bHitGroundSmashed == true && m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_LightAnimationIndex)
		else if (m_bHitGroundSmashed == true)
		{
			if (m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_LightAnimationIndex)
			{
				//¹Ù´Ú¿¡ ²¿¶ó¹ÚÇûÀ»¶§ ÁúÁú ²ø¸².  º¸±â¿¡ ÀÌ»óÇÏ´Ù¸é ¸Â¾ÒÀ» ½Ã¿¡ FlipDirection ÇØ¾ßÇÑ´Ù
				m_pTransformCom->Add_Move({ -2 * fTimeDelta * m_iLookDirection, 0, 0 });

				if (m_bMotionPlaying == false)
				{
					Set_Animation(m_iBound_Ground, 2.f);

					//Set_NextAnimation(m_iBreakFall_Ground, 2.f);
					//
					//if (inputBuffer.back() == CInput{ MOVEKEY_UP, ATTACK_LIGHT })
					//{
					//	m_fImpuse = { -3.f * m_iLookDirection, 2.f };
					//}
					//m_bHitGroundSmashed = false;

					m_bHitGroundSmashed = false;
					Set_BreakFall_Ground();

					Set_bAttackGravity(true);

					//Set_NextAnimation(m_iBreakFall_Ground, 2.f);
					//DirectionInput iMoveKey = inputBuffer.back().direction;
					//
					//if (iMoveKey == MOVEKEY_UP || iMoveKey == MOVEKEY_UP_LEFT)
					//{
					//	Set_fImpulse({ -3.f * m_iLookDirection,1.f });
					//	Set_ForcveGravityTime(0.f);
					//}
					//else if (iMoveKey == MOVEKEY_RIGHT)
					//{
					//	//Set_fImpulse({ 0.f , 0.3f });
					//	Set_fImpulse({ 0.f , 0.1f });
					//
					//	Set_ForcedGravityTime_LittleUp();
					//}
					//
					//
					//else //if (iMoveKey == MOVEKEY_LEFT)
					//{
					//	Set_fImpulse({ -5.f * m_iLookDirection, 0.f });
					//	Set_ForcedGravityTime_LittleUp();
					//}
				}

			}
		}
		else if (Check_bCurAnimationisHitAway() || Check_bCurAnimationisAirHit())
		{
			//³«¹ý ¶Ç´Â ½º¸Å½Ã 
			if (m_bHitGroundSmashed)
			{
				//Set_Animation(m_iHit_Air_LightAnimationIndex);
				//m_pModelCom->CurrentAnimationPositionJump(55.f);
				//Set_NextAnimation(m_iBound_Ground, 5.f);

				//m_bHitGroundSmashed = false;
			}
			else
			{
				//Set_Animation(m_iBound_Ground);

				Set_Animation(m_iBreakFall_Ground, 2.f);
				if (inputBuffer.back() == CInput{ MOVEKEY_UP, ATTACK_LIGHT })
				{
					m_fImpuse = { -3.f * m_iLookDirection, 2.f };
				}
				Set_NextAnimation(m_iIdleAnimationIndex, 2.f);
			}
		}
		//else if (m_bHitGroundSmashed == true && m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_LightAnimationIndex)
		//{
		//	if (m_bMotionPlaying == false)
		//	{
		//		Set_Animation(m_iBreakFall_Ground, 2.f);
		//		Set_NextAnimation(m_iIdleAnimationIndex, 2.f);
		//		m_bHitGroundSmashed = false;
		//	}
		//
		//}
		else if (m_pModelCom->m_iCurrentAnimationIndex == m_iBound_Ground)
		{
			if (m_bMotionPlaying == false)
			{
				Set_Animation(m_iBreakFall_Ground);

			}

		}
		if (m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Ground)
		{

			//³«¹ýÃ¼Å©
			//1È¸¸¸ ½ÇÇàµÉ°Í
			m_fGravityTime += fTimeDelta;
			if (m_fGravityTime * 2.f > m_fJumpPower)
			{
				m_fGravityTime = m_fJumpPower * 0.5f;
			}
			//_float fGravity = (-0.7f * (2 * m_fGravityTime - m_fJumpPower) * (2 * m_fGravityTime - m_fJumpPower) + 4) * 0.1;
			//m_pTransformCom->Add_Move({ m_fImpuse.x * fTimeDelta,-fGravity + m_fImpuse.y * fTimeDelta,0 });
			m_pTransformCom->Add_Move({ m_fImpuse.x * fTimeDelta,0.3f,0 });

		}

		if (m_bJumpLock)
		{
			m_fAccAnimationLock += fTimeDelta;

			if (m_fAccAnimationLock > 0.15f)
			{
				m_bJumpLock = false;
				m_fAccAnimationLock = 0.f;
			}
		}

		m_bDoubleJumpEnable = true;

	}




}



HRESULT CCharacter::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
	//	return E_FAIL;

	/* Com_Model */
	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_strModelName,
	//	TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
	//	return E_FAIL;




	//CCollider_Test::COLLIDER_DESC ColliderDesc{};
	//ColliderDesc.pTransform = m_pTransformCom;
	//ColliderDesc.fSizeX = 1.2f;
	//ColliderDesc.fSizeY = 1.5f;
	//ColliderDesc.fSizeZ = 0.7f;
	//ColliderDesc.Offset = { 0.f, 0.7f, 0.f };


	//CBounding_AABB::BOUNDING_AABB_DESC ColliderDesc{};
	//if (m_iPlayerTeam == 1)
	//	ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_1P_BODY;
	//else
	//	ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_2P_BODY;
	//ColliderDesc.pMineGameObject = this;
	//ColliderDesc.vCenter = { 0.f,0.8f,0.f };
	//ColliderDesc.vExtents = { 0.5f,0.7f,1.f };
	//
	//
	////Com_Collider
	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
	//	TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
	//	return E_FAIL;
	//
	//m_pGameInstance->Add_ColliderObject(ColliderDesc.colliderGroup, m_pColliderCom);



	return S_OK;
}

HRESULT CCharacter::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pOutLineCom->Bind_ShaderResource(m_pShaderCom, "g_OutLineTexture", 1)))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pRenderInstance->Get_LightDesc(CLight_Manager::LIGHT_PLAYER, 0, m_strName);
	if (nullptr == pLightDesc)
		return E_FAIL;

	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vDiffuse, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
	//	return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_iPlayerDirection", pLightDesc->pPlayerDirection, sizeof(_int))))
		return E_FAIL;

	return S_OK;
}

void CCharacter::GetUI_Input(_uint iInputDirX, _uint iInputDirY, DirectionInput eDirInput, ButtonInput eBtnInput)
{


	if (m_iLookDirection == -1)
	{
		switch (eDirInput)
		{
		case Client::MOVEKEY_LEFT:
			eDirInput = MOVEKEY_RIGHT;
			break;
		case Client::MOVEKEY_RIGHT:
			eDirInput = MOVEKEY_LEFT;
			break;
		case Client::MOVEKEY_UP_LEFT:
			eDirInput = MOVEKEY_UP_RIGHT;
			break;
		case Client::MOVEKEY_UP_RIGHT:
			eDirInput = MOVEKEY_UP_LEFT;
			break;
		case Client::MOVEKEY_DOWN_LEFT:
			eDirInput = MOVEKEY_DOWN_RIGHT;
			break;
		case Client::MOVEKEY_DOWN_RIGHT:
			eDirInput = MOVEKEY_DOWN_LEFT;
			break;
		default:
			break;
		}
	}

	m_pUI_Manager->m_eDirInput = eDirInput;
	m_pUI_Manager->m_eBtnInput = eBtnInput;
}

CCharacter* CCharacter::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCharacter* pInstance = new CCharacter(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCharacter"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCharacter::Clone(void* pArg)
{
	CCharacter* pInstance = new CCharacter(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCharacter"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCharacter::Free()
{
	__super::Free();

	Safe_Release(m_pOutLineCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

	Safe_Release(m_pUI_Manager);
	Safe_Release(m_pEffect_Manager);
	Safe_Release(m_pOutLineCom);
	Safe_Release(m_pColliderCom);

}
