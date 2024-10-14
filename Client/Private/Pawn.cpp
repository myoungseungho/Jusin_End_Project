#include "stdafx.h"
#include "..\Public\Pawn.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CPawn::CPawn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
	, m_pUI_Manager{ CUI_Manager::Get_Instance() }
{
	Safe_AddRef(m_pUI_Manager);
}

CPawn::CPawn(const CPawn& Prototype)
	: CGameObject{ Prototype }
	, m_pUI_Manager{ CUI_Manager::Get_Instance() }
{
	Safe_AddRef(m_pUI_Manager);
}

HRESULT CPawn::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPawn::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	SLOT_DESC* pSlotDesc = static_cast<SLOT_DESC*>(pArg);
	m_ePlayerSlot = pSlotDesc->ePlayerSlot;



	return S_OK;
}

void CPawn::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	m_tPawnDesc.bStun = m_bStun;
	m_tPawnDesc.bHit = m_bHit;
	m_tPawnDesc.bAttBuf = m_bAttBuf;

	m_tPawnDesc.iHp = m_iHp;
	m_tPawnDesc.iComboCount = m_iComboCount;

	m_tPawnDesc.iSKillCount = m_iSKillCount;
	m_tPawnDesc.iSKillPoint = m_iSKillPoint;

	m_tPawnDesc.ePlayer_Slot = m_ePlayerSlot;
	m_tPawnDesc.ePlayerID = m_ePawnID;
}


void CPawn::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CPawn::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CPawn::Render(_float fTimeDelta)
{
	return S_OK;
}

void CPawn::Action_AttBuf(_ubyte byKeyID,  PLAYER_SLOT eSlot,  _float fTimeDelta )
{
	if (m_iNumAttBuf <= 1 && m_pGameInstance->Get_DIKeyState(byKeyID))
	{
		m_bAttBuf = TRUE;
		m_pUI_Manager->UsingAttckBuff(5.f, eSlot);
		m_iNumAttBuf--;
	}
	if (m_bAttBuf == TRUE)
	{
		m_fAttBufTimer += fTimeDelta;

		if (m_fAttBufTimer >= 5.f)
		{
			m_bAttBuf = FALSE;
			m_fAttBufTimer = 0.f;
		}
	}
}

void CPawn::Action_Hit(_ubyte byKeyID,_float fStunDuration, _float fTimeDelta)
{
	if (m_pGameInstance->Get_DIKeyState(byKeyID))
	{
		m_iComboCount++;
		m_iHp--;

		m_fStunTImer = fStunDuration;
		m_bStun = TRUE;
		m_bHit = TRUE;

		m_iSKillPoint += 3;
	}
	else
		m_bHit = FALSE;

	SkillGaugeLimit();
	StunRecover(fTimeDelta);
}

void CPawn::SkillGaugeLimit()
{
	if (m_iSKillPoint > 100)
	{
		m_iSKillPoint -= 100;
		m_iSKillCount++;
	}
	else if (m_iSKillPoint < 0)
	{
		m_iSKillPoint += 100;
		m_iSKillCount--;
	}
}

void CPawn::StunRecover(_float fTimeDelta)
{
	if (m_bStun == TRUE)
	{
		m_fStunTImer -= fTimeDelta;

		if (m_fStunTImer <= 0.f)
		{
			m_fStunTImer = 0.f;
			m_bStun = FALSE;
			m_iComboCount = 0;
		}
	}
}

void CPawn::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pUI_Manager);
}
