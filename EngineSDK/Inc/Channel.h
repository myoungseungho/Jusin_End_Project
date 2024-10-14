#pragma once

#include "Base.h"

/* ä��(aiNodeAnim) : �ϳ��� �� ����, �� ���� Ư�� �ð��뿡 � ���¸� ���� �ϴ°�? */
/* ��, ä���� �ð��� ���� ����(Ű������)������ ������. */

BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(ChannelData& chanelData, const vector<class CBone*>& Bones);

	void Compute_TransformationMatrix(_float fCurrentPosition, const vector<class CBone*>& Bones, _uint* pKeyFrameIndex);
	_char* GetName() { return m_szName; };
	_int GetBoneIndex() { return m_iBoneIndex; };

	void Update_FrameIndex(_float fCurrentPosition,_uint* pKeyFrameIndex);

public:
	_char				m_szName[MAX_PATH] = {};

	_uint				m_iBoneIndex = {};
	_uint				m_iNumKeyFrames = {};
	vector<KEYFRAME>	m_KeyFrames;

public:
	static CChannel* Create(ChannelData& chanelData, const vector<class CBone*>& Bones);

	virtual void Free() override;
};

END