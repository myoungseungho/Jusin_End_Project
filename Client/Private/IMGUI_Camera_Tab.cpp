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
	_int index = 0;

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
	// �� ���� UI ȣ��
	IMGUI_Camera_Select_Model(fTimeDelta);

	// ���� ���õ� ��쿡�� ��ų ���� UI�� ǥ��
	if (m_iSelected_Model >= 0) {
		ImGui::Spacing();  // �� �� ����
		ImGui::Separator();  // ��輱 �׸���

		// ��ų ���� UI ȣ��
		IMGUI_Camera_Select_Skill(fTimeDelta);
	}

	// �� â ǥ�� �Լ� ȣ��
	Open_Select_Camera(fTimeDelta);
}


void CIMGUI_Camera_Tab::IMGUI_Camera_Select_Model(_float fTimeDelta)
{
	// Model selection dropdown
	const char* model_options[] = { "1. Son", "2. Hit", "3. Mine", "4. 21" };
	static int previous_model = -1;  // ���� ���� �����ϱ� ���� ����

	_int iSelected_Model = (MODELID)m_iSelected_Model;

	ImGui::Text("Select Model");
	if (ImGui::Combo("Model", &iSelected_Model, model_options, IM_ARRAYSIZE(model_options))) {
		// ���� ����� ��쿡�� ��ų ������ �ʱ�ȭ

		m_iSelected_Model = (MODELID)iSelected_Model;
		if (m_iSelected_Model != previous_model) {
			previous_model = m_iSelected_Model;  // ���ο� �𵨷� ������Ʈ
			m_iSelected_Skill = SKILL_NOT;  // ��ų ���� �ʱ�ȭ
		}
	}

	if (m_iSelected_Model >= 0) {
		// ���� ���õ� ��� ���� ���õ� ���� ���
		ImGui::Text("Currently Selected Model: %s", model_options[m_iSelected_Model]);
	}
}

void CIMGUI_Camera_Tab::IMGUI_Camera_Select_Skill(_float fTimeDelta)
{
	// ���÷� ��ų ���� �κ��� �ۼ�
	const char* skill_options[] = { "Skill 1", "Skill 2", "Skill 3" };

	_int iSelected_Skill = (SKILLID)m_iSelected_Skill;

	ImGui::Text("Select Skill");
	if (ImGui::Combo("Skill", &iSelected_Skill, skill_options, IM_ARRAYSIZE(skill_options))) {
		m_iSelected_Skill = (SKILLID)iSelected_Skill;
		// ��ų ���ÿ� ���� ó��
		UpdateCameraSelection(); // ī�޶� ���� ������Ʈ
	}

	if (m_iSelected_Skill >= 0) {
		// ��ų�� ���õ� ��� ���� ���õ� ��ų�� ���
		ImGui::Text("Currently Selected Skill: %s", skill_options[m_iSelected_Skill]);
	}
}

void CIMGUI_Camera_Tab::Open_Select_Camera(_float fTimeDelta)
{
	if (bShowCameraWindow) {
		ImGui::Begin("Camera_Select", &bShowCameraWindow);  // �� â ����

		IMGUI_Camera_Select(fTimeDelta);  // �� ���� UI

		ImGui::End();  // �� â ����
	}
}

void CIMGUI_Camera_Tab::IMGUI_Camera_Select(_float fTimeDelta)
{
	// ���� �ν��Ͻ����� ���� ī�޶� ��ü ��������
	CGameObject* camera = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera"));
	CMain_Camera* main_Camera = static_cast<CMain_Camera*>(camera);

	// ���� ī�޶� ��� ��������
	vector<CCamera*> cameraList = main_Camera->m_vecVirtualCamera;

	// ī�޶� �̸��� ������ �ӽ� �迭 ����
	vector<const _char*> cameraNames;

	for (auto& iter : cameraList) {
		cameraNames.push_back(iter->GetTabName()); // �� ī�޶��� �̸��� �迭�� ����
	}

	// ���� ���õ� ī�޶� �ε����� ������ ���� ����
	static _int selectedCameraIndex = (_int)main_Camera->Get_Virtual_Camera();

	if (ImGui::Combo("Camera List", &selectedCameraIndex, cameraNames.data(), cameraNames.size())) {
		// ���õ� ī�޶��� �ε����� ���� Ȱ��ȭ �Լ� ȣ��
		Activate_Select_Camera(selectedCameraIndex);
	}
}

// ���ο� �Լ�: ���õ� ī�޶� Ȱ��ȭ�ϴ� �Լ�
void CIMGUI_Camera_Tab::Activate_Select_Camera(_int selectedIndex)
{
	// ���� ī�޶� ��������
	CGameObject* camera = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera"));
	CMain_Camera* main_Camera = static_cast<CMain_Camera*>(camera);

	// ���� ī�޶� ��� ��������
	vector<CCamera*>& cameraList = main_Camera->m_vecVirtualCamera;

	if (selectedIndex < 0 || selectedIndex >= cameraList.size()) {
		// ��ȿ���� ���� �ε����� ��� ó��
		ImGui::Text("Invalid camera index selected.");
		return;
	}

	// ���õ� ī�޶� Ȱ��ȭ
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
			// ���ε� ī�޶� ���� ��� ó�� (��: �⺻ ī�޶� ���� �Ǵ� ���� �޽��� ǥ��)
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
