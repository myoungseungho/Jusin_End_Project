#pragma once
#include "stdafx.h"
#include "..\Public\Effect.h"
#include "GameInstance.h"
#include "Effect_Animation.h"

CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice , pContext }
{
}

CEffect::CEffect(const CGameObject& Prototype)
	:CGameObject{ Prototype }
{
}

HRESULT CEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pAnimation = CEffect_Animation::Create();

	if (nullptr == m_pAnimation)
		return E_FAIL;

	return S_OK;
}

void CEffect::Priority_Update(_float fTimeDelta)
{

}

void CEffect::Update(_float fTimeDelta)
{

}

void CEffect::Late_Update(_float fTimeDelta)
{

}

HRESULT CEffect::Render(_float fTimeDelta)
{
	return S_OK;
}

void CEffect::Add_KeyFrame(_uint KeyFrameNumber, EFFECT_KEYFRAME NewKeyFrame)
{
	m_pAnimation->Add_KeyFrame(KeyFrameNumber, NewKeyFrame);
}

_bool CEffect::Find_KeyFrame(_uint KeyFrameNumber)
{
	return m_pAnimation->Find_KeyFrame(KeyFrameNumber);
}

void CEffect::Set_Effect_Scaled(_float3 ChangeScaled)
{
	m_pTransformCom->Set_Scaled(ChangeScaled.x, ChangeScaled.y, ChangeScaled.z);
}

void CEffect::Set_Effect_Position(_float3 ChangePosition)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(ChangePosition.x, ChangePosition.y, ChangePosition.z, 1.f));
}

void CEffect::Set_Effect_Rotation(_float3 ChangeRotation)
{
	m_pTransformCom->Rotate(ChangeRotation);
}

_float3 CEffect::Get_Effect_Scaled()
{
	return m_pTransformCom->Get_Scaled();
}

_float3 CEffect::Get_Effect_Position()
{
	_float3 Position;

	Position.x = XMVectorGetX(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	Position.y = XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	Position.z = XMVectorGetZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	return Position;
}

_float3 CEffect::Get_Effect_Rotation()
{
	return m_pTransformCom->Get_Rotation();
}

HRESULT CEffect::Ready_Components(_wstring* pModelName, _wstring* pMaskTextureName, _wstring* pDiffuseTexturueName)
{
	return S_OK;
}

HRESULT CEffect::Bind_ShaderResources()
{
	return S_OK;
}

CEffect* CEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect* pInstance = new CEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect::Clone(void* pArg)
{
	CEffect* pInstance = new CEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect::Free()
{
	__super::Free();
}


