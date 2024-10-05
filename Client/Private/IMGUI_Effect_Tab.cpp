#include "stdafx.h"
#include "IMGUI_Effect_Tab.h"


CIMGUI_Effect_Tab::CIMGUI_Effect_Tab()
{
}

HRESULT CIMGUI_Effect_Tab::Initialize()
{
	return S_OK;
}

void CIMGUI_Effect_Tab::Render(_float fTimeDelta)
{
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
}
