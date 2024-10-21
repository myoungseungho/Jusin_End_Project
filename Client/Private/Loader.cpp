 #include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "IMGUI_Shader_Tab.h"
#include "Camera_Free.h"

//#include "Monster.h"
//#include "Player.h"
//#include "SubPlayer.h"
#include "Model_Preview.h"

#include "Monster.h"
#include "Effect_Rect.h"
#include "Shader_Texture.h"
#include "Effect_MoveTex.h"
#include "Effect_Multi.h"
#include "Effect_Single.h"
#include "SpaceSky.h"
#include "Player.h"
#include "Effect.h"


#include "SpaceSun.h"
#include "SpaceRock.h"
#include "SpaceMoon.h"
#include "SpaceStone.h"
#include "SpaceCliff.h"
#include "SpaceEarth.h"
#include "SpaceStage.h"
#include "SpaceGround.h"
#include "SpaceHorizon.h"
#include "SpaceEarth_Light.h"
#include "FallingStar.h"
//#include "Monster.h"
//#include "Terrain.h"
//#include "Camera.h"
//#include "Effect.h"
//#include "Sky.h"


//LOGO
#include "BackGround.h"
#include "UI_Logo.h"

//UI 헤더
#include "UI_Cursor.h"
#include "UI_HpPanel.h"
#include "UI_HpGauge.h"
#include "UI_SubHpGauge.h"
#include "UI_HpEffect.h"
#include "UI_Chara_Icon_Panel.h"
#include "UI_Sub_Chara_Icon_Panel.h"
#include "UI_Chara_Icon.h"
#include "UI_AttBuf.h"
#include "UI_AttBufEffect.h"
#include "UI_AttBufThunderEffect.h"
#include "UI_AttBufNone.h"
#include "UI_AttBufMark.h"
#include "UI_SkillGauge.h"
#include "UI_SkillGaugeBar.h"
#include "UI_SkillNumber.h"
#include "UI_SkillEffect.h"
#include "UI_Timer.h"
#include "UI_TimerPanel.h"
#include "UI_ComboNumber.h"
#include "UI_ComboFont.h"
#include "UI_ComboEffect.h"
#include "UI_GameStartCircle.h"
#include "UI_StartEmblem.h"
#include "UI_ReadyFont.h"
#include "UI_FightFont.h"
#include "UI_KOFont.h"
#include "UI_KOFontEffect.h"
#include "UI_Chara_SubIcon.h"
#include "UI_InputDir.h"
#include "UI_Input_DirPanel.h"
#include "UI_Input_Action.h"
#include "UI_InputDirIcon.h"
#include "UI_Input_ActionIcon.h"
#include "UI_InputDirEffect.h"
#include "UI_Input_IconPanel.h"
#include "UI_FontName.h"
#include "UI_KOPanel.h"
#include "UI_KOParticle.h"

#include "Character.h"
#include "Play_Goku.h"
#include "Play_21.h"


#include "AttackObject.h"
#include "Model_Preview.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance{ CGameInstance::Get_Instance()}
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

/* 이 함수를 호출한 스레드는 어떤 역활? 리소스를 로드한다. 어떤 레벨용? */
_uint APIENTRY LoadingMain(void* pArg)
{
	CoInitializeEx(nullptr, 0);

	CLoader*		pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;
}

HRESULT CLoader::Initialize(LEVELID eNextLevelID)
{
	/* 어떤 레벨에 대한 준비를 해야하는지 */
	m_eLevelID = eNextLevelID;

	m_pGameInstance->Set_LoadingLevel_Index(m_eLevelID);

	InitializeCriticalSection(&m_Critical_Section);

	/* 정해준 레벨에 자원(텍스쳐, 사운드, 모델 등등)을 준비한다. */
	/* 새로운 스레드를 만들어서 준비한다. 기존 내 코드를 수행하던 메인스레드는 로딩 화면(Update, Render) 에 대한 처리를 한다. */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;
	
	return S_OK;
}

/* 추가적으로 생성한 스레드가 호출한 함수. */
HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_Critical_Section);

	HRESULT		hr{};

	m_isFinished = false;

	switch (m_eLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_For_Logo();
		break;

	case LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlayLevel();
		break;

	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_Critical_Section);

	return S_OK;
}


