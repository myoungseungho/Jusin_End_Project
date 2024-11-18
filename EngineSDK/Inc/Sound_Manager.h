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
		LOGO_START,
		Smash_Hit_SFX,
		UI_MENU_OPEN,
		UI_MENU_CLOSE,
		UI_MENU_CURSOR,
		LOBBY_BGM,
		LOBBY_FOOT_SFX,
		NARRATION_CHARASELECT,
		NARRATION_CHARASELECT_GOKU,
		NARRATION_CHARASELECT_21,
		NARRATION_CHARASELECT_BUU,
		NARRATION_CHARASELECT_HIT,
		CHARASELECT_BGM,
		NARRATION_VS,
		NARRATION_VS_SFX,
		LOADING_CREATE_SFX,
		LOADING_BALL_SFX,
		LOADING_BALL_FINISH_SFX,
		QTE_SUCCESS_SFX,
		QTE_SAMEGRAB_SUCCESS_SFX,
		QTE_SAMEGRAB_FAIL_SFX,
		Heavy_Attack_Hit,
		Hit_Down_Forward_Light,
		Hit_Down_Forward_Light_Light_Attack,
		Hit_Down_Forward_Light_Middle_Attack,
		Hit_Down_Forward_Light_Heavy_Attack,
		Hit_BackDash,
		Hit_Grab_Try,
		Hit_Grab_Success,
		Hit_Light_Attack_Final_Attack,
		Hit_Ultimate_1,
		Hit_Ultimate_3_0,
		Hit_Ultimate_3_1,
		Hit_Ultimate_3_2,
		Hit_Ultimate_3_3,
		Hit_Light_Attack_SFX,
		Hit_J_Attack_SFX,
		Hit_Down_Forward_Light_Light_Attack_SFX,
		Hit_Down_Forward_Light_Heavy_Attack_SFX,
		Hit_Down_Backward_Light_Cancle_Attack_SFX,
		Hit_Ultimate_1_Start,
		Hit_Ultimate_1_Attack,
		Hit_Ultimate_3_Attack,
		J_Attack_21,
		Down_Forward_J_21,
		Down_Backward_Light_21,
		Ultimate_1_Start_21,
		Ultimate_3_Start_21,
		Ultimate_3_Middle_21,
		Ultimate_3_Final_21,
		J_Attack_Voice_21,
		Ultimate_1_Start_21_SFX,
		Ultimate_1_Charge_21_SFX,
		Ultimate_1_Expl_21_SFX,
		Ultimate_1_Shot_21_SFX,
		Ultimate_3_Start_21_SFX,
		Ultimate_3_Dash_21_SFX,
		Ultimate_3_Attacks_21_SFX,
		Ultimate_3_Finish_21_SFX,
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
		Light_Attack_Hit_0,
		Light_Attack_Hit_1,
		Light_Attack_Hit_2,
		Light_Attack_Hit_3,
		HIT_J_Attack_0,
		HIT_J_Attack_1,
		HIT_Down_Back_Light_0,
		HIT_Down_Back_Light_1,
		Hit_Air_Chase_0,
		Hit_Air_Chase_1,
		Hit_Hit_0,
		Hit_Hit_1,
		Hit_Hit_2,
	};

	enum class SOUND_GROUP_KEY :_int
	{
		LIGHT_ATTACK_Goku = 200,
		Hit_Goku,
		LIGHT_ATTACK_21,
		Hit_21,
		LIGHT_ATTACK_Goku_SFX,
		LIGHT_ATTACK_HIT,
		J_ATTACK_HIT,
		HIT_Down_Back_Light,
		Hit_Air_Chase,
		Hit_Hit
	};

	enum class SOUND_CATEGORY
	{
		BGM,
		VOICE,
		SFX
	};

	struct ChannelInfo
	{
		FMOD_CHANNEL* channel;
		float baseVolume;
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
	_float Get_Category_Volume(SOUND_CATEGORY category);

	void Set_ImguiPlay(_bool isPlay);

public:
	_bool m_isImguiPlay = { true };

private:
	FMOD_SYSTEM* m_pSoundSystem;
	map<SOUND_KEY_NAME, FMOD_SOUND*> m_soundMap; // 개별 사운드 맵
	map<SOUND_GROUP_KEY_NAME, FMOD_SOUND*> m_groupSoundMap; // 그룹 사운드 맵 (새로 추가)
	map<SOUND_KEY_NAME, ChannelInfo> m_channelMap;
	map<SOUND_GROUP_KEY_NAME, ChannelInfo> m_groupChannelMap;

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

	// 카테고리별 현재 볼륨을 저장하는 맵
	map<SOUND_CATEGORY, float> m_categoryVolumes;
public:

	static CSound_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};


#endif

END