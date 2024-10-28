#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "Main_Camera.h"
#include "GameInstance.h"
#include "RenderInstance.h"
#include "Effect_Manager.h"
#include "Imgui_Manager.h"
#include "UI_ComboNumber.h"

#include "UIObject.h"
#include "UI_GameState.h"

#include "Character.h"
#include "Sound_Manager.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
	, m_pUI_Manager{ CUI_Manager::Get_Instance() }
	, m_pIMGUI_Manager{ CImgui_Manager::Get_Instance() }
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	m_iLevelIndex = LEVEL_GAMEPLAY;

	Create_Effect_Manager();

	//빛 준비
	if (FAILED(Ready_Lights()))
		return E_FAIL;

#pragma region 맵 사본 객체

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


#pragma endregion

#pragma region 캐릭터 사본 객체

	//1P
	CCharacter::Character_DESC CharacterDesc{};
	CharacterDesc.iTeam = 1;
	CharacterDesc.ePlayerSlot = CUI_Define::LPLAYER1;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Play_Goku"), TEXT("Layer_Character"), &CharacterDesc)))
		return E_FAIL;

	CharacterDesc.iTeam = 2;
	CharacterDesc.ePlayerSlot = CUI_Define::RPLAYER1;
	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Play_Goku"), TEXT("Layer_Character"), &CharacterDesc)))
	//	return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Play_21"), TEXT("Layer_Character"), &CharacterDesc)))
		return E_FAIL;


#pragma endregion

#pragma region UI 사본 객체
	//캐릭터 생성 이후 IMGUI_UIMANAGER 초기화 이후 
	if (FAILED(Ready_UIObjects()))
		return E_FAIL;
#pragma endregion

#pragma region 카메라 포인트 로드

	//카메라 생성
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Main_Camera"))))
		return E_FAIL;

	// 카메라 포인트 로드
	_wstring cameraFilePath = L"../Bin/CameraPoints.txt";
	CameraSaveData cameraSaveData;

	HRESULT hr = m_pGameInstance->Load_All_CameraPoints(cameraFilePath, &cameraSaveData);
	if (FAILED(hr)) {
		// 로드 실패 처리
		// 예: 로그 남기기 또는 기본 설정 적용
		return S_OK;
	}

	if (cameraSaveData.models.empty()) {
		// 로드된 데이터가 없으면 기본 설정 적용
		return S_OK;
	}

	// 로드된 데이터를 mainCamera에 적용
	CMain_Camera* mainCamera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, L"Layer_Main_Camera"));
	if (mainCamera) {
		mainCamera->ApplyCameraData(cameraSaveData);
	}

#pragma endregion

#pragma region 사운드 로드
	if (FAILED(Ready_Sound()))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_Z))
	{
		LIGHT_DESC			LightDesc{};

		ZeroMemory(&LightDesc, sizeof(LIGHT_DESC));
		LightDesc.eType = LIGHT_DESC::TYPE_POINT;
		LightDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		LightDesc.fRange = 30.f;
		LightDesc.vDiffuse = _float4(1.2f, 1.15f, 0.7f, 1.0f);
		//LightDesc.vDiffuse = _float4(1.0f, 0.f, 0.f, 1.f);
		LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
		LightDesc.vSpecular = _float4(1.0f, 0.95f, 0.45f, 1.f);

		LightDesc.fAccTime = 0.f;
		LightDesc.fLifeTime = 3.f;
		LightDesc.strName = "Explosion";
		if (FAILED(m_pRenderInstance->Add_Effect_Light(LightDesc.strName, LightDesc)))
			return;
	}
	if (m_pGameInstance->Key_Down(DIK_X))
	{
		LIGHT_DESC			LightDesc{};

		ZeroMemory(&LightDesc, sizeof(LIGHT_DESC));
		LightDesc.eType = LIGHT_DESC::TYPE_POINT;
		LightDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		LightDesc.fRange = 30.f;

		LightDesc.vDiffuse = _float4(0.9f, 1.1f, 1.7f, 1.0f); // 파란빛 계열로 변경

		LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
		LightDesc.vSpecular = _float4(1.0f, 0.95f, 0.45f, 1.f);

		LightDesc.fAccTime = 0.f;
		LightDesc.fLifeTime = 3.f;
		LightDesc.strName = "Ray";
		if (FAILED(m_pRenderInstance->Add_Effect_Light(LightDesc.strName, LightDesc)))
			return;
	}

	m_pUI_Manager->Update(fTimeDelta);
	m_pIMGUI_Manager->Update(fTimeDelta);
	m_pEffect_Manager->Update(fTimeDelta);
	m_pEffect_Manager->Late_Update(fTimeDelta);
}

HRESULT CLevel_GamePlay::Render(_float fTimeDelta)
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨"));

	//m_pIMGUI_Manager->Render(fTimeDelta);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Main_Camera"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof(LIGHT_DESC));
	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vPosition = _float4(30.f, 50.f, 1800.f, 1.f);
	LightDesc.fRange = 10000.f;
	LightDesc.vDiffuse = _float4(0.78f, 0.95f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.3f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 0.1f);


	if (FAILED(m_pRenderInstance->Add_Light(LightDesc)))
		return E_FAIL;
}

