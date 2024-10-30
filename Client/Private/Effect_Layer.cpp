#pragma once
#include "stdafx.h"
#include "Effect_Layer.h"
#include "Effect.h"
#include "GameInstance.h"
#include "RenderInstance.h"
#include "Imgui_Manager.h"
#include "IMGUI_Shader_Tab.h"

CEffect_Layer::CEffect_Layer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:	m_pContext{ pContext },
		m_pDevice{ pDevice },
		m_pGameInstance{CGameInstance::Get_Instance()}
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pGameInstance);
}

CEffect_Layer::CEffect_Layer(const CEffect_Layer& Prototype)
	: m_fDuration{Prototype.m_fDuration}
	, m_iNumKeyFrames{Prototype.m_iNumKeyFrames }
	, m_fTickPerSecond {Prototype.m_fTickPerSecond }
	, m_pTransformCom{ Prototype.m_pTransformCom }
	, m_pColliderCom{ Prototype.m_pColliderCom }
{
	for (auto& pProtoEffect : Prototype.m_MixtureEffects)
	{
		m_bIsCopy = true;

		pProtoEffect->m_ForCopyInform.bIsCopy = m_bIsCopy;

		m_MixtureEffects.emplace_back(static_cast<CEffect*>((pProtoEffect->Clone(&(pProtoEffect->m_ForCopyInform)))));
	}

	for (auto& iter : m_MixtureEffects)
	{
		static_cast<CTexture*>(iter->Get_Component(TEXT("Com_DiffuseTexture")))
			->Set_SRV(static_cast<CIMGUI_Shader_Tab*>(CImgui_Manager::Get_Instance()
				->Access_Shader_Tab(iter->m_iUnique_Index))
				->m_TestEffectModel_Texture->Get_SRV(0));

		static_cast<CTexture*>(iter->Get_Component(TEXT("Com_DiffuseTexture")))
			->Set_SRV(static_cast<CIMGUI_Shader_Tab*>(CImgui_Manager::Get_Instance()
				->Access_Shader_Tab(iter->m_iUnique_Index))
				->m_TestEffectModel_Texture->Get_SRV(1), 1);

		if (iter->m_eEffect_Type == EFFECT_OVERLAP)
			continue;

		static_cast<CIMGUI_Shader_Tab*>(CImgui_Manager::Get_Instance()
			->Access_Shader_Tab(iter->m_iUnique_Index))
			->Add_Clone_EffectToShader_Texture(&(*iter));
	}

	Safe_AddRef(m_pTransformCom);
	Safe_AddRef(m_pColliderCom);
}

