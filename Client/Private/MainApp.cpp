#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"


#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "RenderInstance.h"
#include "Level_Loading.h"
#include "Imgui_Manager.h"
#include "Effect_Manager.h"

CMainApp::CMainApp()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
	, m_pRenderInstance{ CRenderInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pRenderInstance);
}

HRESULT CMainApp::Initialize()
{
	//�����ν��Ͻ� ���� �ʱ�ȭ
	if (FAILED(m_pGameInstance->Initialize_Engine(g_hWnd, true, LEVEL_END, g_iWinSizeX, g_iWinSizeY, &m_pDevice, &m_pContext)))
		return E_FAIL; 

	//�����ν��Ͻ� ���� �ʱ�ȭ
	if (FAILED(m_pRenderInstance->Initialize_Engine(g_hWnd, true, LEVEL_END, g_iWinSizeX, g_iWinSizeY, &m_pDevice, &m_pContext)))
		return E_FAIL;

	//IMGUI ����, �̱���
	Create_IMGUI_Manager();

	if (FAILED(Open_Level(LEVEL_GAMEPLAY)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	m_pGameInstance->Update_Engine(fTimeDelta);
}

HRESULT CMainApp::Render(_float fTimeDelta)
{
	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	//IMGUI ����
	m_pIMGUI_Manager->Render(fTimeDelta);

	//�����Ŵ��� ������ �����ν��Ͻ�
	m_pGameInstance->Render_Engine();
	//������ ������ �����ν��Ͻ�
	m_pRenderInstance->Render_Engine();

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
	m_pIMGUI_Manager = CImgui_Manager::Create(m_pDevice, m_pContext, m_pGameInstance, m_pRenderInstance);

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

	m_pGameInstance->Release_Engine();
	Safe_Release(m_pGameInstance);

	CEffect_Manager::Get_Instance()->Destroy_Instance();

	m_pRenderInstance->Release_Engine();
	Safe_Release(m_pRenderInstance);
	Safe_Release(m_pIMGUI_Manager);
}

