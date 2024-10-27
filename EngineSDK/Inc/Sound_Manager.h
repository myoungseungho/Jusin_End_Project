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
private:
	CSound_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSound_Manager(const CSound_Manager& Prototype);
	virtual ~CSound_Manager() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;


	void Register_Sound(const wstring& filePath, const wstring& alias);
	void Register_Sound_Group(const wstring& groupKey, const std::wstring& filePath, const wstring& alias);
	void Play_Sound(const wstring& alias, _bool loop, _float volume = 1.f);
	void Play_Sound_Group(const wstring& groupKey, _bool loop, _float volume);
	void Stop_Sound(const wstring& alias);
	void Set_Volume(const wstring& alias, float volume);
	void Stop_All_Sounds();

private:
	FMOD_SYSTEM* m_pSoundSystem;
	map<wstring, FMOD_SOUND*> m_soundMap;
	map<wstring, FMOD_CHANNEL*> m_channelMap;
	_uint m_iNumLevels;
	static const _uint MAX_CHANNELS = 64;  // 최대 채널 수 정의
	class FMOD_CHANNELGROUP* m_pChannelGroup = nullptr;

	// 추가된 멤버 변수
	map<wstring, vector<wstring>> m_soundGroupMap;  // 그룹별로 음원 alias를 저장하는 맵
	map<wstring, wstring> m_lastPlayedSound;  // 마지막에 재생된 음원을 저장하는 맵

public:

	static CSound_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};


#endif

END