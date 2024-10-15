#pragma once

#include "stdafx.h"
#include "Effect_Animation.h"
#include "GameInstance.h"
#include "Effect_Animation.h"

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

EFFECT_KEYFRAME CEffect_Animation::Play_Animation(_float CurAnimPos, _bool bIsLoop)
{
	// 키프레임이 없으면 빈 키프레임 반환
	if (m_EffectKeyFrames.empty())
		return EFFECT_KEYFRAME();

	// 현재 애니메이션 위치에 따라 두 키프레임을 찾아 보간
	auto it1 = m_EffectKeyFrames.lower_bound(CurAnimPos);
	auto it2 = (it1 == m_EffectKeyFrames.begin()) ? it1 : std::prev(it1);

	// 만약 CurAnimPos가 마지막 키프레임 이후라면 마지막 상태 처리
	if (it1 == m_EffectKeyFrames.end())
	{
		if (bIsLoop)
		{
			// 반복 재생을 위해 처음으로 되돌림
			CurAnimPos = fmod(CurAnimPos, std::prev(it1)->first);
			it1 = m_EffectKeyFrames.lower_bound(CurAnimPos);
			it2 = (it1 == m_EffectKeyFrames.begin()) ? it1 : std::prev(it1);
		}
		else
		{
			// 반복이 아닌 경우 마지막 키프레임 유지
			return std::prev(it1)->second;
		}
	}

	// 현재 위치가 정확히 하나의 키프레임 위치와 일치하면 그 키프레임 반환
	if (it1 == it2 || it1->first == CurAnimPos)
		return it1->second;

	// 보간이 필요한 경우 두 키프레임 정보를 가져옴
	const EFFECT_KEYFRAME& keyFrame1 = it2->second;
	const EFFECT_KEYFRAME& keyFrame2 = it1->second;

	// 보간 인자 계산 (0과 1 사이의 값)
	float timeDiff = it1->first - it2->first;
	float factor = (CurAnimPos - it2->first) / timeDiff;

	// 보간하여 최종 키프레임 값 생성
	EFFECT_KEYFRAME interpolatedKeyFrame;
	interpolatedKeyFrame.vPosition = Lerp(keyFrame1.vPosition, keyFrame2.vPosition, factor);
	interpolatedKeyFrame.vScale = Lerp(keyFrame1.vScale, keyFrame2.vScale, factor);
	interpolatedKeyFrame.vRotation = Lerp(keyFrame1.vRotation, keyFrame2.vRotation, factor);
	interpolatedKeyFrame.bIsNotPlaying = keyFrame1.bIsNotPlaying; // 단순히 첫 번째 값을 사용

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

	m_EffectKeyFrames.clear();

	Safe_Release(m_pGameInstance);

}
