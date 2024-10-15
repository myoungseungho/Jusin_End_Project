#include "stdafx.h"
#include "..\Public\FallingStar.h"

#include "RenderInstance.h"
#include "GameInstance.h"

CFallingStar::CFallingStar(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject { pDevice, pContext }
{

}

CFallingStar::CFallingStar(const CFallingStar & Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CFallingStar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFallingStar::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	//1.f*0.7f, 0.3f * 0.5f, 0.6f * 0.5f
	m_pTransformCom->Set_Scaled(0.1f, 0.1f, 0.1f);
	//m_pTransformCom->Rotation(XMVectorSet(0.5f, 0.f, 0.5f, 0.f), XMConvertToRadians(180.f));
	m_fSpriteSize = _float2(1.0f / m_fSpriteAnimCount.x, 1.0f / m_fSpriteAnimCount.y);
	return S_OK;
}

void CFallingStar::Priority_Update(_float fTimeDelta)
{
	m_fAccTime += fTimeDelta * 5;

	if (m_fAccTime > 0.2)
	{
		m_fAccTime = 0.f;
		m_fSpriteCurPos.x++;

		if (m_fSpriteCurPos.x == m_fSpriteAnimCount.x)
		{
			m_fSpriteCurPos.x = 0.f;
			m_fSpriteCurPos.y++;
		}
	}

	if (m_fSpriteCurPos.y == m_fSpriteAnimCount.y)
	{
		//m_fSpriteCurPos.y = m_fSpriteAnimCount.y - 1;
		//m_fSpriteCurPos.x = m_fSpriteAnimCount.x - 1;
		m_fSpriteCurPos.y = 0;
		m_fSpriteCurPos.x = 0;
	}
}

void CFallingStar::Update(_float fTimeDelta)
{

}

void CFallingStar::Late_Update(_float fTimeDelta)
{
	_vector vPos = m_pGameInstance->Get_CamPosition_Vector();
	vPos = XMVectorSetZ(vPos, XMVectorGetZ(vPos) + 50);
	//vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 50);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	m_pRenderInstance->Add_RenderObject(CRenderer::RG_GLOW_PRI, this);
}

HRESULT CFallingStar::Render(_float fTimeDelta)
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{

		if (FAILED(m_pShaderCom->Begin(1)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CFallingStar::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Space_FallingStar"),
		TEXT("Com_Texture_Diffuse"), reinterpret_cast<CComponent**>(&m_pTextureCom_Diffuse))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_FallingStar"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CFallingStar::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	if (FAILED(m_pTextureCom_Diffuse->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fSpriteSize", &m_fSpriteSize, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fSpriteCurPos", &m_fSpriteCurPos, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fAccTime, sizeof(float))))
		return E_FAIL;
	
	return S_OK;
}

CFallingStar * CFallingStar::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CFallingStar*		pInstance = new CFallingStar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CFallingStar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFallingStar::Clone(void * pArg)
{
	CFallingStar*		pInstance = new CFallingStar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CFallingStar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFallingStar::Free()
{


	/*for (size_t i = 0; i < SPACE_END; i++)
	{		
		Safe_Release(m_pTextureCom[i]);
	}*/



	Safe_Release(m_pTextureCom_Diffuse);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

	__super::Free();
}
