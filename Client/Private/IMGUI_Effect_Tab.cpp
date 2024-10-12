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
#include <Effect_Layer.h>

const char* Effect[] = { "Each", "Layer", "Layer KeyFrame"};
const char* EffectType[] = { "Single", "MoveTex", "Multi" };

static int CurrentEffect = 0;
static int CurrentEffectType = 0;

static bool openKeyFrameWindow = false;
static std::string selectedEffectName;
static int selectedFrame = -1;

static std::wstring selectedLayerName;

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

    _uint i = m_pEffect_Manager->m_TestEffect.size();

    if (ImGui::BeginCombo("Menu", Effect[CurrentEffect]))
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

    ImGui::Separator();
    ImGui::Separator();

    if (CurrentEffect == 0) 
    {
        Render_For_Each_Effect();
    }

    if (CurrentEffect == 1)
    {
        Render_For_Effect_Layer();
    }

    if (CurrentEffect == 2)
    {
        Render_For_Layer_KeyFrame();
    }

    if(openKeyFrameWindow)
      Render_For_Effect_KeyFrame();

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

void CIMGUI_Effect_Tab::Render_For_Each_Effect()
{
    static int CurrentModel = 0;
    static int CurrentMaskTexture = 0;
    static int CurrentDiffuseTexture = 0;

    if (CurrentEffect == 0)
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

        static char EffectNameBuffer[128] = "";
        ImGui::InputText("Effect Name", EffectNameBuffer, IM_ARRAYSIZE(EffectNameBuffer));

        if (ImGui::Button("Add Effect"))
        {
            std::wstring wEffectName = UTF8ToWString(EffectNameBuffer);
            std::wstring wModelName = UTF8ToWString(ModelName[CurrentModel]);
            EFFECT_TYPE effectType = static_cast<EFFECT_TYPE>(CurrentEffectType);
            m_pEffect_Manager->Add_Test_Effect(effectType, &wEffectName, &wModelName);
        }

        ImGui::SameLine();

        if (ImGui::Button("Delete Selected Effect"))
        {
            _uint EffectIndex = CImgui_Manager::Get_Instance()->Get_CurShaderTab_Index();
            CImgui_Manager::Get_Instance()->Delete_Shader_Tab(EffectIndex);
            m_pEffect_Manager->Delete_Test_Effect(CImgui_Manager::Get_Instance()->Get_CurShaderTab_Id());
        }

        ImGui::SameLine();

        if (ImGui::Button("Delete All Effects"))
        {
            m_pEffect_Manager->Delete_All_Test_Effect();
        }

        ImGui::Separator();
        ImGui::Text("Select Effect Information");

        _int EffectId = CImgui_Manager::Get_Instance()->Get_CurShaderTab_Id();
        _float3 CurPosition = m_pEffect_Manager->Get_Effect_Position(EffectId);
        _float3 CurScaled = m_pEffect_Manager->Get_Effect_Scaled(EffectId);
        _float3 CurRotation = m_pEffect_Manager->Get_Effect_Rotation(EffectId);

        ImGui::Text("Position");
        ImGui::Text("X"); ImGui::SameLine();
        if (ImGui::SliderFloat("##Position X Slider", &CurPosition.x, -100.0f, 100.0f))
            m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition);
        ImGui::SameLine();
        if (ImGui::Button("-##Position X Dec")) { CurPosition.x -= 0.1f; m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition); }
        ImGui::SameLine();
        if (ImGui::Button("+##Position X Inc")) { CurPosition.x += 0.1f; m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition); }
        ImGui::SameLine();
        if (ImGui::InputFloat("##Position X", &CurPosition.x, 0.1f))
            m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition);

        ImGui::Text("Y"); ImGui::SameLine();
        if (ImGui::SliderFloat("##Position Y Slider", &CurPosition.y, -100.0f, 100.0f))
            m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition);
        ImGui::SameLine();
        if (ImGui::Button("-##Position Y Dec")) { CurPosition.y -= 0.1f; m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition); }
        ImGui::SameLine();
        if (ImGui::Button("+##Position Y Inc")) { CurPosition.y += 0.1f; m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition); }
        ImGui::SameLine();
        if (ImGui::InputFloat("##Position Y", &CurPosition.y, 0.1f))
            m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition);

        ImGui::Text("Z"); ImGui::SameLine();
        if (ImGui::SliderFloat("##Position Z Slider", &CurPosition.z, -100.0f, 100.0f))
            m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition);
        ImGui::SameLine();
        if (ImGui::Button("-##Position Z Dec")) { CurPosition.z -= 0.1f; m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition); }
        ImGui::SameLine();
        if (ImGui::Button("+##Position Z Inc")) { CurPosition.z += 0.1f; m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition); }
        ImGui::SameLine();
        if (ImGui::InputFloat("##Position Z", &CurPosition.z, 0.1f))
            m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition);

        ImGui::Separator();

        ImGui::Text("Scale");
        ImGui::Text("X"); ImGui::SameLine();
        if (ImGui::SliderFloat("##Scale X Slider", &CurScaled.x, 0.01f, 100.0f))
            m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled);
        ImGui::SameLine();
        if (ImGui::Button("-##Scale X Dec")) { CurScaled.x = max(0.01f, CurScaled.x - 0.1f); m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled); }
        ImGui::SameLine();
        if (ImGui::Button("+##Scale X Inc")) { CurScaled.x += 0.1f; m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled); }
        ImGui::SameLine();
        if (ImGui::InputFloat("##Scale X", &CurScaled.x, 0.1f)) {
            CurScaled.x = max(0.01f, CurScaled.x);
            m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled);
        }

        ImGui::Text("Y"); ImGui::SameLine();
        if (ImGui::SliderFloat("##Scale Y Slider", &CurScaled.y, 0.01f, 100.0f))
            m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled);
        ImGui::SameLine();
        if (ImGui::Button("-##Scale Y Dec")) { CurScaled.y = max(0.01f, CurScaled.y - 0.1f); m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled); }
        ImGui::SameLine();
        if (ImGui::Button("+##Scale Y Inc")) { CurScaled.y += 0.1f; m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled); }
        ImGui::SameLine();
        if (ImGui::InputFloat("##Scale Y", &CurScaled.y, 0.1f)) {
            CurScaled.y = max(0.01f, CurScaled.y);
            m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled);
        }

        ImGui::Text("Z"); ImGui::SameLine();
        if (ImGui::SliderFloat("##Scale Z Slider", &CurScaled.z, 0.01f, 100.0f))
            m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled);
        ImGui::SameLine();
        if (ImGui::Button("-##Scale Z Dec")) { CurScaled.z = max(0.01f, CurScaled.z - 0.1f); m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled); }
        ImGui::SameLine();
        if (ImGui::Button("+##Scale Z Inc")) { CurScaled.z += 0.1f; m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled); }
        ImGui::SameLine();
        if (ImGui::InputFloat("##Scale Z", &CurScaled.z, 0.1f)) {
            CurScaled.z = max(0.01f, CurScaled.z);
            m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled);
        }

        ImGui::Separator();

        ImGui::Text("Rotation");
        ImGui::Text("X"); ImGui::SameLine();
        if (ImGui::SliderFloat("##Rotation X Slider", &CurRotation.x, 0.0f, 360.0f)) {
            CurRotation.x = std::fmod(CurRotation.x, 360.0f);
            m_pEffect_Manager->Set_Effect_Rotation(EffectId, CurRotation);
        }
        ImGui::SameLine();
        if (ImGui::Button("-##Rotation X Dec")) {
            CurRotation.x = std::fmod(CurRotation.x - 1.0f + 360.0f, 360.0f);
            m_pEffect_Manager->Set_Effect_Rotation(EffectId, CurRotation);
        } ImGui::SameLine();
        if (ImGui::Button("+##Rotation X Inc")) {
            CurRotation.x = std::fmod(CurRotation.x + 1.0f, 360.0f);
            m_pEffect_Manager->Set_Effect_Rotation(EffectId, CurRotation);
        } ImGui::SameLine();
        if (ImGui::InputFloat("##Rotation X", &CurRotation.x, 1.0f)) {
            CurRotation.x = std::fmod(CurRotation.x, 360.0f);
            m_pEffect_Manager->Set_Effect_Rotation(EffectId, CurRotation);
        }

        ImGui::Text("Y"); ImGui::SameLine();
        if (ImGui::SliderFloat("##Rotation Y Slider", &CurRotation.y, 0.0f, 360.0f)) {
            CurRotation.y = std::fmod(CurRotation.y, 360.0f);
            m_pEffect_Manager->Set_Effect_Rotation(EffectId, CurRotation);
        }
        ImGui::SameLine();
        if (ImGui::Button("-##Rotation Y Dec")) {
            CurRotation.y = std::fmod(CurRotation.y - 1.0f + 360.0f, 360.0f);
            m_pEffect_Manager->Set_Effect_Rotation(EffectId, CurRotation);
        } ImGui::SameLine();
        if (ImGui::Button("+##Rotation Y Inc")) {
            CurRotation.y = std::fmod(CurRotation.y + 1.0f, 360.0f);
            m_pEffect_Manager->Set_Effect_Rotation(EffectId, CurRotation);
        } ImGui::SameLine();
        if (ImGui::InputFloat("##Rotation Y", &CurRotation.y, 1.0f)) {
            CurRotation.y = std::fmod(CurRotation.y, 360.0f);
            m_pEffect_Manager->Set_Effect_Rotation(EffectId, CurRotation);
        }

        ImGui::Text("Z"); ImGui::SameLine();
        if (ImGui::SliderFloat("##Rotation Z Slider", &CurRotation.z, 0.0f, 360.0f)) {
            CurRotation.z = std::fmod(CurRotation.z, 360.0f);
            m_pEffect_Manager->Set_Effect_Rotation(EffectId, CurRotation);
        }
        ImGui::SameLine();
        if (ImGui::Button("-##Rotation Z Dec")) {
            CurRotation.z = std::fmod(CurRotation.z - 1.0f + 360.0f, 360.0f);
            m_pEffect_Manager->Set_Effect_Rotation(EffectId, CurRotation);
        } ImGui::SameLine();
        if (ImGui::Button("+##Rotation Z Inc")) {
            CurRotation.z = std::fmod(CurRotation.z + 1.0f, 360.0f);
            m_pEffect_Manager->Set_Effect_Rotation(EffectId, CurRotation);
        } ImGui::SameLine();
        if (ImGui::InputFloat("##Rotation Z", &CurRotation.z, 1.0f)) {
            CurRotation.z = std::fmod(CurRotation.z, 360.0f);
            m_pEffect_Manager->Set_Effect_Rotation(EffectId, CurRotation);
        }

        ImGui::Separator();
    }
}


