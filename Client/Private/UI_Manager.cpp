#include "stdafx.h"

#include "Layer.h"
#include "UI_Manager.h"

#include "UIObject.h"

#include "UI_Define.h"
#include "GameInstance.h"
#include "RenderInstance.h"
#include "Character.h"

IMPLEMENT_SINGLETON(CUI_Manager)

CUI_Manager::CUI_Manager()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

void CUI_Manager::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_F1))
	{
		m_fTotalDuration = 0.f;
		UsingCreateStartUI();
	}

	if (m_pGameInstance->Key_Down(DIK_F2))
	{
		m_fTotalDuration = 0.f;
		UsingCreateEndUI();
	}
}

void CUI_Manager::UsingAttckBuff(_float fAttBufDuration, CUI_Define::PLAYER_SLOT eSlotID)
{
	m_fDuration = fAttBufDuration;

	CUIObject::UI_DESC tAttBufDesc = {};

	if (eSlotID == CUI_Define::LPLAYER1 || eSlotID == CUI_Define::LPLAYER2)
		tAttBufDesc.eLRPos = CUIObject::LEFT;
	else if (eSlotID == CUI_Define::RPLAYER1 || eSlotID == CUI_Define::RPLAYER2)
		tAttBufDesc.eLRPos = CUIObject::RIGHT;

	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_AttBufEffect"), TEXT("Layer_UI_AttBufEffect"), &tAttBufDesc);
	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_AttBufThunderEffect"), TEXT("Layer_UI_AttBufEffect"), &tAttBufDesc);
}

void CUI_Manager::UsingChangeCharacher(CUI_Define::PLAYER_SLOT eCurrSlotID)
{
	

	m_iTeam = CUIObject::POS_END;
	if (eCurrSlotID == CUI_Define::LPLAYER1 || eCurrSlotID == CUI_Define::LPLAYER2)
	{
		if (m_pPawnArray[CUI_Define::LPLAYER2] == nullptr)
			return;

		CCharacter* pSwapPanw = m_pPawnArray[CUI_Define::LPLAYER1];
		m_pPawnArray[CUI_Define::LPLAYER1] = m_pPawnArray[CUI_Define::LPLAYER2];
		m_pPawnArray[CUI_Define::LPLAYER2] = pSwapPanw;

		m_iTeam = CUIObject::LEFT;
	}
	else if (eCurrSlotID == CUI_Define::RPLAYER1 || eCurrSlotID == CUI_Define::RPLAYER2)
	{
		if (m_pPawnArray[CUI_Define::RPLAYER2] == nullptr)
			return;

		CCharacter* pSwapPanw = m_pPawnArray[CUI_Define::RPLAYER1];
		m_pPawnArray[CUI_Define::RPLAYER1] = m_pPawnArray[CUI_Define::RPLAYER2];
		m_pPawnArray[CUI_Define::RPLAYER2] = pSwapPanw;

		m_iTeam = CUIObject::RIGHT;
	}
}

void CUI_Manager::UsingCreateStartUI()
{
	CUIObject::UI_DESC StartDesc = {};
	StartDesc.fSpeedPerSec = 50.f;
	StartDesc.fRotationPerSec = XMConvertToRadians(90.f);

	for (int i = 0; i < 8; i++)
	{
		StartDesc.iNumUI = i;
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_GameStartCircle"), TEXT("Layer_UI_GameStartCircle"), &StartDesc);
	}

	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_ReadyFont"), TEXT("Layer_UI_GameStartFont"));
}

void CUI_Manager::UsingCreateEndUI()
{
	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_KOPanel"), TEXT("Layer_UI_KOFont"));
	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_KOFont"), TEXT("Layer_UI_KOFont"));
	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_KOFontEffect"), TEXT("Layer_UI_KOFont"));
	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_KOParticle"), TEXT("Layer_UI_KOFont"));

}

//void CUI_Manager::SKillCount()
//{
//	if (m_iSkillPoint >= 100)
//	{
//		m_iSkillPoint -= 100;
//		m_iSkillCount++;
//	}
//	else if (m_iSkillPoint < 0)
//	{
//		m_iSkillPoint += 100;
//		m_iSkillCount--;
//	}
//}

void CUI_Manager::UI_Setting_Debug(_float& fSizeX, _float& fSizeY, _float& fPosX, _float& fPosY, _float fSizeRadio)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_LEFT))
	{
		fSizeY--;
		fSizeX = fSizeRadio * fSizeY;
	}

	if (m_pGameInstance->Get_DIKeyState(DIK_RIGHT))
	{
		fSizeY++;
		fSizeX = fSizeRadio * fSizeY;
	}

	if (m_pGameInstance->Get_DIKeyState(DIK_A))
	{
		fPosX--;
	}

	if (m_pGameInstance->Get_DIKeyState(DIK_D))
	{
		fPosX++;
	}

	if (m_pGameInstance->Get_DIKeyState(DIK_W))
	{
		fPosY--;
	}

	if (m_pGameInstance->Get_DIKeyState(DIK_S))
	{
		fPosY++; 
	}
}

void CUI_Manager::InitUIObject()
{
	list<CGameObject*>CharacterList = m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Character"));

	for (auto& iter : CharacterList)
	{
		CUI_Define::PLAYER_SLOT eSlot = dynamic_cast<CCharacter*>(iter)->Get_PawnDesc().ePlayer_Slot;

		if (eSlot == CUI_Define::SLOT_END)
			continue;

		m_pPawnArray[eSlot] = dynamic_cast<CCharacter*>(iter);
	}
}

void CUI_Manager::Free()
{
	Safe_Release(m_pGameInstance);
	Destroy_Instance();

	__super::Free();
}
