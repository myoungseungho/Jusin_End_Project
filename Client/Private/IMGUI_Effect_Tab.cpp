#include "stdafx.h"
#include "IMGUI_Effect_Tab.h"
#include "GameInstance.h"
#include "RenderInstance.h"
#include "Effect_Manager.h"
#include "Imgui_Manager.h"
#include <string>
#include <locale>
#include <codecvt>
#include <IMGUI_Shader_Tab.h>


CIMGUI_Effect_Tab::CIMGUI_Effect_Tab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CIMGUI_Tab{ pDevice,pContext }
    , m_pEffect_Manager{ CEffect_Manager::Get_Instance() }
{
    Safe_AddRef(m_pEffect_Manager);
}

HRESULT CIMGUI_Effect_Tab::Initialize()
{
    return S_OK;
}

void CIMGUI_Effect_Tab::Render(_float fTimeDelta)
{
    Push_Initialize();

    const char* Effect[] = { "Each", "Final" };
    static int CurrentEffect = 0;

    const char* EffectType[] = { "Single", "MoveTex", "Multi" };
    static int CurrentEffectType = 0;

    static int CurrentModel = 0;

    static int CurrentMaskTexture = 0;
    static int CurrentDiffuseTexture = 0;

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

    if (CurrentEffect == 0) // Each ���� ��
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

        if (!TextureName.empty() && ImGui::BeginCombo("Mask Texture Name", TextureName[CurrentMaskTexture].c_str()))
        {
            for (int i = 0; i < TextureName.size(); i++) {
                bool isSelected = (CurrentMaskTexture == i);
                if (ImGui::Selectable(TextureName[i].c_str(), isSelected)) {
                    CurrentMaskTexture = i;
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        if (!TextureName.empty() && ImGui::BeginCombo("Diffuse Texture Name", TextureName[CurrentDiffuseTexture].c_str()))
        {
            for (int i = 0; i < TextureName.size(); i++) {
                bool isSelected = (CurrentDiffuseTexture == i);
                if (ImGui::Selectable(TextureName[i].c_str(), isSelected)) {
                    CurrentDiffuseTexture = i;
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        // Add Effect ��ư�� �߰��Ͽ� Add_Test_Effect ȣ��
        if (ImGui::Button("Add Test Effect"))
        {
            // ���õ� Model Name�� wstring���� ��ȯ
            std::wstring wModelName = UTF8ToWString(ModelName[CurrentModel]);

            // CurrentEffectType�� EFFECT_TYPE���� ��ȯ�Ͽ� ����
            EFFECT_TYPE effectType = static_cast<EFFECT_TYPE>(CurrentEffectType);
            // Add_Test_Effect ȣ��
            m_pEffect_Manager->Add_Test_Effect(effectType, &wModelName);
        }

        if (ImGui::Button("Delete Test Effect"))
        {
            // �׽�Ʈ ����Ʈ ���Ϳ��� ������ ��ü�� �ε��� ã�� ����
            m_pEffect_Manager->Delete_Test_Effect(0);

        }

        ImGui::InputText("Effect Layer", &EffectLayerKey[0], 128);

        if (ImGui::Button("Save To Layer"))
        {
            wstring strEffectLayerTag = UTF8ToWString(EffectLayerKey);

            m_pEffect_Manager->Add_Effect_To_Layer(0, strEffectLayerTag);

            EffectLayerKey.clear();
        }

        auto LayerList = m_pEffect_Manager->Get_Layer_List();

        if (!LayerList.empty())
        {
            vector<string> layerListUTF8;
            for (const auto& layer : LayerList)
            {
                layerListUTF8.push_back(WStringToUTF8(layer)); // wstring�� UTF-8�� ��ȯ�ϴ� �Լ��� �ʿ�
            }

            // ���� ���õ� ���̾� �ε����� ������ ����
            static int selectedLayerIndex = 0;
            const char* currentLayer = layerListUTF8[selectedLayerIndex].c_str();

            if (ImGui::BeginCombo("Select Layer", currentLayer))
            {
                for (int i = 0; i < layerListUTF8.size(); i++)
                {
                    bool isSelected = (selectedLayerIndex == i);
                    if (ImGui::Selectable(layerListUTF8[i].c_str(), isSelected))
                    {
                        selectedLayerIndex = i;
                    }
                    if (isSelected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

            if (ImGui::Button("Delete Layer"))
            {
            }
        }
    }

}

void CIMGUI_Effect_Tab::Push_Initialize()
{
    if (m_isInitialize == false)
    {
        for (const auto& pair : m_pEffect_Manager->m_EffectModel) {
            ModelName.push_back(WStringToUTF8(pair.first));
        }

        for (const auto& pair : m_pEffect_Manager->m_EffectTexture) {
            TextureName.push_back(WStringToUTF8(pair.first));
        }

        m_isInitialize = true;
    }
}

void CIMGUI_Effect_Tab::Save_To_Effect_Layer(_uint iCurTestEffectIndex, const wstring& strEffectLayerTag, void* pArg)
{
    m_pEffect_Manager->Add_Effect_To_Layer(iCurTestEffectIndex, strEffectLayerTag);
}


string CIMGUI_Effect_Tab::WStringToUTF8(const std::wstring& wstr)
{
    if (wstr.empty()) return std::string();
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.to_bytes(wstr);
}

wstring CIMGUI_Effect_Tab::UTF8ToWString(const string& utf8Str)
{
    // UTF-8 ���ڿ��� ���̸� Ȯ���ϰ�, ��ȯ�� wstring�� ���̸� ����մϴ�.
    int wstrLength = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);

    // ��ȯ�� wstring�� ũ�⸦ �Ҵ�
    std::wstring wstr(wstrLength, 0);

    // ��ȯ �۾� ����
    MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &wstr[0], wstrLength);

    // �� ���ڸ� �����ϰ� ��ȯ
    wstr.pop_back(); // null character ����
    return wstr;
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