HRESULT CLevel_GamePlay::Ready_UIObjects()
{
	//UIMANAGER 초기화
	m_pUI_Manager->InitUIObject();

	CUIObject::UI_DESC tHpDesc = {};

	for (int i = 0; i < 2; ++i)
	{
		tHpDesc.eLRPos = static_cast<CUIObject::UI_LRPOS>(i);
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_HpPanel"), TEXT("Layer_UI_HpGauge"), &tHpDesc)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_HpEffect"), TEXT("Layer_UI_HpGauge"), &tHpDesc)))
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

HRESULT CLevel_GamePlay::Ready_Sound()
{
	//배경음
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Audio/BGM/013_bat_space.ogg", CSound_Manager::SOUND_KEY_NAME::SPACE_BGM);
	m_pGameInstance->Play_Sound(CSound_Manager::SOUND_KEY_NAME::SPACE_BGM, true, 0.2f);

#pragma region Goku_Sound

	//약 공격1,2
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::LIGHT_ATTACK, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Light_Attack_1.ogg", CSound_Manager::SOUND_GROUP_KEY_NAME::SFX_Goku_Light_Attack_1);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::LIGHT_ATTACK, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Light_Attack_2.ogg", CSound_Manager::SOUND_GROUP_KEY_NAME::SFX_Goku_Light_Attack_2);
	//약 공격3 (호랴)
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Heavy_Attack.ogg", CSound_Manager::SOUND_KEY_NAME::Goku_Heavy_Attack);

	//어퍼 공격 (소코카)
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Upper_Attack.ogg", CSound_Manager::SOUND_KEY_NAME::Goku_Upper_Attack);
	//원거리 공격
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Range_Attack.ogg", CSound_Manager::SOUND_KEY_NAME::Goku_Range_Attack);
	//에네르기 공격
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Energy_Attack.ogg", CSound_Manager::SOUND_KEY_NAME::Goku_Energy_Attack);

	//오공 히트 총 16개
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Goku_Hit, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Hit_0.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Goku_Hit_0);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Goku_Hit, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Hit_1.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Goku_Hit_1);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Goku_Hit, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Hit_2.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Goku_Hit_2);
#pragma region Hit_긴소리
	//너무 찡얼대는 소리가 많음
	/*m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Goku_Hit, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Hit_3.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Goku_Hit_3);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Goku_Hit, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Hit_4.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Goku_Hit_4);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Goku_Hit, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Hit_5.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Goku_Hit_5);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Goku_Hit, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Hit_6.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Goku_Hit_6);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Goku_Hit, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Hit_7.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Goku_Hit_7);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Goku_Hit, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Hit_8.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Goku_Hit_8);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Goku_Hit, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Hit_9.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Goku_Hit_9);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Goku_Hit, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Hit_10.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Goku_Hit_10);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Goku_Hit, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Hit_11.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Goku_Hit_11);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Goku_Hit, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Hit_12.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Goku_Hit_12);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Goku_Hit, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Hit_13.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Goku_Hit_13);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Goku_Hit, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Hit_14.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Goku_Hit_14);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Goku_Hit, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Hit_15.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Goku_Hit_15);*/
#pragma endregion

	//필살기_0
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Ultimate_0.ogg", CSound_Manager::SOUND_KEY_NAME::Goku_Ultimate_Attack_0);
	//필살기_1
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Ultimate_1.ogg", CSound_Manager::SOUND_KEY_NAME::Goku_Ultimate_Attack_1);
	//필살기_2
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Ultimate_2.ogg", CSound_Manager::SOUND_KEY_NAME::Goku_Ultimate_Attack_2);

#pragma endregion

	

	return S_OK;
}

void CLevel_GamePlay::Create_Effect_Manager()
{
	m_pEffect_Manager = CEffect_Manager::Get_Instance();
	m_pEffect_Manager->Initialize(m_pDevice, m_pContext);
}

HRESULT CLevel_GamePlay::Ready_Character()
{
	CCharacter::Character_DESC SlotDesc = {};

	//플레이어 생성
	SlotDesc.ePlayerSlot = CUI_Define::LPLAYER1;
	SlotDesc.fSpeedPerSec = 5.f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Play_Goku"), TEXT("Layer_Character"), &SlotDesc)))
		return E_FAIL;

	SlotDesc.ePlayerSlot = CUI_Define::LPLAYER2;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Play_21"), TEXT("Layer_Character"), &SlotDesc)))
		return E_FAIL;

	m_pUI_Manager->InitUIObject();

	return S_OK;
}

CLevel_GamePlay* CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();

	CFrameEvent_Manager::Destroy_Instance();
	CEffect_Manager::Get_Instance()->Destroy_Instance();
}
