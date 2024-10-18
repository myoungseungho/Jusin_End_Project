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
#include "Effect_Animation.h"

const char* Effect[] = { "Each", "Layer", "Layer KeyFrame"};
const char* EffectType[] = { "Single", "MoveTex", "Multi" };

static int CurrentEffect = 0;
static int CurrentEffectType = 0;

static bool openKeyFrameWindow = false;
static std::string selectedEffectName;
static int selectedFrame = -1;

static std::wstring selectedLayerName;
static bool initialized = false;

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

    ImGui::SameLine();
    if (ImGui::Button("Save"))
    {
        Save_Effects_File();
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
        Render_For_Layer_KeyFrame(fTimeDelta);
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

HRESULT CIMGUI_Effect_Tab::Save_Effects_File()
{
    wstring filename = L"../Bin/Effects.txt"; // ���� ��� ����

    // m_vecEffectData�� ������ �����͸� �����մϴ�.
    m_vecEffectData.clear(); // ���� ������ �ʱ�ȭ

    // FinalEffects �ʿ��� �� Layer ������ �����ͼ� m_vecEffectData�� �߰�
    for (const auto& layerPair : m_pEffect_Manager->m_FinalEffects)
    {
        const wstring& layerName = layerPair.first;
        CEffect_Layer* pLayer = layerPair.second;

        EFFECT_LAYER_DATA layerData;
        layerData.layerName = layerName;
        layerData.duration = pLayer->m_fDuration;
        layerData.tickPerSecond = pLayer->m_fTickPerSecond;
        layerData.keyFramesCount = pLayer->m_iNumKeyFrames;
        layerData.iNumEffect = pLayer->m_MixtureEffects.size();

        // �� ���̾� ���� ����Ʈ�� ������ ����
        for (auto& pEffect : pLayer->Get_Effects())
        {
            EFFECT_DATA effectData;
            effectData.effectName = pEffect->m_EffectName;
            effectData.modelName = pEffect->m_ModelName;
            effectData.maskTextureName = pEffect->m_MaskTextureName;
            effectData.diffuseTextureName = pEffect->m_DiffuseTextureName;
            effectData.effectType = pEffect->m_eEffect_Type;
            effectData.renderIndex = pEffect->m_iRenderIndex;
            effectData.passIndex = pEffect->m_iPassIndex;
            effectData.uniqueIndex = pEffect->m_iUnique_Index;
            effectData.isLoop = pEffect->m_bIsLoop;
            effectData.isNotPlaying = pEffect->m_bIsNotPlaying;
            effectData.position = pEffect->Get_Effect_Position();
            effectData.scale = pEffect->Get_Effect_Scaled();
            effectData.rotation = pEffect->Get_Effect_Rotation();
            effectData.iNumKeyFrame = pEffect->m_pAnimation->m_EffectKeyFrames.size();

            // ����Ʈ�� Ű������ ���� �߰�
            for (const auto& keyFramePair : pEffect->m_pAnimation->m_EffectKeyFrames)
            {
                EFFECT_KEYFRAME_DATA keyFrameData;
                keyFrameData.keyFrameNumber = keyFramePair.first;
                keyFrameData.position = keyFramePair.second.vPosition;
                keyFrameData.scale = keyFramePair.second.vScale;
                keyFrameData.rotation = keyFramePair.second.vRotation;
                keyFrameData.curTime = keyFramePair.second.fCurTime;
                keyFrameData.duration = keyFramePair.second.fDuration;

                effectData.keyframes.push_back(keyFrameData);
            }

            layerData.effects.push_back(effectData); // ���̾ ����Ʈ �߰�
        }

        m_vecEffectData.push_back(layerData); // ���̾� �����͸� ���� ���Ϳ� �߰�
    }

    // GameInstance�� �ִ� Save_Effects �Լ��� m_vecEffectData ����
    if (FAILED(m_pGameInstance->Save_Effects(filename, &m_vecEffectData)))
    {
        return E_FAIL; // ���� ���� �� ���� ��ȯ
    }

    return S_OK; // ���� ���� �� S_OK ��ȯ
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
            vector<_int> vecTabIndexs;


            vecTabIndexs = m_pEffect_Manager->Delete_All_Test_Effect();

            for (size_t i = 0; i < vecTabIndexs.size(); i++)
            {
                _int Index = vecTabIndexs[i];

                CImgui_Manager::Get_Instance()->Delete_Shader_Tab(Index);
            }
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

void CIMGUI_Effect_Tab::Render_For_Layer_KeyFrame(_float fTimeDelta)
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
                    m_pEffect_Manager->Set_Render_Layer(selectedLayerName);
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        ImGui::SameLine();
        ImGui::Dummy(ImVec2(70.0f, 0.0f)); // ���� 70px ���� �߰�
        ImGui::SameLine();

        static bool isPlaying = false;
        CEffect_Layer* pLayer = m_pEffect_Manager->Find_Effect_Layer(selectedLayerName);

        if (ImGui::Button("Play"))
        {
            isPlaying = true;
        }

        ImGui::SameLine();

        if (ImGui::Button("Pause"))
        {
            isPlaying = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Anim Pos Reset"))
        {
            m_pEffect_Manager->Set_Layer_Animation_Position(selectedLayerName, 0.f);
        }

        if (isPlaying)
        {
            m_pEffect_Manager->Play_Layer_Animation(fTimeDelta, selectedLayerName);
        }

        int totalKeyframes = 0;
        const float frameInterval = 1.0f / 60.0f;

        if (pLayer)
        {
            float layerDuration = pLayer->m_fDuration;
            float tickPerSecond = pLayer->m_fTickPerSecond;
            float AnimCurPos = pLayer->m_fCurrentAnimPosition;

            ImGui::Text("Layer Duration:");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(200.0f);
            if (ImGui::InputFloat("##LayerDuration", &layerDuration, 0.1f, 1.0f, "%.2f"))
            {
                pLayer->m_fDuration = max(0.0f, layerDuration);
            }

            ImGui::SameLine();
            ImGui::Text("Animation Speed:");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(200.0f);
            if (ImGui::InputFloat("##AnimationSpeed", &tickPerSecond, 0.01f, 0.01f, "%.2f"))
            {
                pLayer->m_fTickPerSecond = max(0.01f, tickPerSecond);
            }

            ImGui::SameLine();
            ImGui::Text("Current Anim Pos:");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(200.0f);
            if (ImGui::InputFloat("##AnimPos", &AnimCurPos, 0.001f, 0.001f, "%.3f"))
            {
                pLayer->m_fCurrentAnimPosition = max(0.0f, min(AnimCurPos, pLayer->m_fDuration));
                m_pEffect_Manager->Set_Layer_Animation_Position(selectedLayerName, AnimCurPos);
            }

            totalKeyframes = static_cast<int>(layerDuration / frameInterval);
            pLayer->m_iNumKeyFrames = totalKeyframes;
        }

        ImGui::Separator();

        auto effectNames = m_pEffect_Manager->Get_In_Layer_Effect_List(&selectedLayerName);

        if (!effectNames.empty())
        {
            const int buttonSize = 20;
            const float effectNameWidth = 150.0f;

            ImGui::BeginChild("TimelineRegion", ImVec2(0, 300), false, ImGuiWindowFlags_HorizontalScrollbar);

            float currentXPosition = pLayer ? (pLayer->m_fCurrentAnimPosition / pLayer->m_fDuration) * (totalKeyframes * (buttonSize + 5.0f)) : 0.0f;

            ImDrawList* drawList = ImGui::GetWindowDrawList();
            ImVec2 start = ImGui::GetCursorScreenPos();
            ImVec2 lineStart(start.x + effectNameWidth + currentXPosition, start.y);
            ImVec2 lineEnd(lineStart.x, lineStart.y + 300);

            drawList->AddLine(lineStart, lineEnd, IM_COL32(255, 0, 0, 255), 2.0f);

            ImGui::SetCursorScreenPos(lineStart);
            ImGui::InvisibleButton("##DragRedLine", ImVec2(10.0f, 30.0f));

            if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
            {
                float newXPos = ImGui::GetMousePos().x - start.x - effectNameWidth;
                float normalizedPosition = newXPos / (totalKeyframes * (buttonSize + 5.0f));
                pLayer->m_fCurrentAnimPosition = pLayer->m_fDuration * normalizedPosition;

                if (pLayer->m_fCurrentAnimPosition < 0.0f)
                    pLayer->m_fCurrentAnimPosition = 0.0f;

                if (pLayer->m_fCurrentAnimPosition > pLayer->m_fDuration)
                    pLayer->m_fCurrentAnimPosition = pLayer->m_fDuration;

                if (!isPlaying)
                    m_pEffect_Manager->Set_Layer_Animation_Position(selectedLayerName, pLayer->m_fCurrentAnimPosition);
            }

            std::vector<bool> effectChecks(effectNames.size(), false);

            for (int item = 0; item < effectNames.size(); item++)
            {
                CEffect* pEffect = m_pEffect_Manager->Find_In_Layer_Effect(selectedLayerName, effectNames[item]);
                if (pEffect)
                {
                    effectChecks[item] = pEffect->m_bIsLoop;
                }
            }

            for (int item = 0; item < effectNames.size(); item++)
            {
                ImGui::PushID(item);

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
                ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(180, 197, 230, 230));
                ImGui::PushStyleColor(ImGuiCol_CheckMark, IM_COL32(0, 0, 0, 230));

                bool isChecked = effectChecks[item];
                if (ImGui::Checkbox("##EffectCheck", &isChecked))
                {
                    CEffect* pEffect = m_pEffect_Manager->Find_In_Layer_Effect(selectedLayerName, effectNames[item]);
                    if (pEffect)
                    {
                        pEffect->m_bIsLoop = isChecked;
                    }
                    effectChecks[item] = isChecked;
                }

                ImGui::PopStyleColor(2);
                ImGui::PopStyleVar(2);

                string effectNameUTF8 = WStringToUTF8(effectNames[item]);
                ImGui::SameLine();
                ImGui::TextWrapped("%s", effectNameUTF8.c_str());
                ImGui::SameLine(effectNameWidth);
                ImGui::Dummy(ImVec2(30.0f, 0.0f));
                ImGui::SameLine();

                for (int frame = 0; frame < totalKeyframes; frame++)
                {
                    float curtime = frame * frameInterval;
                    ImGui::SetCursorPosX(effectNameWidth + frame * (buttonSize + 5.0f));
                    ImGui::PushID(frame + item * totalKeyframes);

                    bool isKeyFrameExist = m_pEffect_Manager->Find_KeyFrame(selectedLayerName, effectNames[item], frame);
                    if (isKeyFrameExist)
                    {
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.8f, 0.3f, 0.7f));
                    }

                    if (ImGui::Button("##", ImVec2(buttonSize, buttonSize)))
                    {
                        selectedEffectName = effectNameUTF8;
                        selectedFrame = frame;
                        openKeyFrameWindow = true;
                        initialized = false;
                        SelectCurTime = curtime;
                    }

                    if (isKeyFrameExist)
                    {
                        ImGui::PopStyleColor();
                    }

                    ImGui::PopID();
                    ImGui::SameLine();
                }
                ImGui::NewLine();

                ImGui::PopID();
            }

            ImGui::EndChild();
        }
    }
}

