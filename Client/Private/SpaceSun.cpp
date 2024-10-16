#include "stdafx.h"
#include "..\Public\SpaceSun.h"

#include "RenderInstance.h"
#include "GameInstance.h"

CSpaceSun::CSpaceSun(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject { pDevice, pContext }
{

}

CSpaceSun::CSpaceSun(const CSpaceSun & Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CSpaceSun::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpaceSun::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//m_pTransformCom->Set_Scaled(0.01f, 0.01f, 0.01f);
	//m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(180.f));

	return S_OK;
}

void CSpaceSun::Priority_Update(_float fTimeDelta)
{
	//m_fAccTime += fTimeDelta * 5;
}

void CSpaceSun::Update(_float fTimeDelta)
{

}

void CSpaceSun::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CSpaceSun::Render(_float fTimeDelta)
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		_int iCount = i;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_SunMeshIndex", &iCount, sizeof(int))))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;
		
		if (FAILED(m_pShaderCom->Begin(7)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CSpaceSun::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Space_Light"),
		TEXT("Com_Texture_Light"), reinterpret_cast<CComponent**>(&m_pTextureCom_Light))))
		return E_FAIL;
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Space_Rainbow"),
		TEXT("Com_Texture_Rainbow"), reinterpret_cast<CComponent**>(&m_pTextureCom_Rainbow))))
		return E_FAIL;
	//Prototype_Component_Texture_Space_Light Prototype_Component_Texture_Space_Rainbow
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SpaceSun"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_Rect"), reinterpret_cast<CComponent**>(&m_pVIBufferCom_Rainbow))))
		return E_FAIL;

	m_pTransformCom_Rainbow = CTransform::Create(m_pDevice, m_pContext);
	//611
	if (nullptr == m_pTransformCom_Rainbow)
		return E_FAIL;

	
	return S_OK;
}

HRESULT CSpaceSun::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	//if (FAILED(m_pTextureCom_Diffuse->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
	//	return E_FAIL;

	//if (FAILED(m_pShaderCom->Bind_RawValue("g_fSpriteSize", &m_fSpriteSize, sizeof(_float2))))
	//	return E_FAIL;

	//if (FAILED(m_pShaderCom->Bind_RawValue("g_fSpriteCurPos", &m_fSpriteCurPos, sizeof(_float2))))
	//	return E_FAIL;

	//if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fAccTime, sizeof(float))))
	//	return E_FAIL;
	
	return S_OK;
}

CSpaceSun * CSpaceSun::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSpaceSun*		pInstance = new CSpaceSun(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CSpaceSun"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSpaceSun::Clone(void * pArg)
{
	CSpaceSun*		pInstance = new CSpaceSun(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CSpaceSun"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpaceSun::Free()
{

	Safe_Release(m_pTransformCom_Rainbow);
	Safe_Release(m_pVIBufferCom_Rainbow);
	Safe_Release(m_pTextureCom_Rainbow);
	Safe_Release(m_pTextureCom_Light);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

	__super::Free();
}
