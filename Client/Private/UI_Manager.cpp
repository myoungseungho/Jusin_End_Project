#include "stdafx.h"

#include "Layer.h"
#include "UI_Manager.h"

#include "UIObject.h"

#include "GameInstance.h"
#include "RenderInstance.h"

IMPLEMENT_SINGLETON(CUI_Manager)

CUI_Manager::CUI_Manager()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

void CUI_Manager::UsingAttckBuff(_float fAttBufDuration, CPawn::PLAYER_SLOT eSlotID)
{
	m_fDuration = fAttBufDuration;

	CUIObject::UI_DESC tAttBufDesc = {};

	if (eSlotID == CPawn::LPLAYER1 || eSlotID == CPawn::LPLAYER2)
		tAttBufDesc.eLRPos = CUIObject::LEFT;
	else if(eSlotID == CPawn::RPLAYER1 || eSlotID == CPawn::RPLAYER2)
		tAttBufDesc.eLRPos = CUIObject::RIGHT;

	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_AttBufEffect"), TEXT("Layer_UI_AttBufEffect"),&tAttBufDesc);
	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_AttBufThunderEffect"), TEXT("Layer_UI_AttBufEffect"),&tAttBufDesc);
}

void CUI_Manager::UsingComboCount(_uint iComboCnt)
{
	m_iComboCount = iComboCnt;
}

void CUI_Manager::SKillCount()
{
	if (m_iSkillPoint >= 100)
	{
		m_iSkillPoint -= 100;
		m_iSkillCount++;
	}
	else if (m_iSkillPoint < 0)
	{
		m_iSkillPoint += 100;
		m_iSkillCount--;
	}
}

void CUI_Manager::UI_Setting_Debug(_float& fSizeX, _float& fSizeY, _float& fPosX, _float& fPosY,  _float fSizeRadio)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_O))
	{
		fSizeY--;
		fSizeX = fSizeRadio * fSizeY;
	}

	if (m_pGameInstance->Get_DIKeyState(DIK_P))
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

void CUI_Manager::Free()
{
	CUI_Manager::Get_Instance()->Destroy_Instance();
	Safe_Release(m_pGameInstance);

	__super::Free();
}