HRESULT CLoader::Loading_For_Logo()
{

	/* For.Prototype_Component_Texture_Logo */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/E3_Title/tex/E3_Title_BG01.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Logo_Mark */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo_Mark"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/E3_Title/tex/LOC/E3_Title_Logo.png")))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Logo"),
		CUI_Logo::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	m_isFinished = true;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlayLevel()
{
	/* 게임플레이레벨용 자원을 로드하자. */

	Loading_For_UI();

	/* 텍스쳐를 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));

	/* 컴포넌트를 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("컴포넌트를 로딩 중 입니다."));

	///* For.Prototype_Component_Collider_AABB */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
	//	CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
	//	return E_FAIL;
	///* For.Prototype_Component_Collider_OBB */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
	//	CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
	//	return E_FAIL;
	///* For.Prototype_Component_Collider_Sphere */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
	//	CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
	//	return E_FAIL;

	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델(정점 -> 폴리곤 -> 메시 -> 모델)을 로딩 중 입니다."));	
	
	_matrix			PreTransformMatrix = XMMatrixIdentity();

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	//손오공 오프닝
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Goku_Opening.bin", PreTransformMatrix))))
	//	return E_FAIL;


	//손오공
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Play_Goku"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Goku_SS1.bin", PreTransformMatrix))))
			return E_FAIL;

	//부우
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Bou.bin", PreTransformMatrix))))
	//	return E_FAIL;

	


	//히트 선택화면
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Hit_Select.bin", PreTransformMatrix))))
	//	return E_FAIL;


	//PreTransformMatrix = PreTransformMatrix * XMMatrixRotationX(XMConvertToRadians(180.0f));  //이것도 방향 이상함
	PreTransformMatrix = PreTransformMatrix * XMMatrixRotationX(XMConvertToRadians(180.0f));


	//손오공 3스킬
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Goku_SS3.bin", PreTransformMatrix))))
	//	return E_FAIL;



	//히트
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Hit.bin", PreTransformMatrix))))
	//	return E_FAIL;

	//21호
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Play_21"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Ton.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxSpace */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxSpace"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxSpace.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxShaderRect"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxShaderRect.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_SpaceSky_Diffuse"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Space/SpaceSky/sp_Space.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_SpaceSky_Star1"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Space/SpaceSky/sp_star01.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_SpaceSky_Star2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Space/SpaceSky/sp_star02.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Space_FallingStar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Space/FallingStar/sp_SStar.png"), 1))))
		return E_FAIL;

#pragma region SpaceGround_Resouces
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Space_Ground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Space/Ground/sp_ground.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Space_Ground_Cliff"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Space/Ground/sp_groundcliff.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Space_Ground_Pattern"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Space/Ground/sp_ground_pattern.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Space_Ground_Shadow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Space/Ground/sp_ground_shadow.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Space_Crater"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Space/Ground/sp_crater.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SpaceGround"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Map/Space/Ground/sp_Ground.bin", PreTransformMatrix))))
		return E_FAIL;
#pragma endregion

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Space_Stage"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Space/BRGround/sp_BRGround.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SpaceStage"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Map/Space/BRGround/BRGround.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Space_Cliff"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Space/Cliff/sp_cliff.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SpaceCliff"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Map/Space/Cliff/Cliff.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Space_Horizon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Space/Horizon/sp_horizon_light.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SpaceHorizon"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Map/Space/Horizon/Horizon.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SpaceSun"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Map/Space/Sun/Sun.bin", PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SpaceRainbow"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Map/Space/Sun/Rainbow.bin", PreTransformMatrix))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Space_Light"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Space/Sun/sp_light.png"), 1))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Space_Rainbow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Space/Sun/sp_rainbow.png"), 1))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SpaceStone"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Map/Space/Stone/Stone.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Ton.bin", PreTransformMatrix))))
		return E_FAIL;

#pragma region 선택화면 주석
	//21호 선택화면
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Ton_Select.bin", PreTransformMatrix))))
	//	return E_FAIL;

	//부우 선택화면
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Bou_Select.bin", PreTransformMatrix))))
	//	return E_FAIL;

	//오공 선택화면
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Goku_Select.bin", PreTransformMatrix))))
	//	return E_FAIL;
#pragma endregion

	


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SpaceSky"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Map/Space/SpaceSky/SpaceSky.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_FallingStar"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Map/Space/FallingStar/FallingStar.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SpaceEarth"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Map/Space/Earth/Earth.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SpaceMoon"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Map/Space/Moon/Moon.bin", PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SpaceRock_1"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Map/Space/Rock_1/Rock_1.bin", PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SpaceRock_2"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Map/Space/Rock_2/Rock_2.bin", PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SpaceRock_3"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Map/Space/Rock_3/Rock_3.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SpaceMeteo_1"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Map/Space/Meteo_1/Meteo_1.bin", PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SpaceMeteo_2"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Map/Space/Meteo_2/Meteo_2.bin", PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SpaceMeteo_3"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Map/Space/Meteo_3/Meteo_3.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Space_Earth_Diffuse"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Space/Earth/sp_earth.png"), 1))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Space_Earth_Light"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Space/Earth/Earth_Light.png"), 1))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Space_Earth_Shadow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Space/Earth/Earth_Shadow.png"), 1))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Space_Earth_Cloud"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Space/Earth/Earth_Cloud_%d.png"), 3))))
		return E_FAIL;

