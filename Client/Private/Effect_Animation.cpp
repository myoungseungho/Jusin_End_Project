#pragma once

#include "stdafx.h"
#include "Effect_Animation.h"
#include "GameInstance.h"

CEffect_Animation::CEffect_Animation()
	: m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

CEffect_Animation::CEffect_Animation(const CEffect_Animation& Prototype)
	: m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CEffect_Animation::Initialize()
{
	return S_OK;
}

void CEffect_Animation::Add_KeyFrame(_uint KeyFrameNumber, EFFECT_KEYFRAME NewKeyFrameData)
{
	auto it = m_EffectKeyFrames.find(KeyFrameNumber);
	if (it != m_EffectKeyFrames.end()) {
		it->second = NewKeyFrameData;
	}
	else {
		m_EffectKeyFrames.emplace(KeyFrameNumber, NewKeyFrameData);
	}
}

void CEffect_Animation::Delete_KeyFrame(_uint KeyFrameNumber)
{
	auto it = m_EffectKeyFrames.find(KeyFrameNumber);

	if (it != m_EffectKeyFrames.end())
	{
		m_EffectKeyFrames.erase(it);
	}
}

_bool CEffect_Animation::Find_KeyFrame(_uint KeyFrameNumber)
{
	auto it = m_EffectKeyFrames.find(KeyFrameNumber);

	return it != m_EffectKeyFrames.end();
}

EFFECT_KEYFRAME CEffect_Animation::Get_KeyFrame(_uint KeyFrameNumber)
{
	auto it = m_EffectKeyFrames.find(KeyFrameNumber);

	if (it != m_EffectKeyFrames.end()) {
		return (it->second);
	}
	else {
		return EFFECT_KEYFRAME();
	}
}

EFFECT_KEYFRAME CEffect_Animation::Get_Near_Front_KeyFrame(_uint KeyFrameNumber)
{
	if (m_EffectKeyFrames.empty())
		return EFFECT_KEYFRAME();

	auto it = m_EffectKeyFrames.upper_bound(KeyFrameNumber);
	if (it == m_EffectKeyFrames.begin())
		return EFFECT_KEYFRAME();

	--it;
	return it->second;
}

EFFECT_KEYFRAME CEffect_Animation::Play_Animation(_float CurAnimPos)
{
	// Ű�������� ������ �� Ű������ ��ȯ
	if (m_EffectKeyFrames.empty())
		return EFFECT_KEYFRAME();

	// ���� �ִϸ��̼� ��ġ�� ���� �� Ű�������� ã�� ����
	auto it1 = m_EffectKeyFrames.lower_bound(CurAnimPos);

	// ���� CurAnimPos�� ������ Ű������ ���Ķ�� ������ Ű������ ���¸� ����
	if (it1 == m_EffectKeyFrames.end())
		return std::prev(it1)->second;

	auto it2 = (it1 == m_EffectKeyFrames.begin()) ? it1 : std::prev(it1);

	// ���� ��ġ�� ��Ȯ�� �ϳ��� Ű������ ��ġ�� ��ġ�ϸ� �� Ű������ ��ȯ
	if (it1 == it2 || it1->first == CurAnimPos)
		return it1->second;

	// ������ �ʿ��� ��� �� Ű������ ������ ������
	const EFFECT_KEYFRAME& keyFrame1 = it2->second;
	const EFFECT_KEYFRAME& keyFrame2 = it1->second;

	// ���� ���� ��� (0�� 1 ������ ��)
	float timeDiff = it1->first - it2->first;
	float factor = (CurAnimPos - it2->first) / timeDiff;

	// �����Ͽ� ���� Ű������ �� ����
	EFFECT_KEYFRAME interpolatedKeyFrame;
	interpolatedKeyFrame.vPosition = Lerp(keyFrame1.vPosition, keyFrame2.vPosition, factor);
	interpolatedKeyFrame.vScale = Lerp(keyFrame1.vScale, keyFrame2.vScale, factor);
	interpolatedKeyFrame.vRotation = Lerp(keyFrame1.vRotation, keyFrame2.vRotation, factor);
	interpolatedKeyFrame.bIsNotPlaying = keyFrame1.bIsNotPlaying;  // �ܼ��� ù ��° ���� ���

	return interpolatedKeyFrame;
}


_float3 CEffect_Animation::Lerp(const _float3& start, const _float3& end, _float factor)
{
	XMVECTOR vecStart = XMLoadFloat3(&start);
	XMVECTOR vecEnd = XMLoadFloat3(&end);

	// ���� ���
	XMVECTOR interpolatedVec = XMVectorLerp(vecStart, vecEnd, factor);

	// ����� XMFLOAT3�� ��ȯ �� ��ȯ
	_float3 result;
	XMStoreFloat3(&result, interpolatedVec);

	return result;
}

CEffect_Animation* CEffect_Animation::Create()
{
	CEffect_Animation* pInstance = new CEffect_Animation();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CEffect_Animation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Animation::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);

}
