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

void CSound_Manager::Register_Sound(const std::wstring& filePath, const std::wstring& alias)
{
	// 사운드가 이미 등록되어 있는지 확인
	if (m_soundMap.find(alias) != m_soundMap.end())
	{
		return; // 이미 존재하므로 등록하지 않고 성공으로 반환
	}

	FMOD_SOUND* sound;

	// wstring을 string으로 변환
	wstring_convert<codecvt_utf8<wchar_t>> converter;
	string filePathStr = converter.to_bytes(filePath);

	if (FMOD_System_CreateSound(m_pSoundSystem, filePathStr.c_str(), FMOD_DEFAULT, nullptr, &sound) != FMOD_OK)
	{
		return;
	}

	// 사운드 등록
	m_soundMap[alias] = sound;
}

void CSound_Manager::Register_Sound_Group(const wstring& groupKey, const std::wstring& filePath, const wstring& alias)
{
	// 그룹이 존재하지 않으면 생성
	if (m_soundGroupMap.find(groupKey) == m_soundGroupMap.end())
	{
		m_soundGroupMap[groupKey] = vector<wstring>();
	}

	// 사운드 등록
	Register_Sound(filePath, alias);

	// 그룹에 해당 alias를 추가
	m_soundGroupMap[groupKey].push_back(alias);
}

void CSound_Manager::Play_Sound(const wstring& alias, _bool loop, _float volume)
{
	auto it = m_soundMap.find(alias);
	if (it == m_soundMap.end()) return;

	FMOD_CHANNEL* channel = nullptr;

	FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	FMOD_Sound_SetMode(it->second, mode);

	// 현재 재생 중인 채널 수를 확인
	int playingChannels = 0;
	FMOD_ChannelGroup_GetNumChannels(m_pChannelGroup, &playingChannels);

	// 재생이 완료된 채널을 해제하는 코드 추가
	for (int i = 0; i < playingChannels; ++i)
	{
		FMOD_CHANNEL* tempChannel = nullptr;
		if (FMOD_ChannelGroup_GetChannel(m_pChannelGroup, i, &tempChannel) == FMOD_OK)
		{
			FMOD_BOOL isPlaying = false;
			if (FMOD_Channel_IsPlaying(tempChannel, &isPlaying) == FMOD_OK && !isPlaying)
			{
				FMOD_Channel_Stop(tempChannel);  // 재생이 완료된 채널을 해제
			}
		}
	}

	// 재생 중인 채널 수를 다시 확인
	FMOD_ChannelGroup_GetNumChannels(m_pChannelGroup, &playingChannels);

	if (playingChannels < MAX_CHANNELS) // MAX_CHANNELS는 사용자가 정의한 최대 채널 수
	{
		// 채널 그룹에 속한 새로운 채널을 생성하여 재생
		if (FMOD_System_PlaySound(m_pSoundSystem, it->second, m_pChannelGroup, false, &channel) == FMOD_OK)
		{
			// 볼륨을 설정
			FMOD_Channel_SetVolume(channel, volume);

			m_channelMap[alias] = channel;
		}
	}
	else
	{
		// 모든 채널이 사용 중인 경우, 재생을 실패하게 함
		// 또는 기존 채널을 중단하고 새로운 채널로 교체할 수도 있음
		// 예: FMOD_Channel_Stop(기존채널); FMOD_System_PlaySound(...);
		return;
	}
}

void CSound_Manager::Play_Sound_Group(const wstring& groupKey, _bool loop, _float volume)
{
	// 그룹이 존재하지 않으면 리턴
	if (m_soundGroupMap.find(groupKey) == m_soundGroupMap.end()) return;

	auto& soundList = m_soundGroupMap[groupKey];

	// 현재 그룹 내에서 재생 중인 음원이 있는지 확인
	for (const auto& soundAlias : soundList)
	{
		auto it = m_channelMap.find(soundAlias);
		if (it != m_channelMap.end())
		{
			FMOD_BOOL isPlaying = false;
			if (FMOD_Channel_IsPlaying(it->second, &isPlaying) == FMOD_OK && isPlaying)
			{
				return; // 재생 중인 음원이 있으면 리턴
			}
		}
	}

	// 직전에 재생된 음원을 제외한 리스트 생성
	vector<wstring> availableSounds;
	for (const auto& soundAlias : soundList)
	{
		if (soundAlias != m_lastPlayedSound[groupKey])
		{
			availableSounds.push_back(soundAlias);
		}
	}

	// 재생 가능한 음원이 없으면 모든 음원을 다시 고려
	if (availableSounds.empty())
	{
		availableSounds = soundList;
	}

	// 랜덤으로 하나의 음원을 선택
	int randomIndex = rand() % availableSounds.size();
	wstring selectedSound = availableSounds[randomIndex];

	// 선택한 음원을 재생
	Play_Sound(selectedSound, loop, volume);

	// 마지막에 재생한 음원으로 저장
	m_lastPlayedSound[groupKey] = selectedSound;
}

void CSound_Manager::Stop_Sound(const std::wstring& alias)
{
	auto it = m_channelMap.find(alias);
	if (it == m_channelMap.end()) return;

	if (FMOD_Channel_Stop(it->second) != FMOD_OK)
	{
		return;
	}
}

void CSound_Manager::Set_Volume(const std::wstring& alias, float volume)
{
	auto it = m_channelMap.find(alias);
	if (it == m_channelMap.end()) return;

	if (FMOD_Channel_SetVolume(it->second, volume) != FMOD_OK)
	{
		return;
	}
}

void CSound_Manager::Stop_All_Sounds()
{
	for (auto& pair : m_channelMap)
	{
		FMOD_Channel_Stop(pair.second);
	}
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
	for (auto& pair : m_soundMap)
	{
		FMOD_Sound_Release(pair.second);
	}
	m_soundMap.clear();
	m_channelMap.clear();

	if (m_pSoundSystem)
	{
		FMOD_System_Close(m_pSoundSystem);
		FMOD_System_Release(m_pSoundSystem);
		m_pSoundSystem = nullptr;
	}

	__super::Free();
}