#pragma region Effect Model

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0101"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0101.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0102"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0102.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0103"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0103.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0104"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0104.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0105"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0105.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0106"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0106.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0107"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0107.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0108"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0108.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0109"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0109.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0110"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0110.bin", PreTransformMatrix))))
		return E_FAIL;

	//10

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0111"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0111.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0112"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0112.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0201"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0201.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0202"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0202.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0203"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0203.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0204"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0204.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0205"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0205.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0206"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0206.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0301"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0301.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0302"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0302.bin", PreTransformMatrix))))
		return E_FAIL;

	//20

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0303"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0303.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0304"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0304.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0305"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0305.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0306"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0306.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0307"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0307.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0308"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0308.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0309"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0309.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0310"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0310.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0311"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0311.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels0312"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/TestModel/cmn_SmokeModels0312.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_acmn_pivot_plane01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_pivot_plane01.bin", PreTransformMatrix)))) //lying
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_acmn_povot_plane00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_povot_plane00.bin", PreTransformMatrix)))) // stand
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f);

   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun_cookie"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun_cookie.bin", PreTransformMatrix))))
		return E_FAIL;
 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun201_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun201_eff00.bin", PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun203_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun203_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun204_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun204_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun231_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun231_eff00.bin", PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun232_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun232_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun261_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun261_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun301_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun301_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun403_BindLoop"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun403_BindLoop.bin", PreTransformMatrix))))
		return E_FAIL;
	 
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun403_BindStart"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun403_BindStart.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun403_jizoku"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun403_jizoku.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_AuraBall"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_AuraBall.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_BeamCore"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_BeamCore.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_BeamCorePattern"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_BeamCorePattern.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_bodyaura00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_bodyaura00.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_crossplane00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_crossplane00.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_crossplane01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_crossplane01.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_crossplane02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_crossplane02.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_cylinder00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_cylinder00.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_cylinder01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_cylinder01.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_cylinder01_ura"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_cylinder01_ura.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_cylinder02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_cylinder02.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_cylinder03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_cylinder03.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_cylinder04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_cylinder04.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_cylinderBeam00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_cylinderBeam00.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_cylinderBeam01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_cylinderBeam01.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_futtobiBGpoint"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_futtobiBGpoint.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_futtobiBGtemprate"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_futtobiBGtemprate.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_GrassModel"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_GrassModel.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_halfsphere00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_halfsphere00.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_halfsphere01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_halfsphere01.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_hibana00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_hibana00.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_kakusanaura"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_kakusanaura.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_kidan00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_kidan00.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_KidanTrail"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_KidanTrail.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_puffycylinder"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_puffycylinder.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_puffyplane"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_puffyplane.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_puffyplane_harf"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_puffyplane_harf.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_puffyplane2"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_puffyplane2.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_rock00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_rock00.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_rock01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_rock01.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_rock02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_rock02.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_rock04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_rock04.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_rock05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_rock05.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_rubble01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_rubble01.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_saiyanAura"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_saiyanAura.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_screw"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_screw.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_screw2"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_screw2.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_screw3"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_screw3.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_shenlongAura00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_shenlongAura00.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_shenlongAura01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_shenlongAura01.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_shenlongAura02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_shenlongAura02.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_sphere00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_sphere00.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_sphere00_inv"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_sphere00_inv.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_sphere00_inv2"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_sphere00_inv2.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_stonemodel00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_stonemodel00.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_strike00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_strike00.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_TestStrike"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_TestStrike.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_WindModel00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_WindModel00.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks_cookie"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks_cookie.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks262_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks262_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks262_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks262_eff01.bin", PreTransformMatrix))))
		return E_FAIL;
 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks262_eff10"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks262_eff10.bin", PreTransformMatrix))))
		return E_FAIL;
 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks301_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks301_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks404_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks404_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks404_eff00_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks404_eff00_sub.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks404_eff00_sub2"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks404_eff00_sub2.bin", PreTransformMatrix))))
		return E_FAIL; 
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks406_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks406_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks406_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks406_eff01.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks406_eff02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks406_eff02.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks406_eff03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks406_eff03.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks430_tameaura_00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks430_tameaura_00.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks431_kaijo"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks431_kaijo.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks432_coremodel"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks432_coremodel.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks432_coremodel2"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks432_coremodel2.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks806_sphere00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks806_sphere00.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks808_Sphere"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks808_Sphere.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks808_Sphere_inv"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks808_Sphere_inv.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks814_fistaura"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks814_fistaura.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn_cookie"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn_cookie.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn_halfsphere01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn_halfsphere01.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn_plane01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn_plane01.bin", PreTransformMatrix))))
		return E_FAIL; 
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn_plane03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn_plane03.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn301_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn301_eff01.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn403_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn403_eff01.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn403_eff01_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn403_eff01_sub.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn405_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn405_eff01.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn405_eff01_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn405_eff01_sub.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn406_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn406_eff01.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn406_eff01_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn406_eff01_sub.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn407_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn407_eff01.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn407_eff02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn407_eff02.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn408_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn408_eff01.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn408_eff02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn408_eff02.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn432_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn432_eff01.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn432_eff01_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn432_eff01_sub.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_a01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_a01.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_a02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_a02.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_a03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_a03.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_a04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_a04.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_b01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_b01.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_b02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_b02.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_b03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_b03.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_b03_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_b03_sub.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_b04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_b04.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_b04_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_b04_sub.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_c01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_c01.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_c01_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_c01_sub.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_c02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_c02.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_c02_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_c02_sub.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_c03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_c03.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_c04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_c04.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_d01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_d01.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_d01_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_d01_sub.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_d02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_d02.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_d02_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_d02_sub.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_d03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_d03.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_d04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_d04.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton_cookie"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton_cookie.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton_kidan01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton_kidan01.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton201_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton201_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton204_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton204_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton205_eff1"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton205_eff1.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton231_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton231_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton232_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton232_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton301_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton301_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton401_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton401_eff00.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton401_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton401_eff01.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton401_eff02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton401_eff02.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton401_eff03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton401_eff03.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton401_eff04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton401_eff04.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton430_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton430_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton431_beam"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton431_beam.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton431_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton431_eff01.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton432_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton432_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton433_sphere01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton433_sphere01.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_ryuhai00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_ryuhai00.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_homingshere"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_homingshere.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_guardspark00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_guardspark00.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_dbfly"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_dbfly.bin", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_circle00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_circle00.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_burstCylinder01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_burstCylinder01.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_burstCylinder00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_burstCylinder00.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_AuraDash01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_AuraDash01.bin", PreTransformMatrix))))
		return E_FAIL;
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_AuraDash00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_AuraDash00.bin", PreTransformMatrix))))
		return E_FAIL;

