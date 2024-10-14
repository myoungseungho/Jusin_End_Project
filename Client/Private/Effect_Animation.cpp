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

EFFECT_KEYFRAME CEffect_Animation::Play_Animation(_float CurAnimPos)
{
	if (m_EffectKeyFrames.size() <= 0)
		return EFFECT_KEYFRAME();  // 빈 키프레임을 반환

	// 현재 애니메이션 위치에 따라 두 키프레임을 찾아 보간
	auto it1 = m_EffectKeyFrames.lower_bound(CurAnimPos);
	auto it2 = (it1 == m_EffectKeyFrames.begin()) ? it1 : std::prev(it1);

	if (it1 == m_EffectKeyFrames.end())
		return it2->second;  // 마지막 키프레임을 반환

	const EFFECT_KEYFRAME& keyFrame1 = it2->second;
	const EFFECT_KEYFRAME& keyFrame2 = it1->second;

	if (it1 == it2) {
		return it1->second;
	}

	// 분모가 0이 아닌지 확인하여 보간 계산
	float timeDiff = it1->first - it2->first;
	if (timeDiff == 0.0f) {
		return it1->second;  // 분모가 0이면 해당 키프레임 값 반환
	}

	float factor = (CurAnimPos - it2->first) / timeDiff;

	// 각 구성 요소를 보간
	EFFECT_KEYFRAME interpolatedKeyFrame;
	interpolatedKeyFrame.vPosition = Lerp(keyFrame1.vPosition, keyFrame2.vPosition, factor);
	interpolatedKeyFrame.vScale = Lerp(keyFrame1.vScale, keyFrame2.vScale, factor);
	interpolatedKeyFrame.vRotation = Lerp(keyFrame1.vRotation, keyFrame2.vRotation, factor);
	interpolatedKeyFrame.bIsNotPlaying = keyFrame1.bIsNotPlaying;  // 단순히 첫 번째 값을 사용 (필요 시 추가 로직)

	return interpolatedKeyFrame;
}

_float3 CEffect_Animation::Lerp(const _float3& start, const _float3& end, _float factor)
{
	XMVECTOR vecStart = XMLoadFloat3(&start);
	XMVECTOR vecEnd = XMLoadFloat3(&end);

	// 보간 계산
	XMVECTOR interpolatedVec = XMVectorLerp(vecStart, vecEnd, factor);

	// 결과를 XMFLOAT3로 변환 후 반환
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
