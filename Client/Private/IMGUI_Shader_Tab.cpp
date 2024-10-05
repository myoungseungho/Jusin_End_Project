#include "stdafx.h"
#include "IMGUI_Shader_Tab.h"


CIMGUI_Shader_Tab::CIMGUI_Shader_Tab()
{
}

HRESULT CIMGUI_Shader_Tab::Initialize()
{
	return S_OK;
}

void CIMGUI_Shader_Tab::Render(_float fTimeDelta)
{
}

CIMGUI_Shader_Tab* CIMGUI_Shader_Tab::Create()
{
	CIMGUI_Shader_Tab* pInstance = new CIMGUI_Shader_Tab();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CIMGUI_Shader_Tab"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIMGUI_Shader_Tab::Free()
{
}
