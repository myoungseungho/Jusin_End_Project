#include "stdafx.h"

#include "UI_Loading_DragonBall.h"
#include "RenderInstance.h"

CUI_Loading_DragonBall::CUI_Loading_DragonBall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Loading_DragonBall::CUI_Loading_DragonBall(const CUI_Loading_DragonBall& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Loading_DragonBall::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Loading_DragonBall::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	UI_DESC* pDesc = static_cast<UI_DESC*>(pArg);
	m_iTextureIndex = pDesc->iNumUI;

	m_fSizeX = 70.f, m_fSizeY = 70.f;
	m_fPosX = 300.f , m_fPosY = 600.f;

	InitPosition();

	__super::Set_UI_Setting(m_fSizeX, m_fSizeY, m_fPosX , m_fPosY, 0.f);

	return S_OK;
}

void CUI_Loading_DragonBall::Camera_Update(_float fTimeDelta)
{
	__super::Camera_Update(fTimeDelta);
}

void CUI_Loading_DragonBall::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CUI_Loading_DragonBall::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Loading_DragonBall::Render(_float fTimeDelta)
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureIndex)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Loading_DragonBall::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LoadingDragonBall"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;


	return S_OK;
}

void CUI_Loading_DragonBall::InitPosition()
{
	if (m_iTextureIndex % 2 == 0)
		m_fPosY += 40;

	m_fPosX += m_iTextureIndex * 100.f;
}

CUI_Loading_DragonBall* CUI_Loading_DragonBall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Loading_DragonBall* pInstatnce = new CUI_Loading_DragonBall(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Loading_DragonBall"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Loading_DragonBall::Clone(void* pArg)
{
	CUI_Loading_DragonBall* pInstatnce = new CUI_Loading_DragonBall(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Loading_DragonBall"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Loading_DragonBall::Free()
{
	__super::Free();
}
