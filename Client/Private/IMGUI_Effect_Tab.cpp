#include "stdafx.h"
#include "IMGUI_Effect_Tab.h"
#include "GameInstance.h"
#include "RenderInstance.h"
#include "Effect_Manager.h"
#include <string>
#include <locale>
#include <codecvt>


CIMGUI_Effect_Tab::CIMGUI_Effect_Tab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CIMGUI_Tab{ pDevice,pContext }
	, m_pEffect_Manager{CEffect_Manager::Get_Instance()}
{
	Safe_AddRef(m_pEffect_Manager);
}

HRESULT CIMGUI_Effect_Tab::Initialize()
{
	return S_OK;
}

void CIMGUI_Effect_Tab::Render(_float fTimeDelta)
{

    const char* Effect[] = {"Each", "Final"};
    static int CurrentEffect = 0;

    const char* EffectType[] = { "Single", "Multi", "MoveTex" };
    static int CurrentEffectType = 0;

    std::vector<std::string> ModelName;
    static int CurrentModel = 0;

    ModelName.clear();
    for (const auto& pair : m_pEffect_Manager->m_EffectModel) {
        // 변환 함수 사용
        ModelName.push_back(WStringToUTF8(pair.first));
    }

    // 첫 번째 콤보박스: Each 또는 Final 선택
    if (ImGui::BeginCombo("Add What", Effect[CurrentEffect]))
    {
        for (int i = 0; i < IM_ARRAYSIZE(Effect); i++) {
            bool isSelected = (CurrentEffect == i);

            if (ImGui::Selectable(Effect[i], isSelected)) {
                CurrentEffect = i;
            }
            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    // 각 선택에 따른 다른 콤보박스 렌더링
    if (CurrentEffect == 0) // Each 선택 시
    {
        if (ImGui::BeginCombo("EffectType", EffectType[CurrentEffectType]))
        {
            for (int i = 0; i < IM_ARRAYSIZE(EffectType); i++) {
                bool isSelected = (CurrentEffectType == i);
                if (ImGui::Selectable(EffectType[i], isSelected)) {
                    CurrentEffectType = i;
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        if (!ModelName.empty() && ImGui::BeginCombo("Model Name", ModelName[CurrentModel].c_str()))
        {
            for (int i = 0; i < ModelName.size(); i++) {
                bool isSelected = (CurrentModel == i);
                if (ImGui::Selectable(ModelName[i].c_str(), isSelected)) {
                    CurrentModel = i;
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    }
}

string CIMGUI_Effect_Tab::WStringToUTF8(const std::wstring& wstr)
{
    if (wstr.empty()) return std::string();
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.to_bytes(wstr);
}

CIMGUI_Effect_Tab* CIMGUI_Effect_Tab::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIMGUI_Effect_Tab* pInstance = new CIMGUI_Effect_Tab(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CIMGUI_Effect_Tab"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIMGUI_Effect_Tab::Free()
{
	__super::Free();

    Safe_Release(m_pEffect_Manager);
}