void CIMGUI_Effect_Tab::Render_For_Effect_KeyFrame()
{
    EFFECT_KEYFRAME newKeyFrame;
    _float3 CurPosition = { 0.f, 0.f, 0.f };
    _float3 CurScale = { 0.f, 0.f, 0.f };
    _float3 CurRotation = { 0.f, 0.f, 0.f };
    _bool IsNotPlaying = { false };

    newKeyFrame.fDuration = m_pEffect_Manager->Find_Effect_Layer(selectedLayerName)->m_fDuration;

    // CurTime�� ���õ� ������ �ð��� ���� �ڵ� ����
    const float frameInterval = 1.0f / 60.0f;
    newKeyFrame.fCurTime = selectedFrame * frameInterval;

    ImGui::Begin("Keyframe Editor", &openKeyFrameWindow, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Editing Keyframe for Effect: %s", selectedEffectName.c_str());
    ImGui::Text("Frame: %d", selectedFrame);
    ImGui::SameLine();
    ImGui::Text("CurAnimPos: %.3f", newKeyFrame.fCurTime);
    ImGui::SameLine();
    ImGui::Text("Duration: %.2f", newKeyFrame.fDuration);

    // ó�� â�� �� �� ����� Ű������ �ҷ�����
    if (!initialized)
    {
        bool keyFrameExists = m_pEffect_Manager->Find_KeyFrame(selectedLayerName, UTF8ToWString(selectedEffectName), selectedFrame);

        if (keyFrameExists)
        {
            newKeyFrame = m_pEffect_Manager->Get_KeyFrame(selectedLayerName, UTF8ToWString(selectedEffectName), selectedFrame);

            CurPosition = newKeyFrame.vPosition;
            CurScale = newKeyFrame.vScale;
            CurRotation = newKeyFrame.vRotation;
            IsNotPlaying = newKeyFrame.bIsNotPlaying;
        }
        else
        {
            newKeyFrame = m_pEffect_Manager->Get_Near_Front_KeyFrame(selectedLayerName, UTF8ToWString(selectedEffectName), selectedFrame);

            CurPosition = newKeyFrame.vPosition;
            CurScale = newKeyFrame.vScale;
            CurRotation = newKeyFrame.vRotation;
            IsNotPlaying = newKeyFrame.bIsNotPlaying;
        }


        initialized = true;
    }
    else
    {
        CurPosition = m_pEffect_Manager->Get_Layer_Effect_Position(selectedLayerName, UTF8ToWString(selectedEffectName));
        CurScale = m_pEffect_Manager->Get_Layer_Effect_Scaled(selectedLayerName, UTF8ToWString(selectedEffectName));
        CurRotation = m_pEffect_Manager->Get_Layer_Effect_Rotation(selectedLayerName, UTF8ToWString(selectedEffectName));
        IsNotPlaying = m_pEffect_Manager->Get_Layer_Effect_IsPlaying(selectedLayerName, UTF8ToWString(selectedEffectName));
    }



    ImGui::Text("Effect Keyframe Settings");

    // Is Not Playing üũ�ڽ�
    ImGui::Checkbox("Is Not Playing", &IsNotPlaying);
    {
        m_pEffect_Manager->Set_Layer_Effect_IsNotPlaying(selectedLayerName, UTF8ToWString(selectedEffectName), IsNotPlaying);
    }

    ImGui::Separator();

    m_pEffect_Manager->Set_Layer_Effect_Position(selectedLayerName, UTF8ToWString(selectedEffectName), CurPosition);
    m_pEffect_Manager->Set_Layer_Effect_Scaled(selectedLayerName, UTF8ToWString(selectedEffectName), CurScale);
    m_pEffect_Manager->Set_Layer_Effect_Rotation(selectedLayerName, UTF8ToWString(selectedEffectName), CurRotation);

    // Position ����
    ImGui::Text("Position");

    ImGui::Text("X"); ImGui::SameLine();
    if (ImGui::SliderFloat("##Position X Slider", &CurPosition.x, -100.0f, 100.0f))
        m_pEffect_Manager->Set_Layer_Effect_Position(selectedLayerName, UTF8ToWString(selectedEffectName), CurPosition);
    ImGui::SameLine();
    if (ImGui::Button("-##Position X Dec")) { CurPosition.x -= 0.1f; m_pEffect_Manager->Set_Layer_Effect_Position(selectedLayerName, UTF8ToWString(selectedEffectName), CurPosition); }
    ImGui::SameLine();
    if (ImGui::Button("+##Position X Inc")) { CurPosition.x += 0.1f; m_pEffect_Manager->Set_Layer_Effect_Position(selectedLayerName, UTF8ToWString(selectedEffectName), CurPosition); }
    ImGui::SameLine();
    if (ImGui::InputFloat("##Position X Input", &CurPosition.x, 0.1f)) {
        m_pEffect_Manager->Set_Layer_Effect_Position(selectedLayerName, UTF8ToWString(selectedEffectName), CurPosition);
    }

    ImGui::Text("Y"); ImGui::SameLine();
    if (ImGui::SliderFloat("##Position Y Slider", &CurPosition.y, -100.0f, 100.0f))
        m_pEffect_Manager->Set_Layer_Effect_Position(selectedLayerName, UTF8ToWString(selectedEffectName), CurPosition);
    ImGui::SameLine();
    if (ImGui::Button("-##Position Y Dec")) { CurPosition.y -= 0.1f; m_pEffect_Manager->Set_Layer_Effect_Position(selectedLayerName, UTF8ToWString(selectedEffectName), CurPosition); }
    ImGui::SameLine();
    if (ImGui::Button("+##Position Y Inc")) { CurPosition.y += 0.1f; m_pEffect_Manager->Set_Layer_Effect_Position(selectedLayerName, UTF8ToWString(selectedEffectName), CurPosition); }
    ImGui::SameLine();
    if (ImGui::InputFloat("##Position Y", &CurPosition.y, 0.1f))
    {
        m_pEffect_Manager->Set_Layer_Effect_Position(selectedLayerName, UTF8ToWString(selectedEffectName), CurPosition);
    }

    ImGui::Text("Z"); ImGui::SameLine();
    if (ImGui::SliderFloat("##Position Z Slider", &CurPosition.z, -100.0f, 100.0f))
        m_pEffect_Manager->Set_Layer_Effect_Position(selectedLayerName, UTF8ToWString(selectedEffectName), CurPosition);
    ImGui::SameLine();
    if (ImGui::Button("-##Position Z Dec")) { CurPosition.z -= 0.1f; m_pEffect_Manager->Set_Layer_Effect_Position(selectedLayerName, UTF8ToWString(selectedEffectName), CurPosition); }
    ImGui::SameLine();
    if (ImGui::Button("+##Position Z Inc")) { CurPosition.z += 0.1f; m_pEffect_Manager->Set_Layer_Effect_Position(selectedLayerName, UTF8ToWString(selectedEffectName), CurPosition); }
    ImGui::SameLine();
    if(ImGui::InputFloat("##Position Z", &CurPosition.z, 0.1f))
    {
        m_pEffect_Manager->Set_Layer_Effect_Position(selectedLayerName, UTF8ToWString(selectedEffectName), CurPosition);
    }

    ImGui::Separator();

    // Scale ����
    ImGui::Text("Scale");

    ImGui::Text("X"); ImGui::SameLine();
    if (ImGui::SliderFloat("##Scale X Slider", &CurScale.x, 0.01f, 100.0f))
        m_pEffect_Manager->Set_Layer_Effect_Scaled(selectedLayerName, UTF8ToWString(selectedEffectName), CurScale);
    ImGui::SameLine();
    if (ImGui::Button("-##Scale X Dec")) { CurScale.x = max(0.01f, CurScale.x - 0.1f); m_pEffect_Manager->Set_Layer_Effect_Scaled(selectedLayerName, UTF8ToWString(selectedEffectName), CurScale); }
    ImGui::SameLine();
    if (ImGui::Button("+##Scale X Inc")) { CurScale.x += 0.1f; m_pEffect_Manager->Set_Layer_Effect_Scaled(selectedLayerName, UTF8ToWString(selectedEffectName), CurScale); }
    ImGui::SameLine();
    if(ImGui::InputFloat("##Scale X", &CurScale.x, 0.1f))
    {
        m_pEffect_Manager->Set_Layer_Effect_Scaled(selectedLayerName, UTF8ToWString(selectedEffectName), CurScale);
    }

    ImGui::Text("Y"); ImGui::SameLine();
    if (ImGui::SliderFloat("##Scale Y Slider", &CurScale.y, 0.01f, 100.0f))
        m_pEffect_Manager->Set_Layer_Effect_Scaled(selectedLayerName, UTF8ToWString(selectedEffectName), CurScale);
    ImGui::SameLine();
    if (ImGui::Button("-##Scale Y Dec")) { CurScale.y = max(0.01f, CurScale.y - 0.1f); m_pEffect_Manager->Set_Layer_Effect_Scaled(selectedLayerName, UTF8ToWString(selectedEffectName), CurScale); }
    ImGui::SameLine();
    if (ImGui::Button("+##Scale Y Inc")) { CurScale.y += 0.1f; m_pEffect_Manager->Set_Layer_Effect_Scaled(selectedLayerName, UTF8ToWString(selectedEffectName), CurScale); }
    ImGui::SameLine();
    if(ImGui::InputFloat("##Scale Y", &CurScale.y, 0.1f))
    {
        m_pEffect_Manager->Set_Layer_Effect_Scaled(selectedLayerName, UTF8ToWString(selectedEffectName), CurScale);
    }


    ImGui::Text("Z"); ImGui::SameLine();
    if (ImGui::SliderFloat("##Scale Z Slider", &CurScale.z, 0.01f, 100.0f))
        m_pEffect_Manager->Set_Layer_Effect_Scaled(selectedLayerName, UTF8ToWString(selectedEffectName), CurScale);
    ImGui::SameLine();
    if (ImGui::Button("-##Scale Z Dec")) { CurScale.z = max(0.01f, CurScale.z - 0.1f); m_pEffect_Manager->Set_Layer_Effect_Scaled(selectedLayerName, UTF8ToWString(selectedEffectName), CurScale); }
    ImGui::SameLine();
    if (ImGui::Button("+##Scale Z Inc")) { CurScale.z += 0.1f; m_pEffect_Manager->Set_Layer_Effect_Scaled(selectedLayerName, UTF8ToWString(selectedEffectName), CurScale); }
    ImGui::SameLine();
    if(ImGui::InputFloat("##Scale Z", &CurScale.z, 0.1f))
    {
        m_pEffect_Manager->Set_Layer_Effect_Scaled(selectedLayerName, UTF8ToWString(selectedEffectName), CurScale);
    }

    ImGui::Separator();

    // Rotation ����
    ImGui::Text("Rotation");

    ImGui::Text("X"); ImGui::SameLine();
    if (ImGui::SliderFloat("##Rotation X Slider", &CurRotation.x, 0.0f, 360.0f))
        m_pEffect_Manager->Set_Layer_Effect_Rotation(selectedLayerName, UTF8ToWString(selectedEffectName), CurRotation);
    ImGui::SameLine();
    if (ImGui::Button("-##Rotation X Dec")) { CurRotation.x = fmod(CurRotation.x - 1.0f + 360.0f, 360.0f); m_pEffect_Manager->Set_Layer_Effect_Rotation(selectedLayerName, UTF8ToWString(selectedEffectName), CurRotation); }
    ImGui::SameLine();
    if (ImGui::Button("+##Rotation X Inc")) { CurRotation.x = fmod(CurRotation.x + 1.0f, 360.0f); m_pEffect_Manager->Set_Layer_Effect_Rotation(selectedLayerName, UTF8ToWString(selectedEffectName), CurRotation); }
    ImGui::SameLine();
    if(ImGui::InputFloat("##Rotation X", &CurRotation.x, 1.0f))
    {
        m_pEffect_Manager->Set_Layer_Effect_Rotation(selectedLayerName, UTF8ToWString(selectedEffectName), CurRotation);
    }

    ImGui::Text("Y"); ImGui::SameLine();
    if (ImGui::SliderFloat("##Rotation Y Slider", &CurRotation.y, 0.0f, 360.0f))
        m_pEffect_Manager->Set_Layer_Effect_Rotation(selectedLayerName, UTF8ToWString(selectedEffectName), CurRotation);
    ImGui::SameLine();
    if (ImGui::Button("-##Rotation Y Dec")) { CurRotation.y = fmod(CurRotation.y - 1.0f + 360.0f, 360.0f); m_pEffect_Manager->Set_Layer_Effect_Rotation(selectedLayerName, UTF8ToWString(selectedEffectName), CurRotation); }
    ImGui::SameLine();
    if (ImGui::Button("+##Rotation Y Inc")) { CurRotation.y = fmod(CurRotation.y + 1.0f, 360.0f); m_pEffect_Manager->Set_Layer_Effect_Rotation(selectedLayerName, UTF8ToWString(selectedEffectName), CurRotation); }
    ImGui::SameLine();
    if(ImGui::InputFloat("##Rotation Y", &CurRotation.y, 1.0f))
    {
        m_pEffect_Manager->Set_Layer_Effect_Rotation(selectedLayerName, UTF8ToWString(selectedEffectName), CurRotation);
    }

    ImGui::Text("Z"); ImGui::SameLine();
    if (ImGui::SliderFloat("##Rotation Z Slider", &CurRotation.z, 0.0f, 360.0f))
        m_pEffect_Manager->Set_Layer_Effect_Rotation(selectedLayerName, UTF8ToWString(selectedEffectName), CurRotation);
    ImGui::SameLine();
    if (ImGui::Button("-##Rotation Z Dec")) { CurRotation.z = fmod(CurRotation.z - 1.0f + 360.0f, 360.0f); m_pEffect_Manager->Set_Layer_Effect_Rotation(selectedLayerName, UTF8ToWString(selectedEffectName), CurRotation); }
    ImGui::SameLine();
    if (ImGui::Button("+##Rotation Z Inc")) { CurRotation.z = fmod(CurRotation.z + 1.0f, 360.0f); m_pEffect_Manager->Set_Layer_Effect_Rotation(selectedLayerName, UTF8ToWString(selectedEffectName), CurRotation); }
    ImGui::SameLine();
    if (ImGui::InputFloat("##Rotation Z", &CurRotation.z, 1.0f))
    {
        m_pEffect_Manager->Set_Layer_Effect_Rotation(selectedLayerName, UTF8ToWString(selectedEffectName), CurRotation);
    }

    ImGui::Separator();

    // Keyframe ���� ��ư
    if (ImGui::Button("Save Keyframe"))
    {
        newKeyFrame.vPosition = CurPosition;
        newKeyFrame.vScale = CurScale;
        newKeyFrame.vRotation = CurRotation;
        newKeyFrame.bIsNotPlaying = IsNotPlaying;
        newKeyFrame.fCurTime = selectedFrame * frameInterval;

        // ������Ʈ�� newKeyFrame�� ����
        m_pEffect_Manager->Add_KeyFrame(selectedLayerName, UTF8ToWString(selectedEffectName), selectedFrame, newKeyFrame);

        ImGui::Text("Keyframe saved!");
    }

    ImGui::SameLine();

    if (ImGui::Button("Delete Keyframe"))
    {
        m_pEffect_Manager->Delete_KeyFrame(selectedLayerName, UTF8ToWString(selectedEffectName), selectedFrame);
        ImGui::Text("Keyframe deleted!");
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

    ModelName.clear();
    TextureName.clear();
}
