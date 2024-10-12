#include "stdafx.h"
#include "IMGUI_Camera_Tab.h"
#include "GameInstance.h"
#include "RenderInstance.h"


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
    static int previous_model = -1;  // ���� ���� �����ϱ� ���� ����

    ImGui::Text("Select Model");
    if (ImGui::Combo("Model", &m_iSelected_Model, model_options, IM_ARRAYSIZE(model_options))) {
        // ���� ����� ��쿡�� ��ų ������ �ʱ�ȭ
        if (m_iSelected_Model != previous_model) {
            previous_model = m_iSelected_Model;  // ���ο� �𵨷� ������Ʈ
            m_iSelected_Skill = -1;  // ��ų ���� �ʱ�ȭ
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

    ImGui::Text("Select Skill");
    if (ImGui::Combo("Skill", &m_iSelected_Skill, skill_options, IM_ARRAYSIZE(skill_options))) {
        // ��ų ���ÿ� ���� ó��
    }

    if (m_iSelected_Skill >= 0) {
        // ��ų�� ���õ� ��� ���� ���õ� ��ų�� ���
        ImGui::Text("Currently Selected Skill: %s", skill_options[m_iSelected_Skill]);
    }
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