HRESULT CEffect_Layer::Initialize_Prototype(void* pArg)
{
	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	if (pArg != nullptr)
	{
		LAYER_DESC* pDesc = static_cast<LAYER_DESC*>(pArg);

		_float3 vPos = pDesc->vPosition;
		_float3 vScaled = pDesc->vScaled;
		_float3 vRotation = pDesc->vRotation;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(vPos.x, vPos.y, vPos.z, 1.f));
		m_pTransformCom->Set_Scaled(vScaled.x, vScaled.y, vScaled.z);
		m_pTransformCom->Rotate(vRotation);

		CBounding_Sphere::BOUNDING_SPHERE_DESC ColliderDesc{};
		ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_EFFECT_LAYER;
		ColliderDesc.vCenter = _float3(vPos.x, vPos.y, vPos.z);
		ColliderDesc.fRadius = 0.1f;

		m_pColliderCom = reinterpret_cast<CCollider*>(m_pGameInstance->Clone_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"), &ColliderDesc));

		m_pGameInstance->Add_ColliderObject(ColliderDesc.colliderGroup, m_pColliderCom);

		return S_OK;
	}
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	m_pTransformCom->Set_Scaled(1.f, 1.f, 1.f);
	m_pTransformCom->Rotate(_float3(0.f, 0.f, 0.f));

	CBounding_Sphere::BOUNDING_SPHERE_DESC ColliderDesc{};
	ColliderDesc.colliderGroup = CCollider_Manager::COLLIDERGROUP::CG_EFFECT_LAYER;
	ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColliderDesc.fRadius = 0.1f;

	m_pColliderCom = reinterpret_cast<CCollider*>(m_pGameInstance->Clone_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"), &ColliderDesc));
	
	m_pGameInstance->Add_ColliderObject(ColliderDesc.colliderGroup, m_pColliderCom);
	return S_OK;
}

HRESULT CEffect_Layer::Initialize(const _float4x4* pArg)
{
	if (pArg != nullptr)
	{
		m_pPlayerMatrix = pArg;
		LayerMatrix = m_pTransformCom->Get_WorldMatrix();

		if (0 > m_pPlayerMatrix->_11)
		{
			LayerMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));

			XMVECTOR Scale, Rotation, Position;

			XMMatrixDecompose(&Scale, &Rotation, &Position, XMLoadFloat4x4(m_pPlayerMatrix));

			_float4x4 fLayerMatrix;

			XMStoreFloat4x4(&fLayerMatrix, LayerMatrix);

			//fLayerMatrix._41 *= -1;
			fLayerMatrix._43 *= -1;
			fLayerMatrix._41 += XMVectorGetX(Position);

			LayerMatrix = XMLoadFloat4x4(&fLayerMatrix);
			
		}
		else
		{
			XMVECTOR Scale, Rotation, Position;

			XMMatrixDecompose(&Scale, &Rotation, &Position, XMLoadFloat4x4(m_pPlayerMatrix));

			_float4x4 fLayerMatrix;

			XMStoreFloat4x4(&fLayerMatrix, LayerMatrix);

			fLayerMatrix._41 += XMVectorGetX(Position);

			LayerMatrix = XMLoadFloat4x4(&fLayerMatrix);
		}

		return S_OK;
	}
	else
	{
		_matrix TestMatrix = XMMatrixIdentity();

		LayerMatrix = m_pTransformCom->Multiple_Matrix(TestMatrix);
	}

	return S_OK;
}

void CEffect_Layer::Camera_Update(_float fTimeDelta)
{
}

