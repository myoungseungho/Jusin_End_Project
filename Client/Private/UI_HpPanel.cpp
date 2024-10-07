#include "stdafx.h"

#include "UI_HpPanel.h"
#include "RenderInstance.h"
#include "RenderInstance.h"

CUI_HpPanel::CUI_HpPanel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_HpPanel::CUI_HpPanel(const CUI_HpPanel& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_HpPanel::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_HpPanel::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	__super::Set_UI_Setting(512.f, 128.f, 356.f, 80.f);

	return S_OK;
}

void CUI_HpPanel::Priority_Update(_float fTimeDelta)
{

}

void CUI_HpPanel::Update(_float fTimeDelta)
{

}

void CUI_HpPanel::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_HpPanel::Render()
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

HRESULT CUI_HpPanel::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HpPanel"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_HpPanel* CUI_HpPanel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_HpPanel* pInstatnce = new CUI_HpPanel(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_HpPanel"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_HpPanel::Clone(void* pArg)
{
	CUI_HpPanel* pInstatnce = new CUI_HpPanel(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_HpPanel"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_HpPanel::Free()
{
	__super::Free();
}
