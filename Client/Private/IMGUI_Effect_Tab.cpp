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
    // UTF-8 문자열의 길이를 확인하고, 변환할 wstring의 길이를 계산합니다.
    int wstrLength = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);

    // 변환할 wstring의 크기를 할당
    std::wstring wstr(wstrLength, 0);

    // 변환 작업 수행
    MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &wstr[0], wstrLength);

    // 널 문자를 제거하고 반환
    wstr.pop_back(); // null character 제거
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

        if (ImGui::Button("Delete Selete Effect"))
        {
            // 테스트 이펙트 벡터에서 선택한 객체의 인덱스 찾아 전달
            _uint EffectIndex = CImgui_Manager::Get_Instance()->Get_CurShaderTab_Index();
            CImgui_Manager::Get_Instance()->Delete_Shader_Tab(EffectIndex);

            m_pEffect_Manager->Delete_Test_Effect(CImgui_Manager::Get_Instance()->Get_CurShaderTab_Id());

        }

        ImGui::SameLine();

        if (ImGui::Button("Delete All Effect"))
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

        // X 축
        ImGui::Text("X"); ImGui::SameLine();
        if (ImGui::SliderFloat("##Position X Slider", &CurPosition.x, -100.0f, 100.0f))
            m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition);
        ImGui::SameLine();
        if (ImGui::Button("-##Position X Dec")) { CurPosition.x -= 0.1f; m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition); } ImGui::SameLine();
        if (ImGui::Button("+##Position X Inc")) { CurPosition.x += 0.1f; m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition); } ImGui::SameLine();
        if (ImGui::InputFloat("##Position X", &CurPosition.x, 0.1f))
            m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition);

        // Y 축
        ImGui::Text("Y"); ImGui::SameLine();
        if (ImGui::SliderFloat("##Position Y Slider", &CurPosition.y, -100.0f, 100.0f))
            m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition);
        ImGui::SameLine();
        if (ImGui::Button("-##Position Y Dec")) { CurPosition.y -= 0.1f; m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition); } ImGui::SameLine();
        if (ImGui::Button("+##Position Y Inc")) { CurPosition.y += 0.1f; m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition); } ImGui::SameLine();
        if (ImGui::InputFloat("##Position Y", &CurPosition.y, 0.1f))
            m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition);

        // Z 축
        ImGui::Text("Z"); ImGui::SameLine();
        if (ImGui::SliderFloat("##Position Z Slider", &CurPosition.z, -100.0f, 100.0f))
            m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition);
        ImGui::SameLine();
        if (ImGui::Button("-##Position Z Dec")) { CurPosition.z -= 0.1f; m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition); } ImGui::SameLine();
        if (ImGui::Button("+##Position Z Inc")) { CurPosition.z += 0.1f; m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition); } ImGui::SameLine();
        if (ImGui::InputFloat("##Position Z", &CurPosition.z, 0.1f))
            m_pEffect_Manager->Set_Effect_Position(EffectId, CurPosition);

        ImGui::Separator();

        ImGui::Text("Scale");

        // X 축
        ImGui::Text("X"); ImGui::SameLine();
        if (ImGui::SliderFloat("##Scale X Slider", &CurScaled.x, 0.01f, 100.0f)) // 최소값 0.01
            m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled);
        ImGui::SameLine();
        if (ImGui::Button("-##Scale X Dec")) { CurScaled.x = max(0.01f, CurScaled.x - 0.1f); m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled); } ImGui::SameLine();
        if (ImGui::Button("+##Scale X Inc")) { CurScaled.x += 0.1f; m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled); } ImGui::SameLine();
        if (ImGui::InputFloat("##Scale X", &CurScaled.x, 0.1f)) {
            CurScaled.x = max(0.01f, CurScaled.x);
            m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled);
        }

        // Y 축
        ImGui::Text("Y"); ImGui::SameLine();
        if (ImGui::SliderFloat("##Scale Y Slider", &CurScaled.y, 0.01f, 100.0f)) // 최소값 0.01
            m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled);
        ImGui::SameLine();
        if (ImGui::Button("-##Scale Y Dec")) { CurScaled.y = max(0.01f, CurScaled.y - 0.1f); m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled); } ImGui::SameLine();
        if (ImGui::Button("+##Scale Y Inc")) { CurScaled.y += 0.1f; m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled); } ImGui::SameLine();
        if (ImGui::InputFloat("##Scale Y", &CurScaled.y, 0.1f)) {
            CurScaled.y = max(0.01f, CurScaled.y);
            m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled);
        }

        // Z 축
        ImGui::Text("Z"); ImGui::SameLine();
        if (ImGui::SliderFloat("##Scale Z Slider", &CurScaled.z, 0.01f, 100.0f)) // 최소값 0.01
            m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled);
        ImGui::SameLine();
        if (ImGui::Button("-##Scale Z Dec")) { CurScaled.z = max(0.01f, CurScaled.z - 0.1f); m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled); } ImGui::SameLine();
        if (ImGui::Button("+##Scale Z Inc")) { CurScaled.z += 0.1f; m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled); } ImGui::SameLine();
        if (ImGui::InputFloat("##Scale Z", &CurScaled.z, 0.1f)) {
            CurScaled.z = max(0.01f, CurScaled.z);
            m_pEffect_Manager->Set_Effect_Scaled(EffectId, CurScaled);
        }

        ImGui::Separator();

        ImGui::Text("Rotation");

        // X 축
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

        // Y 축
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

        // Z 축
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
            layerListUTF8.push_back(WStringToUTF8(layer)); // wstring을 UTF-8로 변환하는 함수가 필요
        }

        // 현재 선택된 레이어 인덱스를 유지할 변수
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

        // 선택된 레이어에 전체 객체 추가 버튼
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

