#include "stdafx.h"

#include "UI_Input_ActionIcon.h"
#include "RenderInstance.h"

CUI_Input_ActionIcon::CUI_Input_ActionIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_Input{ pDevice ,pContext }
{
}

CUI_Input_ActionIcon::CUI_Input_ActionIcon(const CUI_Input_ActionIcon& Prototype)
	:CUI_Input{ Prototype }
{
}

HRESULT CUI_Input_ActionIcon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Input_ActionIcon::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	UI_INPUT_DESC* pDirDesc = static_cast<UI_INPUT_DESC*>(pArg);
	m_iTextureIndex = pDirDesc->eActionInput;

	m_fPosX = 50.f;

	m_fPosY = 190;

	if (m_pUI_Manager->m_eDirInput == MOVEKEY_NEUTRAL)
	{
		m_pUI_Manager->m_iNumCommandList++;
		m_fPosX = 20;
	}
	m_iNumCommandList = m_pUI_Manager->m_iNumCommandList;

	__super::Set_UI_Setting(35.f, 35.f, m_fPosX, m_fPosY, 1.f);

	return S_OK;
}

void CUI_Input_ActionIcon::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

}

void CUI_Input_ActionIcon::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	_float fOffSetPosY = m_fPosY + (40 * (m_pUI_Manager->m_iNumCommandList - m_iNumCommandList));

	if (fOffSetPosY >= 575)
		m_bDead = TRUE;

	__super::Set_UI_Setting(35.f, 35.f, m_fPosX, fOffSetPosY, 1.f);
}

void CUI_Input_ActionIcon::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	//if(m_iTextureIndex)
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Input_ActionIcon::Render(_float fTimeDelta)
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureIndex - 1)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(12)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Input_ActionIcon::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ActionInput"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Input_ActionIcon* CUI_Input_ActionIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Input_ActionIcon* pInstatnce = new CUI_Input_ActionIcon(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Input_ActionIcon"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Input_ActionIcon::Clone(void* pArg)
{
	CUI_Input_ActionIcon* pInstatnce = new CUI_Input_ActionIcon(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Input_ActionIcon"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Input_ActionIcon::Free()
{
	__super::Free();
}
