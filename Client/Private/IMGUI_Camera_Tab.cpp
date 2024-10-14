#include "stdafx.h"
#include "IMGUI_Camera_Tab.h"
#include "GameInstance.h"
#include "RenderInstance.h"
#include "Main_Camera.h"

_bool bShowCameraWindow = true;  // IMGUI â ǥ�� ���θ� �����ϴ� ���� ����


CIMGUI_Camera_Tab::CIMGUI_Camera_Tab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CIMGUI_Tab{ pDevice,pContext }
{
}

HRESULT CIMGUI_Camera_Tab::Initialize()
{
	_int index = 1;

	// �𵨰� ��ų ID�� ���� ī�޶� �ε��� ���� �ʱ�ȭ
	m_CameraIndexMap[{MODELID_SON, SKILL1}] = index++; // �տ���, ��ų1 -> ī�޶� �ε��� 1
	m_CameraIndexMap[{MODELID_SON, SKILL2}] = index++; // �տ���, ��ų2 -> ī�޶� �ε��� 2
	m_CameraIndexMap[{MODELID_SON, SKILL3}] = index++; // �տ���, ��ų3 -> ī�޶� �ε��� 3

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
	//�ʱ� ����ī�޶� ����
	if (m_pMainCamera == nullptr)
	{
		CGameObject* camera = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera"));
		m_pMainCamera = static_cast<CMain_Camera*>(camera);

		Safe_AddRef(m_pMainCamera);
		return;
	}


	// �� ���� UI ȣ��
	IMGUI_Camera_Select_Model(fTimeDelta);

	// ���� ���õ� ��쿡�� ��ų ���� UI�� ǥ��
	if (m_iSelected_Model >= 0) {
		ImGui::Spacing();  // �� �� ����
		ImGui::Separator();  // ��輱 �׸���

		// ��ų ���� UI ȣ��
		IMGUI_Camera_Select_Skill(fTimeDelta);

		// ��ų�� ���õ� ��쿡�� ī�޶� ���� UI�� ǥ��
		if (m_iSelected_Skill >= 0) {
			ImGui::Spacing();  // �� �� ����
			ImGui::Separator();  // ��輱 �׸���

			// ī�޶� ���� UI ȣ��
			IMGUI_Show_Camera(fTimeDelta);

			// ����Ʈ �����ֱ�
			IMGUI_Show_Points();

			// Add_Point ��ư ȣ��
			// ������ ��,���ʿ� CameraPoint�� ��ǥ�� �ش� ���� ���� ������� ���ؼ� ���÷� �־�� �Ѵ�.
			IMGUI_Add_Point();

			// �÷��� ��ư
			IMGUI_Play_Button();

			//�� �𵨰� �� ��ų�� ����� ī�޶� ������ �ִ� Point�� �޸������� �����ϴ� ���
			IMGUI_Save_Button();
		}
	}
}


void CIMGUI_Camera_Tab::IMGUI_Camera_Select_Model(_float fTimeDelta)
{
	// Model selection dropdown
	const char* model_options[] = { "1. Son", "2. Hit", "3. Mine", "4. 21" };
	static int previous_model = -1;  // ���� ���� �����ϱ� ���� ����

	int iSelected_Model = static_cast<int>(m_iSelected_Model);

	ImGui::Text("Select Model");
	if (ImGui::Combo("Model", &iSelected_Model, model_options, IM_ARRAYSIZE(model_options))) {
		// ���� ����� ��쿡�� ��ų ������ �ʱ�ȭ
		if (m_iSelected_Model != static_cast<MODELID>(iSelected_Model)) {
			previous_model = m_iSelected_Model;  // ���� �� ������Ʈ
			m_iSelected_Model = static_cast<MODELID>(iSelected_Model); // ���ο� �𵨷� ������Ʈ
			m_iSelected_Skill = SKILL_NOT;  // ��ų ���� �ʱ�ȭ
			UpdateCameraSelection(); // ī�޶� ���� ������Ʈ
		}
	}

	if (m_iSelected_Model >= 0) {
		// ���� ���õ� ��� ���� ���õ� ���� ���
		ImGui::Text("Currently Selected Model: %s", model_options[m_iSelected_Model]);
	}
}

