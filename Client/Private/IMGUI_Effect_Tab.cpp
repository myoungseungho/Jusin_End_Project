#include "stdafx.h"
#include "IMGUI_Effect_Tab.h"
#include "GameInstance.h"
#include "RenderInstance.h"
#include "Effect_Manager.h"


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

const char* Effect[] = { "Each", "Final" };
static int CurrentEffect = 0;
const char* EffectType[] = { "Single", "Multi", "MoveTex" }; // Each 선택 시 이펙트 타입 선택
static int CurrentEffectType = 0;

void CIMGUI_Effect_Tab::Render(_float fTimeDelta)
{
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
}
