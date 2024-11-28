#include "stdafx.h"
#include "SubTitle_Manager.h"
#include "GameInstance.h"
#include "SubTitle.h"
IMPLEMENT_SINGLETON(CSubTitle_Manager)

CSubTitle_Manager::CSubTitle_Manager()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CSubTitle_Manager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	// 자막 텍스트 초기화
	m_mapSubTitleText[GOKU_VS_FRIEZA_SUBTITLE_0] = L"그만둬 프리저---!!!!";
	m_mapSubTitleText[GOKU_VS_FRIEZA_SUBTITLE_1] = L"오공----!!!!";


	m_pSubTitle = static_cast<CSubTitle*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_SubTitle")));

	return S_OK;
}

void CSubTitle_Manager::Camera_Update(_float fTimeDelta)
{
}


void CSubTitle_Manager::Update(_float fTimeDelta)
{
	m_pSubTitle->Update(fTimeDelta);
}

void CSubTitle_Manager::Late_Update(_float fTimeDelta)
{
	m_pSubTitle->Late_Update(fTimeDelta);
}

HRESULT CSubTitle_Manager::Render(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CSubTitle_Manager::Play(SUBTITLE_ID eID, _float duration)
{
	// 자막 ID가 유효한지 확인
	auto iter = m_mapSubTitleText.find(eID);
	if (iter == m_mapSubTitleText.end())
		return E_FAIL;

	m_pSubTitle->Initialize_SubTitle(iter->second, duration);
	m_pSubTitle->SetActive(true);

	return S_OK;
}

void CSubTitle_Manager::Stop(SUBTITLE_ID eID)
{
	m_pSubTitle->SetActive(false);
}



void CSubTitle_Manager::Free()
{
	Safe_Release(m_pSubTitle);
	Safe_Release(m_pGameInstance);

	__super::Free();
}
