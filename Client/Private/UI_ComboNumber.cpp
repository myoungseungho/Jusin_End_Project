#include "stdafx.h"

#include "UI_ComboNumber.h"
#include "RenderInstance.h"

CUI_ComboNumber::CUI_ComboNumber(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_Combo{ pDevice ,pContext }
{
}

CUI_ComboNumber::CUI_ComboNumber(const CUI_ComboNumber& Prototype)
	:CUI_Combo{ Prototype }
{
}

HRESULT CUI_ComboNumber::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ComboNumber::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	UI_COMBO_DESC* pComboDesc = static_cast<UI_COMBO_DESC*>(pArg);
	m_iNumUI = pComboDesc->iNumUI;
	
	switch (m_iNumUI)
	{
	case FIRST:
		__super::Set_UI_Setting(81.f, 108.f, 96.f, 238.f, 0.5f);
		break;

	case SECOND:
		__super::Set_UI_Setting(81.f, 108.f, 150.f, 238.f,0.6f);
		break;

	case THIRD:
		__super::Set_UI_Setting(81.f, 108.f, 204.f, 238.f, 0.7f);
		break;
	}


	return S_OK;
}

void CUI_ComboNumber::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);


	

	

	switch (m_iNumUI)
	{
	case FIRST:
		m_iTextureIndex = m_iComboCount / 100;
		break;

	case SECOND:
		m_iTextureIndex = (m_iComboCount / 10) % 10;
		break;

	case THIRD:
		m_iTextureIndex = m_iComboCount % 10;
		break;
	}


}

void CUI_ComboNumber::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

}

void CUI_ComboNumber::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	if(m_iNumUI == FIRST && m_iComboCount >= 100)
		m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
	
	if(m_iNumUI == SECOND && m_iComboCount >=10 )
		m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);

	if (m_iNumUI == THIRD && m_iComboCount >= 2)
		m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);

}

HRESULT CUI_ComboNumber::Render(_float fTimeDelta)
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureIndex)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(4)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ComboNumber::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ComboNumber"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_ComboNumber* CUI_ComboNumber::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_ComboNumber* pInstatnce = new CUI_ComboNumber(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_ComboNumber"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_ComboNumber::Clone(void* pArg)
{
	CUI_ComboNumber* pInstatnce = new CUI_ComboNumber(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_ComboNumber"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_ComboNumber::Free()
{
	__super::Free();
}
