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

#include "Character.h"

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
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Play_Goku"), TEXT("Layer_Character"), &CharacterDesc)))
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

	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Model_Preview"), TEXT("Layer_Model_Preview"));
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
	m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/EndLess_BGM.wav", L"SFX_BGM");
	m_pGameInstance->Play_Sound(L"SFX_BGM", true, 0.2f);

	////퀘스트 들때
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/QuestNotify.wav", L"SFX_Quest_Notify");
	////퀘스트 하나씩 완료될때
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/TransferConsole_NoticeBoxGet.wav", L"SFX_Quest_Complete");

	////아이템 먹을 때
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Item&Combination/Consume_Eat_2.wav", L"SFX_Consume");

	////박스오브젝트에서 아이템 가져올때
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Item&Combination/PickUpItem.wav", L"SFX_PickUpItem");

	////아이템 조합시
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Item&Combination/chracter_Craft_Food_1.wav", L"SFX_CraftItem");

	////아비게일 노말 공격
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Abigail/NotVoice/Abigail_Axe_NormalAttack01.wav", L"SFX_Abigail_Axe_NormalAttack01");

	////아비게일 노말 공격에 맞은 Enemy의 Hit 소리
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Abigail/NotVoice/Abigail_Axe_NormalAttack_Hit.wav", L"SFX_Abigail_Axe_NormalAttack_Hit");

	////아비게일 아스팔트 발소리
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Abigail/FootWalk/footstepAsphalt_w4.wav", L"SFX_footstepAsphalt_w4");


	////Chicken 공격
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Chicken/chickenAttack.wav", L"SFX_chickenAttack");
	////Chicken Hit
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Chicken/chickenHit.wav", L"SFX_chickenHit");
	////Chicken Die
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Chicken/chickenDie.wav", L"SFX_chickenDie");

	////Boar 공격
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Boar/boarAttack.wav", L"SFX_boarAttack");
	////Boar Hit
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Boar/boarHit.wav", L"SFX_boarHit");
	////Boar Die
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Boar/boarDie.wav", L"SFX_boarDie");
	////Boar Skill에 플레이어가 맞았을 때
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Boar/boar_Skill_Hit.wav", L"SFX_boar_Skill_Hit");

	////Wolf Hit
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Wolf/wolfHit.wav", L"SFX_wolfHit");
	////Wolf Die
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Wolf/wolfDie.wav", L"SFX_wolfDie");
	////Wolf 공격할때 짖는 소리
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Wolf/wolf_attack.wav", L"SFX_wolf_attack");
	////Wolf 공격
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Wolf/wolfAttack.wav", L"SFX_wolfAttack");
	////Wolf Hit
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Wolf/wolfHit.wav", L"SFX_wolfHit");

	////Bear Hit
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Bear/bearHit.wav", L"SFX_bearHit");
	////Bear Die
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Bear/bearDie.wav", L"SFX_bearDie");
	////Bear Attack
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Bear/bearAttack.wav", L"SFX_bearAttack");


	////Alpha Hit
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Alpha/AlphaOmega_atk_hit.wav", L"SFX_AlphaOmega_atk_hit");
	////Alpha Attack1
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Alpha/AlphaOmega_atk01.wav", L"SFX_AlphaOmega_atk01");
	////Alpha Attack2
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Alpha/AlphaOmega_atk02.wav", L"SFX_AlphaOmega_atk02");
	////Alpha Dead
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Alpha/AlphaOmega_dead.wav", L"SFX_AlphaOmega_dead");
	////Alpha Appear
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Alpha/AlphaOmega_appear.wav", L"SFX_AlphaOmega_appear");

	////Alpha SkillStart
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Alpha/AlphaOmega_skill01_Start.wav", L"SFX_AlphaOmega_skill01_Start");
	////Alpha SkillSwing
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Alpha/AlphaOmega_skill01_Swing.wav", L"SFX_AlphaOmega_skill01_Swing");

	////Alpha Appear
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Alpha/Announce_Alpha_Appear_01.wav", L"SFX_Announce_Alpha_Appear_01");

	////Hyunwoo 등장 Talk
	////그냥 싸우면 되는거지? 복잡한건 싫거든
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Hyunwoo/Hyunwoo_firstMove_3_ko.wav", L"SFX_Hyunwoo_firstMove_3_ko");
	////Hyunwoo 죽었을 때 Talk
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Hyunwoo/Hyunwoo_lost_2_ko.wav", L"SFX_Hyunwoo_lost_2_ko");

	////Hyunwoo 공격1
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Hyunwoo/NotVoice/hyunwoo_Skill01_Attack_v1.wav", L"SFX_hyunwoo_Skill01_Attack_v1");
	////Hyunwoo 차징
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Hyunwoo/NotVoice/hyunwoo_Skill04_Charging.wav", L"SFX_hyunwoo_Skill04_Charging");
	////Hyunwoo 공격
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Hyunwoo/NotVoice/hyunwoo_Skill04_Hit #731569.wav", L"SFX_hyunwoo_Skill04_Hit");
	////Hyunwoo Hit
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Enemy/Hyunwoo/NotVoice/hyunwoo_Skill03_Hit.wav", L"SFX_hyunwoo_Skill03_Hit");

	////아비게일 움직임은 그룹 등록
	//m_pGameInstance->Register_Sound_Group(L"MovementSounds", L"../Bin/SoundSDK/AudioClip/Abigail/Abigail_moveintemple_1_ko.wav", L"SFX_Move_1");
	//m_pGameInstance->Register_Sound_Group(L"MovementSounds", L"../Bin/SoundSDK/AudioClip/Abigail/Abigail_moveintemple_2_ko.wav", L"SFX_Move_2");
	//m_pGameInstance->Register_Sound_Group(L"MovementSounds", L"../Bin/SoundSDK/AudioClip/Abigail/Abigail_moveintemple_3_ko.wav", L"SFX_Move_3");
	//m_pGameInstance->Register_Sound_Group(L"MovementSounds", L"../Bin/SoundSDK/AudioClip/Abigail/Abigail_moveinuptown_1_ko.wav", L"SFX_Move_4");
	//m_pGameInstance->Register_Sound_Group(L"MovementSounds", L"../Bin/SoundSDK/AudioClip/Abigail/Abigail_moveinuptown_2_ko.wav", L"SFX_Move_5");
	//m_pGameInstance->Register_Sound_Group(L"MovementSounds", L"../Bin/SoundSDK/AudioClip/Abigail/Abigail_moveinuptown_3_ko.wav", L"SFX_Move_6");

	////아비게일 스킬 사운드 Q Start
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Abigail/NotVoice/Abigail_Skill01_Start.wav", L"SFX_Abigail_Skill01_Start");
	////아비게일 스킬 사운드 Q FirstSpinHit
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Abigail/NotVoice/Abigail_Skill01_FirstSpin_Hit.wav", L"SFX_Abigail_Skill01_FirstSpin_Hit");
	////아비게일 스킬 사운드 Q SecondSpinHit
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Abigail/NotVoice/Abigail_Skill01_SecondSpin_Hit.wav", L"SFX_Abigail_Skill01_SecondSpin_Hit");

	////아비게일 스킬 사운드 W Start
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Abigail/NotVoice/Abigail_Skill02_Start.wav", L"SFX_Abigail_Skill02_Start");
	////아비게일 스킬 사운드 W Hit
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Abigail/NotVoice/Abigail_Skill02_Hit.wav", L"SFX_Abigail_Skill02_Hit");
	////아비게일 스킬 사운드 W Target_Explode
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Abigail/NotVoice/Abigail_Skill02_Target_Explode.wav", L"SFX_Abigail_Skill02_Target_Explode");

	////아비게일 스킬 사운드 E Start
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Abigail/NotVoice/Abigail_Skill03_Start.wav", L"SFX_Abigail_Skill03_Start");
	////아비게일 스킬 사운드 E Hit
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Abigail/NotVoice/Abigail_Skill03_Hit.wav", L"SFX_Abigail_Skill03_Hit");

	////아비게일 스킬 사운드 R Start
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Abigail/NotVoice/Abigail_Skill04_Start.wav", L"SFX_Abigail_Skill04_Start");
	////아비게일 스킬 사운드 R Hit
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Abigail/NotVoice/Abigail_Skil04_Crack.wav", L"SFX_Abigail_Skill04_Hit");
	////아비게일 스킬 사운드 R SlowMotion
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/Abigail/NotVoice/Abigail_SlowMotion.wav", L"SFX_Abigail_SlowMotion");

	////빅토리
	//m_pGameInstance->Register_Sound(L"../Bin/SoundSDK/AudioClip/VictoryCutscene_BGM.wav", L"SFX_VictoryCutscene_BGM");

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
