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
	__super::Priority_Update(fTimeDelta);
}

void CUI_BaseAttBuf::Update(_float fTimeDelta)
{
	_bool bAttBuf = { FALSE };

	if(m_pMainPawn != nullptr)
		bAttBuf = m_pMainPawn->Get_PawnDesc().bAttBuf;

	if (bAttBuf == TRUE || m_fAttBufTimer > 0.f)
	{

		m_fAttBufTimer += fTimeDelta;

		if (m_fAttBufTimer >= m_pUI_Manager->m_fDuration)
		{
			Destory();
		}
	}

}

void CUI_BaseAttBuf::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CUI_BaseAttBuf::Render(_float fTimeDelta)
{
	return S_OK;
}

_bool CUI_BaseAttBuf::UsingAttckBuff()
{
	if (m_pUI_Manager->m_fDuration > 0.f)
	{
		m_fAttBufDuration = m_pUI_Manager->m_fDuration;
		return TRUE;
	}
	m_fAttBufDuration = 0.f;

	return FALSE;
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
