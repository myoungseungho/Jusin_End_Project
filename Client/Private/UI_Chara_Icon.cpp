#include "stdafx.h"

#include "UI_Chara_Icon.h"
#include "RenderInstance.h"

CUI_Chara_Icon::CUI_Chara_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Chara_Icon::CUI_Chara_Icon(const CUI_Chara_Icon& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Chara_Icon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Chara_Icon::Initialize(void* pArg)
{
	m_fPosX = 50.f;
	m_fSizeX = 150.f;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	__super::Set_UI_Setting(m_fSizeX, 150.f, m_fPosX, 40.f, 0.85f);

	return S_OK;
}

void CUI_Chara_Icon::Priority_Update(_float fTimeDelta)
{
	_uint iTexIndex = 0;

	(m_eLRPos == LEFT) ? iTexIndex  = 0: iTexIndex = 2;

	m_iCharaID = m_pUI_Manager->m_pPawnArray[iTexIndex]->Get_PawnDesc().ePlayerID;
}

void CUI_Chara_Icon::Update(_float fTimeDelta)
{

}

void CUI_Chara_Icon::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Chara_Icon::Render(_float fTimeDelta)
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;;

	//m_iCharaID �� ĳ���� ID�� ������ָ� 0��ſ� �־��ִ� �� (ĳ���Ϳ� ���� �̹����� �ٲ��)
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iCharaID)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Chara_Icon::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_CharaIcon"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Chara_Icon* CUI_Chara_Icon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Chara_Icon* pInstatnce = new CUI_Chara_Icon(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Chara_Icon"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Chara_Icon::Clone(void* pArg)
{
	CUI_Chara_Icon* pInstatnce = new CUI_Chara_Icon(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Chara_Icon"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Chara_Icon::Free()
{
	__super::Free();
}
