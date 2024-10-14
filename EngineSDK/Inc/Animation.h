#pragma once

#include "Base.h"

/* �ִϸ��̼� : Attack, Idle, Walk, Jump */
/* �ִϸ��̼� Ŭ������ ��� */
/* �������� �ִϸ��̼�(Attack, Idle, Walk, Jump )��, �� ����(Ư���ִϸ��̼�(Attack)�� ���ۺ��� ������)�� ǥ���ϱ����� � ��(ä��)���� ���������ϴ����� ���� ������ ������. */
/* �����ϰ����� ������ ��ü ��� �ð�.(m_fDuration) */
/* ��� �ӵ� m_fTickPerSecond */

BEGIN(Engine)

class CAnimation final : public CBase
{
public:
	struct SoundEvent
	{
		_float triggerTime;     // ���尡 ����� �ִϸ��̼� �ð�
		wstring soundAlias; // ����� ������ ��Ī
		_bool hasPlayed;        // �ش� ����Ŭ���� ���尡 ����Ǿ����� ����
		_float volume;
	};

private:
	CAnimation();
	CAnimation(const CAnimation& Prototype);
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(AnimationData& animationData, const vector<class CBone*>& Bones, vector<_uint>& KeyFrameIndices);

	_bool Update_TransformationMatrix(_float* pCurrentAnimPosition, _float fTimeDelta, const vector<class CBone*>& Bones, _bool isLoop, vector<_uint>& KeyFrameIndices);
	void Compute_FirstKeyFrameMatrixForBone(const char* boneName, const vector<class CBone*>& Bones, _matrix* outMatrix);
	_uint GetNumChannels() { return m_iNumChannels; };
	_float GetDuration() { return m_fDuration; };

	void Add_SoundEvent(_float triggerTime, const std::wstring& soundAlias, _float volume);


	void Update_FrameIndex(_float* pCurrentAnimPosition,  vector<_uint>& KeyFrameIndices);

public:
	_char			m_szName[MAX_PATH] = {};
	_float			m_fDuration = {};
	_float			m_fTickPerSecond = {};
	_float			m_fCurrentPosition = {};


	/* �� �ִϸ��̼��� ǥ���ϱ����� ��Ʈ���ؾ��ϴ� ���� ���� */
	_uint						m_iNumChannels = {};

	/* �� �ִϸ��̼��� ǥ���ϱ����� ��Ʈ���ؾ��ϴ� ���� */
	vector<class CChannel*>		m_Channels;
	vector<SoundEvent> m_SoundEvents;

private:
	class CGameInstance* m_pGameInstance = { nullptr };
	
public:
	static CAnimation* Create(AnimationData animationData, const vector<class CBone*>& Bones, vector<_uint>& KeyFrameIndices);

	virtual void Free() override;
};

END