#pragma endregion

#pragma region Effect Texture

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke06_dds"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/TextureDDS/cmn_smoke06.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_Default_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/Default_Mask.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_Default_Diffuse"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/Default_Diffuse.png"), 1))))
		return E_FAIL;
	//디폴트

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_bun_402smoke"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/bun_402smoke.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_bun_431tunder00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/bun_431tunder00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_bun_cookie"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/bun_cookie.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_bun431_cseff"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/bun431_cseff.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura_end"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura_end.png"), 1))))
		return E_FAIL;
	// 5

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura02.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura03.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura03_end"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura03_end.png"), 1))))
		return E_FAIL;
	//10

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura03_middle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura03_middle.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura03_small"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura03_small.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura03color00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura03color00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura05"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura05.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura06"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura06.png"), 1))))
		return E_FAIL;
	// 15

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura07"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura07.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura08"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura08.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura09"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura09.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura10"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura10.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura11_gray"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura11_gray.png"), 1))))
		return E_FAIL;
	//20

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura12_blue"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura12_blue.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura12_gray"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura12_gray.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura12_orange"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura12_orange.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura13"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura13.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura14"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura14.png"), 1))))
		return E_FAIL;
	// 25

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_auradust"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_auradust.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_beamscroll00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_beamscroll00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_BG_earth"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_BG_earth.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_BG_EarthCloud"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_BG_EarthCloud.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_BG_EarthCloud01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_BG_EarthCloud01.png"), 1))))
		return E_FAIL;
	//30

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_BG_EarthCloud02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_BG_EarthCloud02.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_BG_Space"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_BG_Space.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_BG_star01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_BG_star01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_BG_star02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_BG_star02.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_bomb00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_bomb00.png"), 1))))
		return E_FAIL;
	// 35

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_bomb01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_bomb01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_bomb02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_bomb02.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_bomb03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_bomb03.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_bomb04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_bomb04.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst00.png"), 1))))
		return E_FAIL;
	//40

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst01_harf"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst01_harf.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst02.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst03.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst04.png"), 1))))
		return E_FAIL;
	//45

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst05"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst05.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst05_hi"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst05_hi.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst06"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst06.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst07"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst07.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst08"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst08.png"), 1))))
		return E_FAIL;
	//50

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_circle00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_circle00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_bun_cmn_circle01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_circle01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_dis00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_dis00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_distortionA"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_distortionA.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_distortionB"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_distortionB.png"), 1))))
		return E_FAIL;
	// 55

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_distortionC"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_distortionC.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_dust00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_dust00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_dust01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_dust01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_Fractal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_Fractal.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton409_ray00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton409_ray00.png"), 1))))
		return E_FAIL;
	//60

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_Fractal2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_Fractal2.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_Fractal3"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_Fractal3.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_Fractal3_gray"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_Fractal3_gray.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_Fractal4"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_Fractal4.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_Fractal5"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_Fractal5.png"), 1))))
		return E_FAIL;
	// 65

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_Fractal6"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_Fractal6.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_glass00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_glass00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_glass02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_glass02.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_gradation_blue"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_gradation_blue.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_gradation_orange"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_gradation_orange.png"), 1))))
		return E_FAIL;
	//70

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_gradation_orange2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_gradation_orange2.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_gradation00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_gradation00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_gradation01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_gradation01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_gradation02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_gradation02.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_gradation03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_gradation03.png"), 1))))
		return E_FAIL;
	// 75

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_grass"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_grass.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_guard00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_guard00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_guard01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_guard01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_guard02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_guard02.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_hibana00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_hibana00.png"), 1))))
		return E_FAIL;
	//80

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_hit00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_hit00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_human00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_human00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_ilm00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_ilm00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_jizokusmoke00end"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_jizokusmoke00end.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_jizokusmoke00end2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_jizokusmoke00end2.png"), 1))))
		return E_FAIL;
	// 85

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_jizokusmoke00loop"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_jizokusmoke00loop.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_jizokusmoke00start"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_jizokusmoke00start.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_kidan00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_kidan00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_kira00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_kira00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_kira01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_kira01.png"), 1))))
		return E_FAIL;
	//90

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_kmhmFractal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_kmhmFractal.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_line00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_line00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_line01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_line01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_line02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_line02.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_line03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_line03.png"), 1))))
		return E_FAIL;
	// 95

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_line04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_line04.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_line04gray"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_line04gray.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_mask00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_mask00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_mask01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_mask01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_mask02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_mask02.png"), 1))))
		return E_FAIL;
	//100

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_mazzle00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_mazzle00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_mazzle01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_mazzle01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_mazzle02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_mazzle02.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_pattern00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_pattern00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_pattern01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_pattern01.png"), 1))))
		return E_FAIL;
	// 5

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_pattern02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_pattern02.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_ray00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_ray00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_ray00new"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_ray00new.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_ray01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_ray01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_rock_PlanetBreak"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_rock_PlanetBreak.png"), 1))))
		return E_FAIL;
	//10

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_rock_smalls_def"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_rock_smalls_def.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_rock_smalls_space"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_rock_smalls_space.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_rock00_def"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_rock00_def.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_rock01_def"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_rock01_def.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_rock02_def"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_rock02_def.png"), 1))))
		return E_FAIL;
	// 15

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_samne00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_samne00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_samne01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_samne01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_samne02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_samne02.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_scrline00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_scrline00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_scrline01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_scrline01.png"), 1))))
		return E_FAIL;
	//20

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_scrline02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_scrline02.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_scrollsmoke"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_scrollsmoke.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_slash00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_slash00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke_scroll"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke_scroll.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke_scroll2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke_scroll2.png"), 1))))
		return E_FAIL;
	// 25

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke02.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke02_blur"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke02_blur.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke03.png"), 1))))
		return E_FAIL;
	//30

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke04.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke04b"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke04b.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke05"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke05.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke06"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke06.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke07"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke07.png"), 1))))
		return E_FAIL;
	// 35

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke08"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke08.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smokemodel00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smokemodel00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_spark00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_spark00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_spark01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_spark01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_spark02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_spark02.png"), 1))))
		return E_FAIL;
	//40

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_sphere00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_sphere00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_sphere01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_sphere01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_sss00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_sss00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_stoneanim00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_stoneanim00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_stoneanim00_gray"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_stoneanim00_gray.png"), 1))))
		return E_FAIL;
	//45

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_stoneanim01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_stoneanim01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_stoneanim01gray"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_stoneanim01gray.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_stonemodel00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_stonemodel00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_strike00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_strike00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_strike01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_strike01.png"), 1))))
		return E_FAIL;
	//50

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_strike02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_strike02.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_test"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_test.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_thunder00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_thunder00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_thunder01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_thunder01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_thunder02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_thunder02.png"), 1))))
		return E_FAIL;
	// 55

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_thunder02yellow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_thunder02yellow.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_tigire00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_tigire00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_TinkleLight00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_TinkleLight00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_trail_mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_trail_mask.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_trail00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_trail00.png"), 1))))
		return E_FAIL;
	//60

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_trail02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_trail02.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_trail04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_trail04.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_water00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_water00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_water01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_water01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_watersplash00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_watersplash00.png"), 1))))
		return E_FAIL;
	// 65

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_watersplash01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_watersplash01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_wave00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_wave00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_white"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_white.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_wind01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_wind01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_thunder01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_thunder01.png"), 1))))
		return E_FAIL;
	//70

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_wipe00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_wipe00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_wave01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_wave01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_wipe01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_wipe01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmnsmokemodel00color"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmnsmokemodel00color.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmnsmokemodel02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmnsmokemodel02.png"), 1))))
		return E_FAIL;
	// 75

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmnsmokemodel02color"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmnsmokemodel02color.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmnsmokemodel03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmnsmokemodel03.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmnsmokemodel03color"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmnsmokemodel03color.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_GKS_cookie"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/GKS_cookie.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_GKS600_cseff"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/GKS600_cseff.png"), 1))))
		return E_FAIL;
	//80

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_GKS600_cseff2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/GKS600_cseff2.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_GKS805_cseff"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/GKS805_cseff.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_GKS808_changeScr"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/GKS808_changeScr.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_GKS808_changeScr2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/GKS808_changeScr2.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_gks814_scrwipe"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/gks814_scrwipe.png"), 1))))
		return E_FAIL;
	// 85

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_htn_burst01_middle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/htn_burst01_middle.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_htn_burst01_smal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/htn_burst01_smal.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_HTN_cookie"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/HTN_cookie.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_htn_gradation01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/htn_gradation01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_htn_gradation02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/htn_gradation02.png"), 1))))
		return E_FAIL;
	//90

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_htn_hibi01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/htn_hibi01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_htn407_hibi01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/htn407_hibi01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_htn407_hibi02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/htn407_hibi02.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_htn407_hibi03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/htn407_hibi03.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_htn431_hibi01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/htn431_hibi01.png"), 1))))
		return E_FAIL;
	// 95

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_htn431_hibi02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/htn431_hibi02.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_sp_EarthCloud"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/sp_EarthCloud.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_sp_EarthCloud01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/sp_EarthCloud01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_sp_EarthCloud02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/sp_EarthCloud02.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_sp_star01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/sp_star01.png"), 1))))
		return E_FAIL;
	//200

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_sp_star02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/sp_star02.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_431kidou"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_431kidou.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_431tunder00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_431tunder00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_beam_mask_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_beam_mask_01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_cookie"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_cookie.png"), 1))))
		return E_FAIL;
	// 205

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_gradation_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_gradation_01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_kmhmFractal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_kmhmFractal.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_line00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_line00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_mask_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_mask_01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_slash01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_slash01.png"), 1))))
		return E_FAIL;
	//210

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_spark01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_spark01.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_thunder00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_thunder00.png"), 1))))
		return E_FAIL;
	//212

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_T_Spark_Core"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/T_Spark_Core.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_T_Smoke_Tiled_D"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/T_Smoke_Tiled_D.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_T_Smoke_SubUV"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/T_Smoke_SubUV.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_T_Fire_Tiled_D"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/T_Fire_Tiled_D.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_T_Fire_SubUV"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/T_Fire_SubUV.png"), 1))))
		return E_FAIL;
	// 217

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_T_Explosion_SubUV"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/T_Explosion_SubUV.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_T_Dust_Particle_D"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/T_Dust_Particle_D.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_T_Burst_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/T_Burst_M.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_T_Water_N"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/T_Water_N.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_homingB_shadow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_homingB_shadow.png"), 1))))
		return E_FAIL;
	//222

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_homingB"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_homingB.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_homingAura_stop"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_homingAura_stop.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_homingA"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_homingA.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_hit02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_hit02.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_hit01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_hit01.png"), 1))))
		return E_FAIL;
	// 227

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_gradation"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_gradation.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_forcedchange"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_forcedchange.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_dbfly"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_dbfly.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura11"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura11.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_scrRock01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_scrRock01.png"), 1))))
		return E_FAIL;
	//232

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_scrRock00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_scrRock00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_ryuhai00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_ryuhai00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_number00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_number00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_homingBair_shadow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_homingBair_shadow.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_homingBair"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_homingBair.png"), 1))))
		return E_FAIL;
	// 237

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_wind00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_wind00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_scrspark00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_scrspark00.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_scrsmoke00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_scrsmoke00.png"), 1))))
		return E_FAIL;

	//240


