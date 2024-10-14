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
	_int index = 1;

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
	//초기 메인카메라 셋팅
	if (m_pMainCamera == nullptr)
	{
		CGameObject* camera = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera"));
		m_pMainCamera = static_cast<CMain_Camera*>(camera);

		Safe_AddRef(m_pMainCamera);
		return;
	}


	// 모델 선택 UI 호출
	IMGUI_Camera_Select_Model(fTimeDelta);

	// 모델이 선택된 경우에만 스킬 선택 UI를 표시
	if (m_iSelected_Model >= 0) {
		ImGui::Spacing();  // 한 줄 띄우기
		ImGui::Separator();  // 경계선 그리기

		// 스킬 선택 UI 호출
		IMGUI_Camera_Select_Skill(fTimeDelta);

		// 스킬이 선택된 경우에만 카메라 선택 UI를 표시
		if (m_iSelected_Skill >= 0) {
			ImGui::Spacing();  // 한 줄 띄우기
			ImGui::Separator();  // 경계선 그리기

			// 카메라 선택 UI 호출
			IMGUI_Show_Camera(fTimeDelta);

			// 포인트 보여주기
			IMGUI_Show_Points();

			// Add_Point 버튼 호출
			// 저장할 때,애초에 CameraPoint에 좌표를 해당 모델의 월드 역행렬을 곱해서 로컬로 넣어야 한다.
			IMGUI_Add_Point();

			// 플레이 버튼
			IMGUI_Play_Button();

			//각 모델과 각 스킬에 연결된 카메라가 가지고 있는 Point를 메모장으로 저장하는 방식
			IMGUI_Save_Button();
		}
	}
}


void CIMGUI_Camera_Tab::IMGUI_Camera_Select_Model(_float fTimeDelta)
{
	// Model selection dropdown
	const char* model_options[] = { "1. Son", "2. Hit", "3. Mine", "4. 21" };
	static int previous_model = -1;  // 이전 모델을 추적하기 위한 변수

	int iSelected_Model = static_cast<int>(m_iSelected_Model);

	ImGui::Text("Select Model");
	if (ImGui::Combo("Model", &iSelected_Model, model_options, IM_ARRAYSIZE(model_options))) {
		// 모델이 변경된 경우에만 스킬 선택을 초기화
		if (m_iSelected_Model != static_cast<MODELID>(iSelected_Model)) {
			previous_model = m_iSelected_Model;  // 이전 모델 업데이트
			m_iSelected_Model = static_cast<MODELID>(iSelected_Model); // 새로운 모델로 업데이트
			m_iSelected_Skill = SKILL_NOT;  // 스킬 선택 초기화
			UpdateCameraSelection(); // 카메라 선택 업데이트
		}
	}

	if (m_iSelected_Model >= 0) {
		// 모델이 선택된 경우 현재 선택된 모델을 출력
		ImGui::Text("Currently Selected Model: %s", model_options[m_iSelected_Model]);
	}
}

void CIMGUI_Camera_Tab::IMGUI_Camera_Select_Skill(_float fTimeDelta)
{
	// 스킬 선택 드롭다운
	const char* skill_options[] = { "Skill 1", "Skill 2", "Skill 3" };
	static int previous_skill = -1;  // 이전 스킬을 추적하기 위한 변수

	int iSelected_Skill = static_cast<int>(m_iSelected_Skill);

	ImGui::Text("Select Skill");
	if (ImGui::Combo("Skill", &iSelected_Skill, skill_options, IM_ARRAYSIZE(skill_options))) {
		// 스킬이 변경된 경우에만 카메라 선택 업데이트
		if (m_iSelected_Skill != static_cast<SKILLID>(iSelected_Skill)) {
			previous_skill = m_iSelected_Skill;  // 이전 스킬 업데이트
			m_iSelected_Skill = static_cast<SKILLID>(iSelected_Skill); // 새로운 스킬로 업데이트
			UpdateCameraSelection(); // 카메라 선택 업데이트
		}
	}

	if (m_iSelected_Skill >= 0) {
		// 스킬이 선택된 경우 현재 선택된 스킬을 출력
		ImGui::Text("Currently Selected Skill: %s", skill_options[m_iSelected_Skill]);
	}
}

