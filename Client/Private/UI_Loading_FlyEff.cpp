#include "stdafx.h"

#include "UI_Loading_FlyEff.h"
#include "RenderInstance.h"

CUI_Loading_FlyEff::CUI_Loading_FlyEff(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Loading_FlyEff::CUI_Loading_FlyEff(const CUI_Loading_FlyEff& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Loading_FlyEff::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Loading_FlyEff::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pHoleTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_LOADING, TEXT("Layer_UI_LoadingHole"), TEXT("Com_Transform")));
	Safe_AddRef(m_pHoleTransform);


	m_fSizeX = 200.f;
	m_fSizeY = 200.f;
	m_fPosX = m_vPrevWinSize.x * 0.5f;
	m_fPosY = m_vPrevWinSize.y * 0.5f;



	__super::Set_UI_Setting(m_fSizeX, m_fSizeY, m_fPosX, m_fPosY, 0.f);

	return S_OK;
}

void CUI_Loading_FlyEff::Camera_Update(_float fTimeDelta)
{
	__super::Camera_Update(fTimeDelta);
}

void CUI_Loading_FlyEff::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);


}

void CUI_Loading_FlyEff::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	RENDER_OBJECT tDesc{};
	tDesc.tGlowDesc.iPassIndex = 2;
	tDesc.tGlowDesc.fGlowFactor = 1.5f;

	m_pRenderInstance->Add_RenderObject(CRenderer::RG_MULTY_GLOW, this,&tDesc);
}

HRESULT CUI_Loading_FlyEff::Render(_float fTimeDelta)
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Loading_FlyEff::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LoadingBallEff"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Loading_FlyEff::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_fAnimFream)))
		return E_FAIL;

	return S_OK;
}

void CUI_Loading_FlyEff::Go_Target()
{
	_vector vTargetPos = m_pHoleTransform->Get_State(CTransform::STATE_POSITION);
	_vector vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
}

CUI_Loading_FlyEff* CUI_Loading_FlyEff::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Loading_FlyEff* pInstatnce = new CUI_Loading_FlyEff(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Loading_FlyEff"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Loading_FlyEff::Clone(void* pArg)
{
	CUI_Loading_FlyEff* pInstatnce = new CUI_Loading_FlyEff(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Loading_FlyEff"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Loading_FlyEff::Free()
{
	Safe_Release(m_pHoleTransform);

	__super::Free();
}
