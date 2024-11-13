#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "Main_Camera.h"
#include "GameInstance.h"
#include "RenderInstance.h"
#include "Effect_Manager.h"
#include "Imgui_Manager.h"
#include "QTE_Manager.h"
#include "UI_ComboNumber.h"

#include "UIObject.h"
#include "UI_GameState.h"

#include "Map_Manager.h"
#include "Character.h"
#include "Sound_Manager.h"
#include "BattleInterface.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
	, m_pUI_Manager{ CUI_Manager::Get_Instance() }
	, m_pIMGUI_Manager{ CImgui_Manager::Get_Instance() }
	, m_pQTE_Manager{ CQTE_Manager::Get_Instance() }
	, m_pMap_Manager {CMap_Manager::Get_Instance() }
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	m_iLevelIndex = LEVEL_GAMEPLAY;
	m_pMap_Manager->Initialize(m_pDevice, m_pContext);
	Create_Effect_Manager();
	Create_QTE_Manager();

#pragma region 이펙트 세팅
	Loading_For_Effect();
#pragma endregion

#pragma region 맵 사본 객체

	if (FAILED(Ready_Volcano()))
		return E_FAIL;

	if (FAILED(Ready_Space()))
		return E_FAIL;
#pragma endregion

#pragma region 캐릭터 사본 객체



	//반드시 1P,2P 순서로 생성해야하는가?

	//1P
	CCharacter::Character_DESC CharacterDesc{};
	CharacterDesc.iTeam = 1;
	CharacterDesc.ePlayerSlot = CUI_Define::LPLAYER1;


	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Play_Goku"), TEXT("Layer_Character"), &CharacterDesc)))
		return E_FAIL;
	
	CharacterDesc.iTeam = 2;
	CharacterDesc.ePlayerSlot = CUI_Define::RPLAYER1;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Play_Hit"), TEXT("Layer_Character"), &CharacterDesc)))
		return E_FAIL;
	
	CharacterDesc.iTeam = 1;
	CharacterDesc.ePlayerSlot = CUI_Define::LPLAYER2;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Play_21"), TEXT("Layer_Character"), &CharacterDesc)))
		return E_FAIL;
	
	
	CharacterDesc.iTeam = 2;
	CharacterDesc.ePlayerSlot = CUI_Define::RPLAYER2;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Play_Hit"), TEXT("Layer_Character"), &CharacterDesc)))
		return E_FAIL;

	//for (int i = 0; i < 4 ; ++i)
	//{
	//		CharacterDesc.iTeam = CBattleInterface_Manager::Get_Instance()->Get_CharaDesc(i).iTeam;
	//		CharacterDesc.ePlayerSlot = CBattleInterface_Manager::Get_Instance()->Get_CharaDesc(i).eSlot;
	//		_wstring strProtypeTag = CBattleInterface_Manager::Get_Instance()->Get_CharaDesc(i).PrototypeTag;
	//
	//		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, strProtypeTag, TEXT("Layer_Character"), &CharacterDesc)))
	//			return E_FAIL;
	//}

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



	//빛 준비
	if (FAILED(Ready_Lights()))
		return E_FAIL;

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

	m_pUI_Manager->GamePlayUpdate(fTimeDelta);
	m_pIMGUI_Manager->Update(fTimeDelta);
	m_pEffect_Manager->Update(fTimeDelta);
	m_pEffect_Manager->Late_Update(fTimeDelta);

	m_pQTE_Manager->Update(fTimeDelta);
	m_pQTE_Manager->Late_Update(fTimeDelta);

}

