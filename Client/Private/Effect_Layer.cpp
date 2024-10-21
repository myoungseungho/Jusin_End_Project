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
		if (pEffect && pEffect->m_EffectName == effectName) // �̸��� ��ġ�ϴ��� Ȯ��
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

	// ������ ���� ���
	float frameInterval = m_fDuration / m_iNumKeyFrames;

	// ���� �ִϸ��̼� ��ġ ������Ʈ
	m_fCurrentAnimPosition += fTimeDelta * m_fTickPerSecond;

	// �ִϸ��̼� ���� �� ��ġ �ʱ�ȭ
	if (m_fCurrentAnimPosition > m_fDuration)
	{
		m_fCurrentAnimPosition = m_fDuration;
	}

	// ���� Ű������ �ε��� ���
	_float currentFrame = (m_fCurrentAnimPosition / frameInterval);

	// �� ȿ���� ���� ���� ��ġ�� �´� �ִϸ��̼� ���� ����
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
	// ������ ���� ���� �ִϸ��̼� ��ġ�� ����
	m_fCurrentAnimPosition = fNewCurPos;

	// ��ȿ�� Ű������ ������ ������ ������ ���
	if (m_iNumKeyFrames <= 0 || m_fDuration <= 0.0f)
		return;

	// ������ ������ ����Ͽ� ���� ��ġ������ �������� ����
	float frameInterval = m_fDuration / m_iNumKeyFrames;
	_float currentFrame = (m_fCurrentAnimPosition / frameInterval);

	// �� ȿ���� ���� ���� ��ġ�� �´� �ִϸ��̼� ���� ����
	for (CEffect* pEffect : m_MixtureEffects)
	{
		if (pEffect)
		{
			// currentFrame ���� �����Ͽ� �ش� ��ġ�� �´� ������ �ִϸ��̼� ���� ����
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
