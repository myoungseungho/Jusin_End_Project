
#include "stdafx.h"

#include "UI_HpGauge.h"
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
	m_fPosX = 330.f;
	m_fSizeX = 464.f;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	__super::Set_UI_Setting(m_fSizeX, 116.f, m_fPosX, 87.f, 0.75f);

	return S_OK;
}

void CUI_HpGauge::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	if(m_pMainPawn != nullptr)
		m_fHpRadio = m_pMainPawn->Get_PawnDesc().iHp / 100.f;
	


	m_fMaskUVTimer += fTimeDelta * 0.25f;

	if (m_bRedAlpha == FALSE )
		m_fRedHpRadio = m_fHpRadio;

	if (m_bCharaStun == TRUE)
	{
		if (m_bHit == FALSE)
		{
			m_bHit = TRUE;
			m_fRedHpRadio = m_fHpRadio;
		}
	}
	else
		m_bHit = FALSE;


	//캐릭터가 스턴이면 알파값 true 레드게이지 알파값은 0으로 초기화 
	m_bCharaStun ? m_bRedAlpha = TRUE , m_fRedGaugeTimer = 0.f : m_fRedGaugeTimer += fTimeDelta * 2.f;


	if (m_bRedAlpha == TRUE && m_fRedGaugeTimer >= 1.f)
	{
		m_bRedAlpha = FALSE;
	
	}
}

void CUI_HpGauge::Update(_float fTimeDelta)
{
	(m_fHpRadio >= 1.f) ? m_iShaderID = 2 : m_iShaderID = 1;
}

void CUI_HpGauge::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_HpGauge::Render(_float fTimeDelta)
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;;

	if (FAILED(m_pShaderCom->Begin(m_iShaderID)))
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

	/* For.Com_MaskTexture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HpGaugeAnimMask"),
		TEXT("Com_MaskTexture"), reinterpret_cast<CComponent**>(&m_pMaskTexture))))
		return E_FAIL;

}

HRESULT CUI_HpGauge::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pMaskTexture->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Radio", &m_fHpRadio, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRedRadio", &m_fRedHpRadio, sizeof(_float))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_RawValue("g_MaskTimer", &m_fMaskUVTimer, sizeof(_float))))
		return E_FAIL;

 	if (FAILED(m_pShaderCom->Bind_RawValue("g_DestroyTimer", &m_fRedGaugeTimer, sizeof(_float))))
		return E_FAIL;

	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_bState", &(m_bRedAlpha), sizeof(_bool))))
		return E_FAIL;

	_vector vColor = { 0.043f , 0.952f , 0.945 , 1.f };

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &vColor, sizeof(_vector))))
		return E_FAIL;

	return S_OK;
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
	Safe_Release(m_pMaskTexture);

	__super::Free();
}
