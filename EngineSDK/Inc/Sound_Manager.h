#pragma once
#ifndef _CSOUND_H_
#define _CSOUND_H_

#include <fmod.h>
#include "GameObject.h"
#include "Engine_Defines.h"

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

BEGIN(Engine)

class CSound_Manager final : public CGameObject
{
public:
	enum class SOUND_KEY_NAME :_int
	{
		SPACE_BGM = 0,
		Goku_Heavy_Attack,
		Goku_Upper_Attack,
		Goku_Range_Attack,
		Goku_Energy_Attack,
		Goku_Ultimate_Attack_0,
		Goku_Ultimate_Attack_1,
		Goku_Ultimate_Attack_2,
		Goku_Heavy_Attack_SFX,
		Goku_Energy_SFX,
		Common_DownHook_Dash_SFX,
		Common_Dash_SFX,
		Goku_Range_Attack_SFX,
		Goku_Ultimate_0_SFX,
		Goku_Ultimate_1_SFX,
		Goku_Ultimate_2_SFX,
		Goku_Ultimate_3_SFX,
		Heavy_Attack_21,
		Light_Attack_21_SFX,
		Heavy_Attack_21_SFX,
		Chase_Attack_21,
		Grab_Attack_21,
		Grab_Attack_0_21_SFX,
		Grab_Attack_1_21_SFX,
		NARRATION_READY,
		NARRATION_FIGHT,
		LOGO_BGM,
	};

	enum class SOUND_GROUP_KEY_NAME :_int
	{
		Light_Attack_Goku_1 = 100,
		Light_Attack_Goku_2,
		Goku_Hit_0,
		Goku_Hit_1,
		Goku_Hit_2,
		Goku_Hit_3,
		Goku_Hit_4,
		Goku_Hit_5,
		Goku_Hit_6,
		Goku_Hit_7,
		Goku_Hit_8,
		Goku_Hit_9,
		Goku_Hit_10,
		Goku_Hit_11,
		Goku_Hit_12,
		Goku_Hit_13,
		Goku_Hit_14,
		Goku_Hit_15,
		Light_Attack_21_1,
		Light_Attack_21_2,
		Light_Attack_21_3,
		Light_Attack_21_4,
		Hit_21_0,
		Hit_21_1,
		Hit_21_2,
		Hit_21_3,
		Hit_21_4,
		Hit_21_5,
		Light_Attack_Goku_1_SFX,
		Light_Attack_Goku_2_SFX,
		Light_Attack_Goku_3_SFX,
	};

	enum class SOUND_GROUP_KEY :_int
	{
		LIGHT_ATTACK_Goku = 200,
		Hit_Goku,
		LIGHT_ATTACK_21,
		Hit_21,
		LIGHT_ATTACK_Goku_SFX
	};

	enum class SOUND_CATEGORY
	{
		BGM,
		VOICE,
		SFX
	};

private:
	CSound_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSound_Manager(const CSound_Manager& Prototype);
	virtual ~CSound_Manager() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Camera_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;


	void Register_Sound(const std::wstring& filePath, SOUND_KEY_NAME alias, SOUND_CATEGORY category, _bool loop = false);
	void Register_Sound_Group(SOUND_GROUP_KEY groupKey, const std::wstring& filePath, SOUND_GROUP_KEY_NAME alias, SOUND_CATEGORY category, _bool loop);
	void Play_Sound(SOUND_KEY_NAME alias, _bool loop, _float volume);
	void Play_Group_Sound(SOUND_GROUP_KEY groupKey, _bool loop, _float volume);
	void Stop_Sound(SOUND_KEY_NAME alias);
	void Stop_Group_Sound(SOUND_GROUP_KEY groupKey);
	void Set_Volume(SOUND_KEY_NAME alias, float volume);
	void Set_Group_Volume(SOUND_GROUP_KEY groupKey, float volume);
	void Set_Category_Volume(SOUND_CATEGORY category, float volume);

	void Set_ImguiPlay(_bool isPlay);

public:
	_bool m_isImguiPlay = { true };

private:
	FMOD_SYSTEM* m_pSoundSystem;
	map<SOUND_KEY_NAME, FMOD_SOUND*> m_soundMap; // 개별 사운드 맵
	map<SOUND_GROUP_KEY_NAME, FMOD_SOUND*> m_groupSoundMap; // 그룹 사운드 맵 (새로 추가)
	map<SOUND_KEY_NAME, FMOD_CHANNEL*> m_channelMap;
	map<SOUND_GROUP_KEY_NAME, FMOD_CHANNEL*> m_groupChannelMap; // 그룹 사운드 채널 맵 (새로 추가)

	_uint m_iNumLevels;
	static const _uint MAX_CHANNELS = 64;  // 최대 채널 수 정의
	class FMOD_CHANNELGROUP* m_pChannelGroup = nullptr;

	// 추가된 멤버 변수
	map<SOUND_GROUP_KEY, vector<SOUND_GROUP_KEY_NAME>> m_soundGroupMap;  // 그룹별로 음원 alias를 저장하는 맵
	map<SOUND_GROUP_KEY, SOUND_GROUP_KEY_NAME> m_lastPlayedSound;  // 마지막에 재생된 음원을 저장하는 맵

	// 개별 사운드용 맵
	map<SOUND_KEY_NAME, SOUND_CATEGORY> m_soundCategoryMap;
	// 그룹 사운드용 맵
	map<SOUND_GROUP_KEY_NAME, SOUND_CATEGORY> m_groupSoundCategoryMap;
public:

	static CSound_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};


#endif

END