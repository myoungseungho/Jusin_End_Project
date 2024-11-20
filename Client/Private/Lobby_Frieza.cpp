#include "stdafx.h"
#include "..\Public\Lobby_Frieza.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Level_Lobby.h"

CLobby_Frieza::CLobby_Frieza(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CLobby_Frieza::CLobby_Frieza(const CLobby_Frieza& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CLobby_Frieza::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLobby_Frieza::Initialize(void* pArg)
{
	CTransform::TRANSFORM_DESC Desc{};
	Desc.fRotationPerSec = 1.f;
	Desc.fSpeedPerSec = 10.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State_Position(_float3(-56.f, 0.f, 0.7f));
	m_pTransformCom->Rotation({ 0.f , 1.f, 0.f }, XMConvertToRadians(100.f));
	
	//아이들
	m_pModelCom->SetUp_Animation(0, true, 0.1f);

	return S_OK;
}

void CLobby_Frieza::Camera_Update(_float fTimeDelta)
{

}

void CLobby_Frieza::Update(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta);

}

void CLobby_Frieza::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CLobby_Frieza::Render(_float fTimeDelta)
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		/* 모델이 가지고 있는 머테리얼 중 i번째 메시가 사용해야하는 머테리얼구조체의 aiTextureType_DIFFUSE번째 텍스쳐를 */
		/* m_pShaderCom에 있는 g_DiffuseTexture변수에 던져. */
		if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;

		//m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i);

		/* 모델이 가지고 있는 뼈들 중에서 현재 렌더링할려고 했던 i번째ㅑ 메시가 사용하는 뼈들을 배열로 만들어서 쉐이더로 던져준다.  */
		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;


		if (FAILED(m_pShaderCom->Begin(3)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLobby_Frieza::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Model_Lobby_Frieza"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLobby_Frieza::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CLobby_Frieza::Entry_Level()
{
	_vector position = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float x = XMVectorGetX(position);
	_float z = XMVectorGetZ(position);

	_bool isGameEntry = x<-49.898f && z>-5.4f;

	if (isGameEntry)
	{
		CLevel_Lobby* level_Lobby = static_cast<CLevel_Lobby*>(m_pGameInstance->Get_Level());
		level_Lobby->Change_Level();
	}
}

CLobby_Frieza* CLobby_Frieza::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLobby_Frieza* pInstance = new CLobby_Frieza(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CLobby_Frieza"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLobby_Frieza::Clone(void* pArg)
{
	CLobby_Frieza* pInstance = new CLobby_Frieza(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CLobby_Frieza"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLobby_Frieza::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

	__super::Free();
}
