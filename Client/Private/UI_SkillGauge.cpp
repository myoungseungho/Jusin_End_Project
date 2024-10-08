#include "stdafx.h"

#include "UI_SkillGauge.h"
#include "RenderInstance.h"

CUI_SkillGauge::CUI_SkillGauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_Skill{ pDevice ,pContext }
{
}

CUI_SkillGauge::CUI_SkillGauge(const CUI_SkillGauge& Prototype)
	:CUI_Skill{ Prototype }
{
}

HRESULT CUI_SkillGauge::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SkillGauge::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	__super::Set_UI_Setting(416.f, 52.f, 332.f, 668.f);

	return S_OK;
}

void CUI_SkillGauge::Priority_Update(_float fTimeDelta)
{

}

void CUI_SkillGauge::Update(_float fTimeDelta)
{

}

void CUI_SkillGauge::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);

	
}

HRESULT CUI_SkillGauge::Render(_float fTimeDelta)
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SkillGauge::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SKillGauge"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_SkillGauge* CUI_SkillGauge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_SkillGauge* pInstatnce = new CUI_SkillGauge(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_SkillGauge"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_SkillGauge::Clone(void* pArg)
{
	CUI_SkillGauge* pInstatnce = new CUI_SkillGauge(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_SkillGauge"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_SkillGauge::Free()
{
	__super::Free();
}