void CIMGUI_Effect_Tab::Render_For_Effect_Layer()
{
    ImGui::InputText("New Layer", &EffectLayerKey[0], 128);

    if (ImGui::Button("Select Effect Save To New Layer"))
    {
        wstring strEffectLayerTag = UTF8ToWString(EffectLayerKey);

        _uint EffectIndex = CImgui_Manager::Get_Instance()->Get_CurShaderTab_Index();

        m_pEffect_Manager->Add_Effect_To_Layer(EffectIndex, strEffectLayerTag);

        EffectLayerKey.clear();
    }

    if (ImGui::Button("All Effect Save To New Layer"))
    {
        wstring strEffectLayerTag = UTF8ToWString(EffectLayerKey);

        m_pEffect_Manager->Add_All_Effect_To_Layer(strEffectLayerTag);

        EffectLayerKey.clear();
    }

    ImGui::Separator();

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

        if (ImGui::Button("Add Selected Effect To Select Layer"))
        {
            _uint EffectIndex = CImgui_Manager::Get_Instance()->Get_CurShaderTab_Index();
            const wstring selectedLayerKey = LayerList[selectedLayerIndex];

            m_pEffect_Manager->Add_Effect_To_Layer(EffectIndex, selectedLayerKey);
        }

        // ���õ� ���̾ ��ü ��ü �߰� ��ư
        if (ImGui::Button("Add All Effects To Select Layer"))
        {
            const wstring selectedLayerKey = LayerList[selectedLayerIndex];

            m_pEffect_Manager->Add_All_Effect_To_Layer(selectedLayerKey);
        }

        if (ImGui::Button("Delete Layer"))
        {
            const wstring selectedLayerKey = LayerList[selectedLayerIndex];

            m_pEffect_Manager->Delete_Layer(selectedLayerKey);

            selectedLayerIndex = 0;
        }
    
    }
}