void CIMGUI_Camera_Tab::IMGUI_Show_Camera(_float fTimeDelta)
{
	// 모델과 스킬이 모두 선택된 경우에만 카메라 이름을 표시
	if (m_iSelected_Model >= 0 && m_iSelected_Skill >= 0)
	{
		// 게임 인스턴스에서 메인 카메라 객체 가져오기
		CGameObject* camera = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera"));
		if (!camera) {
			ImGui::Text("Main camera not found.");
			return;
		}

		// 현재 선택된 카메라 인덱스 가져오기
		_int cameraIndex = m_pMainCamera->Get_Virtual_Camera();

		// 가상 카메라 목록 가져오기
		vector<CCamera*> cameraList = m_pMainCamera->m_vecVirtualCamera;

		if (cameraIndex < 0 || cameraIndex >= cameraList.size()) {
			ImGui::Text("Invalid camera index selected.");
			return;
		}

		// 선택된 카메라의 이름 가져오기
		const char* selectedCameraName = cameraList[cameraIndex]->GetTabName();

		// 카메라 이름 표시
		ImGui::Text("Selected Camera: %s", selectedCameraName);
	}
}

// 새로운 함수: 선택된 카메라를 활성화하는 함수
void CIMGUI_Camera_Tab::Activate_Select_Camera(_int selectedIndex)
{
	// 가상 카메라 목록 가져오기
	vector<CCamera*>& cameraList = m_pMainCamera->m_vecVirtualCamera;

	if (selectedIndex < 0 || selectedIndex >= cameraList.size()) {
		// 유효하지 않은 인덱스일 경우 처리
		ImGui::Text("Invalid camera index selected.");
		return;
	}

	// 선택된 카메라 활성화
	m_pMainCamera->Set_Virtual_Camera((CMain_Camera::VIRTUAL_CAMERA)selectedIndex);
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


void CIMGUI_Camera_Tab::IMGUI_Show_Points() {
	if (m_iSelected_Model >= 0 && m_iSelected_Skill >= 0) {
		std::vector<CCamera::CameraPoint>& points = m_pMainCamera->Get_VectorPoint();  // 포인트 벡터 가져오기

		if (points.empty()) {
			ImGui::Text("No camera points available.");
			return;
		}

		ImGui::Separator();
		ImGui::Text("Camera Points:");

		for (size_t i = 0; i < points.size(); ++i) {
			ImGui::PushID(static_cast<int>(i));  // 각 포인트에 고유 ID 부여

			bool isSelected = (m_selectedPoint == static_cast<int>(i));
			if (isSelected) {
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));  // 노란색 텍스트
			}

			ImGui::BulletText("Point %d:", static_cast<int>(i) + 1);

			if (isSelected) {
				ImGui::PopStyleColor();  // 텍스트 색상 복원
			}

			ImGui::SameLine();
			ImGui::Dummy(ImVec2(50, 0)); // 50 픽셀의 가로 간격 (필요에 따라 조정)
			ImGui::SameLine();

			// Delete 버튼
			if (ImGui::Button("Delete")) {
				// 삭제 확인 팝업을 띄우려면 추가 구현 필요
				IMGUI_Delete_Point(static_cast<int>(i));
				ImGui::PopID();
				break;  // 삭제 후 루프 종료
			}

			// Modify 버튼
			ImGui::SameLine();
			if (ImGui::Button("Modify")) {
				IMGUI_Modify_Point(static_cast<int>(i));
			}

			// 포인트 정보 표시 (선택 상태에 따라 텍스트 색상 변경)
			if (isSelected) {
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));  // 노란색 텍스트
			}

			CCamera::CameraPoint& point = points[i];
			ImGui::Text("  Position: (%.2f, %.2f, %.2f)", point.position.x, point.position.y, point.position.z);
			ImGui::Text("  Quaternion: (%.2f, %.2f, %.2f)", point.rotation.x, point.rotation.y, point.rotation.z);
			ImGui::Text("  Duration: %.2f", point.duration);
			ImGui::Text("  Interpolation: %s",
				(point.interpolationType == CCamera::InterpolationType::INTERPOLATION_LINEAR_MODE)
				? "Linear"
				: (point.interpolationType == CCamera::InterpolationType::INTERPOLATION_DAMPING_MODE)
				? "Damping"
				: "Skip");

			// **Damping 값 표시 (Damping Mode인 경우)**
			if (point.interpolationType == CCamera::InterpolationType::INTERPOLATION_DAMPING_MODE) {
				ImGui::Text("  Damping: %.2f", point.damping);
			}

			// 선택된 포인트에 대한 수정 UI 표시
			if (isSelected)
			{
				ImGui::PopStyleColor();  // 텍스트 색상 복원
				IMGUI_Modify_Point_UI(static_cast<int>(i));
			}

			ImGui::Separator();
			ImGui::PopID();
		}
	}
}


