#include "stdafx.h"
#include "..\Public\SpaceMeteoBreak.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "Effect_Manager.h"

CSpaceMeteoBreak::CSpaceMeteoBreak(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject { pDevice, pContext }
{

}

CSpaceMeteoBreak::CSpaceMeteoBreak(const CSpaceMeteoBreak & Prototype)
	: CGameObject{ Prototype },
	m_vFragmentMoveDir{
		{0.f, 1.f, 0.f},
		{0.f, 1.f, 1.f},
		{0.f, 0.f, 1.f},
		{-0.5f, 0.1f, 1.f}, // 오른쪽 속도 1 말고 다르게
		{0.f, 0.1f, -1.2f},
		{-0.5f, -0.1f, -0.3f}, //6
		{-0.3f, -0.2f, 0.8f},
		{0.f, -1.f, 0.3f},
		{0.f, -0.1f, -1.2f},
		{-0.2f, -0.2f, -0.8f},
		{0.f, -1.f, -0.3f}
	}
{

}

HRESULT CSpaceMeteoBreak::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpaceMeteoBreak::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//m_pTransformCom->Set_Scaled(0.01f, 0.01f, 0.01f);
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(100.f, 30.f, 0.f, 1.f));
	_vector vMainPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	for (size_t i = 0; i < 11; i++)
		XMStoreFloat4(&m_vFragmentPosition[i], vMainPos);

	//Update(4.f);

	return S_OK;
}

void CSpaceMeteoBreak::Camera_Update(_float fTimeDelta)
{
	//m_fAccTime += fTimeDelta * 5;
	if (m_pGameInstance->Key_Down(DIK_F10))
	{
		_vector vMainPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		for (size_t i = 0; i < 11; i++)
			XMStoreFloat4(&m_vFragmentPosition[i], vMainPos);

		m_fBrakeSwitchTime = false;
		m_isBrakeSwitch = false;
		m_isFastSwitch = true;
		m_fAccTime = 0.f;
		m_fBrakeSwitchTime = 0.f;
		m_fSpeed = 10.5f;
	}
}

void CSpaceMeteoBreak::Update(_float fTimeDelta)
{
	if (m_isBrakeSwitch == false)
	{
		m_fBrakeSwitchTime += fTimeDelta;

		if (m_fBrakeSwitchTime > 1.f)
		{
			m_fBrakeSwitchTime = 0.f;
			m_isBrakeSwitch = true;



			_float4x4 Result4x4;

				XMStoreFloat4x4(&Result4x4, m_pTransformCom->Get_WorldMatrix());
				//Result4x4._41 = 0.f;
				//Result4x4._42 = 0.f;
				//Result4x4._43 = 0.f;
			//	Result4x4 = Character_Make_Matrix(fOffset, bFlipDirection);

			//CEffect_Manager::Get_Instance()->Copy_Layer(TEXT("Smoke03_Stop"), &Result4x4);
			//CEffect_Layer* pEffect = CEffect_Manager::Get_Instance()->Copy_Layer_AndGet(TEXT("BurstU-3_01"), &Result4x4);
			CEffect_Layer* paEffect = CEffect_Manager::Get_Instance()->Copy_Layer_AndGet(TEXT("Meteo_Wind"), &Result4x4);

			//if (pEffect != nullptr)
			//	pEffect->Set_Layer_Scaled({ 30.f,30.f,30.f });
			if (paEffect != nullptr)
				paEffect->Set_Layer_Scaled({ 30.f,30.f,30.f });
		}
	}
	else
	{
		m_fAccTime += fTimeDelta;

		if (m_isFastSwitch == true && m_fFastTimeLimit < m_fAccTime)
		{
			m_isFastSwitch = false;
			m_fSpeed = 0.3f;
		}


		for (size_t i = 0; i < 11; i++)
		{
			m_vFragmentPosition[i].x += m_vFragmentMoveDir[i].x * fTimeDelta * m_fSpeed;
			m_vFragmentPosition[i].y += m_vFragmentMoveDir[i].y * fTimeDelta * m_fSpeed;
			m_vFragmentPosition[i].z += m_vFragmentMoveDir[i].z * fTimeDelta * m_fSpeed;
		}
	}
}

void CSpaceMeteoBreak::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CSpaceMeteoBreak::Render(_float fTimeDelta)
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (m_isBrakeSwitch == false)
	{
		_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Begin(12)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(i)))
				return E_FAIL;
		}
	}
	else
	{
		for (size_t i = 0; i < 11; i++)
		{
			_uint		iNumMeshes = m_pFragmentModelCom[i]->Get_NumMeshes();

			for (size_t j = 0; j < iNumMeshes; j++)
			{
				if (FAILED(m_pTextureCom_Diffuse->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
					return E_FAIL;

				_int iMeshIndex = j;
				if (FAILED(m_pShaderCom->Bind_RawValue("g_iMeteoIndex", &iMeshIndex, sizeof(_int))))
					return E_FAIL;

				if (FAILED(m_pShaderCom->Bind_RawValue("g_iMeteoPosition", &m_vFragmentPosition[i], sizeof(_float4))))
					return E_FAIL;
		
				if (FAILED(m_pShaderCom->Begin(11)))
					return E_FAIL;

				if (FAILED(m_pFragmentModelCom[i]->Render(j)))
					return E_FAIL;
			}
		}
	}
	
	return S_OK;
}

HRESULT CSpaceMeteoBreak::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxSpace"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_sp_meteobrake01"),
		TEXT("Com_Texture_Diffuse"), reinterpret_cast<CComponent**>(&m_pTextureCom_Diffuse))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_MeteoBreak"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	for (size_t i = 0; i < 11; i++)
	{
		wstring strTagName = TEXT("Prototype_Component_Model_MeteoBrake_") + to_wstring(i + 1);
		wstring strComName = TEXT("Com_FragmentModel_") + to_wstring(i + 1);
		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, strTagName.c_str(),
			strComName.c_str(), reinterpret_cast<CComponent**>(&m_pFragmentModelCom[i]))))
			return E_FAIL;
	}


	
	return S_OK;
}

HRESULT CSpaceMeteoBreak::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	//if (FAILED(m_pShaderCom->Bind_RawValue("g_fSpriteSize", &m_fSpriteSize, sizeof(_float2))))
	//	return E_FAIL;

	//if (FAILED(m_pShaderCom->Bind_RawValue("g_fSpriteCurPos", &m_fSpriteCurPos, sizeof(_float2))))
	//	return E_FAIL;

	//if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fAccTime, sizeof(float))))
	//	return E_FAIL;
	
	return S_OK;
}

CSpaceMeteoBreak * CSpaceMeteoBreak::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSpaceMeteoBreak*		pInstance = new CSpaceMeteoBreak(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CSpaceMeteoBreak"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSpaceMeteoBreak::Clone(void * pArg)
{
	CSpaceMeteoBreak*		pInstance = new CSpaceMeteoBreak(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CSpaceMeteoBreak"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpaceMeteoBreak::Free()
{
	Safe_Release(m_pTextureCom_Diffuse);
	for (size_t i = 0; i < 11; i++)
		Safe_Release(m_pFragmentModelCom[i]);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

	__super::Free();
}
