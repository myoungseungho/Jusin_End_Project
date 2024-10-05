#include "stdafx.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Imgui_Manager.h"
#include "GameInstance.h"
#include "RenderInstance.h"

bool bShowImGuiWindows = true;  // IMGUI 창 표시 여부를 제어하는 전역 변수

IMPLEMENT_SINGLETON(CImgui_Manager)

CImgui_Manager::CImgui_Manager()
{
}
// IMGUI 창 표시 여부를 제어하는 전역 변수
CImgui_Manager::CImgui_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CRenderInstance* pRenderInstance)
	:m_pDevice{ pDevice }, m_pContext{ pContext }, m_pRenderInstance{ pRenderInstance }

{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(pRenderInstance);
}

HRESULT CImgui_Manager::Initialize()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pDevice, m_pContext);
	ImGui_ImplWin32_EnableDpiAwareness();

	return S_OK;
}

void CImgui_Manager::Priority_Update(_float fTimeDelta)
{
}


void CImgui_Manager::Update(_float fTimeDelta)
{
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

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return S_OK;
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
		ImGui::EndMainMenuBar();
	}

	if (bShowImGuiWindows) {  // 이 조건을 통해 모든 ImGui 창의 표시 여부를 제어

		//쉐이더
		//이펙트
		//애니메이션
		//UI

		static _bool bShowAnimation = true;
		static _bool bShowEffect = true;
		static _bool bShowUI = true;
		static _bool bShowShader = true;

		ImGui::Begin("Main Tab", &bShowImGuiWindows); // 메인 창 시작
		if (ImGui::BeginTabBar("DragonBall_Tool")) { // 탭 바 시작

			if (ImGui::BeginTabItem("Animation", &bShowAnimation)) { // "Levels" 탭 아이템 시작
				//레벨 창
				if (bShowAnimation) {
					IMGUI_Show_Animation();
				}
				ImGui::EndTabItem(); // "Animation" 탭 아이템 종료
			}

			if (ImGui::BeginTabItem("Effect", &bShowEffect)) { // "ObjectList" 탭 아이템 시작
				//원형 오브젝트 창
				if (bShowEffect) {
					IMGUI_Show_Effect();
				}
				ImGui::EndTabItem(); // "Effect" 탭 아이템 종료
			}

			if (ImGui::BeginTabItem("UI", &bShowUI)) { // "ObjectList" 탭 아이템 시작
				//원형 오브젝트 창
				if (bShowUI) {
					IMGUI_Show_UI();
				}
				ImGui::EndTabItem(); // "UI" 탭 아이템 종료
			}

			if (ImGui::BeginTabItem("Shader", &bShowShader)) { // "Shader" 탭 아이템 시작
				if (bShowShader) {
					IMGUI_Show_Shader();
				}
				ImGui::EndTabItem(); // "Shader" 탭 아이템 종료
			}

			ImGui::EndTabBar(); // 탭 바 종료
		}
		ImGui::End(); // 메인 창 종료
	}
}

HRESULT CImgui_Manager::IMGUI_Show_Animation()
{
	return S_OK;
}

HRESULT CImgui_Manager::IMGUI_Show_Effect()
{
	return S_OK;
}

HRESULT CImgui_Manager::IMGUI_Show_UI()
{
	return S_OK;
}

HRESULT CImgui_Manager::IMGUI_Show_Shader()
{
	return S_OK;
}




CImgui_Manager* CImgui_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CRenderInstance* pRenderInstance)
{
	CImgui_Manager* pInstance = new CImgui_Manager(pDevice, pContext, pRenderInstance);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CImgui_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CImgui_Manager::Free()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pRenderInstance);

	__super::Free();
}
