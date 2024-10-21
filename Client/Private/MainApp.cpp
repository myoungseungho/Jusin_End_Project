#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"


#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "RenderInstance.h"
#include "Level_Loading.h"
#include "Imgui_Manager.h"

#include "UI_Loading.h"
#include "UI_LoadingMark.h"
#include "UI_Loading_Font.h"

#include "thread"

CMainApp::CMainApp()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
	, m_pRenderInstance{ CRenderInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pRenderInstance);
}

HRESULT CMainApp::Initialize()
{
	//게임인스턴스 엔진 초기화
	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, g_hWnd, true, LEVEL_END, g_iWinSizeX, g_iWinSizeY, &m_pDevice, &m_pContext)))
		return E_FAIL;

	//렌더인스턴스 엔진 초기화
	if (FAILED(m_pRenderInstance->Initialize_Engine(g_hWnd, true, LEVEL_END, g_iWinSizeX, g_iWinSizeY, &m_pDevice, &m_pContext)))
		return E_FAIL;

	//쉐이더 생성
	if (FAILED(Ready_Prototype_Component_ForStatic()))
		return E_FAIL;

	//IMGUI 생성, 싱글턴
	Create_IMGUI_Manager();


	//스레드풀 초기화
	//하드웨어의 스레드 수를 넘겨준다. (소프트웨어 스레드 수 아님)
	if (FAILED(m_pGameInstance->Initialize_ThreadPool(4)))
		return E_FAIL;

	if (FAILED(Open_Level(LEVEL_GAMEPLAY)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	m_pGameInstance->Update_Engine(fTimeDelta);
}

// 1/50 -> 0.02초 고정 Update
void CMainApp::Fixed_Update(_float fTimeDelta)
{
}

HRESULT CMainApp::Render(_float fTimeDelta)
{
	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();


	//나머지 렌더는 렌더인스턴스
	m_pRenderInstance->Render_Engine(fTimeDelta);

	//레벨매니저 렌더는 게임인스턴스
	m_pGameInstance->Render_Engine(fTimeDelta);


	//IMGUI 렌더
	//m_pImgui_Manager->Render(fTimeDelta);

	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVELID eStartLevelID)
{
	if (FAILED(m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pDevice, m_pContext, eStartLevelID))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Create_IMGUI_Manager()
{
	m_pImgui_Manager = CImgui_Manager::Get_Instance();
	m_pImgui_Manager->Initialize(m_pDevice, m_pContext);

	Safe_AddRef(m_pImgui_Manager);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component_ForStatic()
{
	//Component_VIBuffer

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_UI_LoadingBackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LoadingBackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/CmnBG/tex/E3_Title_BG01.png")))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Loading */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Loading"),
		CUI_Loading::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_UI_GameStartCircle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LoadingMark"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/Middle/GameStart/GameStart%d.png"), 8))))
		return E_FAIL;


	/* For.Prototype_GameObject_UI_LoadingMark */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_LoadingMark"),
		CUI_LoadingMark::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_UI_LoadingFont */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LoadingFont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/DB_load_00.png")))))
		return E_FAIL;


	/* For.Prototype_GameObject_UI_LoadingFont */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_LoadingFont"),
		CUI_Loading_Font::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//For  Prototype Collision
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
	//	CCollider_Test::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	//Component_Shader

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_UI_VtxRect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI_VtxRect"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_UI_VtxRect.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Single_Eff_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Single_Eff_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
		CCollider_Test::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CMainApp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	m_pImgui_Manager->Free();
	Safe_Release(m_pImgui_Manager);

	m_pGameInstance->Release_Engine();
	Safe_Release(m_pGameInstance);

	m_pRenderInstance->Release_Engine();
	Safe_Release(m_pRenderInstance);
}

