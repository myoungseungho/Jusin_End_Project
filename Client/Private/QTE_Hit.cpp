#include "stdafx.h"
#include "..\Public\QTE_Hit.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "QTE_Hit_Situation.h"


CQTE_Hit::CQTE_Hit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CQTE_Hit::CQTE_Hit(const CQTE_Hit& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CQTE_Hit::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CQTE_Hit::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_vecHit_Situation.resize(Hit_Situation_ID_END);

	CQTE_Hit_Situation::QTE_HIT_SITUATION_DESC Desc{};
	Desc.lifeTime = 15.f;
	Desc.create_Num = 5;
	Desc.ID = 

	CQTE_Hit_Situation* situation = static_cast<CQTE_Hit_Situation*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_QTE_Hit_Situation"), &Desc));
	//A 상황 셋팅 넣어두기
	m_vecHit_Situation[Hit_Situation_ID_A] = situation;

	return S_OK;
}

void CQTE_Hit::Camera_Update(_float fTimeDelta)
{

}

void CQTE_Hit::Update(_float fTimeDelta)
{
	if (m_current_Situation_ID == Hit_Situation_ID_END)
		return;

	//선택된 시뮬레이션만 Update
	m_vecHit_Situation[m_current_Situation_ID]->Update(fTimeDelta);
}

void CQTE_Hit::Late_Update(_float fTimeDelta)
{
	if (m_current_Situation_ID == Hit_Situation_ID_END)
		return;

	//선택된 시뮬레이션만 Late_Update
	m_vecHit_Situation[m_current_Situation_ID]->Late_Update(fTimeDelta);
}

HRESULT CQTE_Hit::Render(_float fTimeDelta)
{
	return S_OK;
}

void CQTE_Hit::Start_Hit(Hit_Situation_ID _ID, CGameObject* pCall_Object)
{
	m_current_Situation_ID = _ID;
	m_vecHit_Situation[m_current_Situation_ID]->Start(pCall_Object);
}


CQTE_Hit* CQTE_Hit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CQTE_Hit* pInstance = new CQTE_Hit(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CQTE_Hit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CQTE_Hit::Clone(void* pArg)
{
	CQTE_Hit* pInstance = new CQTE_Hit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CQTE_Hit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQTE_Hit::Free()
{
	for (auto& iter : m_vecHit_Situation)
		Safe_Release(iter);

	__super::Free();
}
