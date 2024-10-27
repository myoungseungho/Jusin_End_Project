#include "..\Public\Sound_Manager.h"
#include "Layer.h"
#include "GameObject.h"
#include <locale>
#include <codecvt>

CSound_Manager::CSound_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CSound_Manager::CSound_Manager(const CSound_Manager& Prototype)
	: CGameObject(Prototype)
{
}


HRESULT CSound_Manager::Initialize_Prototype()
{
	// FMOD 시스템 초기화
	FMOD_System_Create(&m_pSoundSystem, FMOD_VERSION);
	FMOD_System_Init(m_pSoundSystem, MAX_CHANNELS, FMOD_INIT_NORMAL, nullptr);

	// 채널 그룹 생성
	FMOD_System_CreateChannelGroup(m_pSoundSystem, nullptr, &m_pChannelGroup);

	return S_OK;
}

HRESULT CSound_Manager::Initialize(void* pArg)
{
	return S_OK;
}

void CSound_Manager::Priority_Update(_float fTimeDelta)
{

}

void CSound_Manager::Update(_float fTimeDelta)
{

}

void CSound_Manager::Late_Update(_float fTimeDelta)
{

}

HRESULT CSound_Manager::Render(_float fTimeDelta)
{
	return S_OK;
}

void CSound_Manager::Register_Sound(const std::wstring& filePath, SOUND_KEY_NAME alias)
{
	// 이미 등록된 사운드인지 확인
	if (m_soundMap.find(alias) != m_soundMap.end())
	{
		return; // 이미 존재하므로 등록하지 않고 반환
	}

	FMOD_SOUND* sound;

	// wstring을 string으로 변환
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string filePathStr = converter.to_bytes(filePath);

	if (FMOD_System_CreateSound(m_pSoundSystem, filePathStr.c_str(), FMOD_DEFAULT, nullptr, &sound) != FMOD_OK)
	{
		return;
	}

	// 사운드 등록
	m_soundMap[alias] = sound;
}

void CSound_Manager::Register_Sound_Group(SOUND_GROUP_KEY groupKey, const std::wstring& filePath, SOUND_GROUP_KEY_NAME alias)
{
	// 그룹이 존재하지 않으면 생성
	if (m_soundGroupMap.find(groupKey) == m_soundGroupMap.end())
	{
		m_soundGroupMap[groupKey] = std::vector<SOUND_GROUP_KEY_NAME>();
	}

	// 이미 등록된 사운드인지 확인
	if (m_groupSoundMap.find(alias) == m_groupSoundMap.end())
	{
		// 그룹 사운드 등록
		FMOD_SOUND* sound;

		// wstring을 string으로 변환
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::string filePathStr = converter.to_bytes(filePath);

		if (FMOD_System_CreateSound(m_pSoundSystem, filePathStr.c_str(), FMOD_DEFAULT, nullptr, &sound) != FMOD_OK)
		{
			return;
		}

		// 그룹 사운드 맵에 등록
		m_groupSoundMap[alias] = sound;
	}

	// 그룹에 해당 alias를 추가
	m_soundGroupMap[groupKey].push_back(alias);
}

void CSound_Manager::Play_Sound(SOUND_KEY_NAME alias, _bool loop, _float volume)
{
	auto it = m_soundMap.find(alias);
	if (it == m_soundMap.end()) return;

	FMOD_CHANNEL* channel = nullptr;

	FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	FMOD_Sound_SetMode(it->second, mode);

	// 재생 완료된 채널 정리
	int playingChannels = 0;
	FMOD_ChannelGroup_GetNumChannels(m_pChannelGroup, &playingChannels);

	for (int i = 0; i < playingChannels; ++i)
	{
		FMOD_CHANNEL* tempChannel = nullptr;
		if (FMOD_ChannelGroup_GetChannel(m_pChannelGroup, i, &tempChannel) == FMOD_OK)
		{
			FMOD_BOOL isPlaying = false;
			if (FMOD_Channel_IsPlaying(tempChannel, &isPlaying) == FMOD_OK && !isPlaying)
			{
				FMOD_Channel_Stop(tempChannel);  // 재생 완료된 채널 정지
			}
		}
	}

	// 재생 중인 채널 수 다시 확인
	FMOD_ChannelGroup_GetNumChannels(m_pChannelGroup, &playingChannels);

	if (playingChannels < MAX_CHANNELS) // MAX_CHANNELS는 최대 채널 수
	{
		// 새로운 채널을 생성하여 재생
		if (FMOD_System_PlaySound(m_pSoundSystem, it->second, m_pChannelGroup, false, &channel) == FMOD_OK)
		{
			// 볼륨 설정
			FMOD_Channel_SetVolume(channel, volume);

			m_channelMap[alias] = channel;
		}
	}
	else
	{
		// 모든 채널이 사용 중인 경우 처리 (예: 기존 채널 중단 후 재생)
		return;
	}
}