void CIMGUI_Camera_Tab::IMGUI_Delete_Point(_int index)
{
	// 포인트 삭제
	m_pMainCamera->Remove_Point(index);

	// 선택된 포인트가 삭제된 경우 선택 해제
	if (m_selectedPoint == index) {
		m_selectedPoint = -1;
	}
	else if (m_selectedPoint > index) {
		// 삭제된 포인트 이전의 포인트가 삭제되면, 선택된 포인트의 인덱스가 감소합니다.
		m_selectedPoint -= 1;
	}
}

void CIMGUI_Camera_Tab::IMGUI_Modify_Point(_int index)
{
	if (m_selectedPoint == index) {
		// 이미 선택된 상태라면 선택 해제
		m_selectedPoint = -1;
		m_isEditing = false;
	}
	else {
		// 새로운 포인트 선택
		m_selectedPoint = index;
		m_isEditing = true;

		// 선택된 포인트의 데이터를 임시 변수에 복사
		vector<CCamera::CameraPoint>& points = m_pMainCamera->Get_VectorPoint();
		if (index >= 0 && index < static_cast<int>(points.size())) {
			CCamera::CameraPoint& point = points[index];
			m_tempPointData.duration = point.duration;
			m_tempPointData.interpType = point.interpolationType;
			m_tempPointData.damping = point.damping;
		}

		// 선택된 포인트의 위치와 회전으로 카메라 업데이트
		m_pMainCamera->Move_Point(index);
	}
}

