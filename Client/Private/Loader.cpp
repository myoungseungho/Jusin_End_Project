#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "IMGUI_Shader_Tab.h"
#include "Main_Camera.h"
#include "Virtual_Camera.h"
#include "Monster.h"
#include "Player.h"
#include "Energy_Effect.h"
#include "Energy_Effect_2p.h"
#include "Melee_Effect.h"
#include "Melee_Effect_2p.h"
#include "Range_Effect.h"
#include "Range_Effect_2p.h"
#include "Line_Draw.h"
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
	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;
	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;
	/* For.Prototype_Component_Collider_Sphere */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델(정점 -> 폴리곤 -> 메시 -> 모델)을 로딩 중 입니다."));	
	
	// Identity 매트릭스를 초기화
	_matrix PreTransformMatrix = XMMatrixIdentity();

	// 크기를 3배씩 해주는 스케일 매트릭스 생성
	XMMATRIX scaleMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f);

	// Y축을 기준으로 180도 회전하는 회전 매트릭스 생성
	XMMATRIX rotationMatrix = XMMatrixRotationX(XMConvertToRadians(-90.f));

	PreTransformMatrix = scaleMatrix * rotationMatrix;

	//손오공
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Goku_SS3_Rotate0.bin", PreTransformMatrix))))
		return E_FAIL;

	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),
		CMonster::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Main_Camera"),
		CMain_Camera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Virtual_Camera"),
		CVirtual_Camera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Energy_Effect"),
		CEnergy_Effect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Energy_Effect_2p"),
		CEnergy_Effect_2p::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Range_Effect"),
		CRange_Effect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Range_Effect_2p"),
		CRange_Effect_2p::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Melee_Effect"),
		CMelee_Effect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Melee_Effect_2p"),
		CMelee_Effect_2p::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Line_Draw"),
		CLine_Draw::Create(m_pDevice, m_pContext))))
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
