#include "stdafx.h"
#include "..\Public\QTE_Hit_Situation.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "QTE_Hit_UI_Icon.h"
CQTE_Hit_Situation::CQTE_Hit_Situation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CQTE_Hit_Situation::CQTE_Hit_Situation(const CQTE_Hit_Situation& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CQTE_Hit_Situation::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CQTE_Hit_Situation::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	QTE_HIT_SITUATION_DESC* Desc = static_cast<QTE_HIT_SITUATION_DESC*>(pArg);
	m_fLifeTime = Desc->lifeTime;
	m_iCreate_Num = Desc->create_Num;

	for (size_t i = 0; i < m_iCreate_Num; i++)
	{
		CQTE_Hit_UI_Icon::QTE_Hit_UI_ICON_DESC Desc{};
		Desc.fX = {};
		Desc.fY = {};
		Desc.fSizeX = {};
		Desc.fSizeY = {};
		Desc.iTextureNumber = {};

		CQTE_Hit_UI_Icon* ui_Icon = static_cast<CQTE_Hit_UI_Icon*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_QTE_Hit_UI_Icon"), &Desc));
		//일단 만들고 비활성화
		ui_Icon->SetActive(false);

		m_vecHitUIIcon.push_back(ui_Icon);
	}
	return S_OK;
}

void CQTE_Hit_Situation::Camera_Update(_float fTimeDelta)
{

}

void CQTE_Hit_Situation::Update(_float fTimeDelta)
{
	for (auto& iter : m_vecHitUIIcon)
		iter->Update(fTimeDelta);
}

void CQTE_Hit_Situation::Late_Update(_float fTimeDelta)
{
	for (auto& iter : m_vecHitUIIcon)
		iter->Late_Update(fTimeDelta);
}

HRESULT CQTE_Hit_Situation::Render(_float fTimeDelta)
{
	return S_OK;
}

CQTE_Hit_Situation* CQTE_Hit_Situation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CQTE_Hit_Situation* pInstance = new CQTE_Hit_Situation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CQTE_Hit_Situation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CQTE_Hit_Situation::Clone(void* pArg)
{
	CQTE_Hit_Situation* pInstance = new CQTE_Hit_Situation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CQTE_Hit_Situation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQTE_Hit_Situation::Free()
{
	for (auto& iter : m_vecHitUIIcon)
		Safe_Release(iter);

	__super::Free();
}
