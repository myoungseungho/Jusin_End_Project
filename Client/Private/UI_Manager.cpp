#include "stdafx.h"

#include "Layer.h"
#include "UI_Manager.h"

#include "GameInstance.h"
#include "RenderInstance.h"

IMPLEMENT_SINGLETON(CUI_Manager)

CUI_Manager::CUI_Manager()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

void CUI_Manager::UsingAttckBuff(_float fAttBufDuration)
{
	m_fDuration = fAttBufDuration;
	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_AttBufEffect"), TEXT("Layer_UI_AttBufEffect"));
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