void CIMGUI_Effect_Tab::Render_For_Effect_KeyFrame()
{
    ImGui::Begin("Keyframe Editor", &openKeyFrameWindow, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Editing Keyframe for Effect: %s", selectedEffectName.c_str());
    ImGui::Text("Frame: %d", selectedFrame);

    static EFFECT_KEYFRAME newKeyFrame;

    ImGui::Text("Effect Keyframe Settings");

    // Scale 섹션
    ImGui::Text("Scale");

    // X 축
    ImGui::Text("X"); ImGui::SameLine();
    ImGui::SliderFloat("##Scale X Slider", &newKeyFrame.vScale.x, 0.0f, 100.0f); ImGui::SameLine();
    if (ImGui::Button("-##Scale X Dec")) newKeyFrame.vScale.x -= 0.1f; ImGui::SameLine();
    if (ImGui::Button("+##Scale X Inc")) newKeyFrame.vScale.x += 0.1f; ImGui::SameLine();
    ImGui::InputFloat("##Scale X", &newKeyFrame.vScale.x, 0.1f);

    // Y 축
    ImGui::Text("Y"); ImGui::SameLine();
    ImGui::SliderFloat("##Scale Y Slider", &newKeyFrame.vScale.y, 0.0f, 100.0f); ImGui::SameLine();
    if (ImGui::Button("-##Scale Y Dec")) newKeyFrame.vScale.y -= 0.1f; ImGui::SameLine();
    if (ImGui::Button("+##Scale Y Inc")) newKeyFrame.vScale.y += 0.1f; ImGui::SameLine();
    ImGui::InputFloat("##Scale Y", &newKeyFrame.vScale.y, 0.1f);

    // Z 축
    ImGui::Text("Z"); ImGui::SameLine();
    ImGui::SliderFloat("##Scale Z Slider", &newKeyFrame.vScale.z, 0.0f, 100.0f); ImGui::SameLine();
    if (ImGui::Button("-##Scale Z Dec")) newKeyFrame.vScale.z -= 0.1f; ImGui::SameLine();
    if (ImGui::Button("+##Scale Z Inc")) newKeyFrame.vScale.z += 0.1f; ImGui::SameLine();
    ImGui::InputFloat("##Scale Z", &newKeyFrame.vScale.z, 0.1f);

    ImGui::Separator();

    // Rotation 섹션
    ImGui::Text("Rotation");

    // X 축
    ImGui::Text("X"); ImGui::SameLine();
    ImGui::SliderFloat("##Rotation X Slider", &newKeyFrame.vRotation.x, 0.0f, 360.0f); ImGui::SameLine();
    if (ImGui::Button("-##Rotation X Dec")) newKeyFrame.vRotation.x -= 0.1f; ImGui::SameLine();
    if (ImGui::Button("+##Rotation X Inc")) newKeyFrame.vRotation.x += 0.1f; ImGui::SameLine();
    ImGui::InputFloat("##Rotation X", &newKeyFrame.vRotation.x, 0.1f);

    // Y 축
    ImGui::Text("Y"); ImGui::SameLine();
    ImGui::SliderFloat("##Rotation Y Slider", &newKeyFrame.vRotation.y, 0.0f, 360.0f); ImGui::SameLine();
    if (ImGui::Button("-##Rotation Y Dec")) newKeyFrame.vRotation.y -= 0.1f; ImGui::SameLine();
    if (ImGui::Button("+##Rotation Y Inc")) newKeyFrame.vRotation.y += 0.1f; ImGui::SameLine();
    ImGui::InputFloat("##Rotation Y", &newKeyFrame.vRotation.y, 0.1f);

    // Z 축
    ImGui::Text("Z"); ImGui::SameLine();
    ImGui::SliderFloat("##Rotation Z Slider", &newKeyFrame.vRotation.z, 0.0f, 360.0f); ImGui::SameLine();
    if (ImGui::Button("-##Rotation Z Dec")) newKeyFrame.vRotation.z -= 0.1f; ImGui::SameLine();
    if (ImGui::Button("+##Rotation Z Inc")) newKeyFrame.vRotation.z += 0.1f; ImGui::SameLine();
    ImGui::InputFloat("##Rotation Z", &newKeyFrame.vRotation.z, 0.1f);

    ImGui::Separator();

    // Position 섹션
    ImGui::Text("Position");

    // X 축
    ImGui::Text("X"); ImGui::SameLine();
    ImGui::SliderFloat("##Position X Slider", &newKeyFrame.vPosition.x, -100.0f, 100.0f); ImGui::SameLine();
    if (ImGui::Button("-##Position X Dec")) newKeyFrame.vPosition.x -= 0.1f; ImGui::SameLine();
    if (ImGui::Button("+##Position X Inc")) newKeyFrame.vPosition.x += 0.1f; ImGui::SameLine();
    ImGui::InputFloat("##Position X", &newKeyFrame.vPosition.x, 0.1f);

    // Y 축
    ImGui::Text("Y"); ImGui::SameLine();
    ImGui::SliderFloat("##Position Y Slider", &newKeyFrame.vPosition.y, -100.0f, 100.0f); ImGui::SameLine();
    if (ImGui::Button("-##Position Y Dec")) newKeyFrame.vPosition.y -= 0.1f; ImGui::SameLine();
    if (ImGui::Button("+##Position Y Inc")) newKeyFrame.vPosition.y += 0.1f; ImGui::SameLine();
    ImGui::InputFloat("##Position Y", &newKeyFrame.vPosition.y, 0.1f);

    // Z 축
    ImGui::Text("Z"); ImGui::SameLine();
    ImGui::SliderFloat("##Position Z Slider", &newKeyFrame.vPosition.z, -100.0f, 100.0f); ImGui::SameLine();
    if (ImGui::Button("-##Position Z Dec")) newKeyFrame.vPosition.z -= 0.1f; ImGui::SameLine();
    if (ImGui::Button("+##Position Z Inc")) newKeyFrame.vPosition.z += 0.1f; ImGui::SameLine();
    ImGui::InputFloat("##Position Z", &newKeyFrame.vPosition.z, 0.1f);

    ImGui::Separator();

    // fCurTime: 슬라이더, 입력창, 증감 버튼
    ImGui::Text("Current Time");
    ImGui::SliderFloat("##CurTime Slider", &newKeyFrame.fCurTime, 0.0f, 100.0f); ImGui::SameLine();
    if (ImGui::Button("-##CurTime")) newKeyFrame.fCurTime -= 0.1f; ImGui::SameLine();
    if (ImGui::Button("+##CurTime")) newKeyFrame.fCurTime += 0.1f; ImGui::SameLine();
    ImGui::InputFloat("CurTime", &newKeyFrame.fCurTime);

    ImGui::Separator();

    // fDuration: 입력창과 0.5씩 증감 버튼
    ImGui::Text("Duration");
    if (ImGui::Button("-##Duration")) newKeyFrame.fDuration -= 0.5f; ImGui::SameLine();
    if (ImGui::Button("+##Duration")) newKeyFrame.fDuration += 0.5f; ImGui::SameLine();
    ImGui::InputFloat("Duration", &newKeyFrame.fDuration);

    ImGui::Separator();

    // Add Keyframe 버튼과 Save Keyframe, Reset Keyframe 버튼
    if (ImGui::Button("Add Keyframe"))
    {
        m_pEffect_Manager->Add_KeyFrame(CImgui_Manager::Get_Instance()->Get_CurShaderTab_Id(), newKeyFrame);
        ImGui::Text("Keyframe added!");
    }

    ImGui::SameLine();

    if (ImGui::Button("Save Keyframe"))
    {
        // Save 기능 호출
        ImGui::Text("Keyframe saved!");
    }

    ImGui::SameLine();

    if (ImGui::Button("Reset Keyframe"))
    {
        // 키프레임 초기화
        newKeyFrame = EFFECT_KEYFRAME();
        ImGui::Text("Keyframe reset!");
    }

    ImGui::Separator();

    ImGui::End();
}


void CIMGUI_Effect_Tab::Render_For_Layer_KeyFrame()
{
    // 레이어 목록 가져오기
    auto LayerList = m_pEffect_Manager->Get_Layer_List();

    if (!LayerList.empty())
    {
        vector<string> layerListUTF8;
        for (const auto& layer : LayerList)
        {
            layerListUTF8.push_back(WStringToUTF8(layer)); // wstring을 UTF-8로 변환
        }

        static int selectedLayerIndex = 0;
        const char* currentLayer = layerListUTF8[selectedLayerIndex].c_str();

        // 레이어 선택 콤보 박스
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

        ImGui::Separator();
        // 선택된 레이어의 이름을 wstring으로 변환
        wstring selectedLayerWString = LayerList[selectedLayerIndex];

        // 선택된 레이어의 이펙트 목록 가져오기
        auto effectNames = m_pEffect_Manager->Get_In_Layer_Effect_List(&selectedLayerWString);

        CEffect_Layer* pLayer = m_pEffect_Manager->Find_Effect_Layer(selectedLayerWString);

        if (pLayer)
        {
            static float layerDuration = pLayer->m_fDuration;

            ImGui::Text("Layer Duration:"); // 레이블 텍스트 표시
            ImGui::SameLine();               // 같은 줄에 배치

            ImGui::InputFloat("##LayerDuration", &layerDuration, 1.0f, 1.0f, "%.1f");

            // layerDuration 값을 pLayer에 반영
            pLayer->m_fDuration = max(0.0f, layerDuration);
        }
       

        if (!effectNames.empty())
        {
            const int frameCount = 200; // 총 프레임 수
            const int buttonSize = 20;  // 버튼 크기
            const float effectNameWidth = 150.0f; // 이펙트 이름 표시 너비

            ImGui::BeginChild("TimelineRegion", ImVec2(0, 300), false, ImGuiWindowFlags_HorizontalScrollbar);

            // 상단에 프레임 번호 표시
            ImGui::Text(" "); // 이름과의 간격을 위한 빈 텍스트
            ImGui::SameLine(effectNameWidth); // 이펙트 이름 너비에 맞춤
            for (int frame = 0; frame < frameCount; frame++)
            {
                ImGui::SetCursorPosX(effectNameWidth + frame * (buttonSize + 5.0f)); // 버튼 크기와 간격 조정
                ImGui::Text("%d", frame);
                ImGui::SameLine();
            }
            ImGui::NewLine();

            // 각 이펙트의 이름 및 프레임 버튼 표시
            for (int item = 0; item < effectNames.size(); item++)
            {
                // UTF-8 변환 후 이펙트 이름 표시
                string effectNameUTF8 = WStringToUTF8(effectNames[item]);
                ImGui::TextWrapped("%s", effectNameUTF8.c_str());

                ImGui::SameLine(effectNameWidth); // 이름 뒤에 충분한 간격을 줌

                // 각 프레임의 버튼 배치
                for (int frame = 0; frame < frameCount; frame++)
                {
                    ImGui::SetCursorPosX(effectNameWidth + frame * (buttonSize + 5.0f)); // 버튼 위치 조정
                    ImGui::PushID(frame + item * frameCount);

                    // 키프레임을 나타내는 버튼
                    if (ImGui::Button("##", ImVec2(buttonSize, buttonSize)))
                    {
                        // 버튼 클릭 시 선택된 효과 이름과 프레임 번호를 저장
                        selectedEffectName = effectNameUTF8;
                        selectedFrame = frame;
                        openKeyFrameWindow = true;
                    }

                    ImGui::PopID();
                    ImGui::SameLine();
                }
                ImGui::NewLine(); // 다음 이펙트는 새 줄로 이동
            }

            ImGui::EndChild();
        }
    }
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
