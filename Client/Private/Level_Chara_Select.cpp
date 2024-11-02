#include "stdafx.h"
#include "..\Public\Level_Chara_Select.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Define.h"
#include "UIObject.h"


CLevel_Chara_Select::CLevel_Chara_Select(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{
}

HRESULT CLevel_Chara_Select::Initialize()
{
	m_iLevelIndex = LEVEL_CHARACTER;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Sound()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Chara_Select::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_SPACE))
	{
		if (FAILED(m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY))))
			return;
	}
}

HRESULT CLevel_Chara_Select::Render(_float fTimeDelta)
{
	SetWindowText(g_hWnd, TEXT("로고레벨"));

	return S_OK;
}

HRESULT CLevel_Chara_Select::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	/* For.BackGround */
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_CHARACTER, TEXT("Prototype_GameObject_CharaSelectBG"), strLayerTag)))
		return E_FAIL;

	CUIObject::UI_DESC ArrowDesc = {};
	ArrowDesc.fSpeedPerSec = 300.f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_CHARACTER, TEXT("Prototype_GameObject_CharaSelectArrow"), TEXT("Layer_MarkArrow"), &ArrowDesc)))
		return E_FAIL;

	CUIObject::UI_DESC SelectIconDesc = {};
	for (size_t i = 0; i < CUI_Define::PAWN_END; i++)
	{
		SelectIconDesc.iNumUI = i;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_CHARACTER, TEXT("Prototype_GameObject_CharaSelectIcon"), strLayerTag, &SelectIconDesc)))
			return E_FAIL;
	}
	
	

	return S_OK;
}

HRESULT CLevel_Chara_Select::Ready_Sound()
{
	return S_OK;
}

CLevel_Chara_Select* CLevel_Chara_Select::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Chara_Select* pInstance = new CLevel_Chara_Select(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Chara_Select"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Chara_Select::Free()
{
	m_pGameInstance->Stop_Sound(CSound_Manager::SOUND_KEY_NAME::LOGO_BGM);

	__super::Free();
}
