#include "stdafx.h"
#include "..\Public\ParryingRangedObject.h"

#include "RenderInstance.h"
#include "GameInstance.h"

#include "UI_Define.h"
#include "Character.h"
#include "Main_Camera.h"
#include "Effect_Layer.h"

#include "Effect_Manager.h"


CParryingRangedObject::CParryingRangedObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CParryingRangedObject::CParryingRangedObject(const CParryingRangedObject& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CParryingRangedObject::Initialize_Prototype()
{

	
	return S_OK;
}

HRESULT CParryingRangedObject::Initialize(void* pArg)
{

	if (nullptr == pArg)
		return E_FAIL;


	PARRYING_RANGED_DESC* pDesc = static_cast<PARRYING_RANGED_DESC*>(pArg);


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	

	if (FAILED(Ready_Components(pDesc)))
		return E_FAIL;


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPos);


	CEffect_Layer::COPY_DESC tDesc{};
	tDesc.pPlayertMatrix = m_pTransformCom->Get_WorldMatrixPtr();
	m_pEffect_Layer = CEffect_Manager::Get_Instance()->Copy_Layer_AndGet(TEXT("Parrying_Ball"), &tDesc);

	


	return S_OK;
}

void CParryingRangedObject::Camera_Update(_float fTimeDelta)
{

}

void CParryingRangedObject::Update(_float fTimeDelta)
{

	if (Check_UpdateStop(fTimeDelta))
		return;
;


	m_fAccLifeTime += fTimeDelta;

	
	if(m_bDestroyObject == false)
	{
		if (m_fAccLifeTime < 1.5)
		{
			//m_pTransformCom->Add_Move({ 0.f,15 * fTimeDelta, 10 * fTimeDelta });
			m_pTransformCom->Add_Move({ 0.f,20 * fTimeDelta, 10 * fTimeDelta });
		}
		else
		{
			if (m_bFlipEnable)
			{
				m_pEffect_Layer->Set_Copy_Layer_Rotation({ 0.f,0.f,180.f });

				m_pTransformCom->Add_Move({ rand() % 10 - 5.f,0.f,0.f });
				m_bFlipEnable = false;
			}

			m_pTransformCom->Add_Move({ 0.f,-20 * fTimeDelta,10 * fTimeDelta });
		}

		if (m_fAccLifeTime > 5)
		{
			m_pEffect_Layer->m_bIsDoneAnim = true;
			m_pEffect_Layer = nullptr;
			Destory();
		}
	}


	//m_pTransformCom->Add_Move({ 0.f,5 * fTimeDelta,0.f });

	

}

void CParryingRangedObject::Late_Update(_float fTimeDelta)
{

	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CParryingRangedObject::Render(_float fTimeDelta)
{



	return S_OK;
}


_bool CParryingRangedObject::Check_UpdateStop(_float fTimeDelta)
{
	

	return false;
}




HRESULT CParryingRangedObject::Ready_Components(PARRYING_RANGED_DESC* pDesc)
{




	return S_OK;
}



CParryingRangedObject* CParryingRangedObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParryingRangedObject* pInstance = new CParryingRangedObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CParryingRangedObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CParryingRangedObject::Clone(void* pArg)
{
	CParryingRangedObject* pInstance = new CParryingRangedObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CParryingRangedObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParryingRangedObject::Free()
{
	__super::Free();

}
