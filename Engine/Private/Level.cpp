#include "..\Public\Level.h"

#include "GameInstance.h"
#include "RenderInstance.h"

CLevel::CLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
	, m_pRenderInstance{ CRenderInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pRenderInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CLevel::Initialize()
{
	return S_OK;
}

void CLevel::Update(_float fTimeDelta)
{

}

HRESULT CLevel::Render(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CLevel::Clear_Resources()
{
	return m_pGameInstance->Clear_LevelResources(m_iLevelIndex);
}

void CLevel::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pRenderInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
