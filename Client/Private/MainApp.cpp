#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"


#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "RenderInstance.h"
#include "Level_Loading.h"
#include "Imgui_Manager.h"
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
	//�����ν��Ͻ� ���� �ʱ�ȭ
	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, g_hWnd, true, LEVEL_END, g_iWinSizeX, g_iWinSizeY, &m_pDevice, &m_pContext)))
		return E_FAIL;

	//�����ν��Ͻ� ���� �ʱ�ȭ
	if (FAILED(m_pRenderInstance->Initialize_Engine(g_hWnd, true, LEVEL_END, g_iWinSizeX, g_iWinSizeY, &m_pDevice, &m_pContext)))
		return E_FAIL;

	//���̴� ����
	if (FAILED(Ready_Prototype_Component_ForStatic()))
		return E_FAIL;

	//IMGUI ����, �̱���
	Create_IMGUI_Manager();

	//������Ǯ �ʱ�ȭ
	//�ϵ������ ������ ���� �Ѱ��ش�. (����Ʈ���� ������ �� �ƴ�)
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

// 1/50 -> 0.02�� ���� Update
void CMainApp::Fixed_Update(_float fTimeDelta)
{

}

HRESULT CMainApp::Render(_float fTimeDelta)
{
	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	//�����Ŵ��� ������ �����ν��Ͻ�
	m_pGameInstance->Render_Engine();

	//������ ������ �����ν��Ͻ�
	m_pRenderInstance->Render_Engine(fTimeDelta);

	//IMGUI ������ �ε����� �ϸ� �ȵ�
	if (m_pGameInstance->Get_CurrentLevel_Index() != LEVEL_LOADING)
	{
		//IMGUI ����
		m_pImgui_Manager->Render(fTimeDelta);
	}

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

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component_ForStatic()
{
	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxLineTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxLineTex.hlsl"), VTXLINE::Elements, VTXLINE::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Line"),
		CVIBuffer_Line::Create(m_pDevice, m_pContext))))
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

	m_pGameInstance->Release_Engine();
	Safe_Release(m_pGameInstance);

	m_pRenderInstance->Release_Engine();
	Safe_Release(m_pRenderInstance);
}

