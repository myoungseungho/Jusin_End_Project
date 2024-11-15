#include "stdafx.h"
#include "..\Public\Level_Loading.h"

#include "Loader.h"
#include "Level_Logo.h"
#include "Level_Lobby.h"
#include "Level_Chara_Select.h"
#include "Level_GamePlay.h"
#include "Level_VS.h"

#include "GameInstance.h"
#include "SpaceSun.h"
#include "UIObject.h"

#include "UI_Loading_BG.h"
#include "UI_LoadingMark.h"
#include "UI_Loading_Font.h"
#include "UI_LoadingSpaceLight.h"
#include "UI_Loading_FlyEff.h"
#include "UI_Loading_DragonBall.h"

_bool CLevel_Loading::m_bIsLevelPrepared = false;

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{
}

HRESULT CLevel_Loading::Initialize(LEVELID eNextLevelID)
{
	m_iLevelIndex = LEVEL_LOADING;
	m_eNextLevelID = eNextLevelID;

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Ready_Layer()))
		return E_FAIL;

	//m_pLoader = CLoader::Create(m_pDevice, m_pContext, eNextLevelID);
	//if (nullptr == m_pLoader)
	//	return E_FAIL;
	
	m_bIsLevelPrepared = TRUE;
	
	return S_OK;
}

HRESULT CLevel_Loading::Ready_Prototype_Component()
{
	if (m_bIsLevelPrepared)
		return S_OK;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LoadingBackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/CmnBG/tex/E3_Title_BG01.png")))))
		return E_FAIL;

	/* For.Prototyp_Component_Texture_UI_LoadingBackGround_Mask */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LoadingBackGround_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/CharaSelect_S3/tex/stage/stage_bg_0.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_GameStartCircle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LoadingMark"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/InGame/Middle/GameStart/GameStart%d.png"), 8))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_UI_LoadingFont */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LoadingFont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/InGame/DB_load_00.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_LoadingSpaceLight */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LoadingSpaceLight"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Stage_Image/sp_light.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_LoadingSpaceLight */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LoadingFlyEffect"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/CmnBG_Eff_Lens_5.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_LoadingBallEff */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LoadingBallEff"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/cp_dg_ball_Eff%d.png"),9))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_LoadingBallEff */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LoadingDragonBall"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/DragonBall%d.png"), 8))))
		return E_FAIL;

#pragma endregion

	/* For.Prototype_GameObject_UI_Loading */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Loading"),
		CUI_Loading_BG::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_LoadingMark */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_LoadingMark"),
		CUI_LoadingMark::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_LoadingFont */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_LoadingFont"),
		CUI_Loading_Font::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_LoadingSpaceLight */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_LoadingSpaceLight"),
		CUI_LoadingSpaceLight::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Loading_FlyEff */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Loading_FlyEff"),
		CUI_Loading_FlyEff::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Loading_DragonBall */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Loading_DragonBall"),
		CUI_Loading_DragonBall::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	m_bIsLevelPrepared = true;

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer()
{
	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOADING, TEXT("Prototype_GameObject_UI_Loading"), TEXT("Layer_UI_LoadingBackGround"));

	CUIObject::UI_DESC Desc = {};
	Desc.fSpeedPerSec = 50.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);
	for (int i = 0; i < 8; i++)
	{
		Desc.iNumUI = i;
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOADING, TEXT("Prototype_GameObject_UI_LoadingMark"), TEXT("Layer_UI_LoadingBackGround"), &Desc);

	}

	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOADING, TEXT("Prototype_GameObject_UI_LoadingFont"), TEXT("Layer_UI_LoadingBackGround"));

	CUIObject::UI_DESC BallDesc = {};
	for (int i = 0; i < 7; i++)
	{
		BallDesc.iNumUI = i;
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOADING, TEXT("Prototype_GameObject_UI_Loading_DragonBall"), TEXT("Layer_UI_LoadingHole"), &BallDesc);
	}

	CUIObject::UI_DESC FlyDesc = {};
	FlyDesc.fSpeedPerSec = 200.f;

	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOADING, TEXT("Prototype_GameObject_UI_Loading_FlyEff"), TEXT("Layer_UI_LoadingBackGround"),&FlyDesc);
	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOADING, TEXT("Prototype_GameObject_UI_LoadingSpaceLight"), TEXT("Layer_UI_LoadingBackGround"));

	
	return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{
	//if (true == m_pLoader->isFinished())
	//{
	//	CLevel* pNextLevel = { nullptr };
	//
	//	switch (m_eNextLevelID)
	//	{
	//	case LEVEL_LOGO:
	//		pNextLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
	//		break;
	//
	//	case LEVEL_GAMEPLAY:
	//		pNextLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
	//		break;
	//	case LEVEL_LOBBY:
	//		pNextLevel = CLevel_Lobby::Create(m_pDevice, m_pContext);
	//		break;
	//	case LEVEL_CHARACTER:
	//		pNextLevel = CLevel_Chara_Select::Create(m_pDevice, m_pContext);
	//		break;
	//
	//	case LEVEL_VS:
	//		pNextLevel = CLevel_VS::Create(m_pDevice, m_pContext);
	//		break;
	//
	//
	//	}
	//
	//	if (FAILED(m_pGameInstance->Change_Level(pNextLevel)))
	//		return;
	//}
}

HRESULT CLevel_Loading::Render(_float fTimeDelta)
{
	m_pLoader->Draw_Text();

	return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Loading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