void CIMGUI_Camera_Tab::IMGUI_Camera_Select_Skill(_float fTimeDelta)
{
	// ��ų ���� ��Ӵٿ�
	const char* skill_options[] = { "Skill 1", "Skill 2", "Skill 3" };
	static int previous_skill = -1;  // ���� ��ų�� �����ϱ� ���� ����

	int iSelected_Skill = static_cast<int>(m_iSelected_Skill);

	ImGui::Text("Select Skill");
	if (ImGui::Combo("Skill", &iSelected_Skill, skill_options, IM_ARRAYSIZE(skill_options))) {
		// ��ų�� ����� ��쿡�� ī�޶� ���� ������Ʈ
		if (m_iSelected_Skill != static_cast<SKILLID>(iSelected_Skill)) {
			previous_skill = m_iSelected_Skill;  // ���� ��ų ������Ʈ
			m_iSelected_Skill = static_cast<SKILLID>(iSelected_Skill); // ���ο� ��ų�� ������Ʈ
			UpdateCameraSelection(); // ī�޶� ���� ������Ʈ
		}
	}

	if (m_iSelected_Skill >= 0) {
		// ��ų�� ���õ� ��� ���� ���õ� ��ų�� ���
		ImGui::Text("Currently Selected Skill: %s", skill_options[m_iSelected_Skill]);
	}
}

void CIMGUI_Camera_Tab::IMGUI_Show_Camera(_float fTimeDelta)
{
	// �𵨰� ��ų�� ��� ���õ� ��쿡�� ī�޶� �̸��� ǥ��
	if (m_iSelected_Model >= 0 && m_iSelected_Skill >= 0)
	{
		// ���� �ν��Ͻ����� ���� ī�޶� ��ü ��������
		CGameObject* camera = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera"));
		if (!camera) {
			ImGui::Text("Main camera not found.");
			return;
		}

		// ���� ���õ� ī�޶� �ε��� ��������
		_int cameraIndex = m_pMainCamera->Get_Virtual_Camera();

		// ���� ī�޶� ��� ��������
		vector<CCamera*> cameraList = m_pMainCamera->m_vecVirtualCamera;

		if (cameraIndex < 0 || cameraIndex >= cameraList.size()) {
			ImGui::Text("Invalid camera index selected.");
			return;
		}

		// ���õ� ī�޶��� �̸� ��������
		const char* selectedCameraName = cameraList[cameraIndex]->GetTabName();

		// ī�޶� �̸� ǥ��
		ImGui::Text("Selected Camera: %s", selectedCameraName);
	}
}

// ���ο� �Լ�: ���õ� ī�޶� Ȱ��ȭ�ϴ� �Լ�
void CIMGUI_Camera_Tab::Activate_Select_Camera(_int selectedIndex)
{
	// ���� ī�޶� ��� ��������
	vector<CCamera*>& cameraList = m_pMainCamera->m_vecVirtualCamera;

	if (selectedIndex < 0 || selectedIndex >= cameraList.size()) {
		// ��ȿ���� ���� �ε����� ��� ó��
		ImGui::Text("Invalid camera index selected.");
		return;
	}

	// ���õ� ī�޶� Ȱ��ȭ
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
			// ���ε� ī�޶� ���� ��� ó�� (��: �⺻ ī�޶� ���� �Ǵ� ���� �޽��� ǥ��)
			ImGui::Text("No camera mapped for this model and skill combination.");
		}
	}
}


