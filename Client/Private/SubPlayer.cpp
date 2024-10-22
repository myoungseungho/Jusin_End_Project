#include "stdafx.h"
#include "..\Public\SubPlayer.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CSubPlayer::CSubPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPawn{ pDevice, pContext }

{
	Safe_AddRef(m_pUI_Manager);
}

CSubPlayer::CSubPlayer(const CSubPlayer& Prototype)
	: CPawn{ Prototype }
{
	Safe_AddRef(m_pUI_Manager);
}

HRESULT CSubPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSubPlayer::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(5.f, 0.f, 0.f, 1.f));

	m_pModelCom->SetUp_Animation(16, true);
	m_iHp = 100;

	m_ePawnID = ANDROID21;

	return S_OK;
}

void CSubPlayer::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}


void CSubPlayer::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	Action_Hit(DIK_I, 0.25f, fTimeDelta);
	//Action_AttBuf(DIK_V, m_ePlayerSlot, fTimeDelta);

	m_pModelCom->Play_Animation(fTimeDelta);
}

void CSubPlayer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pRenderInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CSubPlayer::Render(_float fTimeDelta)
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;

		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CSubPlayer::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Play_Goku"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CSubPlayer::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CSubPlayer* CSubPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSubPlayer* pInstance = new CSubPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CSubPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSubPlayer::Clone(void* pArg)
{
	CSubPlayer* pInstance = new CSubPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CSubPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
