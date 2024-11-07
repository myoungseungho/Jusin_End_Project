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
#pragma region 디버그
	// F5 키 입력 감지
	if (m_pGameInstance->Key_Down(DIK_F5))
	{
		if (m_bIsQTEActive)
		{
			// QTE가 활성화되어 있으면 즉시 종료
			End_QTE();
		}
		else
		{
			// QTE가 비활성화되어 있으면 시작
			Start_QTE();
		}
	}
#pragma endregion

#pragma region 활성화

	if (m_bIsQTEActive)
	{
		for (auto& iter : m_vecHitUIIcon)
			iter->Update(fTimeDelta);

		m_fTimer -= fTimeDelta;
	}
}

void CQTE_Hit_Situation::Late_Update(_float fTimeDelta)
{
	if (m_bIsQTEActive)
	{
		for (auto& iter : m_vecHitUIIcon)
			iter->Late_Update(fTimeDelta);
	}
}

HRESULT CQTE_Hit_Situation::Render(_float fTimeDelta)
{
	return S_OK;
}

void CQTE_Hit_Situation::Start_QTE()
{
	if (m_bIsQTEActive)
		return; // 이미 QTE가 활성화되어 있으면 무시

	m_bIsQTEActive = true;
}

void CQTE_Hit_Situation::End_QTE()
{
	m_bIsQTEActive = false;
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
