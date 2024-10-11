#include "stdafx.h"

#include "UI_Combo.h"
#include "RenderInstance.h"

CUI_Combo::CUI_Combo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice , pContext }
{

}

CUI_Combo::CUI_Combo(const CUI_Combo& Prototype)
	:CUIObject{ Prototype }
{

}

HRESULT CUI_Combo::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Combo::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUI_Combo::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUI_Combo::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (m_bCharaStun == TRUE)
	{
		m_iComboCount = m_pMainPawn->Get_PawnDesc().iComboCount;
		m_bComboEnd = TRUE;
	}

	if (m_bComboEnd == TRUE)
	{
		m_fAlphaTimer += fTimeDelta;
		if (m_fAlphaTimer >= 1.f)
		{
			m_fAlphaTimer = 0.f;
			m_bComboEnd = FALSE;
		}
	}
	else
		m_iComboCount = 0.f;
}

void CUI_Combo::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CUI_Combo::Render(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CUI_Combo::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Combo::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_Combo::Free()
{

	__super::Free();
}
