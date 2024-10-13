#include "stdafx.h"
#include "IMGUI_Camera_Tab.h"
#include "GameInstance.h"
#include "RenderInstance.h"
#include "Main_Camera.h"

_bool bShowCameraWindow = true;  // IMGUI 창 표시 여부를 제어하는 전역 변수


CIMGUI_Camera_Tab::CIMGUI_Camera_Tab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CIMGUI_Tab{ pDevice,pContext }
{
}

HRESULT CIMGUI_Camera_Tab::Initialize()
{
	return S_OK;
}

void CIMGUI_Camera_Tab::IMGUI_Camera_Select_Model(_float fTimeDelta)
{
	// Model selection dropdown
	const char* model_options[] = { "1. Son", "2. Hit", "3. Mine", "4. 21" };
	static int previous_model = -1;  // 이전 모델을 추적하기 위한 변수

	ImGui::Text("Select Model");
	if (ImGui::Combo("Model", &m_iSelected_Model, model_options, IM_ARRAYSIZE(model_options))) {
		// 모델이 변경된 경우에만 스킬 선택을 초기화
		if (m_iSelected_Model != previous_model) {
			previous_model = m_iSelected_Model;  // 새로운 모델로 업데이트
			m_iSelected_Skill = -1;  // 스킬 선택 초기화
		}
	}

	if (m_iSelected_Model >= 0) {
		// 모델이 선택된 경우 현재 선택된 모델을 출력
		ImGui::Text("Currently Selected Model: %s", model_options[m_iSelected_Model]);
	}
}

void CIMGUI_Camera_Tab::IMGUI_Camera_Select_Skill(_float fTimeDelta)
{
	// 예시로 스킬 선택 부분을 작성
	const char* skill_options[] = { "Skill 1", "Skill 2", "Skill 3" };

	ImGui::Text("Select Skill");
	if (ImGui::Combo("Skill", &m_iSelected_Skill, skill_options, IM_ARRAYSIZE(skill_options))) {
		// 스킬 선택에 따른 처리
	}

	if (m_iSelected_Skill >= 0) {
		// 스킬이 선택된 경우 현재 선택된 스킬을 출력
		ImGui::Text("Currently Selected Skill: %s", skill_options[m_iSelected_Skill]);
	}
}

void CIMGUI_Camera_Tab::Open_Select_Camera(_float fTimeDelta)
{
	if (bShowCameraWindow) {
		ImGui::Begin("Camera_Select", &bShowCameraWindow);  // 새 창 생성

		IMGUI_Camera_Select(fTimeDelta);  // 모델 선택 UI

		ImGui::End();  // 새 창 종료
	}
}

void CIMGUI_Camera_Tab::IMGUI_Camera_Select(_float fTimeDelta)
{
	// 게임 인스턴스에서 메인 카메라 객체 가져오기
	CGameObject* camera = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera"));
	CMain_Camera* main_Camera = static_cast<CMain_Camera*>(camera);

	// 가상 카메라 목록 가져오기
	vector<CCamera*> cameraList = main_Camera->m_vecVirtualCamera;

	// 카메라 이름을 보관할 임시 배열 생성
	vector<const _char*> cameraNames;

	for (auto& iter : cameraList) {
		cameraNames.push_back(iter->GetTabName()); // 각 카메라의 이름을 배열에 저장
	}

	// 현재 선택된 카메라 인덱스를 추적할 정적 변수
	static _int selectedCameraIndex = (_int)main_Camera->Get_Virtual_Camera();

	if (ImGui::Combo("Camera List", &selectedCameraIndex, cameraNames.data(), cameraNames.size())) {
		// 선택된 카메라의 인덱스에 따라 활성화 함수 호출
		Activate_Select_Camera(selectedCameraIndex, cameraList);
	}
}

// 새로운 함수: 선택된 카메라를 활성화하는 함수
void CIMGUI_Camera_Tab::Activate_Select_Camera(_int selectedIndex, const vector<CCamera*>& cameraList)
{
	if (selectedIndex < 0 || selectedIndex >= cameraList.size()) {
		return;  // 유효하지 않은 인덱스일 경우 처리하지 않음
	}

	//메인카메라 가져오기, 나중에 이건 다 최적화해야함
	CGameObject* camera = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera"));
	CMain_Camera* main_Camera = static_cast<CMain_Camera*>(camera);

	// 선택된 카메라 가져오기
	CCamera* selectedCamera = *next(cameraList.begin(), selectedIndex);
	// 선택된 카메라 셋팅하기
	main_Camera->Set_Virtual_Camera((CMain_Camera::VIRTUAL_CAMERA)selectedIndex);
}

void CIMGUI_Camera_Tab::Render(_float fTimeDelta)
{
	// 모델 선택 UI 호출
	IMGUI_Camera_Select_Model(fTimeDelta);

	// 모델이 선택된 경우에만 스킬 선택 UI를 표시
	if (m_iSelected_Model >= 0) {
		ImGui::Spacing();  // 한 줄 띄우기
		ImGui::Separator();  // 경계선 그리기

		// 스킬 선택 UI 호출
		IMGUI_Camera_Select_Skill(fTimeDelta);
	}

	// 새 창 표시 함수 호출
	Open_Select_Camera(fTimeDelta);
}


CIMGUI_Camera_Tab* CIMGUI_Camera_Tab::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIMGUI_Camera_Tab* pInstance = new CIMGUI_Camera_Tab(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CIMGUI_Camera_Tab"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIMGUI_Camera_Tab::Free()
{
	__super::Free();
}
