#include "stdafx.h"

#include "UI_BaseAttBuf.h"
#include "RenderInstance.h"

CUI_BaseAttBuf::CUI_BaseAttBuf(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice , pContext }
{
	
}

CUI_BaseAttBuf::CUI_BaseAttBuf(const CUI_BaseAttBuf& Prototype)
	:CUIObject{ Prototype }
{

}

HRESULT CUI_BaseAttBuf::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_BaseAttBuf::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUI_BaseAttBuf::Priority_Update(_float fTimeDelta)
{

}

void CUI_BaseAttBuf::Update(_float fTimeDelta)
{
	if (UsingAttckBuff() == TRUE)
	{
		m_fAttBufTimer += fTimeDelta;
	}

	if (m_fAttBufTimer >= 5.f)
	{
		m_pUI_Manager->UsingAttckBuff(FALSE);
		m_fAttBufTimer = 0.f;
		
	}
}

void CUI_BaseAttBuf::Late_Update(_float fTimeDelta)
{
	
}

HRESULT CUI_BaseAttBuf::Render(_float fTimeDelta)
{
	return S_OK;
}

_bool CUI_BaseAttBuf::UsingAttckBuff()
{
	return m_pUI_Manager->m_bUsingAttckBuff;
}

HRESULT CUI_BaseAttBuf::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_BaseAttBuf::Ready_Components()
{
	if(FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_BaseAttBuf::Free()
{

	__super::Free();
}
