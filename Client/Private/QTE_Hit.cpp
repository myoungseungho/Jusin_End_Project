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

	CQTE_Hit_Situation::QTE_HIT_SITUATION_DESC Desc{};
	Desc.lifeTime = 5.f;
	Desc.create_Num = 3;

	CQTE_Hit_Situation* situation = static_cast<CQTE_Hit_Situation*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_QTE_Hit_Situation"), &Desc));
	//A 상황 셋팅 넣어두기
	m_vecHit_Situation[Hit_Situation_ID_A] = situation;

#pragma endregion


	return S_OK;
}

void CQTE_Hit::Camera_Update(_float fTimeDelta)
{

}

void CQTE_Hit::Update(_float fTimeDelta)
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
		for (auto& iter : m_vecHit_Situation)
			iter->Update(fTimeDelta);

		m_fTimer -= fTimeDelta;

	}

#pragma endregion


}

void CQTE_Hit::Late_Update(_float fTimeDelta)
{
	for (auto& iter : m_vecHit_Situation)
		iter->Late_Update(fTimeDelta);
}

HRESULT CQTE_Hit::Render(_float fTimeDelta)
{
	return S_OK;
}

void CQTE_Hit::Start_QTE()
{
	if (m_bIsQTEActive)
		return; // 이미 QTE가 활성화되어 있으면 무시

	m_bIsQTEActive = true;

}

void CQTE_Hit::End_QTE()
{
	m_bIsQTEActive = false;
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