#pragma endregion

#pragma region Effect Object
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_MoveTex"),
		CEffect_MoveTex::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Multi"),
		CEffect_Multi::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Single"),
		CEffect_Single::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

#pragma region 주석 리소스 파일

	//손오공 오프닝
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Goku_Opening.bin", PreTransformMatrix))))
	//	return E_FAIL;


	//손오공
	/*if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Goku_SS1.bin", PreTransformMatrix))))
		return E_FAIL;*/


		//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Play_Goku"),
		//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Goku_SS1.bin", PreTransformMatrix))))
		//	return E_FAIL;

		//부우
		//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
		//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Bou.bin", PreTransformMatrix))))
		//	return E_FAIL;


		//히트 선택화면
		//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
		//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Hit_Select.bin", PreTransformMatrix))))
		//	return E_FAIL;


		//PreTransformMatrix = PreTransformMatrix * XMMatrixRotationX(XMConvertToRadians(180.0f));  //이것도 방향 이상함
	PreTransformMatrix = PreTransformMatrix * XMMatrixRotationX(XMConvertToRadians(180.0f));


	//손오공 3스킬
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Goku_SS3.bin", PreTransformMatrix))))
	//	return E_FAIL;



	//히트
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Hit.bin", PreTransformMatrix))))
	//	return E_FAIL;

	//21호
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Play_21"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Ton.bin", PreTransformMatrix))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Ton.bin", PreTransformMatrix))))
	//	return E_FAIL;



	//21호 선택화면
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Ton_Select.bin", PreTransformMatrix))))
	//	return E_FAIL;

	//부우 선택화면
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Bou_Select.bin", PreTransformMatrix))))
	//	return E_FAIL;

	//오공 선택화면
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Goku_Select.bin", PreTransformMatrix))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Goku_SS3.bin", PreTransformMatrix))))
	//	return E_FAIL;
