#include "stdafx.h"

#include "UI_StartEmblem.h"
#include "RenderInstance.h"

CUI_StartEmblem::CUI_StartEmblem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_Start{ pDevice ,pContext }
{
}

CUI_StartEmblem::CUI_StartEmblem(const CUI_StartEmblem& Prototype)
	:CUI_Start{ Prototype }
{
}

HRESULT CUI_StartEmblem::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_StartEmblem::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	UI_DESC* pUI_Desc = static_cast<UI_DESC*>(pArg);

	m_iTextureIndex = pUI_Desc->iNumUI;

	m_fSizeX = 800.f;
	m_fSizeY = 800.f;

	Set_AnimPosition(500, 10.f);
	Set_AnimPosition(400, 50.f, TRUE, 5.f);
	Set_AnimPosition(500, 80.f);
	Set_AnimPosition(600, 80.f);
	Set_AnimPosition(1000, 150.f, TRUE, 3.f);

	__super::Set_UI_Setting(m_fSizeX, m_fSizeY, g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f, 0.f);

	return S_OK;
}

void CUI_StartEmblem::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUI_StartEmblem::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	Action_Rotaion(fTimeDelta);
	Action_ScaleAnim(fTimeDelta);

}

void CUI_StartEmblem::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_StartEmblem::Render(_float fTimeDelta)
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureIndex)))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Begin(6)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_StartEmblem::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_GameStartCircle"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

void CUI_StartEmblem::Action_Rotaion(_float fTimeDelta)
{
	if (m_iTextureIndex != 0)
	{
		_float iOffsetRotation = 0;
		m_iTextureIndex % 2 == 0 ? iOffsetRotation = 1 : iOffsetRotation = -1;
		m_pTransformCom->Turn({ 0,0,iOffsetRotation }, fTimeDelta);
	}
}

CUI_StartEmblem* CUI_StartEmblem::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_StartEmblem* pInstatnce = new CUI_StartEmblem(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_StartEmblem"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_StartEmblem::Clone(void* pArg)
{
	CUI_StartEmblem* pInstatnce = new CUI_StartEmblem(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_StartEmblem"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_StartEmblem::Free()
{
	__super::Free();
}