
#include "stdafx.h"

#include "UI_HpGauge.h"
#include "RenderInstance.h"
#include "RenderInstance.h"

CUI_HpGauge::CUI_HpGauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_HpGauge::CUI_HpGauge(const CUI_HpGauge& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_HpGauge::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_HpGauge::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//512.f, 128.f
	m_fTemp = 512.f;
	__super::Set_UI_Gauge_Setting(m_fTemp, 128.f, 500.f, 512.f, -0.f);
	//356.f
	m_iCharaCurrHp = 100;

	return S_OK;
}

void CUI_HpGauge::Priority_Update(_float fTimeDelta)
{
	//ÀÓÀÇ°ª 100
	m_fHpRadio = m_iCharaCurrHp / 100.f;
}

void CUI_HpGauge::Update(_float fTimeDelta)
{

}

void CUI_HpGauge::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_HpGauge::Render(_float fTimeDelta)
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	//if (FAILED(m_pShaderCom->Bind_RawValue("g_HpRadio", &m_fHpRadio, sizeof(_float))))
	//	return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_HpGauge::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HpGauge"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

}

CUI_HpGauge* CUI_HpGauge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_HpGauge* pInstatnce = new CUI_HpGauge(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_HpGauge"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_HpGauge::Clone(void* pArg)
{
	CUI_HpGauge* pInstatnce = new CUI_HpGauge(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_HpGauge"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_HpGauge::Free()
{
	__super::Free();
}
