#pragma once
#include "stdafx.h"
#include "Effect_Layer.h"
#include "Effect.h"

CEffect_Layer::CEffect_Layer()
{
}

CEffect_Layer::CEffect_Layer(const CEffect_Layer& Prototype)
	: m_fDuration{Prototype.m_fDuration}
	, m_iNumKeyFrames{Prototype.m_iNumKeyFrames }
	, m_fTickPerSecond {Prototype.m_fTickPerSecond }
	, m_MixtureEffects {Prototype.m_MixtureEffects }
{
}

HRESULT CEffect_Layer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Layer::Initialize(void* pArg)
{
	return S_OK;
}

void CEffect_Layer::Priority_Update(_float fTimeDelta)
{
}

void CEffect_Layer::Update(_float fTimeDelta)
{
	for (auto& pEffect : m_MixtureEffects)
		pEffect->Update(fTimeDelta);
}

void CEffect_Layer::Late_Update(_float fTimeDelta)
{
	for (auto& pEffect : m_MixtureEffects)
		pEffect->Late_Update(fTimeDelta);
}

HRESULT CEffect_Layer::Render(_float fTimeDelta)
{
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
		m_fCurrentAnimPosition = m_fDuration;
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

CEffect_Layer* CEffect_Layer::Create()
{
	return new CEffect_Layer();
}

CEffect_Layer* CEffect_Layer::Clone(void* pArg)
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
	__super::Free();

	for (auto& pMixtureEffect : m_MixtureEffects)
		Safe_Release(pMixtureEffect);

	m_MixtureEffects.clear();
}
