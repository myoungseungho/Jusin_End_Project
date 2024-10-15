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

void CIMGUI_Effect_Tab::Render_For_Layer_KeyFrame(_float fTimeDelta)
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
                    selectedLayerName = LayerList[i]; // 선택된 레이어 이름을 저장
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        ImGui::SameLine();
        ImGui::Dummy(ImVec2(70.0f, 0.0f)); // 가로 70px 간격 추가
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

        // 애니메이션이 재생 중일 때 m_fCurrentAnimPosition 업데이트
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

            // 진행 바 (짧은 세로선)
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            ImVec2 start = ImGui::GetCursorScreenPos();
            ImVec2 lineStart(start.x + effectNameWidth + currentXPosition, start.y);
            ImVec2 lineEnd(lineStart.x, lineStart.y + 300); // 세로선 길이 50으로 설정

            drawList->AddLine(lineStart, lineEnd, IM_COL32(255, 0, 0, 255), 2.0f); // 빨간색 세로선

            // 드래그 가능한 영역
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

                if(!isPlaying)
                    m_pEffect_Manager->Set_Layer_Animation_Position(selectedLayerName, pLayer->m_fCurrentAnimPosition);
            }

            std::vector<bool> effectChecks(effectNames.size(), false);

            for (int item = 0; item < effectNames.size(); item++)
            {
                CEffect* pEffect = m_pEffect_Manager->Find_Layer_Effect(selectedLayerName, effectNames[item]);
                if (pEffect)
                {
                    effectChecks[item] = pEffect->m_bIsLoop;  // 초기 상태 동기화
                }
            }

            for (int item = 0; item < effectNames.size(); item++)
            {
                string effectNameUTF8 = WStringToUTF8(effectNames[item]);
                ImGui::TextWrapped("%s", effectNameUTF8.c_str());

                ImGui::SameLine(effectNameWidth);

                ImGui::PushID(item);

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));  // 패딩 조정으로 크기 조절
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));   // 아이템 간 간격 줄임
                ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(180, 197, 230, 230));       // 체크박스 배경 색상 변경
                ImGui::PushStyleColor(ImGuiCol_CheckMark, IM_COL32(0, 0, 0, 230));         // 체크마크 색상 변경

                bool isChecked = effectChecks[item];
                if (ImGui::Checkbox("##EffectCheck", &isChecked))
                {
                    // 체크박스를 클릭할 때마다 해당 이펙트를 찾아 m_bIsLoop 값 업데이트
                    CEffect* pEffect = m_pEffect_Manager->Find_Layer_Effect(selectedLayerName, effectNames[item]);
                    if (pEffect)
                    {
                        pEffect->m_bIsLoop = isChecked; // 체크 상태에 따라 m_bIsLoop 변경
                    }

                    // 체크 상태를 vector에 업데이트
                    effectChecks[item] = isChecked;
                }

                // 스타일 원래대로 복구
                ImGui::PopStyleColor(2);
                ImGui::PopStyleVar(2);

                ImGui::PopID();

                // 간격 추가
                ImGui::SameLine();
                ImGui::Dummy(ImVec2(30.0f, 0.0f)); // 여유 공간을 추가하여 체크박스와 첫 번째 버튼 간격 조정
                

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

    EFFECT_KEYFRAME newKeyFrame;
    _float3 CurPosition = { 0.f, 0.f, 0.f };
    _float3 CurScale = { 0.f, 0.f, 0.f };
    _float3 CurRotation = { 0.f, 0.f, 0.f };
    _bool IsNotPlaying = { false };

    // 처음 창을 열 때 저장된 키프레임 불러오기
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

    // CurTime을 선택된 프레임 시간에 따라 자동 설정
    const float frameInterval = 1.0f / 60.0f;
    newKeyFrame.fCurTime = selectedFrame * frameInterval;

    ImGui::Text("Effect Keyframe Settings");

    // Is Not Playing 체크박스
    ImGui::Checkbox("Is Not Playing", &IsNotPlaying);
    {
        m_pEffect_Manager->Set_Layer_Effect_IsNotPlaying(selectedLayerName, UTF8ToWString(selectedEffectName), IsNotPlaying);
    }

    ImGui::Separator();

    m_pEffect_Manager->Set_Layer_Effect_Position(selectedLayerName, UTF8ToWString(selectedEffectName), CurPosition);
    m_pEffect_Manager->Set_Layer_Effect_Scaled(selectedLayerName, UTF8ToWString(selectedEffectName), CurScale);
    m_pEffect_Manager->Set_Layer_Effect_Rotation(selectedLayerName, UTF8ToWString(selectedEffectName), CurRotation);

    // Position 섹션
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

    // Scale 섹션
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

    // Rotation 섹션
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

    // Keyframe 저장 버튼
    if (ImGui::Button("Save Keyframe"))
    {
        newKeyFrame.vPosition = CurPosition;
        newKeyFrame.vScale = CurScale;
        newKeyFrame.vRotation = CurRotation;
        newKeyFrame.bIsNotPlaying = IsNotPlaying;
        newKeyFrame.fCurTime = selectedFrame * frameInterval;

        // 업데이트된 newKeyFrame을 저장
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
}
