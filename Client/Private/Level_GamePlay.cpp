#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "Camera_Free.h"
#include "GameInstance.h"
#include "RenderInstance.h"
//#include "LandObject.h"
#include "Monster.h"
#include "UI_ComboNumber.h"

#include "Pawn.h"
#include "UIObject.h"
#include "UI_GameState.h"
#include "Character.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext } 
	, m_pUI_Manager{ CUI_Manager::Get_Instance() }
{
	Safe_AddRef(m_pUI_Manager);
}

HRESULT CLevel_GamePlay::Initialize()
{
	m_iLevelIndex = LEVEL_GAMEPLAY;


	//카메라 생성
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	_bool bTest = false;
	if(bTest)
	{

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Model_Preview"), TEXT("Layer_Model_Preview"))))
			return E_FAIL;
	}
	else
	{


		if (Ready_Character())
			return E_FAIL;

		//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Play_21"), TEXT("Layer_Character"))))
		//	return E_FAIL;


	}

	if (FAILED(Ready_UIObjects()))
		return E_FAIL;
	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_F1))
	{
		m_pUI_Manager->m_fTotalDuration = 0.f;
		m_pUI_Manager->UsingCreateStartUI();
	}

	if (m_pGameInstance->Key_Down(DIK_F2))
	{
		m_pUI_Manager->m_fTotalDuration = 0.f;
		m_pUI_Manager->UsingCreateEndUI();
	}
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨"));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring & strLayerTag)
{
	CCamera_Free::CAMERA_FREE_DESC			CameraDesc{};

	CameraDesc.vEye = _float3(0.f, 10.f, -10.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1000.f;
	CameraDesc.fSpeedPerSec = 10.f;
	CameraDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	CameraDesc.fSensor = 0.1f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"), strLayerTag, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};
	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	//LightDesc.vDirection = _float4(1.f, 1.f, 1.f, 0.f);
	LightDesc.vDirection = _float4(-0.5f, -0.2f, 0.5f, 0.f);
	LightDesc.vDiffuse = _float4(0.8f, 0.85f, 1.0f, 1.0f);
	LightDesc.vAmbient = _float4(0.7f, 0.7f, 0.7f, 1.f);
	LightDesc.vSpecular = _float4(0.f, 0.f, 0.f, 1.f);

	if (FAILED(m_pRenderInstance->Add_Light(LightDesc)))
		return E_FAIL;
}

HRESULT CLevel_GamePlay::Ready_UIObjects()
{
	CUIObject::UI_DESC tHpDesc = {};

	for (int i = 0; i < 2; ++i)
	{
		tHpDesc.eLRPos = static_cast<CUIObject::UI_LRPOS>(i);
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_HpPanel"), TEXT("Layer_UI_HpGauge"), &tHpDesc)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_HpEffect"), TEXT("Layer_UI_HpGauge"),&tHpDesc)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_HpGauge"), TEXT("Layer_UI_HpGauge"), &tHpDesc)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SubHpGauge"), TEXT("Layer_UI_HpGauge"), &tHpDesc)))
			return E_FAIL;
	}
	//캐릭터 아이콘

	CUIObject::UI_DESC Icon_Desc = {};

	for (int i = 0; i < 2; ++i)
	{
		Icon_Desc.eLRPos = static_cast<CUIObject::UI_LRPOS>(i);

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Chara_Icon_Panel"), TEXT("Layer_UI_Chara_Icon"), &Icon_Desc)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Chara_Icon"), TEXT("Layer_UI_Chara_Icon"), &Icon_Desc)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Chara_SubIcon"), TEXT("Layer_UI_Chara_Icon"), &Icon_Desc)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Sub_Chara_Icon_Panel"), TEXT("Layer_UI_Chara_Icon"), &Icon_Desc)))
			return E_FAIL;
	}


	//공격력 버프

	CUIObject::UI_DESC AttBufoDesc = {};

	for (int i = 0; i < 2; ++i)
	{
		AttBufoDesc.eLRPos = static_cast<CUIObject::UI_LRPOS>(i);

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_AttBufMark"), TEXT("Layer_UI_AttBufMark"), &AttBufoDesc)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_AttBuf"), TEXT("Layer_UI_AttBuf"), &AttBufoDesc)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_AttBufNone"), TEXT("Layer_UI_AttBuf"), &AttBufoDesc)))
			return E_FAIL;
	}

	// 스킬
	CUIObject::UI_DESC SkilloDesc = {};
	for (int i = 0; i < 2; i++)
	{
		SkilloDesc.eLRPos = static_cast<CUIObject::UI_LRPOS>(i);

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SkillGauge"), TEXT("Layer_UI_SkillGauge"), &SkilloDesc)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SkillGaugeBar"), TEXT("Layer_UI_SkillGauge"), &SkilloDesc)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SkillNumber"), TEXT("Layer_UI_SkillNumber"), &SkilloDesc)))
			return E_FAIL;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SkillEffect"), TEXT("Layer_UI_SkillGauge"), &SkilloDesc);


	}

	//타이머
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Timer"), TEXT("Layer_UI_Timer"))))
		return E_FAIL;

	//콤보
	CUIObject::UI_DESC ComboDesc = {};
	for (int i = 0; i < 3; ++i)
	{
		ComboDesc.iNumUI = i;

		for (int k = 0; k < 2; k++)
		{
			ComboDesc.eLRPos = static_cast<CUIObject::UI_LRPOS>(k);
			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_ComboNumber"), TEXT("Layer_UI_Combo_Number"), &ComboDesc);
		}
	}

	for (int k = 0; k < 2; k++)
	{
		ComboDesc.eLRPos = static_cast<CUIObject::UI_LRPOS>(k);

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_ComboFont"), TEXT("Layer_UI_Combo_Font"), &ComboDesc);
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_ComboEffect"), TEXT("Layer_UI_Combo_Effect"), &ComboDesc);
	}

	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_InputDirPanel"), TEXT("Layer_UI_Input"));
	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_InputDir"), TEXT("Layer_UI_Input"));

	CUIObject::UI_DESC KeyInputDesc = {};

	for (int i = 0; i < 6; i++)
	{
		KeyInputDesc.iNumUI = i;
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_InputAction"), TEXT("Layer_UI_InputAction"), &KeyInputDesc);
	}

	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_TimerPanel"), TEXT("Layer_UI_Timer"));


	CUIObject::UI_DESC FontNameDesc = {};
	for (int i = 0; i < 2; ++i)	
	{
		KeyInputDesc.eLRPos = static_cast<CUIObject::UI_LRPOS>(i);
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_FontName"), TEXT("Layer_UI_FontName"), &KeyInputDesc);
		
	}

}

HRESULT CLevel_GamePlay::Ready_Character()
{
	CCharacter::Character_DESC SlotDesc = {};


	//플레이어 생성
	SlotDesc.ePlayerSlot = CCharacter::RPLAYER1;
	SlotDesc.fSpeedPerSec = 5.f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Play_Goku"), TEXT("Layer_Character"),&SlotDesc)))
		return E_FAIL;

	SlotDesc.ePlayerSlot = CCharacter::RPLAYER2;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Play_21"), TEXT("Layer_Character"), &SlotDesc)))
		return E_FAIL;


	m_pUI_Manager->InitUIObject();



	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	Safe_Release(m_pUI_Manager);

	__super::Free();
}