HRESULT CLevel_GamePlay::Render(_float fTimeDelta)
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨"));

	m_pEffect_Manager->Render(fTimeDelta);

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

	return S_OK;
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

		//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_HpEffect"), TEXT("Layer_UI_HpGauge"), &tHpDesc)))
		//	return E_FAIL;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_HpGauge"), TEXT("Layer_UI_HpGauge"), &tHpDesc)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SubHpGauge"), TEXT("Layer_UI_HpGauge"), &tHpDesc)))
			return E_FAIL;


		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SubHpPanel"), TEXT("Layer_UI_HpGauge"), &tHpDesc)))
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

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_AttBufMark"), TEXT("Layer_UI_AttBufEffect"), &AttBufoDesc)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_AttBuf"), TEXT("Layer_UI_AttBufEffect"), &AttBufoDesc)))
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

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_TimerPanel"), TEXT("Layer_UI_Timer"))))
		return E_FAIL;

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

	CUIObject::UI_DESC FontNameDesc = {};
	for (int i = 0; i < 2; ++i)
	{
		KeyInputDesc.eLRPos = static_cast<CUIObject::UI_LRPOS>(i);
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_FontName"), TEXT("Layer_UI_FontName"), &KeyInputDesc);

	}


	//옵션

	//사운드 옵션
	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Opt_Sound_Panel"), TEXT("Layer_UI_Option_Sound"));
	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Opt_Sound_Effect"), TEXT("Layer_UI_Option_Sound"));
	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Opt_Sound_Title"), TEXT("Layer_UI_Option_Sound"));
	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Opt_Sound_Font"), TEXT("Layer_UI_Option_Sound"));
	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Opt_Sound_Arrow"), TEXT("Layer_UI_Option_Sound"));

	for (int i = 0; i < 3; i++)
	{
		CUIObject::UI_DESC VolumePanelDesc = {};
		VolumePanelDesc.iNumUI = i;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Opt_Sound_Volume_Panel"), TEXT("Layer_UI_Option_Sound"), &VolumePanelDesc);
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Opt_Sound_Volume_Gauge"), TEXT("Layer_UI_Option_Sound"), &VolumePanelDesc);
	}




	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Sound()
{
	//배경음
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Audio/BGM/013_bat_space.ogg", CSound_Manager::SOUND_KEY_NAME::SPACE_BGM, CSound_Manager::SOUND_CATEGORY::BGM, true);
	m_pGameInstance->Play_Sound(CSound_Manager::SOUND_KEY_NAME::SPACE_BGM, true, 0.2f);

#pragma region Goku_Sound

	//약 공격1,2
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::LIGHT_ATTACK_Goku, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Light_Attack_1.ogg", CSound_Manager::SOUND_GROUP_KEY_NAME::Light_Attack_Goku_1, CSound_Manager::SOUND_CATEGORY::VOICE, false);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::LIGHT_ATTACK_Goku, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Light_Attack_2.ogg", CSound_Manager::SOUND_GROUP_KEY_NAME::Light_Attack_Goku_2, CSound_Manager::SOUND_CATEGORY::VOICE, false);
	//약 공격3 (호랴)
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Heavy_Attack.ogg", CSound_Manager::SOUND_KEY_NAME::Goku_Heavy_Attack, CSound_Manager::SOUND_CATEGORY::VOICE, false);

	//어퍼 공격 (소코카)
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Upper_Attack.ogg", CSound_Manager::SOUND_KEY_NAME::Goku_Upper_Attack, CSound_Manager::SOUND_CATEGORY::VOICE, false);
	//원거리 공격
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Range_Attack.ogg", CSound_Manager::SOUND_KEY_NAME::Goku_Range_Attack, CSound_Manager::SOUND_CATEGORY::VOICE, false);
	//에네르기 공격
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Energy_Attack.ogg", CSound_Manager::SOUND_KEY_NAME::Goku_Energy_Attack, CSound_Manager::SOUND_CATEGORY::VOICE, false);

	//오공 히트 총 16개
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Hit_Goku, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Hit_0.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Goku_Hit_0, CSound_Manager::SOUND_CATEGORY::VOICE, false);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Hit_Goku, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Hit_1.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Goku_Hit_1, CSound_Manager::SOUND_CATEGORY::VOICE, false);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Hit_Goku, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Hit_2.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Goku_Hit_2, CSound_Manager::SOUND_CATEGORY::VOICE, false);

	//필살기_0
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Ultimate_0.ogg", CSound_Manager::SOUND_KEY_NAME::Goku_Ultimate_Attack_0, CSound_Manager::SOUND_CATEGORY::VOICE, false);
	//필살기_1
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Ultimate_1.ogg", CSound_Manager::SOUND_KEY_NAME::Goku_Ultimate_Attack_1, CSound_Manager::SOUND_CATEGORY::VOICE, false);
	//필살기_2
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_Voice/Ultimate_2.ogg", CSound_Manager::SOUND_KEY_NAME::Goku_Ultimate_Attack_2, CSound_Manager::SOUND_CATEGORY::VOICE, false);




	//SFX

	//Light_Attack
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::LIGHT_ATTACK_Goku_SFX, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_SFX/Light_Attack_SFX_0.ogg", CSound_Manager::SOUND_GROUP_KEY_NAME::Light_Attack_Goku_1_SFX, CSound_Manager::SOUND_CATEGORY::SFX, false);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::LIGHT_ATTACK_Goku_SFX, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_SFX/Light_Attack_SFX_1.ogg", CSound_Manager::SOUND_GROUP_KEY_NAME::Light_Attack_Goku_2_SFX, CSound_Manager::SOUND_CATEGORY::SFX, false);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::LIGHT_ATTACK_Goku_SFX, L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_SFX/Light_Attack_SFX_2.ogg", CSound_Manager::SOUND_GROUP_KEY_NAME::Light_Attack_Goku_3_SFX, CSound_Manager::SOUND_CATEGORY::SFX, false);

	//대쉬
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_SFX/Dash_SFX.wav", CSound_Manager::SOUND_KEY_NAME::Common_Dash_SFX, CSound_Manager::SOUND_CATEGORY::SFX, false);
	//다운 훅 대쉬
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_SFX/DownHook_Dash_SFX.wav", CSound_Manager::SOUND_KEY_NAME::Common_DownHook_Dash_SFX, CSound_Manager::SOUND_CATEGORY::SFX, false);
	//에네르기파
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_SFX/Energy_SFX.ogg", CSound_Manager::SOUND_KEY_NAME::Goku_Energy_SFX, CSound_Manager::SOUND_CATEGORY::SFX, false);
	//강공격
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_SFX/Heavy_Attack_SFX.ogg", CSound_Manager::SOUND_KEY_NAME::Goku_Heavy_Attack_SFX, CSound_Manager::SOUND_CATEGORY::SFX, false);
	//원거리 공격
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_SFX/Range_Attack_SFX.ogg", CSound_Manager::SOUND_KEY_NAME::Goku_Range_Attack_SFX, CSound_Manager::SOUND_CATEGORY::SFX, false);


	//초필살기 1
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_SFX/Ultimate_0.ogg", CSound_Manager::SOUND_KEY_NAME::Goku_Ultimate_0_SFX, CSound_Manager::SOUND_CATEGORY::SFX, false);
	//초필살기 2
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_SFX/Ultimate_1.ogg", CSound_Manager::SOUND_KEY_NAME::Goku_Ultimate_1_SFX, CSound_Manager::SOUND_CATEGORY::SFX, false);
	//초필살기 3
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_SFX/Ultimate_2.ogg", CSound_Manager::SOUND_KEY_NAME::Goku_Ultimate_2_SFX, CSound_Manager::SOUND_CATEGORY::SFX, false);
	//초필살기 4
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_SFX/Ultimate_3.ogg", CSound_Manager::SOUND_KEY_NAME::Goku_Ultimate_3_SFX, CSound_Manager::SOUND_CATEGORY::SFX, false);

	//스매쉬 Hit
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/Goku/Real_SFX/Smash_Hit.ogg", CSound_Manager::SOUND_KEY_NAME::Smash_Hit_SFX, CSound_Manager::SOUND_CATEGORY::SFX, false);

