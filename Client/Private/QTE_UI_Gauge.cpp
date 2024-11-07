#include "stdafx.h"
#include "..\Public\QTE_UI_Gauge.h"

#include "RenderInstance.h"
#include "GameInstance.h"

CQTE_UI_Gauge::CQTE_UI_Gauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CQTE_UI_Gauge::CQTE_UI_Gauge(const CQTE_UI_Gauge& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CQTE_UI_Gauge::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CQTE_UI_Gauge::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	CQTE_UI_Gauge::QTE_UI_Gauge_DESC* desc = static_cast<CQTE_UI_Gauge::QTE_UI_Gauge_DESC*>(pArg);

	m_fSizeX = desc->fSizeX;
	m_fSizeY = desc->fSizeY;
	m_fX = desc->fX;
	m_fY = desc->fY;
	m_fPlayTime = desc->playTime;

	m_pTransformCom->Set_Scaled(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.9f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CQTE_UI_Gauge::Camera_Update(_float fTimeDelta)
{

}

void CQTE_UI_Gauge::Update(_float fTimeDelta)
{
	//시간이 다 흘렀다면
	if (m_fElapsedTime >= m_fPlayTime)
	{
		m_fElapsedTime = m_fPlayTime;
		return;
	}

	m_fElapsedTime += fTimeDelta;
}

void CQTE_UI_Gauge::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CQTE_UI_Gauge::Render(_float fTimeDelta)
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(24)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CQTE_UI_Gauge::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI_VtxRect"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_QTE_Gauge"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CQTE_UI_Gauge::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	// 시간 값을 셰이더로 전달
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fElapsedTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_MaxTime", &m_fPlayTime, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CQTE_UI_Gauge* CQTE_UI_Gauge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CQTE_UI_Gauge* pInstance = new CQTE_UI_Gauge(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CQTE_UI_Gauge"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CQTE_UI_Gauge::Clone(void* pArg)
{
	CQTE_UI_Gauge* pInstance = new CQTE_UI_Gauge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CQTE_UI_Gauge"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQTE_UI_Gauge::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);

	__super::Free();
}
