#pragma once
#include "stdafx.h"
#include "IMGUI_Effect_Tab.h"
#include "imgui.h"
#include "Effect_Manager.h"


CIMGUI_Effect_Tab::CIMGUI_Effect_Tab()
	: m_pEffect_Manager {CEffect_Manager::Get_Instance()}
{
}

HRESULT CIMGUI_Effect_Tab::Initialize()
{
	return S_OK;
}

void CIMGUI_Effect_Tab::Render(_float fTimeDelta)
{
	bool bShowLevelWindow = true;
	if (ImGui::Button("Add Cell Data")) {
	}
}

CIMGUI_Effect_Tab* CIMGUI_Effect_Tab::Create()
{
	CIMGUI_Effect_Tab* pInstance = new CIMGUI_Effect_Tab();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CIMGUI_Effect_Tab"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIMGUI_Effect_Tab::Free()
{
	Safe_Release(m_pEffect_Manager);
}