void CIMGUI_Camera_Tab::IMGUI_Show_Points() {
	if (m_iSelected_Model >= 0 && m_iSelected_Skill >= 0) {
		std::vector<CCamera::CameraPoint>& points = m_pMainCamera->Get_VectorPoint();  // ����Ʈ ���� ��������

		if (points.empty()) {
			ImGui::Text("No camera points available.");
			return;
		}

		ImGui::Separator();
		ImGui::Text("Camera Points:");

		for (size_t i = 0; i < points.size(); ++i) {
			ImGui::PushID(static_cast<int>(i));  // �� ����Ʈ�� ���� ID �ο�

			bool isSelected = (m_selectedPoint == static_cast<int>(i));
			if (isSelected) {
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));  // ����� �ؽ�Ʈ
			}

			ImGui::BulletText("Point %d:", static_cast<int>(i) + 1);

			if (isSelected) {
				ImGui::PopStyleColor();  // �ؽ�Ʈ ���� ����
			}

			ImGui::SameLine();
			ImGui::Dummy(ImVec2(50, 0)); // 50 �ȼ��� ���� ���� (�ʿ信 ���� ����)
			ImGui::SameLine();

			// Delete ��ư
			if (ImGui::Button("Delete")) {
				// ���� Ȯ�� �˾��� ������ �߰� ���� �ʿ�
				IMGUI_Delete_Point(static_cast<int>(i));
				ImGui::PopID();
				break;  // ���� �� ���� ����
			}

			// Modify ��ư
			ImGui::SameLine();
			if (ImGui::Button("Modify")) {
				IMGUI_Modify_Point(static_cast<int>(i));
			}

			// ����Ʈ ���� ǥ�� (���� ���¿� ���� �ؽ�Ʈ ���� ����)
			if (isSelected) {
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));  // ����� �ؽ�Ʈ
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

			// **Damping �� ǥ�� (Damping Mode�� ���)**
			if (point.interpolationType == CCamera::InterpolationType::INTERPOLATION_DAMPING_MODE) {
				ImGui::Text("  Damping: %.2f", point.damping);
			}

			// ���õ� ����Ʈ�� ���� ���� UI ǥ��
			if (isSelected)
			{
				ImGui::PopStyleColor();  // �ؽ�Ʈ ���� ����
				IMGUI_Modify_Point_UI(static_cast<int>(i));
			}

			ImGui::Separator();
			ImGui::PopID();
		}
	}
}


void CIMGUI_Camera_Tab::IMGUI_Delete_Point(_int index)
{
	// ����Ʈ ����
	m_pMainCamera->Remove_Point(index);

	// ���õ� ����Ʈ�� ������ ��� ���� ����
	if (m_selectedPoint == index) {
		m_selectedPoint = -1;
	}
	else if (m_selectedPoint > index) {
		// ������ ����Ʈ ������ ����Ʈ�� �����Ǹ�, ���õ� ����Ʈ�� �ε����� �����մϴ�.
		m_selectedPoint -= 1;
	}
}

void CIMGUI_Camera_Tab::IMGUI_Modify_Point(_int index)
{
	if (m_selectedPoint == index) {
		// �̹� ���õ� ���¶�� ���� ����
		m_selectedPoint = -1;
		m_isEditing = false;
	}
	else {
		// ���ο� ����Ʈ ����
		m_selectedPoint = index;
		m_isEditing = true;

		// ���õ� ����Ʈ�� �����͸� �ӽ� ������ ����
		vector<CCamera::CameraPoint>& points = m_pMainCamera->Get_VectorPoint();
		if (index >= 0 && index < static_cast<int>(points.size())) {
			CCamera::CameraPoint& point = points[index];
			m_tempPointData.duration = point.duration;
			m_tempPointData.interpType = point.interpolationType;
			m_tempPointData.damping = point.damping;
		}

		// ���õ� ����Ʈ�� ��ġ�� ȸ������ ī�޶� ������Ʈ
		m_pMainCamera->Move_Point(index);
	}
}

void CIMGUI_Camera_Tab::IMGUI_Modify_Point_UI(_int index)
{
	// �ش� ����Ʈ�� ���� ���� ��������
	std::vector<CCamera::CameraPoint>& points = m_pMainCamera->Get_VectorPoint();

	// ���õ� ����Ʈ�� ���� ������� Ȯ��
	if (!m_isEditing || m_selectedPoint != index) {
		return;
	}

	ImGui::Indent(); // �鿩���� ����

	// Duration ���� (�ӽ� ������ ���ε�)
	ImGui::InputFloat("Duration", &m_tempPointData.duration, 0.1f, 1.0f, "%.2f");

	// Interpolation Type ���� (�ӽ� ������ ���ε�)
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

	// Damping ��� ���� (Damping Mode�� ���� ǥ��)
	if (m_tempPointData.interpType == CCamera::InterpolationType::INTERPOLATION_DAMPING_MODE)
	{
		ImGui::SliderFloat("Damping Coefficient", &m_tempPointData.damping, 0.0f, 2.0f, "%.2f");
	}

	// "Save" ��ư �߰�
	if (ImGui::Button("Save")) {
		// ����ڿ��� ������� �˸�
		IMGUI_Point_Modify_Save();
	}

	ImGui::Unindent(); // �鿩���� ����
}

