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
