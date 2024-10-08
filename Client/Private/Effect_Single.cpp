#pragma once
#include "stdafx.h"
#include "Effect_Single.h"
#include "GameInstance.h"

CEffect_Single::CEffect_Single(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect{ pDevice ,pContext }
{
}

CEffect_Single::CEffect_Single(const CGameObject& Prototype)
	: CEffect{ Prototype }
{
}

HRESULT CEffect_Single::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Single::Initialize(void* pArg)
{
	EFFECT_DESC* pEffectDesc = static_cast<EFFECT_DESC*>(pArg);

	if (FAILED(Ready_Components(&(pEffectDesc->ModelName), &(pEffectDesc->MaskTextureName),&(pEffectDesc->DiffuseTextureName))))
		return E_FAIL;

	return S_OK;
}

void CEffect_Single::Priority_Update(_float fTimeDelta)
{
}

void CEffect_Single::Update(_float fTimeDelta)
{
}

void CEffect_Single::Late_Update(_float fTimeDelta)
{
}

HRESULT CEffect_Single::Render(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CEffect_Single::Ready_Components(_wstring* pModelName, _wstring* pMaskTextureName, _wstring* pDiffuseTexturueName)
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Eff_VtxMesh"),
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

CEffect_Single* CEffect_Single::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Single* pInstance = new CEffect_Single(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEffect_Single"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Single::Clone(void* pArg)
{
	CEffect_Single* pInstance = new CEffect_Single(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CEffect_Single"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Single::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pMaskTextureCom);
}