#pragma endregion

#pragma region 21_Sound

	//약공
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::LIGHT_ATTACK_21, L"../Bin/SoundSDK/AudioClip/Chara/21/Real_Voice/Light_Attack_1.ogg", CSound_Manager::SOUND_GROUP_KEY_NAME::Light_Attack_21_1, CSound_Manager::SOUND_CATEGORY::VOICE, false);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::LIGHT_ATTACK_21, L"../Bin/SoundSDK/AudioClip/Chara/21/Real_Voice/Light_Attack_2.ogg", CSound_Manager::SOUND_GROUP_KEY_NAME::Light_Attack_21_2, CSound_Manager::SOUND_CATEGORY::VOICE, false);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::LIGHT_ATTACK_21, L"../Bin/SoundSDK/AudioClip/Chara/21/Real_Voice/Light_Attack_3.ogg", CSound_Manager::SOUND_GROUP_KEY_NAME::Light_Attack_21_3, CSound_Manager::SOUND_CATEGORY::VOICE, false);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::LIGHT_ATTACK_21, L"../Bin/SoundSDK/AudioClip/Chara/21/Real_Voice/Light_Attack_4.ogg", CSound_Manager::SOUND_GROUP_KEY_NAME::Light_Attack_21_4, CSound_Manager::SOUND_CATEGORY::VOICE, false);

	//강공
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/21/Real_Voice/Heavy_Attack.ogg", CSound_Manager::SOUND_KEY_NAME::Heavy_Attack_21, CSound_Manager::SOUND_CATEGORY::VOICE, false);

	//히트
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Hit_21, L"../Bin/SoundSDK/AudioClip/Chara/21/Real_Voice/Hit_21_0.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Hit_21_0, CSound_Manager::SOUND_CATEGORY::VOICE, false);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Hit_21, L"../Bin/SoundSDK/AudioClip/Chara/21/Real_Voice/Hit_21_1.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Hit_21_1, CSound_Manager::SOUND_CATEGORY::VOICE, false);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Hit_21, L"../Bin/SoundSDK/AudioClip/Chara/21/Real_Voice/Hit_21_2.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Hit_21_2, CSound_Manager::SOUND_CATEGORY::VOICE, false);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Hit_21, L"../Bin/SoundSDK/AudioClip/Chara/21/Real_Voice/Hit_21_3.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Hit_21_3, CSound_Manager::SOUND_CATEGORY::VOICE, false);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Hit_21, L"../Bin/SoundSDK/AudioClip/Chara/21/Real_Voice/Hit_21_4.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Hit_21_4, CSound_Manager::SOUND_CATEGORY::VOICE, false);
	m_pGameInstance->Register_Sound_Group(CSound_Manager::SOUND_GROUP_KEY::Hit_21, L"../Bin/SoundSDK/AudioClip/Chara/21/Real_Voice/Hit_21_5.wav", CSound_Manager::SOUND_GROUP_KEY_NAME::Hit_21_5, CSound_Manager::SOUND_CATEGORY::VOICE, false);

	//그랩
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/21/Real_Voice/Grab.ogg", CSound_Manager::SOUND_KEY_NAME::Grab_Attack_21, CSound_Manager::SOUND_CATEGORY::VOICE, false);
	//추적
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/21/Real_Voice/ChaseDash.ogg", CSound_Manager::SOUND_KEY_NAME::Chase_Attack_21, CSound_Manager::SOUND_CATEGORY::VOICE, false);

	//그랩_SFX_0
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/21/Real_SFX/Grab_0.ogg", CSound_Manager::SOUND_KEY_NAME::Grab_Attack_0_21_SFX, CSound_Manager::SOUND_CATEGORY::SFX, false);
	//그랩_SFX_1
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Chara/21/Real_SFX/Grab_1.ogg", CSound_Manager::SOUND_KEY_NAME::Grab_Attack_1_21_SFX, CSound_Manager::SOUND_CATEGORY::SFX, false);
#pragma endregion