void CIMGUI_Camera_Tab::IMGUI_Point_Modify_Save()
{
	// ���õ� ����Ʈ �ε��� Ȯ��
	if (m_selectedPoint < 0) {
		return;
	}

	vector<CCamera::CameraPoint>& points = m_pMainCamera->Get_VectorPoint();
	if (m_selectedPoint >= static_cast<int>(points.size())) {
		return;
	}

	// ���õ� ����Ʈ�� �ӽ� ������ ����
	CCamera::CameraPoint& point = points[m_selectedPoint];
	point.duration = m_tempPointData.duration;
	point.interpolationType = m_tempPointData.interpType;
	point.damping = m_tempPointData.damping;

	m_pMainCamera->Modify_Transform(m_selectedPoint);
}

void CIMGUI_Camera_Tab::IMGUI_Add_Point()
{
	// �𵨰� ��ų�� ��� ���õ� ��쿡�� Add_Point ��ư�� ǥ��
	if (m_iSelected_Model >= 0 && m_iSelected_Skill >= 0)
	{
		// ���� �ν��Ͻ����� ���� ī�޶� ��ü ��������
		CGameObject* camera = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera"));
		if (!camera) {
			ImGui::Text("Main camera not found.");
			return;
		}

		// ���� Ȱ��ȭ�� ���� ī�޶� ��������
		int cameraIndex = static_cast<int>(m_pMainCamera->Get_Virtual_Camera());

		// ���� ī�޶� ��� ��������
		vector<CCamera*>& cameraList = m_pMainCamera->m_vecVirtualCamera;

		if (cameraIndex < 0 || cameraIndex >= static_cast<int>(cameraList.size())) {
			ImGui::Text("Invalid camera index selected.");
			return;
		}

		// ����� �Է��� ������ ����
		static float duration = 1.0f;
		static int selected_interp = 0;
		static float damping = 1.0f; // �⺻ Damping ���
		const char* interp_options[] = { "Linear", "Damping", "Skip" }; // ���ο� ��� �߰�

		// Add_Point ��ư�� �Է� �ʵ� ��ġ
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

		// Damping ��� ���� (Damping Mode�� ���� ǥ��)
		if (selected_interp == 1) // "Damping" ���� ��
		{
			ImGui::SliderFloat("Damping Coefficient", &damping, 0.0f, 2.0f, "%.2f");
		}

		// Add_Point ��ư
		if (ImGui::Button("Add Point")) {
			// Interpolation Type ����
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

			// �ش���� Transform���� �����Ʈ���� Ptr�� �־�� �Ѵ�.
			// �� ī�޶� ���ε� ���� Transform�� �������°͵� ������ؾ���
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

			// ���� ī�޶��� Add_Point �Լ��� ȣ���Ͽ� ����Ʈ �߰�
			m_pMainCamera->Add_Point(duration, interpType, worldMatrixPtr, (interpType == CCamera::InterpolationType::INTERPOLATION_DAMPING_MODE) ? damping : 1.0f);

			// ����ڿ��� �߰����� �˸�
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "Added new camera point.");

			// �Է� �ʵ� �ʱ�ȭ
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
		//�Լ� ����
		//���� ī�޶� ���ÿ��� list�� Play�Ѵ�.
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
		// ������ ���ϸ� ����
		std::wstring filename = L"../Bin/CameraPoints.txt";

		CCamera::CameraSaveData saveData = { m_pMainCamera->m_vecVirtualCamera, m_CameraIndexMap };

		// ���� �Ŵ����� ���� ����
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
