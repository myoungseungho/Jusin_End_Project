#pragma once
#include "stdafx.h"
#include "Effect_Multi.h"
#include "GameInstance.h"


CEffect_Multi::CEffect_Multi(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CEffect{ pDevice ,pContext }
{
}

CEffect_Multi::CEffect_Multi(const CGameObject& Prototype)
    : CEffect{ Prototype }
{
}

HRESULT CEffect_Multi::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEffect_Multi::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_eEffect_Type = EFFECT_MULTI;

	EFFECT_DESC* pEffectDesc = static_cast<EFFECT_DESC*>(pArg);

	m_ModelName = pEffectDesc->ModelName;
	m_MaskTextureName = pEffectDesc->MaskTextureName;
	m_DiffuseTextureName = pEffectDesc->DiffuseTextureName;

	if (FAILED(Ready_Components(&m_ModelName, &m_MaskTextureName, &m_DiffuseTextureName)))
		return E_FAIL;

    return S_OK;
}

void CEffect_Multi::Priority_Update(_float fTimeDelta)
{
}

void CEffect_Multi::Update(_float fTimeDelta)
{
}

void CEffect_Multi::Late_Update(_float fTimeDelta)
{
}

HRESULT CEffect_Multi::Render(_float fTimeDelta)
{
    return S_OK;
}

HRESULT CEffect_Multi::Ready_Components(_wstring* pModelName, _wstring* pMaskTextureName, _wstring* pDiffuseTexturueName)
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Single_Eff_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	wstring ModelName = L"Prototype_Component_" + *pModelName;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, ModelName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	wstring MaskTextureName = L"Prototype_Component_" + *pMaskTextureName;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, MaskTextureName,
		TEXT("Com_MaskTexture"), reinterpret_cast<CComponent**>(&m_pMaskTextureCom))))
		return E_FAIL;

	wstring DiffuseTexturueName = L"Prototype_Component_" + *pDiffuseTexturueName;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, DiffuseTexturueName,
		TEXT("Com_DiffuseTexture"), reinterpret_cast<CComponent**>(&m_pDiffuseTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Multi::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

CEffect_Multi* CEffect_Multi::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Multi* pInstance = new CEffect_Multi(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEffect_Multi"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Multi::Clone(void* pArg)
{
	CEffect_Multi* pInstance = new CEffect_Multi(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CEffect_Multi"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Multi::Free()
{
    __super::Free();


}