#pragma region UI 나레이션 사운드
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Audio/Narration/JPN/NA_0509_BattleCall_Start01.ogg", CSound_Manager::SOUND_KEY_NAME::NARRATION_READY, CSound_Manager::SOUND_CATEGORY::VOICE, false);
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Audio/Narration/JPN/NA_0513_BattleCall_Fight01.ogg", CSound_Manager::SOUND_KEY_NAME::NARRATION_FIGHT, CSound_Manager::SOUND_CATEGORY::VOICE, false);
#pragma endregion

#pragma region UI 사운드
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Audio/UI/ARC_MENU_SYS_MenuOpen.ogg", CSound_Manager::SOUND_KEY_NAME::UI_MENU_OPEN, CSound_Manager::SOUND_CATEGORY::SFX, false);
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Audio/UI/ARC_MENU_SYS_MenuClose.ogg", CSound_Manager::SOUND_KEY_NAME::UI_MENU_CLOSE, CSound_Manager::SOUND_CATEGORY::SFX, false);
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Audio/UI/ARC_MENU_SYS_Cursor.ogg", CSound_Manager::SOUND_KEY_NAME::UI_MENU_CURSOR, CSound_Manager::SOUND_CATEGORY::SFX, false);
#pragma endregion

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Space()
{
	CMap_Manager::Map_Object_Key tDesc{};
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_SpaceSky");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceSky"), TEXT("Layer_SpaceSky"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_SpaceHorizon");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceHorizon"), TEXT("Layer_SpaceHorizon"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_SpaceSun");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceSun"), TEXT("Layer_SpaceSun"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_SpaceGround");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceGround"), TEXT("Layer_SpaceGround"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_SpaceCliff");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceCliff"), TEXT("Layer_SpaceSky"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_SpaceStage");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceStage"), TEXT("Layer_SpaceStage"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_SpaceEarth");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceEarth"), TEXT("Layer_Space_Earth"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_SpaceEarth_Light");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceEarth_Light"), TEXT("Layer_SpaceEarth_Light"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_FallingStar");

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_FallingStar"), TEXT("Layer__FallingStar"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_SpaceMoon");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceMoon"), TEXT("Layer_SpaceMoon"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_SpaceStone");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceStone"), TEXT("Layer_SpaceStone"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_SpaceRock");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceRock"), TEXT("Layer_SpaceRock"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_SpaceMeteoBreak");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SpaceMeteoBreak"), TEXT("Layer_MeteoBreak"), &tDesc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Volcano()
{
	CMap_Manager::Map_Object_Key tDesc{};
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_Volcano_Stage");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Volcano_Stage"), TEXT("Layer_VolcanoStage"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_Volcano_Cliff_Back");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Volcano_Cliff_Back"), TEXT("Layer_VolcanoStage"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_Volcano_Cliff_Far");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Volcano_Cliff_Far"), TEXT("Layer_VolcanoStage"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_Volcano_Cliff04");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Volcano_Cliff04"), TEXT("Layer_VolcanoStage"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_Volcano_Cliff03");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Volcano_Cliff03"), TEXT("Layer_VolcanoStage"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_Volcano_Cliff02");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Volcano_Cliff02"), TEXT("Layer_VolcanoStage"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_Volcano_Cliff01");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Volcano_Cliff01"), TEXT("Layer_VolcanoStage"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_Volcano_Ground");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Volcano_Ground"), TEXT("Layer_VolcanoStage"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_Volcano_GroundRock");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Volcano_GroundRock"), TEXT("Layer_VolcanoStage"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_Volcano_Island01");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Volcano_Island01"), TEXT("Layer_VolcanoStage"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_Volcano_Island02");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Volcano_Island02"), TEXT("Layer_VolcanoStage"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_Volcano_Lava_Pool");

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Volcano_Lava_Pool"), TEXT("Layer_VolcanoStage"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_Volcano_Lava_Fall");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Volcano_Lava_Fall"), TEXT("Layer_VolcanoStage"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_Volcano_Lava_Ground");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Volcano_Lava_Ground"), TEXT("Layer_VolcanoStage"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_Volcano_SkyCloud");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Volcano_SkyCloud"), TEXT("Layer_VolcanoStage"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_Volcano_Smoke");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Volcano_Smoke"), TEXT("Layer_VolcanoStage"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_Volcano_Mountain");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Volcano_Mountain"), TEXT("Layer_VolcanoStage"), &tDesc)))
		return E_FAIL;
	tDesc.m_PrototypeKey = TEXT("Prototype_GameObject_Volcano_Destructive");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Volcano_Destructive"), TEXT("Layer_VolcanoStage"), &tDesc)))
		return E_FAIL;
	
	return S_OK;
}

