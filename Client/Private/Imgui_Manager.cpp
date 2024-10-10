#include "stdafx.h"
#include <string>

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Imgui_Manager.h"
#include "GameInstance.h"
#include "RenderInstance.h"

#include "IMGUI_Shader_Tab.h"
#include "IMGUI_Animation_Tab.h"
#include "IMGUI_Effect_Tab.h"
#include "IMGUI_UI_Tab.h"

#include "imnodes.h"

_bool bShowImGuiWindows = true;  // IMGUI 창 표시 여부를 제어하는 전역 변수
_bool bShowImGuiRenderTarget = false;  // IMGUI 창 표시 여부를 제어하는 전역 변수

IMPLEMENT_SINGLETON(CImgui_Manager)

// IMGUI 창 표시 여부를 제어하는 전역 변수
CImgui_Manager::CImgui_Manager()
	: m_pGameInstance{ CGameInstance::Get_Instance() }, m_pRenderInstance{ CRenderInstance::Get_Instance() }

{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pRenderInstance);
}

HRESULT CImgui_Manager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImNodes::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pDevice, m_pContext);
	ImGui_ImplWin32_EnableDpiAwareness();

	//IMGUI 탭 객체 생성
	m_vecTabs.push_back(CIMGUI_Animation_Tab::Create(m_pDevice, m_pContext));
	m_vecTabs.push_back(CIMGUI_UI_Tab::Create(m_pDevice, m_pContext));
	m_vecTabs.push_back(CIMGUI_Effect_Tab::Create(m_pDevice, m_pContext));

	return S_OK;
}

void CImgui_Manager::Priority_Update(_float fTimeDelta)
{

}


void CImgui_Manager::Update(_float fTimeDelta)
{
	if (m_pGameInstance->MouseDown(DIMK_LBUTTON))
	{
		_int iMeshIndex = Pick_Effect_Mesh();

		if (iMeshIndex != -1)
		{
			_int iCount = 0;
			for (auto& tab : m_vecShader_Tabs)
			{
				if (tab.second->m_iNumberId == iMeshIndex)
				{
					tab.second->m_TabPick = true;
					tab.second->TabPos_Init();
				}
				else
					tab.second->m_TabPick = false;

				iCount++;
			}
		}
	}

	if (m_pGameInstance->MouseDown(DIMK_RBUTTON))
	{
		Delete_Shader_Tab(0);
	}
}

void CImgui_Manager::Late_Update(_float fTimeDelta)
{

}

HRESULT CImgui_Manager::Render(_float fTimeDelta)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Render IMGUI UI elements
	Render_IMGUI(fTimeDelta);
	Render_ShaderTabs(fTimeDelta);
	
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	
	return S_OK;
}

void CImgui_Manager::Push_Shader_Tab(CTexture* pTexture)
{
	m_vecShader_Tabs[to_string(m_iShaderCount)] = (CIMGUI_Shader_Tab::Create(m_pDevice, m_pContext, pTexture));
	m_vecShader_Tabs[to_string(m_iShaderCount)]->m_iNumberId = m_iShaderCount;

	m_iShaderCount++;
}

void CImgui_Manager::Delete_Shader_Tab(_int iIndex)
{

	Safe_Release(m_vecShader_Tabs[to_string(iIndex)]);
	m_vecShader_Tabs.erase(to_string(iIndex));

}

_int CImgui_Manager::Pick_Effect_Mesh()
{
	_int isPick = { -1 };

	POINT ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_bool isOverMainImGui = (ptMouse.x >= MainImGuiPos.x && ptMouse.x <= MainImGuiPos.x + MainImGuiSize.x &&
		ptMouse.y >= MainImGuiPos.y && ptMouse.y <= MainImGuiPos.y + MainImGuiSize.y);

	_bool isOverShaderImGui = (ptMouse.x >= ShaderImGuiPos.x && ptMouse.x <= ShaderImGuiPos.x + ShaderImGuiSize.x &&
		ptMouse.y >= ShaderImGuiPos.y && ptMouse.y <= ShaderImGuiPos.y + ShaderImGuiSize.y);

	if (!isOverMainImGui && !isOverShaderImGui)
	{
		isPick = m_pRenderInstance->Picked_Effect_Index();
	}


	return isPick;
}

void CImgui_Manager::Render_IMGUI(_float fTimeDelta)
{
	// 토글 버튼
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Option")) {
			if (ImGui::MenuItem("IMGUI ON", NULL, &bShowImGuiWindows)) {
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Render_Target")) {
			if (ImGui::MenuItem("Render_Target", NULL, &bShowImGuiRenderTarget)) {
				 m_pRenderInstance->SetActive_RenderTarget(bShowImGuiRenderTarget);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (bShowImGuiWindows) {  // 이 조건을 통해 모든 ImGui 창의 표시 여부를 제어

		static _bool bShowAnimation = true;
		static _bool bShowEffect = true;
		static _bool bShowUI = true;
		static _bool bShowShader = true;

		ImGui::Begin("Main Tab", &bShowImGuiWindows); // 메인 창 시작
		if (ImGui::BeginTabBar("DragonBall_Tool")) { // 탭 바 시작

			for (auto& tab : m_vecTabs) 
			{
				if (ImGui::BeginTabItem(tab->GetTabName())) 
				{
					tab->Render(fTimeDelta);
					ImGui::EndTabItem();
				}
			}

			ImGui::EndTabBar(); // 탭 바 종료
		}
		MainImGuiPos = ImGui::GetWindowPos();
		MainImGuiSize = ImGui::GetWindowSize();
		ImGui::End();  // 창을 닫음
	}
}

void CImgui_Manager::Render_ShaderTabs(_float fTimeDelta)
{
	for (auto& tab : m_vecShader_Tabs)
	{
		ImGui::Begin("Shader Tab");
	
		if (/*ImGui::BeginTabItem(to_string(tab->m_iNumberId).c_str(), &tab->m_TabPick) || */tab.second->m_TabPick == true)
		{
			ImGui::Text("Mesh Index : %d", tab.second->m_iNumberId);
			m_iCurShaderTabId = tab.second->m_iNumberId;

			ImGui::SameLine();
			if (ImGui::Button("Out_Line"))
				m_pRenderInstance->Show_OutLine();
			if (tab.second->m_TabPick == false)
			{
				tab.second->TabPos_Init();
				tab.second->m_TabPick = true;
			}

			m_iCurShaderTabIndex = tab.second->m_iNumberId;
			tab.second->Render(fTimeDelta);
			//ImGui::EndTabItem();
		}
		else
			tab.second->m_TabPick = false;


		ShaderImGuiPos = ImGui::GetWindowPos();
		ShaderImGuiSize = ImGui::GetWindowSize();
		ImGui::End(); // 메인 창 종료
	}

}

void CImgui_Manager::Free()
{
	for (auto& iter : m_vecTabs)
		Safe_Release(iter);

	for (auto& iter : m_vecShader_Tabs)
		Safe_Release(iter.second);

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pRenderInstance);


	__super::Free();
}
