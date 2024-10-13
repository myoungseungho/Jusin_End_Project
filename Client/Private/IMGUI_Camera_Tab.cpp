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
	_int index = 0;

	// 모델과 스킬 ID에 따른 카메라 인덱스 매핑 초기화
	m_CameraIndexMap[{MODELID_SON, SKILL1}] = index++; // 손오공, 스킬1 -> 카메라 인덱스 1
	m_CameraIndexMap[{MODELID_SON, SKILL2}] = index++; // 손오공, 스킬2 -> 카메라 인덱스 2
	m_CameraIndexMap[{MODELID_SON, SKILL3}] = index++; // 손오공, 스킬3 -> 카메라 인덱스 3

	m_CameraIndexMap[{MODELID_HIT, SKILL1}] = index++;
	m_CameraIndexMap[{MODELID_HIT, SKILL2}] = index++;
	m_CameraIndexMap[{MODELID_HIT, SKILL3}] = index++;

	m_CameraIndexMap[{MODELID_MINE, SKILL1}] = index++;
	m_CameraIndexMap[{MODELID_MINE, SKILL2}] = index++;
	m_CameraIndexMap[{MODELID_MINE, SKILL3}] = index++;

	m_CameraIndexMap[{MODELID_21, SKILL1}] = index++;
	m_CameraIndexMap[{MODELID_21, SKILL2}] = index++;
	m_CameraIndexMap[{MODELID_21, SKILL3}] = index;

	return S_OK;
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


void CIMGUI_Camera_Tab::IMGUI_Camera_Select_Model(_float fTimeDelta)
{
	// Model selection dropdown
	const char* model_options[] = { "1. Son", "2. Hit", "3. Mine", "4. 21" };
	static int previous_model = -1;  // 이전 모델을 추적하기 위한 변수

	_int iSelected_Model = (MODELID)m_iSelected_Model;

	ImGui::Text("Select Model");
	if (ImGui::Combo("Model", &iSelected_Model, model_options, IM_ARRAYSIZE(model_options))) {
		// 모델이 변경된 경우에만 스킬 선택을 초기화

		m_iSelected_Model = (MODELID)iSelected_Model;
		if (m_iSelected_Model != previous_model) {
			previous_model = m_iSelected_Model;  // 새로운 모델로 업데이트
			m_iSelected_Skill = SKILL_NOT;  // 스킬 선택 초기화
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

	_int iSelected_Skill = (SKILLID)m_iSelected_Skill;

	ImGui::Text("Select Skill");
	if (ImGui::Combo("Skill", &iSelected_Skill, skill_options, IM_ARRAYSIZE(skill_options))) {
		m_iSelected_Skill = (SKILLID)iSelected_Skill;
		// 스킬 선택에 따른 처리
		UpdateCameraSelection(); // 카메라 선택 업데이트
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
		Activate_Select_Camera(selectedCameraIndex);
	}
}

// 새로운 함수: 선택된 카메라를 활성화하는 함수
void CIMGUI_Camera_Tab::Activate_Select_Camera(_int selectedIndex)
{
	// 메인 카메라 가져오기
	CGameObject* camera = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera"));
	CMain_Camera* main_Camera = static_cast<CMain_Camera*>(camera);

	// 가상 카메라 목록 가져오기
	vector<CCamera*>& cameraList = main_Camera->m_vecVirtualCamera;

	if (selectedIndex < 0 || selectedIndex >= cameraList.size()) {
		// 유효하지 않은 인덱스일 경우 처리
		ImGui::Text("Invalid camera index selected.");
		return;
	}

	// 선택된 카메라 활성화
	main_Camera->Set_Virtual_Camera((CMain_Camera::VIRTUAL_CAMERA)selectedIndex);
}

void CIMGUI_Camera_Tab::UpdateCameraSelection()
{
	if (m_iSelected_Model >= 0 && m_iSelected_Skill >= 0)
	{
		auto key = std::make_pair(m_iSelected_Model, m_iSelected_Skill);
		auto it = m_CameraIndexMap.find(key);

		if (it != m_CameraIndexMap.end())
		{
			_int cameraIndex = it->second;
			Activate_Select_Camera((_int)cameraIndex);
		}
		else
		{
			// 매핑된 카메라가 없을 경우 처리 (예: 기본 카메라 선택 또는 오류 메시지 표시)
			ImGui::Text("No camera mapped for this model and skill combination.");
		}
	}
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
