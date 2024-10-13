#include "stdafx.h"

#include "UI_StartFont.h"
#include "RenderInstance.h"

CUI_StartFont::CUI_StartFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_GameStart{ pDevice ,pContext }
{
}

CUI_StartFont::CUI_StartFont(const CUI_StartFont& Prototype)
	:CUI_GameStart{ Prototype }
{
}

HRESULT CUI_StartFont::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_StartFont::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	UI_DESC* pUI_Desc = static_cast<UI_DESC*>(pArg);

	m_fSizeX = 600.f;
	m_fSizeY = 300.f;

	//Set_AnimPosition(500, 10.f);
	//Set_AnimPosition(400, 50.f, TRUE, 5.f);
	//Set_AnimPosition(500, 80.f);
	//Set_AnimPosition(600, 80.f);
	//Set_AnimPosition(1000, 150.f, TRUE, 3.f);

	__super::Set_UI_Setting(m_fSizeX, m_fSizeY, g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f, 0.f);

	return S_OK;
}

void CUI_StartFont::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUI_StartFont::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	//Action_ScaleAnim(fTimeDelta);

	/*if (m_pGameInstance->Key_Down(DIK_B))
		m_bState = true;
	if (m_pGameInstance->Key_Down(DIK_N))
		m_bState = false;*/

	if (m_bState == false)
		m_fMaskTimer += 0.01f;
	else
		m_fMaskTimer -= 0.01f;

	if (m_fMaskTimer >= 1.f && m_bState == false)
		m_fMaskTimer = 1.f;
}

void CUI_StartFont::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_StartFont::Render(_float fTimeDelta)
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

HRESULT CUI_StartFont::Ready_Components()
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

HRESULT CUI_StartFont::Bind_ShaderResources()
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

	if (FAILED(m_pShaderCom->Bind_RawValue("g_DestroyTimer", &m_fMaskTimer, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_bState", &m_bState, sizeof(_bool))))
		return E_FAIL;

	return S_OK;
}


CUI_StartFont* CUI_StartFont::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_StartFont* pInstatnce = new CUI_StartFont(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_StartFont"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_StartFont::Clone(void* pArg)
{
	CUI_StartFont* pInstatnce = new CUI_StartFont(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_StartFont"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_StartFont::Free()
{
	Safe_Release(m_pMaskTexture);

	__super::Free();
}
