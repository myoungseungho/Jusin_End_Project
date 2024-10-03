#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
//#include "LandObject.h"
//#include "Monster.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext } 
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	m_iLevelIndex = LEVEL_GAMEPLAY;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨"));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring & strLayerTag)
{
	/*if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera"), strLayerTag)))
		return E_FAIL;*/

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
}
