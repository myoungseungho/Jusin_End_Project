#include "stdafx.h"
#include "..\Public\Level_Loading.h"

#include "Loader.h"
#include "Level_GamePlay.h"

#include "GameInstance.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{
}

HRESULT CLevel_Loading::Initialize(LEVELID eNextLevelID)
{
	m_iLevelIndex = LEVEL_LOADING;
	m_eNextLevelID = eNextLevelID;


	m_pLoader = CLoader::Create(m_pDevice, m_pContext, eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;

	return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{
	if (true == m_pLoader->isFinished())
	{
		CLevel* pNextLevel = { nullptr };

		switch (m_eNextLevelID)
		{
		case LEVEL_GAMEPLAY:
			pNextLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
			break;
		}

		if (FAILED(m_pGameInstance->Change_Level(pNextLevel)))
			return;
	}
}

HRESULT CLevel_Loading::Render(_float fTimeDelta)
{
	m_pLoader->Draw_Text();

	return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Loading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
