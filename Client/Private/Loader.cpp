#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "IMGUI_Shader_Tab.h"
#include "Camera_Free.h"
#include "Monster.h"

//#include "Monster.h"
//#include "Terrain.h"
//#include "Camera.h"
//#include "Player.h"
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

	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델(정점 -> 폴리곤 -> 메시 -> 모델)을 로딩 중 입니다."));	
	
	_matrix			PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	//손오공
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Goku_SS3_Rotate0.bin", PreTransformMatrix))))
		return E_FAIL;

#pragma region Effect Model

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun_cookie"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun_cookie.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun201_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun201_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun203_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun203_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun204_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun204_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun231_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun231_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	// 5

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun232_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun232_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun261_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun261_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun301_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun301_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun403_BindLoop"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun403_BindLoop.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun403_BindStart"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun403_BindStart.bin", PreTransformMatrix))))
		return E_FAIL;

	//10

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_bun403_jizoku"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/bun403_jizoku.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_AuraBall"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_AuraBall.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_BeamCore"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_BeamCore.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_BeamCorePattern"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_BeamCorePattern.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_bodyaura00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_bodyaura00.bin", PreTransformMatrix))))
		return E_FAIL;

	// 15

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_crossplane00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_crossplane00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_crossplane01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_crossplane01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_crossplane02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_crossplane02.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_cylinder00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_cylinder00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_cylinder01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_cylinder01.bin", PreTransformMatrix))))
		return E_FAIL;

	//20

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_cylinder01_ura"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_cylinder01_ura.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_cylinder02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_cylinder02.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_cylinder03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_cylinder03.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_cylinder04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_cylinder04.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_cylinderBeam00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_cylinderBeam00.bin", PreTransformMatrix))))
		return E_FAIL;

	// 25

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_cylinderBeam01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_cylinderBeam01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_futtobiBGpoint"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_futtobiBGpoint.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_futtobiBGtemprate"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_futtobiBGtemprate.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_GrassModel"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_GrassModel.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_halfsphere00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_halfsphere00.bin", PreTransformMatrix))))
		return E_FAIL;

	//30

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_halfsphere01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_halfsphere01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_hibana00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_hibana00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_kakusanaura"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_kakusanaura.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_kidan00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_kidan00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_KidanTrail"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_KidanTrail.bin", PreTransformMatrix))))
		return E_FAIL;

	// 35

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_pivot_plane01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_pivot_plane01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_povot_plane00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_povot_plane00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_puffycylinder"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_puffycylinder.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_puffyplane"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_puffyplane.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_puffyplane_harf"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_puffyplane_harf.bin", PreTransformMatrix))))
		return E_FAIL;

	//40

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_puffyplane2"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_puffyplane2.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_rock00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_rock00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_rock01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_rock01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_rock02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_rock02.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_rock04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_rock04.bin", PreTransformMatrix))))
		return E_FAIL;

	// 45

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_rock05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_rock05.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_rubble01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_rubble01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_saiyanAura"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_saiyanAura.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_screw"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_screw.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_screw2"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_screw2.bin", PreTransformMatrix))))
		return E_FAIL;

	//50

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_screw3"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_screw3.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_shenlongAura00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_shenlongAura00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_shenlongAura01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_shenlongAura01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_shenlongAura02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_shenlongAura02.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_SmokeModels00.bin", PreTransformMatrix))))
		return E_FAIL;

	// 55

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_SmokeModels01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_SmokeModels02.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_SmokeModels03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_SmokeModels03.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_sphere00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_sphere00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_sphere00_inv"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_sphere00_inv.bin", PreTransformMatrix))))
		return E_FAIL;

	//60

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_sphere00_inv2"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_sphere00_inv2.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_stonemodel00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_stonemodel00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_strike00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_strike00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_TestStrike"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_TestStrike.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_cmn_WindModel00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/cmn_WindModel00.bin", PreTransformMatrix))))
		return E_FAIL;

	// 65

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks_cookie"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks_cookie.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks262_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks262_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks262_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks262_eff01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks262_eff10"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks262_eff10.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks301_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks301_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	//70

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks404_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks404_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks404_eff00_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks404_eff00_sub.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks404_eff00_sub2"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks404_eff00_sub2.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks406_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks406_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks406_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks406_eff01.bin", PreTransformMatrix))))
		return E_FAIL;

	// 75

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks406_eff02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks406_eff02.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks406_eff03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks406_eff03.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks430_tameaura_00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks430_tameaura_00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks431_kaijo"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks431_kaijo.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks432_coremodel"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks432_coremodel.bin", PreTransformMatrix))))
		return E_FAIL;

	//80

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks432_coremodel2"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks432_coremodel2.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks806_sphere00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks806_sphere00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks808_Sphere"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks808_Sphere.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks808_Sphere_inv"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks808_Sphere_inv.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_gks814_fistaura"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/gks814_fistaura.bin", PreTransformMatrix))))
		return E_FAIL;

	// 85

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn_cookie"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn_cookie.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn_halfsphere01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn_halfsphere01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn_plane01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn_plane01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn_plane03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn_plane03.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn301_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn301_eff01.bin", PreTransformMatrix))))
		return E_FAIL;

	//90

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn403_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn403_eff01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn403_eff01_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn403_eff01_sub.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn405_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn405_eff01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn405_eff01_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn405_eff01_sub.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn406_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn406_eff01.bin", PreTransformMatrix))))
		return E_FAIL;

	// 95

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn406_eff01_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn406_eff01_sub.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn407_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn407_eff01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn407_eff02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn407_eff02.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn408_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn408_eff01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn408_eff02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn408_eff02.bin", PreTransformMatrix))))
		return E_FAIL;

	//100

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn432_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn432_eff01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn432_eff01_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn432_eff01_sub.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_a01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_a01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_a02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_a02.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_a03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_a03.bin", PreTransformMatrix))))
		return E_FAIL;

	// 105

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_a04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_a04.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_b01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_b01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_b02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_b02.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_b03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_b03.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_b03_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_b03_sub.bin", PreTransformMatrix))))
		return E_FAIL;

	//110

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_b04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_b04.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_b04_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_b04_sub.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_c01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_c01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_c01_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_c01_sub.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_c02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_c02.bin", PreTransformMatrix))))
		return E_FAIL;

	// 115

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_c02_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_c02_sub.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_c03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_c03.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_c04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_c04.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_d01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_d01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_d01_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_d01_sub.bin", PreTransformMatrix))))
		return E_FAIL;

	//120

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_d02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_d02.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_d02_sub"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_d02_sub.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_d03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_d03.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_htn600cs_eff_hibi_d04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/htn600cs_eff_hibi_d04.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton_cookie"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton_cookie.bin", PreTransformMatrix))))
		return E_FAIL;

	// 125

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton_kidan01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton_kidan01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton201_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton201_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton204_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton204_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton205_eff1"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton205_eff1.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton231_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton231_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	//130

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton232_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton232_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton301_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton301_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton401_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton401_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton401_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton401_eff01.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton401_eff02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton401_eff02.bin", PreTransformMatrix))))
		return E_FAIL;

	// 135

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton401_eff03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton401_eff03.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton401_eff04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton401_eff04.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton430_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton430_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton431_beam"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton431_beam.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton431_eff01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton431_eff01.bin", PreTransformMatrix))))
		return E_FAIL;

	//140

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton432_eff00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton432_eff00.bin", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Effect_ton433_sphere01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Eff/Model/ton433_sphere01.bin", PreTransformMatrix))))
		return E_FAIL;

	//142

#pragma endregion



	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),
		CMonster::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
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
