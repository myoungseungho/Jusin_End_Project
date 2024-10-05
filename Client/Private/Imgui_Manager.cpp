#include "stdafx.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Imgui_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CImgui_Manager)

bool bShowImGuiWindows = true;

CImgui_Manager::CImgui_Manager()
{
}
// IMGUI 창 표시 여부를 제어하는 전역 변수
CImgui_Manager::CImgui_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice{ pDevice }, m_pContext{ pContext }

{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}


HRESULT CImgui_Manager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CImgui_Manager::Initialize(void* pArg)
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
	//->Add_RenderObject(CRenderer::RG_IMGUI, this);
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
		static bool bShowAnimation = true;
		static bool bShowEffect = true;
		static bool bShowUI = true;
		static bool bShowShader = true;


		ImGui::Begin("Main Tab", &bShowImGuiWindows); // 메인 창 시작
		if (ImGui::BeginTabBar("DragonBall_Tool")) { // 탭 바 시작

			if (ImGui::BeginTabItem("Animation", &bShowAnimation)) { // "Levels" 탭 아이템 시작
				//레벨 창
				if (bShowAnimation) {
					IMGUI_Show_Animation();
				}
				ImGui::EndTabItem(); // "Levels" 탭 아이템 종료
			}

			if (ImGui::BeginTabItem("Effect", &bShowEffect)) { // "ObjectList" 탭 아이템 시작
				//원형 오브젝트 창
				if (bShowEffect) {
					IMGUI_Show_Effect();
				}
				ImGui::EndTabItem(); // "ObjectList" 탭 아이템 종료
			}

			if (ImGui::BeginTabItem("UI", &bShowUI)) { // "ObjectList" 탭 아이템 시작
				//원형 오브젝트 창
				if (bShowUI) {
					IMGUI_Show_UI();
				}
				ImGui::EndTabItem(); // "ObjectList" 탭 아이템 종료
			}

			if (ImGui::BeginTabItem("Shader", &bShowShader)) { // "Settings" 탭 아이템 시작
				if (bShowShader) {
					IMGUI_Show_Shader();
				}
				ImGui::EndTabItem(); // "ObjectList" 탭 아이템 종료
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




CImgui_Manager* CImgui_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CImgui_Manager* pInstance = new CImgui_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
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

	__super::Free();
}
