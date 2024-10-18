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
	CTransform::TRANSFORM_DESC tDesc{};
	tDesc.fRotationPerSec = XMConvertToRadians(90.f);

	if (FAILED(__super::Initialize(&tDesc)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		EFFECT_DESC* pEffectDesc = static_cast<EFFECT_DESC*>(pArg);

		_float3 vPos = pEffectDesc->vPosition;
		_float3 vScale = pEffectDesc->vScaled;
		_float3 vRot = pEffectDesc->vRotation;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(vPos.x, vPos.y, vPos.z, 1.f));
		m_pTransformCom->Set_Scaled(vScale.x, vScale.y, vScale.z);
		m_pTransformCom->Rotate(vRot);

		m_EffectName = pEffectDesc->EffectName;
		m_ModelName = pEffectDesc->ModelName;
		m_MaskTextureName = pEffectDesc->MaskTextureName;
		m_DiffuseTextureName = pEffectDesc->DiffuseTextureName;
		m_iRenderIndex = pEffectDesc->iRenderIndex;
		m_iPassIndex = pEffectDesc->iPassIndex;
		m_iNumWidthImage = pEffectDesc->iNumWidthImage;
		m_iNumHeighthImage = pEffectDesc->iNumHeightImage;

		m_iUnique_Index = pEffectDesc->iUnique_Index;

		if (pEffectDesc->SRV_Ptr != nullptr)
			m_pDiffuseTextureCom->Set_SRV(static_cast<ID3D11ShaderResourceView*>(pEffectDesc->SRV_Ptr));
	}

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

void CEffect::Delete_KeyFrame(_uint KeyFrameNumber)
{
	m_pAnimation->Delete_KeyFrame(KeyFrameNumber);
}

_bool CEffect::Find_KeyFrame(_uint KeyFrameNumber)
{
	return m_pAnimation->Find_KeyFrame(KeyFrameNumber);
}

EFFECT_KEYFRAME CEffect::Get_KeyFrame(_uint KeyFrameNumber)
{
	return m_pAnimation->Get_KeyFrame(KeyFrameNumber);
}

EFFECT_KEYFRAME CEffect::Get_Near_Front_KeyFrame(_uint frameNumber)
{
	return m_pAnimation->Get_Near_Front_KeyFrame(frameNumber);
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

void CEffect::Set_Effect_IsNotPlaying(_bool bIsNotPlaying)
{
	m_bIsNotPlaying = bIsNotPlaying;
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

HRESULT CEffect::Play_Animation(_float CurrentFrame)
{
	if (m_pAnimation == nullptr)
		return E_FAIL;


	EFFECT_KEYFRAME ResultKeyFrame = m_pAnimation->Play_Animation(CurrentFrame, m_bIsLoop);

	Set_Effect_Scaled(ResultKeyFrame.vScale);
	Set_Effect_Position(ResultKeyFrame.vPosition);
	Set_Effect_Rotation(ResultKeyFrame.vRotation);

	return S_OK;
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

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pMaskTextureCom);
	Safe_Release(m_pAnimation);
}