void CIMGUI_Effect_Tab::Render_For_Layer_KeyFrame()
{
    // ���̾� ��� ��������
    auto LayerList = m_pEffect_Manager->Get_Layer_List();

    if (!LayerList.empty())
    {
        vector<string> layerListUTF8;
        for (const auto& layer : LayerList)
        {
            layerListUTF8.push_back(WStringToUTF8(layer)); // wstring�� UTF-8�� ��ȯ
        }

        static int selectedLayerIndex = 0;
        const char* currentLayer = layerListUTF8[selectedLayerIndex].c_str();

        // ���̾� ���� �޺� �ڽ�
        if (ImGui::BeginCombo("Select Layer", currentLayer))
        {
            for (int i = 0; i < layerListUTF8.size(); i++)
            {
                bool isSelected = (selectedLayerIndex == i);
                if (ImGui::Selectable(layerListUTF8[i].c_str(), isSelected))
                {
                    selectedLayerIndex = i;
                    selectedLayerName = LayerList[i]; // ���õ� ���̾� �̸��� ����
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        ImGui::SameLine();
        ImGui::Dummy(ImVec2(70.0f, 0.0f)); // ���� 20px ���� �߰�
        ImGui::SameLine();

        if (ImGui::Button("Play"))
        {
            // ��� ��� �߰�
            // ��: m_pEffect_Manager->Play(selectedLayerName);
        }

        ImGui::SameLine();

        if (ImGui::Button("Pause"))
        {
            // �Ͻ����� ��� �߰�
            // ��: m_pEffect_Manager->Pause(selectedLayerName);
        }

        // ������ ���̾��� Duration ��������
        CEffect_Layer* pLayer = m_pEffect_Manager->Find_Effect_Layer(selectedLayerName);
        if (pLayer)
        {
            float layerDuration = pLayer->m_fDuration; // ���� Duration ��

            // Duration ������ ���� �Է¶�
            ImGui::Text("Layer Duration:");
            ImGui::SameLine();
            if (ImGui::InputFloat("##LayerDuration", &layerDuration, 0.1f, 1.0f, "%.2f"))
            {
                // Duration ���� �����Ǹ� ���̾ �ݿ�
                pLayer->m_fDuration = max(0.0f, layerDuration); // ������ �������� �ʵ���
            }
        }

        ImGui::Separator();

        // ���õ� ���̾��� ����Ʈ ��� ��������
        auto effectNames = m_pEffect_Manager->Get_In_Layer_Effect_List(&selectedLayerName);

        if (!effectNames.empty())
        {
            const int frameCount = 200; // �� ������ ��
            const int buttonSize = 20;  // ��ư ũ��
            const float effectNameWidth = 150.0f; // ����Ʈ �̸� ǥ�� �ʺ�

            ImGui::BeginChild("TimelineRegion", ImVec2(0, 300), false, ImGuiWindowFlags_HorizontalScrollbar);

            // ��ܿ� ������ ��ȣ ǥ��
            ImGui::Text(" "); // �̸����� ������ ���� �� �ؽ�Ʈ
            ImGui::SameLine(effectNameWidth); // ����Ʈ �̸� �ʺ� ����
            for (int frame = 0; frame < frameCount; frame++)
            {
                ImGui::SetCursorPosX(effectNameWidth + frame * (buttonSize + 5.0f)); // ��ư ũ��� ���� ����
                ImGui::Text("%d", frame);
                ImGui::SameLine();
            }
            ImGui::NewLine();

            // �� ����Ʈ�� �̸� �� ������ ��ư ǥ��
            for (int item = 0; item < effectNames.size(); item++)
            {
                // UTF-8 ��ȯ �� ����Ʈ �̸� ǥ��
                string effectNameUTF8 = WStringToUTF8(effectNames[item]);
                ImGui::TextWrapped("%s", effectNameUTF8.c_str());

                ImGui::SameLine(effectNameWidth); // �̸� �ڿ� ����� ������ ��

                // �� �������� ��ư ��ġ
                for (int frame = 0; frame < frameCount; frame++)
                {
                    ImGui::SetCursorPosX(effectNameWidth + frame * (buttonSize + 5.0f)); // ��ư ��ġ ����
                    ImGui::PushID(frame + item * frameCount);

                    // Ű�������� �����ϴ��� Ȯ��
                    bool isKeyFrameExist = m_pEffect_Manager->Find_KeyFrame(selectedLayerName, effectNames[item], frame);


                    // ���� ����
                    if (isKeyFrameExist)
                    {
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 0.8f, 0.3f, 0.7f)); // ���� ���� ����
                    }

                    // Ű�������� ��Ÿ���� ��ư
                    if (ImGui::Button("##", ImVec2(buttonSize, buttonSize)))
                    {
                        // ��ư Ŭ�� �� ���õ� ȿ�� �̸��� ������ ��ȣ�� ����
                        selectedEffectName = effectNameUTF8;
                        selectedFrame = frame;
                        openKeyFrameWindow = true;
                    }

                    // ���� ����
                    if (isKeyFrameExist)
                    {
                        ImGui::PopStyleColor();
                    }

                    ImGui::PopID();
                    ImGui::SameLine();
                }
                ImGui::NewLine(); // ���� ����Ʈ�� �� �ٷ� �̵�
            }

            ImGui::EndChild();
        }
    }
}


void CIMGUI_Effect_Tab::Render_For_Effect_KeyFrame()
{
    ImGui::Begin("Keyframe Editor", &openKeyFrameWindow, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Editing Keyframe for Effect: %s", selectedEffectName.c_str());
    ImGui::Text("Frame: %d", selectedFrame);

    static EFFECT_KEYFRAME newKeyFrame;

    ImGui::Text("Effect Keyframe Settings");

    // Is Not Playing üũ�ڽ�
    ImGui::Checkbox("Is Not Playing", &newKeyFrame.bIsNotPlaying);

    ImGui::Separator();

    // Position ����
    ImGui::Text("Position");
    _int EffectId = CImgui_Manager::Get_Instance()->Get_CurShaderTab_Id();
    _float3 CurPosition = m_pEffect_Manager->Get_Effect_Position(EffectId);

    // X Position
    ImGui::Text("X"); ImGui::SameLine();
    if (ImGui::SliderFloat("##Position X Slider", &CurPosition.x, -100.0f, 100.0f))
        m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition);
    ImGui::SameLine();
    if (ImGui::Button("-##Position X Dec")) { CurPosition.x -= 0.1f; m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition); }
    ImGui::SameLine();
    if (ImGui::Button("+##Position X Inc")) { CurPosition.x += 0.1f; m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition); }
    ImGui::SameLine();
    ImGui::InputFloat("##Position X", &CurPosition.x, 0.1f);

    // Y Position
    ImGui::Text("Y"); ImGui::SameLine();
    if (ImGui::SliderFloat("##Position Y Slider", &CurPosition.y, -100.0f, 100.0f))
        m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition);
    ImGui::SameLine();
    if (ImGui::Button("-##Position Y Dec")) { CurPosition.y -= 0.1f; m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition); }
    ImGui::SameLine();
    if (ImGui::Button("+##Position Y Inc")) { CurPosition.y += 0.1f; m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition); }
    ImGui::SameLine();
    ImGui::InputFloat("##Position Y", &CurPosition.y, 0.1f);

    // Z Position
    ImGui::Text("Z"); ImGui::SameLine();
    if (ImGui::SliderFloat("##Position Z Slider", &CurPosition.z, -100.0f, 100.0f))
        m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition);
    ImGui::SameLine();
    if (ImGui::Button("-##Position Z Dec")) { CurPosition.z -= 0.1f; m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition); }
    ImGui::SameLine();
    if (ImGui::Button("+##Position Z Inc")) { CurPosition.z += 0.1f; m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition); }
    ImGui::SameLine();
    ImGui::InputFloat("##Position Z", &CurPosition.z, 0.1f);

    ImGui::Separator();

    // Scale ����
    ImGui::Text("Scale");
    _float3 CurScale = m_pEffect_Manager->Get_Effect_Scaled(EffectId);

    // X Scale
    ImGui::Text("X"); ImGui::SameLine();
    if (ImGui::SliderFloat("##Scale X Slider", &CurScale.x, 0.01f, 100.0f))
        m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScale);
    ImGui::SameLine();
    if (ImGui::Button("-##Scale X Dec")) { CurScale.x = max(0.01f, CurScale.x - 0.1f); m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScale); }
    ImGui::SameLine();
    if (ImGui::Button("+##Scale X Inc")) { CurScale.x += 0.1f; m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScale); }
    ImGui::SameLine();
    ImGui::InputFloat("##Scale X", &CurScale.x, 0.1f);

    // Y Scale
    ImGui::Text("Y"); ImGui::SameLine();
    if (ImGui::SliderFloat("##Scale Y Slider", &CurScale.y, 0.01f, 100.0f))
        m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScale);
    ImGui::SameLine();
    if (ImGui::Button("-##Scale Y Dec")) { CurScale.y = max(0.01f, CurScale.y - 0.1f); m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScale); }
    ImGui::SameLine();
    if (ImGui::Button("+##Scale Y Inc")) { CurScale.y += 0.1f; m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScale); }
    ImGui::SameLine();
    ImGui::InputFloat("##Scale Y", &CurScale.y, 0.1f);

    // Z Scale
    ImGui::Text("Z"); ImGui::SameLine();
    if (ImGui::SliderFloat("##Scale Z Slider", &CurScale.z, 0.01f, 100.0f))
        m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScale);
    ImGui::SameLine();
    if (ImGui::Button("-##Scale Z Dec")) { CurScale.z = max(0.01f, CurScale.z - 0.1f); m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScale); }
    ImGui::SameLine();
    if (ImGui::Button("+##Scale Z Inc")) { CurScale.z += 0.1f; m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScale); }
    ImGui::SameLine();
    ImGui::InputFloat("##Scale Z", &CurScale.z, 0.1f);

    ImGui::Separator();

    // Rotation ����
    ImGui::Text("Rotation");
    _float3 CurRotation = m_pEffect_Manager->Get_Effect_Rotation(EffectId);

    // X Rotation
    ImGui::Text("X"); ImGui::SameLine();
    if (ImGui::SliderFloat("##Rotation X Slider", &CurRotation.x, 0.0f, 360.0f))
        m_pEffect_Manager->Set_Effect_Rotation(EffectId, CurRotation);
    ImGui::SameLine();
    if (ImGui::Button("-##Rotation X Dec")) { CurRotation.x = fmod(CurRotation.x - 1.0f + 360.0f, 360.0f); m_pEffect_Manager->Set_Effect_Rotation(EffectId, CurRotation); }
    ImGui::SameLine();
    if (ImGui::Button("+##Rotation X Inc")) { CurRotation.x = fmod(CurRotation.x + 1.0f, 360.0f); m_pEffect_Manager->Set_Effect_Rotation(EffectId, CurRotation); }
    ImGui::SameLine();
    ImGui::InputFloat("##Rotation X", &CurRotation.x, 1.0f);

    // Y Rotation
    ImGui::Text("Y"); ImGui::SameLine();
    if (ImGui::SliderFloat("##Rotation Y Slider", &CurRotation.y, 0.0f, 360.0f))
        m_pEffect_Manager->Set_Effect_Rotation(EffectId, CurRotation);
    ImGui::SameLine();
    if (ImGui::Button("-##Rotation Y Dec")) { CurRotation.y = fmod(CurRotation.y - 1.0f + 360.0f, 360.0f); m_pEffect_Manager->Set_Effect_Rotation(EffectId, CurRotation); }
    ImGui::SameLine();
    if (ImGui::Button("+##Rotation Y Inc")) { CurRotation.y = fmod(CurRotation.y + 1.0f, 360.0f); m_pEffect_Manager->Set_Effect_Rotation(EffectId, CurRotation); }
    ImGui::SameLine();
    ImGui::InputFloat("##Rotation Y", &CurRotation.y, 1.0f);

    // Z Rotation
    ImGui::Text("Z"); ImGui::SameLine();
    if (ImGui::SliderFloat("##Rotation Z Slider", &CurRotation.z, 0.0f, 360.0f))
        m_pEffect_Manager->Set_Effect_Rotation(EffectId, CurRotation);
    ImGui::SameLine();
    if (ImGui::Button("-##Rotation Z Dec")) { CurRotation.z = fmod(CurRotation.z - 1.0f + 360.0f, 360.0f); m_pEffect_Manager->Set_Effect_Rotation(EffectId, CurRotation); }
    ImGui::SameLine();
    if (ImGui::Button("+##Rotation Z Inc")) { CurRotation.z = fmod(CurRotation.z + 1.0f, 360.0f); m_pEffect_Manager->Set_Effect_Rotation(EffectId, CurRotation); }
    ImGui::SameLine();
    ImGui::InputFloat("##Rotation Z", &CurRotation.z, 1.0f);

    ImGui::Separator();

    // Current Time
    ImGui::Text("Current Time");
    ImGui::SliderFloat("##CurTime Slider", &newKeyFrame.fCurTime, 0.0f, 100.0f);
    ImGui::SameLine();
    ImGui::InputFloat("##CurTime", &newKeyFrame.fCurTime);

    ImGui::Separator();

    // Duration
    ImGui::Text("Duration");
    if (CEffect_Layer* pLayer = m_pEffect_Manager->Find_Effect_Layer(selectedLayerName))
        newKeyFrame.fDuration = pLayer->m_fDuration;
    ImGui::InputFloat("##Duration", &newKeyFrame.fDuration, 0.5f, 0.5f, "%.3f", ImGuiInputTextFlags_ReadOnly);

    ImGui::Separator();

    // Keyframe ���� ��ư
    if (ImGui::Button("Add Keyframe"))
    {
        m_pEffect_Manager->Add_KeyFrame(selectedLayerName, wstring(selectedEffectName.begin(), selectedEffectName.end()), selectedFrame, newKeyFrame);
        ImGui::Text("Keyframe added!");
    }

    ImGui::SameLine();

    if (ImGui::Button("Save Keyframe"))
    {
        ImGui::Text("Keyframe saved!");
    }

    ImGui::SameLine();

    if (ImGui::Button("Reset Keyframe"))
    {
        newKeyFrame = EFFECT_KEYFRAME();
        ImGui::Text("Keyframe reset!");
    }

    ImGui::Separator();

    ImGui::End();
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
