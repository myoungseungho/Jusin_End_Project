#include "stdafx.h"

#include "UI_FightFont.h"
#include "RenderInstance.h"

CUI_FightFont::CUI_FightFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_Start{ pDevice ,pContext }
{
}

CUI_FightFont::CUI_FightFont(const CUI_FightFont& Prototype)
	:CUI_Start{ Prototype }
{
}

HRESULT CUI_FightFont::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_FightFont::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	UI_DESC* pUI_Desc = static_cast<UI_DESC*>(pArg);

	m_fSizeX = 400.f;
	m_fSizeY = 200.f;

	//Set_AnimPosition(500, 10.f);
	//Set_AnimPosition(400, 50.f, TRUE, 5.f);
	//Set_AnimPosition(500, 80.f);
	//Set_AnimPosition(600, 80.f);
	//Set_AnimPosition(1000, 150.f, TRUE, 3.f);

	m_fOffSetPosY = 50.f;

	__super::Set_UI_Setting(m_fSizeX, m_fSizeY, g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f  - m_fOffSetPosY, 0.f);

	return S_OK;
}

void CUI_FightFont::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);


	
}

void CUI_FightFont::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	Action_StartAnim(fTimeDelta);
	//Action_ScaleAnim(fTimeDelta);



}

void CUI_FightFont::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_FightFont::Render(_float fTimeDelta)
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;;

	if (FAILED(m_pShaderCom->Begin(7)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_FightFont::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_GameStartFont"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_MaskTexture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_GameStartFont"),
		TEXT("Com_MaskTexture "), reinterpret_cast<CComponent**>(&m_pMaskTexture))))
		return E_FAIL;	

}

HRESULT CUI_FightFont::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;


	//1번이 BG(노란색) 2번이 Main(검정색)
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pMaskTexture->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", 1)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_MaskTimer", &m_fMaskTimer, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_DestroyTimer", &m_fAlphaTimer, sizeof(_float))))
		return E_FAIL;

	//m_fAlphaTimer 

	return S_OK;
}

void CUI_FightFont::Action_StartAnim(_float fTimeDelta)
{
	if (m_bStart == FALSE && m_fOffSetPosY >= 0.f)
	{
		m_fStartAnimTimer += fTimeDelta;

		m_fOffSetPosY -= m_fStartAnimTimer;

		__super::Set_UI_Setting(m_fSizeX, m_fSizeY, g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f - m_fOffSetPosY, 0.f);
	}
	else
	{
		m_bStart = TRUE;
		m_fStartAnimTimer = 0.f;
	}

	if (m_bStart == TRUE && m_bEnd == FALSE)
	{
		m_fMaskTimer += fTimeDelta;

		if (m_fMaskTimer >= 3.f)
		{
			m_bEnd = TRUE;
			m_fMaskTimer = 1.f;

		}
	}
	if (m_bEnd)
	{
		m_fMaskTimer -= fTimeDelta;
		m_fAlphaTimer += fTimeDelta;
	}
}


CUI_FightFont* CUI_FightFont::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_FightFont* pInstatnce = new CUI_FightFont(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_FightFont"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_FightFont::Clone(void* pArg)
{
	CUI_FightFont* pInstatnce = new CUI_FightFont(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_FightFont"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_FightFont::Free()
{
	Safe_Release(m_pMaskTexture);

	__super::Free();
}
