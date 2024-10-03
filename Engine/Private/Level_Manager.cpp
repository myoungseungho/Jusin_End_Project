#include "..\Public\Level_Manager.h"
#include "Level.h"

CLevel_Manager::CLevel_Manager()	
{
	
}

HRESULT CLevel_Manager::Initialize()
{
	return S_OK;
}

void CLevel_Manager::Update(_float fTimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return;

	m_pCurrentLevel->Update(fTimeDelta);
}

HRESULT CLevel_Manager::Render()
{
	if (nullptr == m_pCurrentLevel)
		return E_FAIL;

	return m_pCurrentLevel->Render();
}

HRESULT CLevel_Manager::Change_Level(CLevel * pNewLevel)
{
	if(nullptr != m_pCurrentLevel)
		m_pCurrentLevel->Clear_Resources();

	Safe_Release(m_pCurrentLevel);


	m_pCurrentLevel = pNewLevel;

	return S_OK;
}

CLevel_Manager * CLevel_Manager::Create()
{
	CLevel_Manager*		pInstance = new CLevel_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Manager::Free()
{
	__super::Free();
	
	Safe_Release(m_pCurrentLevel);
}