void CSound_Manager::Play_Sound_Group(SOUND_GROUP_KEY groupKey, _bool loop, _float volume)
{
	// 그룹이 존재하지 않으면 반환
	if (m_soundGroupMap.find(groupKey) == m_soundGroupMap.end()) return;

	auto& soundList = m_soundGroupMap[groupKey];

	// 그룹 내에서 재생 중인 사운드가 있는지 확인
	for (const auto& soundAlias : soundList)
	{
		auto it = m_groupChannelMap.find(soundAlias);
		if (it != m_groupChannelMap.end())
		{
			FMOD_BOOL isPlaying = false;
			if (FMOD_Channel_IsPlaying(it->second, &isPlaying) == FMOD_OK && isPlaying)
			{
				return; // 재생 중인 사운드가 있으면 반환
			}
		}
	}

	// 마지막으로 재생된 사운드를 제외한 사운드 목록 생성
	std::vector<SOUND_GROUP_KEY_NAME> availableSounds;
	for (const auto& soundAlias : soundList)
	{
		if (soundAlias != m_lastPlayedSound[groupKey])
		{
			availableSounds.push_back(soundAlias);
		}
	}

	// 재생 가능한 사운드가 없으면 모든 사운드를 다시 고려
	if (availableSounds.empty())
	{
		availableSounds = soundList;
	}

	// 랜덤으로 하나의 사운드 선택
	int randomIndex = rand() % availableSounds.size();
	SOUND_GROUP_KEY_NAME selectedSound = availableSounds[randomIndex];

	// 선택한 사운드 재생
	Play_Group_Sound(selectedSound, loop, volume);

	// 마지막으로 재생한 사운드 저장
	m_lastPlayedSound[groupKey] = selectedSound;
}

void CSound_Manager::Play_Group_Sound(SOUND_GROUP_KEY_NAME alias, _bool loop, _float volume)
{
	auto it = m_groupSoundMap.find(alias);
	if (it == m_groupSoundMap.end()) return;

	FMOD_CHANNEL* channel = nullptr;

	FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	FMOD_Sound_SetMode(it->second, mode);

	// 재생 완료된 채널 정리
	int playingChannels = 0;
	FMOD_ChannelGroup_GetNumChannels(m_pChannelGroup, &playingChannels);

	for (int i = 0; i < playingChannels; ++i)
	{
		FMOD_CHANNEL* tempChannel = nullptr;
		if (FMOD_ChannelGroup_GetChannel(m_pChannelGroup, i, &tempChannel) == FMOD_OK)
		{
			FMOD_BOOL isPlaying = false;
			if (FMOD_Channel_IsPlaying(tempChannel, &isPlaying) == FMOD_OK && !isPlaying)
			{
				FMOD_Channel_Stop(tempChannel);  // 재생 완료된 채널 정지
			}
		}
	}

	// 재생 중인 채널 수 다시 확인
	FMOD_ChannelGroup_GetNumChannels(m_pChannelGroup, &playingChannels);

	if (playingChannels < MAX_CHANNELS)
	{
		// 새로운 채널을 생성하여 재생
		if (FMOD_System_PlaySound(m_pSoundSystem, it->second, m_pChannelGroup, false, &channel) == FMOD_OK)
		{
			// 볼륨 설정
			FMOD_Channel_SetVolume(channel, volume);

			m_groupChannelMap[alias] = channel;
		}
	}
	else
	{
		// 모든 채널이 사용 중인 경우 처리
		return;
	}
}

void CSound_Manager::Stop_Sound(SOUND_KEY_NAME alias)
{
	auto it = m_channelMap.find(alias);
	if (it == m_channelMap.end()) return;

	FMOD_Channel_Stop(it->second);
}

void CSound_Manager::Stop_Group_Sound(SOUND_GROUP_KEY_NAME alias)
{
	auto it = m_groupChannelMap.find(alias);
	if (it == m_groupChannelMap.end()) return;

	FMOD_Channel_Stop(it->second);
}


void CSound_Manager::Set_Volume(SOUND_KEY_NAME alias, float volume)
{
	auto it = m_channelMap.find(alias);
	if (it == m_channelMap.end()) return;

	FMOD_Channel_SetVolume(it->second, volume);
}

void CSound_Manager::Set_Group_Volume(SOUND_GROUP_KEY_NAME alias, float volume)
{
	auto it = m_groupChannelMap.find(alias);
	if (it == m_groupChannelMap.end()) return;

	FMOD_Channel_SetVolume(it->second, volume);
}

CSound_Manager* CSound_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSound_Manager* pInstance = new CSound_Manager(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype())) {
		MessageBox(0, L"CSound_Manager Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSound_Manager::Clone(void* pArg)
{
	return this;
}

void CSound_Manager::Free()
{
	// 개별 사운드 해제
	for (auto& pair : m_soundMap)
	{
		FMOD_Sound_Release(pair.second);
	}
	m_soundMap.clear();
	m_channelMap.clear();

	// 그룹 사운드 해제
	for (auto& pair : m_groupSoundMap)
	{
		FMOD_Sound_Release(pair.second);
	}
	m_groupSoundMap.clear();
	m_groupChannelMap.clear();

	if (m_pSoundSystem)
	{
		FMOD_System_Close(m_pSoundSystem);
		FMOD_System_Release(m_pSoundSystem);
		m_pSoundSystem = nullptr;
	}

	__super::Free();
}
