#include "stdafx.h"
#include "IMGUI_UI_Tab.h"


CIMGUI_UI_Tab::CIMGUI_UI_Tab()
{
}

HRESULT CIMGUI_UI_Tab::Initialize()
{
	return S_OK;
}

void CIMGUI_UI_Tab::Render(_float fTimeDelta)
{
}

CIMGUI_UI_Tab* CIMGUI_UI_Tab::Create()
{
	CIMGUI_UI_Tab* pInstance = new CIMGUI_UI_Tab();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CIMGUI_UI_Tab"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIMGUI_UI_Tab::Free()
{
}