void CLevel_GamePlay::Create_Effect_Manager()
{
	m_pEffect_Manager = CEffect_Manager::Get_Instance();
	m_pEffect_Manager->Initialize(m_pDevice, m_pContext);
}

void CLevel_GamePlay::Create_QTE_Manager()
{
	m_pQTE_Manager = CQTE_Manager::Get_Instance();
	m_pQTE_Manager->Initialize(m_pDevice, m_pContext);
}

HRESULT CLevel_GamePlay::Loading_For_Effect()
{
	vector<EFFECT_LAYER_DATA>* pLoaded = static_cast<vector<EFFECT_LAYER_DATA>*>(m_pGameInstance->Load_All_Effects());

	return CEffect_Manager::Get_Instance()->Set_Saved_Effects(pLoaded);
}

HRESULT CLevel_GamePlay::Ready_Character()
{
	CCharacter::Character_DESC SlotDesc = {};

	//플레이어 생성
	SlotDesc.ePlayerSlot = CUI_Define::LPLAYER1;
	SlotDesc.fSpeedPerSec = 5.f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Play_Goku"), TEXT("Layer_Character"), &SlotDesc)))
		return E_FAIL;

	//SlotDesc.ePlayerSlot = CUI_Define::LPLAYER2;
	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Play_21"), TEXT("Layer_Character"), &SlotDesc)))
	//	return E_FAIL;



	//2팀

	SlotDesc.ePlayerSlot = CUI_Define::RPLAYER1;
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
	Safe_Release(m_pEffect_Manager);

	Safe_Release(m_pQTE_Manager);
	Safe_Release(m_pUI_Manager);
	Safe_Release(m_pMap_Manager);
}
