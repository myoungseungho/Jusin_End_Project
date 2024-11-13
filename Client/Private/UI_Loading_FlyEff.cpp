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

	m_fAngle += fTimeDelta * 180.f;
	m_fPosDuration += fTimeDelta;

	if (m_fAngle >= 360.f)
		m_fAngle = 0.f;

	_float WarfPosX = XMVectorGetX(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) + 100.f * cos(m_fAngle * (3.14f / 180.f));
	_float WarfPosZ = XMVectorGetZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) - 100.f * sin(m_fAngle * (3.14f / 180.f));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { WarfPosX  , WarfPosZ  , 0.f,1.f});

	m_fCurrPos = _float2(WarfPosX, WarfPosZ);

	if(fTimeDelta >= 0.2f)
		m_fPrevPos = _float2(WarfPosX, WarfPosZ);
}

void CUI_Loading_FlyEff::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Loading_FlyEff::Render(_float fTimeDelta)
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(32)))
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LoadingFlyEffect"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Loading_FlyEff::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCurrPos", &m_fCurrPos, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vPrevPos", &m_fPrevPos, sizeof(_float2))))
		return E_FAIL;

	return S_OK;
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
	__super::Free();
}