void CIMGUI_Camera_Tab::IMGUI_Modify_Point_UI(_int index)
{
	// 해당 포인트에 대한 참조 가져오기
	std::vector<CCamera::CameraPoint>& points = m_pMainCamera->Get_VectorPoint();

	// 선택된 포인트와 수정 모드인지 확인
	if (!m_isEditing || m_selectedPoint != index) {
		return;
	}

	ImGui::Indent(); // 들여쓰기 시작

	// Duration 수정 (임시 변수에 바인딩)
	ImGui::InputFloat("Duration", &m_tempPointData.duration, 0.1f, 1.0f, "%.2f");

	// Interpolation Type 수정 (임시 변수에 바인딩)
	const char* interp_options[] = { "Linear", "Damping", "Skip" };
	int interpIndex = static_cast<int>(m_tempPointData.interpType);
	if (ImGui::BeginCombo("Interpolation Type", interp_options[interpIndex])) {
		for (int n = 0; n < IM_ARRAYSIZE(interp_options); n++) {
			bool is_selected = (interpIndex == n);
			if (ImGui::Selectable(interp_options[n], is_selected)) {
				m_tempPointData.interpType = static_cast<CCamera::InterpolationType>(n);
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	// Damping 계수 수정 (Damping Mode일 때만 표시)
	if (m_tempPointData.interpType == CCamera::InterpolationType::INTERPOLATION_DAMPING_MODE)
	{
		ImGui::SliderFloat("Damping Coefficient", &m_tempPointData.damping, 0.0f, 2.0f, "%.2f");
	}

	// "Save" 버튼 추가
	if (ImGui::Button("Save")) {
		// 사용자에게 저장됨을 알림
		IMGUI_Point_Modify_Save();
	}

	ImGui::Unindent(); // 들여쓰기 종료
}

void CIMGUI_Camera_Tab::IMGUI_Point_Modify_Save()
{
	// 선택된 포인트 인덱스 확인
	if (m_selectedPoint < 0) {
		return;
	}

	vector<CCamera::CameraPoint>& points = m_pMainCamera->Get_VectorPoint();
	if (m_selectedPoint >= static_cast<int>(points.size())) {
		return;
	}

	// 선택된 포인트에 임시 데이터 적용
	CCamera::CameraPoint& point = points[m_selectedPoint];
	point.duration = m_tempPointData.duration;
	point.interpolationType = m_tempPointData.interpType;
	point.damping = m_tempPointData.damping;

	m_pMainCamera->Modify_Transform(m_selectedPoint);
}

void CIMGUI_Camera_Tab::IMGUI_Add_Point()
{
	// 모델과 스킬이 모두 선택된 경우에만 Add_Point 버튼을 표시
	if (m_iSelected_Model >= 0 && m_iSelected_Skill >= 0)
	{
		// 게임 인스턴스에서 메인 카메라 객체 가져오기
		CGameObject* camera = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera"));
		if (!camera) {
			ImGui::Text("Main camera not found.");
			return;
		}

		// 현재 활성화된 가상 카메라 가져오기
		int cameraIndex = static_cast<int>(m_pMainCamera->Get_Virtual_Camera());

		// 가상 카메라 목록 가져오기
		vector<CCamera*>& cameraList = m_pMainCamera->m_vecVirtualCamera;

		if (cameraIndex < 0 || cameraIndex >= static_cast<int>(cameraList.size())) {
			ImGui::Text("Invalid camera index selected.");
			return;
		}

		// 사용자 입력을 저장할 변수
		static float duration = 1.0f;
		static int selected_interp = 0;
		static float damping = 1.0f; // 기본 Damping 계수
		const char* interp_options[] = { "Linear", "Damping", "Skip" }; // 새로운 모드 추가

		// Add_Point 버튼과 입력 필드 배치
		ImGui::Separator();
		ImGui::Text("Add Camera Point:");
		ImGui::InputFloat("Duration", &duration, 0.1f, 1.0f, "%.2f");

		if (ImGui::BeginCombo("Interpolation Type", interp_options[selected_interp])) {
			for (int n = 0; n < IM_ARRAYSIZE(interp_options); n++) {
				bool is_selected = (selected_interp == n);
				if (ImGui::Selectable(interp_options[n], is_selected))
					selected_interp = n;
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		// Damping 계수 수정 (Damping Mode일 때만 표시)
		if (selected_interp == 1) // "Damping" 선택 시
		{
			ImGui::SliderFloat("Damping Coefficient", &damping, 0.0f, 2.0f, "%.2f");
		}

		// Add_Point 버튼
		if (ImGui::Button("Add Point")) {
			// Interpolation Type 설정
			CCamera::InterpolationType interpType = CCamera::InterpolationType::INTERPOLATION_LINEAR_MODE;
			switch (selected_interp) {
			case 0:
				interpType = CCamera::InterpolationType::INTERPOLATION_LINEAR_MODE;
				break;
			case 1:
				interpType = CCamera::InterpolationType::INTERPOLATION_DAMPING_MODE;
				break;
			case 2:
				interpType = CCamera::InterpolationType::INTERPOLATION_SKIP_MODE;
				break;
			}

			// 해당모델의 Transform에서 월드매트리스 Ptr이 있어야 한다.
			// 각 카메라에 매핑된 모델의 Transform을 가져오는것도 만들긴해야함
			CGameObject* model = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"));
			if (!model) {
				ImGui::TextColored(ImVec4(1, 0, 0, 1), "Model not found.");
				return;
			}
			CTransform* modelTransform = static_cast<CTransform*>(model->Get_Component(TEXT("Com_Transform")));
			if (!modelTransform) {
				ImGui::TextColored(ImVec4(1, 0, 0, 1), "Model Transform component not found.");
				return;
			}
			const _float4x4* worldMatrixPtr = modelTransform->Get_WorldMatrixPtr();

			// 메인 카메라의 Add_Point 함수를 호출하여 포인트 추가
			m_pMainCamera->Add_Point(duration, interpType, worldMatrixPtr, (interpType == CCamera::InterpolationType::INTERPOLATION_DAMPING_MODE) ? damping : 1.0f);

			// 사용자에게 추가됨을 알림
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "Added new camera point.");

			// 입력 필드 초기화
			duration = 1.0f;
			selected_interp = 0;
			damping = 1.0f;
		}
	}
}

void CIMGUI_Camera_Tab::IMGUI_Play_Button()
{
	//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 35);
	ImVec2 button_size = ImVec2(20, 20);
	if (ImGui::InvisibleButton("play_button", button_size))
	{
		//함수 연결
		//현재 카메라 셋팅에서 list를 Play한다.
		m_pMainCamera->Play();
	}

	ImVec2 p = ImGui::GetItemRectMin();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	float size = 20.0f;
	ImVec2 triangle_pos[3] = {
		ImVec2(p.x + (button_size.x - size) * 0.5f, p.y),
		ImVec2(p.x + (button_size.x + size) * 0.5f, p.y + size / 2),
		ImVec2(p.x + (button_size.x - size) * 0.5f, p.y + size)
	};

	ImU32 triangle_color;

	if (ImGui::IsItemHovered())
		triangle_color = IM_COL32(155, 255, 155, 255);
	else
		triangle_color = IM_COL32(85, 255, 85, 255);

	draw_list->AddTriangleFilled(triangle_pos[0], triangle_pos[1], triangle_pos[2], triangle_color);
}


void CIMGUI_Camera_Tab::IMGUI_Save_Button()
{
	if (ImGui::Button("Save"))
	{
		// 저장할 파일명 지정
		std::wstring filename = L"../Bin/CameraPoints.txt";

		CCamera::CameraSaveData saveData = { m_pMainCamera->m_vecVirtualCamera, m_CameraIndexMap };

		// 파일 매니저를 통해 저장
		m_pGameInstance->Save_All_CameraPoints(filename, &saveData);
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
	Safe_Release(m_pMainCamera);
	__super::Free();
}