void CEffect_Layer::Update(_float fTimeDelta)
{
	for (auto& pEffect : m_MixtureEffects)
		pEffect->Update(fTimeDelta);

	if (m_bIsCopy)
	{
		for (auto& pEffect : m_MixtureEffects)
		{
			pEffect->Get_Layer_Matrix(LayerMatrix);
		}
		Play_Effect_Animation(fTimeDelta);
	}


	m_pColliderCom->UpdateVector(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

}

void CEffect_Layer::Late_Update(_float fTimeDelta)
{
	if (m_bIsRender)
	{
		for (auto& pEffect : m_MixtureEffects)
		{
			pEffect->Late_Update(fTimeDelta);
		}
	}

	CRenderInstance::Get_Instance()->Add_DebugComponent(m_pColliderCom);
}

HRESULT CEffect_Layer::Render(_float fTimeDelta)
{
	m_pColliderCom->Render(fTimeDelta);

	return S_OK;
}

HRESULT CEffect_Layer::Add_Effect(CEffect* pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	m_MixtureEffects.emplace_back(pEffect);

	return S_OK;
}

vector<class CEffect*> CEffect_Layer::Get_Effects()
{
	return m_MixtureEffects;
}

CEffect* CEffect_Layer::Find_Effect(const std::wstring& effectName)
{
	for (CEffect* pEffect : m_MixtureEffects)
	{
		if (pEffect && pEffect->m_EffectName == effectName) // 이름이 일치하는지 확인
		{
			return pEffect;
		}
	}
	return nullptr;
}

HRESULT CEffect_Layer::Play_Effect_Animation(_float fTimeDelta)
{
	if (m_iNumKeyFrames <= 0 || m_fTickPerSecond <= 0.0f)
		return E_FAIL;

	// 프레임 간격 계산
	float frameInterval = m_fDuration / m_iNumKeyFrames;

	// 현재 애니메이션 위치 업데이트
	m_fCurrentAnimPosition += fTimeDelta * m_fTickPerSecond;

	// 애니메이션 종료 시 위치 초기화
	if (m_fCurrentAnimPosition > m_fDuration)
	{
		//m_fCurrentAnimPosition = 0.f;
		m_bIsDoneAnim = true;
	}

	// 현재 키프레임 인덱스 계산
	_float currentFrame = (m_fCurrentAnimPosition / frameInterval);

	// 각 효과에 대해 현재 위치에 맞는 애니메이션 값을 적용
	for (CEffect* pEffect : m_MixtureEffects)
	{
		if (pEffect)
		{
			pEffect->Play_Animation(currentFrame);
		}
	}

	return S_OK;
}

HRESULT CEffect_Layer::Set_In_Layer_Effect()
{
	for (CEffect* pEffect : m_MixtureEffects)
	{
		if (pEffect)
		{
			pEffect->Get_Layer_Matrix(m_pTransformCom->Get_WorldMatrix());
		}
	}

	return S_OK;
}

void CEffect_Layer::Set_Animation_Position(_float fNewCurPos)
{
	// 포지션 값을 현재 애니메이션 위치로 설정
	m_fCurrentAnimPosition = fNewCurPos;

	// 유효한 키프레임 개수와 프레임 간격을 계산
	if (m_iNumKeyFrames <= 0 || m_fDuration <= 0.0f)
		return;

	// 프레임 간격을 계산하여 현재 위치에서의 프레임을 얻음
	float frameInterval = m_fDuration / m_iNumKeyFrames;
	_float currentFrame = (m_fCurrentAnimPosition / frameInterval);

	// 각 효과에 대해 현재 위치에 맞는 애니메이션 값을 적용
	for (CEffect* pEffect : m_MixtureEffects)
	{
		if (pEffect)
		{
			// currentFrame 값을 전달하여 해당 위치에 맞는 보간된 애니메이션 상태 설정
			pEffect->Play_Animation(currentFrame);
		}
	}
}

HRESULT CEffect_Layer::Set_Layer_Scaled(_float3 ChangeScaled)
{
	m_pTransformCom->Set_Scaled(ChangeScaled.x, ChangeScaled.y, ChangeScaled.z);

	return S_OK;
}

HRESULT CEffect_Layer::Set_Layer_Position(_float3 ChangePosition)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(ChangePosition.x, ChangePosition.y, ChangePosition.z, 1.f));
	return S_OK;
}

HRESULT CEffect_Layer::Set_Layer_Rotation(_float3 ChangeRotation)
{
	m_pTransformCom->Rotate(ChangeRotation);

	return S_OK;
}

_float3 CEffect_Layer::Get_Layer_Scaled()
{
	return m_pTransformCom->Get_Scaled();
}

_float3 CEffect_Layer::Get_Layer_Position()
{
	_float3 Position;

	Position.x = XMVectorGetX(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	Position.y = XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	Position.z = XMVectorGetZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	return Position;
}

_float3 CEffect_Layer::Get_Layer_Rotation()
{
	return m_pTransformCom->Get_Rotation();
}

CEffect_Layer* CEffect_Layer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CEffect_Layer* pInstance = new CEffect_Layer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CEffect_Layer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CEffect_Layer* CEffect_Layer::Clone(const _float4x4* pArg)
{
	CEffect_Layer* pInstance = new CEffect_Layer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CEffect_Layer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Layer::Free()
{
	if (m_bIsCopy == true)
	{
		for (auto& iter : m_MixtureEffects)
		{
			static_cast<CIMGUI_Shader_Tab*>(CImgui_Manager::Get_Instance()
				->Access_Shader_Tab(iter->m_iUnique_Index))
				->Delete_Clone_EffectToShader_Texture(&(*iter));
		}
	}



	__super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);

	for (auto& pMixtureEffect : m_MixtureEffects)
		Safe_Release(pMixtureEffect);

	m_MixtureEffects.clear();
}
