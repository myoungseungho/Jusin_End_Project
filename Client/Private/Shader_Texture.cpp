#include "stdafx.h"
#include "..\Public\Shader_Texture.h"

#include "RenderInstance.h"
#include "GameInstance.h"

#include "Imgui_Manager.h"
#include "IMGUI_Shader_Tab.h"

CShader_Texture::CShader_Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CShader_Texture::CShader_Texture(const CShader_Texture& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CShader_Texture::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CShader_Texture::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	m_fSizeX = 512;
	m_fSizeY = 512;
	m_fX = g_iWinSizeX >> 1;
	m_fY = g_iWinSizeY >> 1;
	m_isTex = true;
	CImgui_Manager* pImGui_Manager = CImgui_Manager::Get_Instance();

	static_cast<CIMGUI_Shader_Tab*>(pImGui_Manager->Access_Shader_Tab())->Push_ShaderTexture(this);

	m_pTransformCom->Set_Scaled(m_fSizeX, m_fSizeY, 1.f);

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION,
	//	XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION,
	//	XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));


	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CShader_Texture::Priority_Update(_float fTimeDelta)
{
	m_fTime += fTimeDelta;
}

void CShader_Texture::Update(_float fTimeDelta)
{
}

void CShader_Texture::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CShader_Texture::Render(_float fTimeDelta)
{
	if (FAILED(m_pRenderInstance->Begin_MRT(m_Key)))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(m_pRenderInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShader_Texture::Ready_Components(void* pArg)
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxShaderRect"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	m_Key = static_cast<SHADER_TEXTURE_DESC*>(pArg)->prototypeKey;
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_Key,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CShader_Texture::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	//if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
	//	return E_FAIL;

	//if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
	//	return E_FAIL;
	//
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fTime, sizeof(float))))
		return E_FAIL;

	if (m_isTex == true)
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("isBindTexture", &m_isTex, sizeof(bool))))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("isBindTexture", &m_isTex, sizeof(bool))))
			return E_FAIL;
	}


	return S_OK;
}

CShader_Texture* CShader_Texture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CShader_Texture* pInstance = new CShader_Texture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CShader_Texture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CShader_Texture::Clone(void* pArg)
{
	CShader_Texture* pInstance = new CShader_Texture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CShader_Texture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader_Texture::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
