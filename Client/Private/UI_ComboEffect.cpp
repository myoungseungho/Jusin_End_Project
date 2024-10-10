#include "stdafx.h"

#include "UI_ComboEffect.h"
#include "RenderInstance.h"

CUI_ComboEffect::CUI_ComboEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_Combo{ pDevice ,pContext }
{
}

CUI_ComboEffect::CUI_ComboEffect(const CUI_ComboEffect& Prototype)
	:CUI_Combo{ Prototype }
{
}

HRESULT CUI_ComboEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ComboEffect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	__super::Set_UI_Setting(384.f, 96.f, 181.f, 286.f);

	return S_OK;
}

void CUI_ComboEffect::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);


}

void CUI_ComboEffect::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

}

void CUI_ComboEffect::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	if(m_bCharaStun == FALSE && m_iComboCount >= 2)
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_ComboEffect::Render(_float fTimeDelta)
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

HRESULT CUI_ComboEffect::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ComboEffect"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_ComboEffect* CUI_ComboEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_ComboEffect* pInstatnce = new CUI_ComboEffect(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_ComboEffect"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_ComboEffect::Clone(void* pArg)
{
	CUI_ComboEffect* pInstatnce = new CUI_ComboEffect(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_ComboEffect"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_ComboEffect::Free()
{
	__super::Free();
}
