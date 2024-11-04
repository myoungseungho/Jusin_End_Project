#include "stdafx.h"
#include "..\Public\Level_Lobby.h"

#include "GameInstance.h"
#include "Level_Loading.h"


CLevel_Lobby::CLevel_Lobby(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{
}

HRESULT CLevel_Lobby::Initialize()
{
	m_iLevelIndex = LEVEL_LOBBY;

	//사운드 준비
	if (Ready_Sound())
		return E_FAIL;

	return S_OK;
}

void CLevel_Lobby::Update(_float fTimeDelta)
{
	
}

HRESULT CLevel_Lobby::Render(_float fTimeDelta)
{
	SetWindowText(g_hWnd, TEXT("로비레벨"));

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Sound()
{
	return S_OK;
}

CLevel_Lobby* CLevel_Lobby::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Lobby* pInstance = new CLevel_Lobby(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Lobby"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Lobby::Free()
{
	__super::Free();
}
