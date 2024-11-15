#include "stdafx.h"
#include "QTE_Manager.h"
#include "GameInstance.h"
#include "QTE_Same_Grab.h"
#include "QTE_Hit.h"
#include "QTE_Continuous_Attack.h"

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

	if (m_pGameInstance->Key_Down(DIK_F1))
		Start_QTE(QTE_ID::QTE_ID_HIT, Hit_Situation_ID_A);
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

void CQTE_Manager::Start_QTE(QTE_ID ID, Hit_Situation_ID Hit_Situation_ID)
{
	switch (ID)
	{
	case Client::CQTE_Manager::QTE_ID_SAME_GRAB:
		static_cast<CQTE_Same_Grab*>(m_vecQTE[ID])->Start();
		break;
	case Client::CQTE_Manager::QTE_ID_HIT:
		static_cast<CQTE_Hit*>(m_vecQTE[ID])->Start_Hit((CQTE_Hit::Hit_Situation_ID)Hit_Situation_ID);
		break;
	case Client::CQTE_Manager::QTE_ID_CONTINUOUS_ATTACK:
		static_cast<CQTE_Continuous_Attack*>(m_vecQTE[ID])->Start();
		break;
	}
}

void CQTE_Manager::Free()
{
	Safe_Release(m_pGameInstance);

	for (auto& iter : m_vecQTE)
		Safe_Release(iter);

	__super::Free();
}
