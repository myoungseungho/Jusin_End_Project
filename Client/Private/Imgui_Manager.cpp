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
#include "IMGUI_Object_Tab.h"
#include "IMGUI_Level_Tab.h"
#include "IMGUI_Camera_Tab.h"

#include "imnodes.h"

_bool bShowImGuiWindows = true;  // IMGUI 창 표시 여부를 제어하는 전역 변수
_bool bShowImGuiRenderTarget = false;  // IMGUI 창 표시 여부를 제어하는 전역 변수
_bool bShowImGuiDebug_Component = false;  // IMGUI 창 표시 여부를 제어하는 전역 변수
_bool bShowImGuiDebug_COut = false;  // IMGUI 창 표시 여부를 제어하는 전역 변수
_bool bShowImGuiLayerView = false;

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

	ImGui::StyleColorsLight();

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pDevice, m_pContext);
	ImGui_ImplWin32_EnableDpiAwareness();

	//IMGUI 탭 객체 생성
	m_vecTabs.push_back(CIMGUI_Camera_Tab::Create(m_pDevice, m_pContext));
	m_vecTabs.push_back(CIMGUI_Level_Tab::Create(m_pDevice, m_pContext));
	m_vecTabs.push_back(CIMGUI_Animation_Tab::Create(m_pDevice, m_pContext));
	m_vecTabs.push_back(CIMGUI_UI_Tab::Create(m_pDevice, m_pContext));
	m_vecTabs.push_back(CIMGUI_Effect_Tab::Create(m_pDevice, m_pContext));
	m_vecTabs.push_back(CIMGUI_Object_Tab::Create(m_pDevice, m_pContext));

	return S_OK;
}

void CImgui_Manager::Priority_Update(_float fTimeDelta)
{

}


void CImgui_Manager::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Mouse_Down(DIMK_LBUTTON))
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
}

void CImgui_Manager::Late_Update(_float fTimeDelta)
{

}

HRESULT CImgui_Manager::Render(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	++m_iNumCount;

	if (m_fTimeAcc >= 1.f)
	{
		m_fTimeAcc = 0.f;
		m_iNumRender = m_iNumCount;
		m_iNumCount = 0;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Render IMGUI UI elements
	Render_IMGUI(fTimeDelta);
	Render_ShaderTabs(fTimeDelta);
	Render_EffectAnimationTabs(fTimeDelta);

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return S_OK;
}

void CImgui_Manager::Show_Debug_COut(_bool bShow)
{
	if (bShow) // true일 때 콘솔을 켜기
	{
		if (::AllocConsole() == TRUE)
		{
			FILE* nfp[3];
			freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
			freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
			freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
			std::ios::sync_with_stdio();
		}
	}
	else // false일 때 콘솔을 끄기
	{
		HWND hwndConsole = GetConsoleWindow(); // 콘솔 창의 핸들을 가져옴
		if (hwndConsole)
		{
			::FreeConsole(); // 콘솔과의 연결 해제
			PostMessage(hwndConsole, WM_CLOSE, 0, 0); // 콘솔 창에 닫기 메시지 보내기
		}
	}
}

void CImgui_Manager::Push_Shader_Tab(CTexture* pTexture)
{
	m_vecShader_Tabs[to_string(m_iShaderCount)] = (CIMGUI_Shader_Tab::Create(m_pDevice, m_pContext, pTexture));
	m_vecShader_Tabs[to_string(m_iShaderCount)]->m_iNumberId = m_iShaderCount;

	m_iShaderCount++;
}

void CImgui_Manager::Save_Shader_Tab(_int iIndex, string fileName)
{
	m_vecShader_Tabs[to_string(iIndex)]->Click_Save_Shader_Tab(fileName);
}

void CImgui_Manager::Load_Shader_Tab(CTexture* pTexture, string strFilename, _int iIndex)
{
	m_vecShader_Tabs[to_string(iIndex)] = (CIMGUI_Shader_Tab::Create_Load(m_pDevice, m_pContext, pTexture, strFilename));
	m_vecShader_Tabs[to_string(iIndex)]->m_iNumberId = iIndex;
	m_vecShader_Tabs[to_string(iIndex)]->Click_Load_Shader_Tab(strFilename.c_str());
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

	_bool isOverMainImGui = (ptMouse.x >= m_ImGuiScreen.MainImGuiPos.x && ptMouse.x <= m_ImGuiScreen.MainImGuiPos.x + m_ImGuiScreen.MainImGuiSize.x &&
		ptMouse.y >= m_ImGuiScreen.MainImGuiPos.y && ptMouse.y <= m_ImGuiScreen.MainImGuiPos.y + m_ImGuiScreen.MainImGuiSize.y);

	_bool isOverShaderImGui = (ptMouse.x >= m_ImGuiScreen.ShaderImGuiPos.x && ptMouse.x <= m_ImGuiScreen.ShaderImGuiPos.x + m_ImGuiScreen.ShaderImGuiSize.x &&
		ptMouse.y >= m_ImGuiScreen.ShaderImGuiPos.y && ptMouse.y <= m_ImGuiScreen.ShaderImGuiPos.y + m_ImGuiScreen.ShaderImGuiSize.y);

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
		ImGui::Text("FPS : %d", m_iNumRender);

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

		if (ImGui::BeginMenu("Debug_Component")) {
			if (ImGui::MenuItem("Debug_Component", NULL, &bShowImGuiDebug_Component)) {
				m_pRenderInstance->SetActive_Debug_Component(bShowImGuiDebug_Component);
			}
			ImGui::EndMenu();
		}

		if (ImGui::Checkbox("Layer_View", &bShowImGuiLayerView)) {
			m_pRenderInstance->Show_Layer_View();
		}

		if (ImGui::BeginMenu("Debug_COut")) {
			if (ImGui::MenuItem("Debug_COut", NULL, &bShowImGuiDebug_COut)) {
				Show_Debug_COut(bShowImGuiDebug_COut);
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (bShowImGuiWindows) {  // 이 조건을 통해 모든 ImGui 창의 표시 여부를 제어

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
		m_ImGuiScreen.MainImGuiPos = ImGui::GetWindowPos();
		m_ImGuiScreen.MainImGuiSize = ImGui::GetWindowSize();
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
		{
			tab.second->m_TabPick = false;
			tab.second->Update(fTimeDelta);
		}


		m_ImGuiScreen.ShaderImGuiPos = ImGui::GetWindowPos();
		m_ImGuiScreen.ShaderImGuiSize = ImGui::GetWindowSize();
		ImGui::End(); // 메인 창 종료
	}

}

void CImgui_Manager::Render_EffectAnimationTabs(_float fTimeDelta)
{
}

void CImgui_Manager::Free()
{
	__super::Free();

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
}
