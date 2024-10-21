#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "Camera_Free.h"
#include "GameInstance.h"
#include "RenderInstance.h"
#include "Effect_Manager.h"
#include "Imgui_Manager.h"
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

	if (FAILED(Ready_Effect_Manager()))
		return E_FAIL;


	if (FAILED(Ready_Lights()))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceSky"), TEXT("Layer_SpaceSky"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceHorizon"), TEXT("Layer_SpaceHorizon"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceSun"), TEXT("Layer_SpaceSun"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceGround"), TEXT("Layer_SpaceGround"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceCliff"), TEXT("Layer_SpaceSky"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceStage"), TEXT("Layer_SpaceStage"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceEarth"), TEXT("Layer_Space_Earth"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceEarth_Light"), TEXT("Layer_SpaceEarth_Light"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_FallingStar"), TEXT("Layer__FallingStar"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceMoon"), TEXT("Layer_SpaceMoon"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceStone"), TEXT("Layer_SpaceStone"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceRock"), TEXT("Layer_SpaceRock"))))
		return E_FAIL;
	
	//몬스터 생성
	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster"), TEXT("Layer_Monster"))))
	//	return E_FAIL;


	//_bool bTest = false;
	//if (bTest)
	//{
	//
	//	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Model_Preview"), TEXT("Layer_Model_Preview"))))
	//		return E_FAIL;
	//}
	//else
	//{


		//if (Ready_Character())
		//	return E_FAIL;

		//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Play_21"), TEXT("Layer_Character"))))
		//	return E_FAIL;


		//1P
		CCharacter::Character_DESC CharacterDesc{};
		CharacterDesc.iTeam = 1;
		CharacterDesc.ePlayerSlot = CCharacter::LPLAYER1;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Play_Goku"), TEXT("Layer_Character"), &CharacterDesc)))
			return E_FAIL;

		CharacterDesc.iTeam = 2;
		CharacterDesc.ePlayerSlot = CCharacter::RPLAYER1;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Play_Goku"), TEXT("Layer_Character"), &CharacterDesc)))
			return E_FAIL;

		m_pUI_Manager->InitUIObject();

		//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster"), TEXT("Layer_Target"))))
		//	return E_FAIL;





	//}

	// 해당 레벨에 맞는 파일 경로를 설정
	wstring filePath = L"../Bin/Level_GamePlay_Objects.txt";

		if (FAILED(Ready_Lights()))
			return E_FAIL;
		
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceSky"), TEXT("Layer_SpaceSky"))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceHorizon"), TEXT("Layer_SpaceHorizon"))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceSun"), TEXT("Layer_SpaceSun"))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceGround"), TEXT("Layer_SpaceGround"))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceCliff"), TEXT("Layer_SpaceSky"))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceStage"), TEXT("Layer_SpaceStage"))))
			return E_FAIL;
		
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceEarth"), TEXT("Layer_Space_Earth"))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceEarth_Light"), TEXT("Layer_SpaceEarth_Light"))))
			return E_FAIL;
		
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_FallingStar"), TEXT("Layer__FallingStar"))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceMoon"), TEXT("Layer_SpaceMoon"))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceStone"), TEXT("Layer_SpaceStone"))))
			return E_FAIL;
		

		if (FAILED(ParseInitialize(filePath))) 
	
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

	CImgui_Manager::Get_Instance()->Update(fTimeDelta);
	m_pEffect_Manager->Update(fTimeDelta);
	m_pEffect_Manager->Late_Update(fTimeDelta);
}

HRESULT CLevel_GamePlay::Render(_float fTimeDelta)
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨"));

	CImgui_Manager::Get_Instance()->Render(fTimeDelta);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring & strLayerTag)
{
	CCamera_Free::CAMERA_FREE_DESC			CameraDesc{};

	CameraDesc.vEye = _float3(0.f, 10.f, -15.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 1.f);
	CameraDesc.fFovy = XMConvertToRadians(54.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1000.f; //1000.f;
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


	//LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	////LightDesc.vDirection = _float4(1.f, 1.f, 1.f, 0.f);
	//LightDesc.vDirection = _float4(-0.5f, -0.2f, 0.5f, 0.f);
	//LightDesc.vDiffuse = _float4(0.8f, 0.85f, 1.0f, 1.0f);
	//LightDesc.vAmbient = _float4(0.7f, 0.7f, 0.7f, 1.f);
	//LightDesc.vSpecular = _float4(0.f, 0.f, 0.f, 1.f);

	//if (FAILED(m_pRenderInstance->Add_Light(LightDesc)))
	//	return E_FAIL;


	ZeroMemory(&LightDesc, sizeof(LIGHT_DESC));
	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vPosition = _float4(30.f, 50.f, 1800.f, 1.f);
	LightDesc.fRange = 10000.f;
	LightDesc.vDiffuse = _float4(0.78f,0.95f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.3f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 0.1f);


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
	SlotDesc.ePlayerSlot = CCharacter::LPLAYER1;
	SlotDesc.fSpeedPerSec = 5.f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Play_Goku"), TEXT("Layer_Character"), &SlotDesc)))
		return E_FAIL;

	SlotDesc.ePlayerSlot = CCharacter::LPLAYER2;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Play_21"), TEXT("Layer_Character"), &SlotDesc)))
		return E_FAIL;


	m_pUI_Manager->InitUIObject();

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Effect_Manager()
{
	m_pEffect_Manager = CEffect_Manager::Get_Instance();

	if(m_pEffect_Manager == nullptr)
		return E_FAIL;

	Safe_AddRef(m_pEffect_Manager);

	m_pEffect_Manager->Initialize();

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
	m_pEffect_Manager->Free();
	Safe_Release(m_pEffect_Manager);

	__super::Free();

}