#pragma endregion
	

	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Model_Preview"),
		CModel_Preview::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Play_Goku"),
		CPlay_Goku::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Play_21"),
		CPlay_21::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Attack"),
		CAttacKObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SpaceRock"),
		CSpaceRock::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SpaceStone"),
		CSpaceStone::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SpaceMoon"),
		CSpaceMoon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SpaceSun"),
		CSpaceSun::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SpaceHorizon"),
		CSpaceHorizon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SpaceCliff"),
		CSpaceCliff::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SpaceStage"),
		CSpaceStage::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SpaceGround"),
		CSpaceGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SpaceEarth_Light"),
		CSpaceEarth_Light::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SpaceEarth"),
		CSpaceEarth::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FallingStar"),
		CFallingStar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SpaceSky"),
		CSpaceSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Shader_Texture"),
		CShader_Texture::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Rect"),
		CEffect_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"),
		CEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_UI()
{
	//텍스처 컴포넌트

	//커서

	//Combo
	/* For.Prototype_Component_Texture_UI_ComboNumber */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ComboNumber"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/Left/Number/cp_combo_count_red_%d.png"),10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_ComboFont */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ComboFont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/Left/cp_combo_hit_red.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_ComboEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ComboEffect"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/Left/cp_combo_eff.png")))))
		return E_FAIL;

	//HP

	/* For.Prototype_Component_Texture_UI_HpPanel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HpPanel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/cp_gauge_physical_frame01_S3.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_HpGauge*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HpGauge"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/HpGauge.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_HpGaugeAnimMask*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HpGaugeAnimMask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/cp_lifegauge_animmask.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SubHp*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SubHp"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/cp_gauge_physical_frame01.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_HpEffect*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HpEffect"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/cp_lifegauge_base.png")))))
		return E_FAIL;


	//시간 초

	/* For.Prototype_Component_Texture_UI_Timer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Timer"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/TimerNumber/TimerNum%d.png"), 11))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_UI_Time_Panel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_TimerPanel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/cp_gauge_Timebase.png")))))
		return E_FAIL;

	//스킬 게이지

	/* For.Prototype_Component_Texture_UI_SKillGauge */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SKillGauge"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/cp_tensiontex_base00_S3.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SKillGaugeBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SKillGaugeBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/Cp_Tensiontex/cp_tensiontex_0%d.png"),8))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SPNum */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SPNum"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/SP_Number/SP_Num%d.png"), 8))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SkillFlowEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SkillFlowEffect"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/cp_lifegauge_animmask_S3.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SkillEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SkillEffect"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/cp_tension_eff.png")))))
		return E_FAIL;

	//캐릭터 아이콘

	/* For.Prototype_Component_Texture_UI_CharaIconPanel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_CharaIconPanel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame//Top/Cp_CharaIconPanel.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_CharaIconPanel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SubCharaIconPanel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/SubIconPanel%d.png"),2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_CharIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_CharaIcon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Chara_Image//Face_A/Chara_Icon/CharaIcon%d.png"), 4))))
		return E_FAIL;

	//공격 버프

	/* For.Prototype_Component_Texture_UI_AttBufNone */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_AttBufNone"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/AttBufEffect/cp_spicon_none.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_AttBuf */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_AttBuf"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/AttBufEffect/cp_spicon0%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_AttBufEff */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_AttBufEff"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/AttBufEffect/cp_spicon_eff0%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_AttBufMark */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_AttBufMark"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/AttBufEffect/cp_spicon_S.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_AttBufElectricEff */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_AttBufElectricEff"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/AttBufEffect/ElectricEff/cp_spicon_ElectricEff%d.png"), 6))))
		return E_FAIL;

	//게임 스타트 
	/* For.Prototype_Component_Texture_UI_GameStartCircle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_GameStartCircle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/Middle/GameStart/GameStart%d.png"), 8))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_GameStartEmblem */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_GameStartEmblem"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/Middle/GameStart/Emblem%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_GameReadyFont */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_GameReadyFont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/Middle/Font/Ready%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_GameFightFont */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_GameFightFont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/Middle/Font/Fight%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_GameKOFont */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_GameKOFont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/Middle/Font/BP_roundfinish_KO.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_GameKOFontEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_GameKOFontEffect"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/Middle/Font/BP_roundfinish_KO_eff.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_KOPanel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_KOPanel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/BP_roundfinish_eff0%d.png"),7))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_KOParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_KOParticle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/BP_roundfinish_KO_eff_sumi.png")))))
		return E_FAIL;

	//KeyInput 

	/* For.Prototype_Component_Texture_UI_DirInputPanel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_DirInputPanel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/KeyInputPanel.png")))))
		return E_FAIL;

	/* Prototype_Component_Texture_UI_ActionInputPanel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ActionInput"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/KeyInputButton%d.png"),7))))
		return E_FAIL;

	/* Prototype_Component_Texture_UI_DirInputIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_DirInputIcon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/UI_DirInput%d.png"), 9))))
		return E_FAIL;


	/* Prototype_Component_Texture_UI_InputPanel*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_InputPanel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/TON_Icon_none.png")))))
		return E_FAIL;

	/* Prototype_Component_Texture_UI_DirKeyInputEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_DirKeyInputEffect"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/window_arrow_key_%d.png"),2))))
		return E_FAIL;

	/* Prototype_Component_Texture_UI_InputIconPanel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_InputIconPanel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/Training_BlackBG_00.png")))))
		return E_FAIL;


	//Font

	/* Prototype_Component_Texture_UI_FontName */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_FontName"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/Chara_Name%d.png"), 4))))
		return E_FAIL;



	//게임 오브젝트

	/* For.Prototype_GameObject_Cursor */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Cursor"),
		CUI_Cursor::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_HpPanel */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HpPanel"),
		CUI_HpPanel::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_HpGauge */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HpGauge"),
		CUI_HpGauge::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_SubHpGauge */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_SubHpGauge"),
		CUI_SubHpGauge::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_HpEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HpEffect"),
		CUI_HpEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Chara_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Chara_Icon_Panel"),
		CUI_Chara_Icon_Panel::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Sub_Chara_Icon_Panel */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Sub_Chara_Icon_Panel"),
		CUI_Sub_Chara_Icon_Panel::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Chara_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Chara_Icon"),
		CUI_Chara_Icon::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_UI_Chara_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Chara_SubIcon"),
		CUI_Chara_SubIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_AttBufMark */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_AttBufMark"),
		CUI_AttBufMark::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_AttBufNone */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_AttBufNone"),
		CUI_AttBufNone::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_AttBuf */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_AttBuf"),
		CUI_AttBuf::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_AttBufEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_AttBufEffect"),
		CUI_AttBufEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_AttBufThunderEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_AttBufThunderEffect"),
		CUI_AttBufThunderEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_SkillGauge */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillGauge"),
		CUI_SkillGauge::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_SkillGaugeBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillGaugeBar"),
		CUI_SkillGaugeBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_SkillGauge */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillNumber"),
		CUI_SkillNumber::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_SkillEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillEffect"),
		CUI_SkillEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//시간 초

	/* For.Prototype_GameObject_UI_Timer */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Timer"),
		CUI_Timer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_TimerPanel */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_TimerPanel"),
		CUI_TimerPanel::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//콤보

	/* For.Prototype_GameObject_UI_ComboNumber */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_ComboNumber"),
		CUI_ComboNumber::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_ComboFont */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_ComboFont"),
		CUI_ComboFont::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_ComboEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_ComboEffect"),
		CUI_ComboEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//게임 스테이트

	/* For.Prototype_GameObject_UI_ComboEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_GameStartCircle"),
		CUI_GameStartCircle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_ReadyFont */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_ReadyFont"),
		CUI_ReadyFont::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_FightFont */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_FightFont"),
		CUI_FightFont::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_FightFont */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_StartEmblem"),
		CUI_StartEmblem::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_KOFont */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_KOFont"),
		CUI_KOFont::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_KOFontEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_KOFontEffect"),
		CUI_KOFontEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_KOPanel */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_KOPanel"),
		CUI_KOPanel::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* Prototype_GameObject_UI_KOParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_KOParticle"),
		CUI_KOParticle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_InputDir */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_InputDir"),
		CUI_InputDir::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_InputDirPanel */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_InputDirPanel"),
		CUI_Input_DirPanel::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* Prototype_GameObject_UI_InputAction */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_InputAction"),
		CUI_Input_Action::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* Prototype_GameObject_UI_DirInputIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_DirInputIcon"),
		CUI_InputDirIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* Prototype_GameObject_UI_DirInputIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_InputIconPanel"),
		CUI_Input_IconPanel::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* Prototype_GameObject_UI_ActionInputIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_ActionInputIcon"),
		CUI_Input_ActionIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* Prototype_GameObject_UI_DirInputEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_DirInputEffect"),
		CUI_InputDirEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	

	/* Prototype_GameObject_UI_FontName */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_FontName"),
		CUI_FontName::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	//UI_StartFont

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID)
{
	CLoader*		pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_Critical_Section);

	Safe_Release(m_pGameInstance);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
