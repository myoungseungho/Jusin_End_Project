#include "stdafx.h"
#include "QTE_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CQTE_Manager)

CQTE_Manager::CQTE_Manager()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CQTE_Manager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	//Same_Grab
	CGameObject* SameGrab = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_QTE_Same_Grab"));
	m_vecQTE.push_back(SameGrab);

	//Hit
	CGameObject* Hit = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_QTE_Hit"));
	m_vecQTE.push_back(Hit);

	//Continuous_Attack
	CGameObject* ConAttack = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_QTE_Continuous_Attack"));
	m_vecQTE.push_back(ConAttack);

	return S_OK;
}

void CQTE_Manager::Camera_Update(_float fTimeDelta)
{
}


void CQTE_Manager::Update(_float fTimeDelta)
{
	for (auto& iter : m_vecQTE)
		iter->Update(fTimeDelta);
}

void CQTE_Manager::Late_Update(_float fTimeDelta)
{
	for (auto& iter : m_vecQTE)
		iter->Late_Update(fTimeDelta);
}

HRESULT CQTE_Manager::Render(_float fTimeDelta)
{
	return S_OK;
}

void CQTE_Manager::Start_Hit(CQTE_Hit::Hit_Situation_ID _ID)
{
	static_cast<CQTE_Hit*>(m_vecQTE[_ID])->Start_Hit(_ID);
}

void CQTE_Manager::Free()
{
	Safe_Release(m_pGameInstance);

	for (auto& iter : m_vecQTE)
		Safe_Release(iter);

	__super::Free();
}
