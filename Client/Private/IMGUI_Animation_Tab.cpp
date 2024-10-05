#include "stdafx.h"
#include "IMGUI_Animation_Tab.h"


CIMGUI_Animation_Tab::CIMGUI_Animation_Tab()
{
}

HRESULT CIMGUI_Animation_Tab::Initialize()
{
	return S_OK;
}

void CIMGUI_Animation_Tab::Render(_float fTimeDelta)
{
}

CIMGUI_Animation_Tab* CIMGUI_Animation_Tab::Create()
{
	CIMGUI_Animation_Tab* pInstance = new CIMGUI_Animation_Tab();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CIMGUI_Animation_Tab"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIMGUI_Animation_Tab::Free()
{
}
