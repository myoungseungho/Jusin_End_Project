#include "stdafx.h"

#include "UI_KOPanel.h"
#include "RenderInstance.h"

CUI_KOPanel::CUI_KOPanel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_GameState{ pDevice ,pContext }
{
}

CUI_KOPanel::CUI_KOPanel(const CUI_KOPanel& Prototype)
	:CUI_GameState{ Prototype }
{
}

HRESULT CUI_KOPanel::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_KOPanel::Initialize(void* pArg)
{
	m_fSizeX = 1000.f;
	m_fSizeY = 250.f;
	m_fPosX = 640;
	m_fPosY = 360;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	UI_DESC* pUI_Desc = static_cast<UI_DESC*>(pArg);

	m_pTransformCom->Rotation({ 0,0,1 }, XMConvertToRadians(345.f));

	__super::Set_UI_Setting(m_fSizeX, m_fSizeY, m_fPosX, m_fPosY , 0.0f);

	return S_OK;
}

void CUI_KOPanel::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	if (m_bDeadCheck)
		m_fLightTimer += fTimeDelta * 4.f;

	m_fDestroyTimer += fTimeDelta;
}

void CUI_KOPanel::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	Action_Anim(0.25f, fTimeDelta);

	if (m_fDestroyTimer >= 1.f)
	{
		m_fDestroyTimer = 0.f;
		m_bDeadCheck = TRUE;
	}

	if (m_fLightTimer >= 0.25f)
	{
		m_fLightTimer = 0.f;
		m_bDead = TRUE;
	}

}

void CUI_KOPanel::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_KOPanel::Render(_float fTimeDelta)
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;;

	if (FAILED(m_pShaderCom->Begin(17)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_KOPanel::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_KOPanel"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;


}

HRESULT CUI_KOPanel::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 6)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_MaskTimer", &m_fDestroyTimer, sizeof(_float))))
		return E_FAIL;

	//if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlphaTimer", &m_fLightTimer, sizeof(_float))))
	//	return E_FAIL;

	return S_OK;
}


CUI_KOPanel* CUI_KOPanel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_KOPanel* pInstatnce = new CUI_KOPanel(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_KOPanel"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_KOPanel::Clone(void* pArg)
{
	CUI_KOPanel* pInstatnce = new CUI_KOPanel(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_KOPanel"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_KOPanel::Free()
{

	__super::Free();
}
