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

		vector<CCamera::CameraPoint>& points = m_pMainCamera->Get_VectorPoint();  // ����Ʈ ���� ��������

		if (points.empty()) {
			ImGui::Text("No camera points available.");
			return;
		}

		ImGui::Separator();
		ImGui::Text("Camera Points:");

		for (size_t i = 0; i < points.size(); ++i) {
			ImGui::PushID(static_cast<int>(i));  // �� ����Ʈ�� ���� ID �ο�

			bool isSelected = (m_selectedPoints.find(i) != m_selectedPoints.end());
			if (isSelected) {
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));  // ����� �ؽ�Ʈ
			}

			ImGui::BulletText("Point %d:", static_cast<int>(i) + 1);

			if (isSelected) {
				ImGui::PopStyleColor();  // �ؽ�Ʈ ���� ����
			}

			ImGui::SameLine();
			ImGui::Dummy(ImVec2(50, 0));
			ImGui::SameLine();

			// Delete ��ư
			if (ImGui::Button("Delete")) {
				points.erase(points.begin() + i);  // ���Ϳ��� ����
				ImGui::PopID();
				break;  // ���� �� ���� ����
			}

			ImGui::SameLine();
			if (ImGui::Button("Modify")) {
				IMGUI_Modify_Point(static_cast<int>(i));
			}

			if (isSelected) {
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));  // ����� �ؽ�Ʈ
			}

			const auto& point = points[i];
			ImGui::Text("  Position: (%.2f, %.2f, %.2f)", point.position.x, point.position.y, point.position.z);
			ImGui::Text("  Quaternion: (%.2f, %.2f, %.2f)", point.rotation.x, point.rotation.y, point.rotation.z);
			ImGui::Text("  Duration: %.2f", point.duration);

			ImGui::Text("  Interpolation: %s",
				(point.interpolationType == CCamera::InterpolationType::INTERPOLATION_LINEAR_MODE)
				? "Linear"
				: (point.interpolationType == CCamera::InterpolationType::INTERPOLATION_SPLINE_MODE)
				? "Spline"
				: "Skip");

			if (isSelected) {
				ImGui::PopStyleColor();  // �ؽ�Ʈ ���� ����
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

	// ���õ� ��Ͽ��� ����
	m_selectedPoints.erase(index);
}

void CIMGUI_Camera_Tab::IMGUI_Modify_Point(_int index)
{
	// ���� ���� ���
	if (m_selectedPoints.find(index) != m_selectedPoints.end()) {
		// �̹� ���õ� ���¶�� ���� ����
		m_selectedPoints.erase(index);
	}
	else {
		// ���õ��� ���� ���¶�� ���� �߰�
		m_selectedPoints.insert(index);

		//�ش� Modify_Point�� ������
		//����ī�޶󿡼� ���õ� ����ī�޶��� Point�� index�� �ִ� ��ġ�� �����̼����� �������� ������Ʈ��
		m_pMainCamera->Move_Point(index);
	}
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
		const char* interp_options[] = { "Linear", "Spline", "Skip" }; // ���ο� ��� �߰�

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

		// Add_Point ��ư
		if (ImGui::Button("Add Point")) {
			CCamera::InterpolationType interpType = CCamera::InterpolationType::INTERPOLATION_LINEAR_MODE;
			switch (selected_interp) {
			case 0:
				interpType = CCamera::InterpolationType::INTERPOLATION_LINEAR_MODE;
				break;
			case 1:
				interpType = CCamera::InterpolationType::INTERPOLATION_SPLINE_MODE;
				break;
			case 2:
				interpType = CCamera::InterpolationType::INTERPOLATION_SKIP_MODE;
				break;
			}

			// �ش���� Transform���� �����Ʈ���� Ptr�� �־�� �Ѵ�.
			// �� ī�޶� ���ε� ���� Transform�� �������°͵� ������ؾ���
			CGameObject* model = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"));
			const _float4x4* worldMatrixPtr = static_cast<CTransform*>(model->Get_Component(TEXT("Com_Transform")))->Get_WorldMatrixPtr();

			// ���� ī�޶��� Add_Point �Լ��� ȣ���Ͽ� ����Ʈ �߰�
			m_pMainCamera->Add_Point(duration, interpType, worldMatrixPtr);

			// ����ڿ��� �߰����� �˸�
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "Added new camera point.");
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
