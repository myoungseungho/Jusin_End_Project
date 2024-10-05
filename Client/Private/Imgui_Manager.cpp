#include "stdafx.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Imgui_Manager.h"
#include "GameInstance.h"
#include "RenderInstance.h"

bool bShowImGuiWindows = true;  // IMGUI â ǥ�� ���θ� �����ϴ� ���� ����

IMPLEMENT_SINGLETON(CImgui_Manager)

CImgui_Manager::CImgui_Manager()
{
}
// IMGUI â ǥ�� ���θ� �����ϴ� ���� ����
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
	// ��� ��ư
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Option")) {
			if (ImGui::MenuItem("IMGUI ON", NULL, &bShowImGuiWindows)) {
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (bShowImGuiWindows) {  // �� ������ ���� ��� ImGui â�� ǥ�� ���θ� ����

		//���̴�
		//����Ʈ
		//�ִϸ��̼�
		//UI

		static _bool bShowAnimation = true;
		static _bool bShowEffect = true;
		static _bool bShowUI = true;
		static _bool bShowShader = true;

		ImGui::Begin("Main Tab", &bShowImGuiWindows); // ���� â ����
		if (ImGui::BeginTabBar("DragonBall_Tool")) { // �� �� ����

			if (ImGui::BeginTabItem("Animation", &bShowAnimation)) { // "Levels" �� ������ ����
				//���� â
				if (bShowAnimation) {
					IMGUI_Show_Animation();
				}
				ImGui::EndTabItem(); // "Animation" �� ������ ����
			}

			if (ImGui::BeginTabItem("Effect", &bShowEffect)) { // "ObjectList" �� ������ ����
				//���� ������Ʈ â
				if (bShowEffect) {
					IMGUI_Show_Effect();
				}
				ImGui::EndTabItem(); // "Effect" �� ������ ����
			}

			if (ImGui::BeginTabItem("UI", &bShowUI)) { // "ObjectList" �� ������ ����
				//���� ������Ʈ â
				if (bShowUI) {
					IMGUI_Show_UI();
				}
				ImGui::EndTabItem(); // "UI" �� ������ ����
			}

			if (ImGui::BeginTabItem("Shader", &bShowShader)) { // "Shader" �� ������ ����
				if (bShowShader) {
					IMGUI_Show_Shader();
				}
				ImGui::EndTabItem(); // "Shader" �� ������ ����
			}

			ImGui::EndTabBar(); // �� �� ����
		}
		ImGui::End(); // ���� â ����
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
