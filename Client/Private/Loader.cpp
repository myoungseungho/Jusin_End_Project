#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "IMGUI_Shader_Tab.h"
#include "Camera_Free.h"
#include "Monster.h"
#include "Effect_Rect.h"
#include "Shader_Texture.h"
#include "Effect_NoneLight.h"
#include "Effect_Blend.h"
#include "Effect_ZNone.h"
#include "Effect_Overlap.h"
#include "SpaceSky.h"
#include "Player.h"
#include "Effect.h"

#include "SpaceSun.h"
#include "SpaceMoon.h"
#include "SpaceStone.h"
#include "SpaceCliff.h"
#include "SpaceEarth.h"
#include "SpaceStage.h"
#include "SpaceGround.h"
#include "SpaceHorizon.h"
#include "SpaceEarth_Light.h"
#include "FallingStar.h"
#include "SpaceRock.h"
//#include "Monster.h"
//#include "Terrain.h"
//#include "Camera.h"
//#include "Effect.h"
//#include "Sky.h"


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
	case LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlayLevel();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_Critical_Section);

	return S_OK;
}


HRESULT CLoader::Loading_For_GamePlayLevel()
{
	/* 게임플레이레벨용 자원을 로드하자. */

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
	
	_matrix			 
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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_Default_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/Default_Mask.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_Default_Diffuse"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/Default_Diffuse.dds"), 1))))
		return E_FAIL;
	//디폴트

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_bun_402smoke"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/bun_402smoke.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_bun_431tunder00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/bun_431tunder00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_bun_cookie"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/bun_cookie.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_bun431_cseff"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/bun431_cseff.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura_end"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura_end.dds"), 1))))
		return E_FAIL;
	// 5

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura02.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura03.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura03_end"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura03_end.dds"), 1))))
		return E_FAIL;
	//10

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura03_middle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura03_middle.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura03_small"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura03_small.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura03color00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura03color00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura05"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura05.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura06"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura06.dds"), 1))))
		return E_FAIL;
	// 15

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura07"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura07.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura08"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura08.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura09"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura09.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura10"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura10.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura11_gray"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura11_gray.dds"), 1))))
		return E_FAIL;
	//20

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura12_blue"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura12_blue.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura12_gray"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura12_gray.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura12_orange"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura12_orange.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura13"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura13.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura14"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura14.dds"), 1))))
		return E_FAIL;
	// 25

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_auradust"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_auradust.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_beamscroll00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_beamscroll00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_BG_earth"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_BG_earth.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_BG_EarthCloud"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_BG_EarthCloud.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_BG_EarthCloud01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_BG_EarthCloud01.dds"), 1))))
		return E_FAIL;
	//30

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_BG_EarthCloud02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_BG_EarthCloud02.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_BG_Space"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_BG_Space.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_BG_star01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_BG_star01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_BG_star02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_BG_star02.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_bomb00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_bomb00.dds"), 1))))
		return E_FAIL;
	// 35

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_bomb01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_bomb01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_bomb02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_bomb02.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_bomb03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_bomb03.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_bomb04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_bomb04.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst00.dds"), 1))))
		return E_FAIL;
	//40

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst01_harf"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst01_harf.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst02.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst03.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst04.dds"), 1))))
		return E_FAIL;
	//45

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst05"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst05.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst05_hi"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst05_hi.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst06"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst06.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst07"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst07.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_burst08"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_burst08.dds"), 1))))
		return E_FAIL;
	//50

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_circle00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_circle00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_bun_cmn_circle01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_circle01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_dis00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_dis00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_distortionA"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_distortionA.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_distortionB"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_distortionB.dds"), 1))))
		return E_FAIL;
	// 55

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_distortionC"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_distortionC.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_dust00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_dust00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_dust01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_dust01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_Fractal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_Fractal.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton409_ray00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton409_ray00.dds"), 1))))
		return E_FAIL;
	//60

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_Fractal2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_Fractal2.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_Fractal3"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_Fractal3.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_Fractal3_gray"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_Fractal3_gray.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_Fractal4"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_Fractal4.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_Fractal5"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_Fractal5.dds"), 1))))
		return E_FAIL;
	// 65

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_Fractal6"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_Fractal6.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_glass00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_glass00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_glass02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_glass02.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_gradation_blue"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_gradation_blue.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_gradation_orange"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_gradation_orange.dds"), 1))))
		return E_FAIL;
	//70

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_gradation_orange2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_gradation_orange2.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_gradation00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_gradation00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_gradation01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_gradation01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_gradation02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_gradation02.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_gradation03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_gradation03.dds"), 1))))
		return E_FAIL;
	// 75

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_grass"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_grass.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_guard00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_guard00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_guard01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_guard01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_guard02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_guard02.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_hibana00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_hibana00.dds"), 1))))
		return E_FAIL;
	//80

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_hit00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_hit00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_human00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_human00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_ilm00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_ilm00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_jizokusmoke00end"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_jizokusmoke00end.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_jizokusmoke00end2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_jizokusmoke00end2.dds"), 1))))
		return E_FAIL;
	// 85

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_jizokusmoke00loop"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_jizokusmoke00loop.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_jizokusmoke00start"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_jizokusmoke00start.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_kidan00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_kidan00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_kira00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_kira00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_kira01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_kira01.dds"), 1))))
		return E_FAIL;
	//90

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_kmhmFractal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_kmhmFractal.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_line00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_line00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_line01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_line01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_line02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_line02.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_line03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_line03.dds"), 1))))
		return E_FAIL;
	// 95

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_line04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_line04.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_line04gray"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_line04gray.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_mask00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_mask00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_mask01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_mask01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_mask02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_mask02.dds"), 1))))
		return E_FAIL;
	//100

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_mazzle00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_mazzle00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_mazzle01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_mazzle01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_mazzle02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_mazzle02.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_pattern00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_pattern00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_pattern01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_pattern01.dds"), 1))))
		return E_FAIL;
	// 5

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_pattern02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_pattern02.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_ray00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_ray00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_ray00new"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_ray00new.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_ray01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_ray01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_rock_PlanetBreak"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_rock_PlanetBreak.dds"), 1))))
		return E_FAIL;
	//10

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_rock_smalls_def"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_rock_smalls_def.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_rock_smalls_space"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_rock_smalls_space.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_rock00_def"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_rock00_def.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_rock01_def"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_rock01_def.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_rock02_def"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_rock02_def.dds"), 1))))
		return E_FAIL;
	// 15

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_samne00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_samne00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_samne01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_samne01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_samne02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_samne02.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_scrline00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_scrline00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_scrline01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_scrline01.dds"), 1))))
		return E_FAIL;
	//20

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_scrline02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_scrline02.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_scrollsmoke"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_scrollsmoke.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_slash00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_slash00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke_scroll"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke_scroll.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke_scroll2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke_scroll2.dds"), 1))))
		return E_FAIL;
	// 25

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke02.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke02_blur"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke02_blur.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke03.dds"), 1))))
		return E_FAIL;
	//30

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke04.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke04b"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke04b.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke05"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke05.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke06"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke06.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke07"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke07.dds"), 1))))
		return E_FAIL;
	// 35

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smoke08"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smoke08.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_smokemodel00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_smokemodel00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_spark00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_spark00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_spark01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_spark01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_spark02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_spark02.dds"), 1))))
		return E_FAIL;
	//40

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_sphere00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_sphere00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_sphere01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_sphere01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_sss00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_sss00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_stoneanim00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_stoneanim00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_stoneanim00_gray"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_stoneanim00_gray.dds"), 1))))
		return E_FAIL;
	//45

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_stoneanim01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_stoneanim01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_stoneanim01gray"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_stoneanim01gray.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_stonemodel00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_stonemodel00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_strike00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_strike00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_strike01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_strike01.dds"), 1))))
		return E_FAIL;
	//50

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_strike02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_strike02.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_test"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_test.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_thunder00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_thunder00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_thunder01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_thunder01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_thunder02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_thunder02.dds"), 1))))
		return E_FAIL;
	// 55

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_thunder02yellow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_thunder02yellow.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_tigire00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_tigire00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_TinkleLight00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_TinkleLight00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_trail_mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_trail_mask.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_trail00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_trail00.dds"), 1))))
		return E_FAIL;
	//60

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_trail02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_trail02.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_trail04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_trail04.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_water00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_water00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_water01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_water01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_watersplash00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_watersplash00.dds"), 1))))
		return E_FAIL;
	// 65

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_watersplash01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_watersplash01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_wave00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_wave00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_white"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_white.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_wind01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_wind01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_thunder01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_thunder01.dds"), 1))))
		return E_FAIL;
	//70

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_wipe00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_wipe00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_wave01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_wave01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_wipe01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_wipe01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmnsmokemodel00color"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmnsmokemodel00color.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmnsmokemodel02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmnsmokemodel02.dds"), 1))))
		return E_FAIL;
	// 75

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmnsmokemodel02color"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmnsmokemodel02color.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmnsmokemodel03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmnsmokemodel03.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmnsmokemodel03color"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmnsmokemodel03color.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_GKS_cookie"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/GKS_cookie.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_GKS600_cseff"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/GKS600_cseff.dds"), 1))))
		return E_FAIL;
	//80

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_GKS600_cseff2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/GKS600_cseff2.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_GKS805_cseff"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/GKS805_cseff.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_GKS808_changeScr"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/GKS808_changeScr.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_GKS808_changeScr2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/GKS808_changeScr2.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_gks814_scrwipe"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/gks814_scrwipe.dds"), 1))))
		return E_FAIL;
	// 85

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_htn_burst01_middle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/htn_burst01_middle.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_htn_burst01_smal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/htn_burst01_smal.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_HTN_cookie"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/HTN_cookie.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_htn_gradation01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/htn_gradation01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_htn_gradation02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/htn_gradation02.dds"), 1))))
		return E_FAIL;
	//90

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_htn_hibi01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/htn_hibi01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_htn407_hibi01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/htn407_hibi01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_htn407_hibi02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/htn407_hibi02.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_htn407_hibi03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/htn407_hibi03.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_htn431_hibi01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/htn431_hibi01.dds"), 1))))
		return E_FAIL;
	// 95

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_htn431_hibi02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/htn431_hibi02.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_sp_EarthCloud"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/sp_EarthCloud.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_sp_EarthCloud01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/sp_EarthCloud01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_sp_EarthCloud02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/sp_EarthCloud02.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_sp_star01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/sp_star01.dds"), 1))))
		return E_FAIL;
	//200

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_sp_star02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/sp_star02.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_431kidou"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_431kidou.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_431tunder00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_431tunder00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_beam_mask_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_beam_mask_01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_cookie"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_cookie.dds"), 1))))
		return E_FAIL;
	// 205

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_gradation_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_gradation_01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_kmhmFractal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_kmhmFractal.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_line00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_line00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_mask_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_mask_01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_slash01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_slash01.dds"), 1))))
		return E_FAIL;
	//210

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_spark01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_spark01.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_ton_thunder00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/ton_thunder00.dds"), 1))))
		return E_FAIL;
	//212

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_T_Spark_Core"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/T_Spark_Core.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_T_Smoke_Tiled_D"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/T_Smoke_Tiled_D.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_T_Smoke_SubUV"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/T_Smoke_SubUV.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_T_Fire_Tiled_D"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/T_Fire_Tiled_D.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_T_Fire_SubUV"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/T_Fire_SubUV.dds"), 1))))
		return E_FAIL;
	// 217

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_T_Explosion_SubUV"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/T_Explosion_SubUV.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_T_Dust_Particle_D"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/T_Dust_Particle_D.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_T_Burst_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/T_Burst_M.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_T_Water_N"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/T_Water_N.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_homingB_shadow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_homingB_shadow.dds"), 1))))
		return E_FAIL;
	//222

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_homingB"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_homingB.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_homingAura_stop"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_homingAura_stop.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_homingA"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_homingA.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_hit02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_hit02.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_hit01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_hit01.dds"), 1))))
		return E_FAIL;
	// 227

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_gradation"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_gradation.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_forcedchange"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_forcedchange.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_dbfly"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_dbfly.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_aura11"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_aura11.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_scrRock01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_scrRock01.dds"), 1))))
		return E_FAIL;
	//232

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_scrRock00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_scrRock00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_ryuhai00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_ryuhai00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_number00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_number00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_homingBair_shadow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_homingBair_shadow.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_homingBair"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_homingBair.dds"), 1))))
		return E_FAIL;
	// 237

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_wind00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_wind00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_scrspark00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_scrspark00.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_cmn_scrsmoke00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/ModelData/Eff/Texture/cmn_scrsmoke00.dds"), 1))))
		return E_FAIL;

	//240


#pragma endregion

#pragma region Effect Object
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_NoneLight"),
		CEffect_NoneLight::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Blend"),
		CEffect_Blend::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_ZNone"),
		CEffect_ZNone::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Overlap"),
		CEffect_Overlap::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

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

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),
		CMonster::Create(m_pDevice, m_pContext))))
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
