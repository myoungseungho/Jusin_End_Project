#include "stdafx.h"
#include "SubTitle_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CSubTitle_Manager)

CSubTitle_Manager::CSubTitle_Manager()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CSubTitle_Manager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	// 자막 텍스트 초기화
	m_mapSubTitleText[SUBTITLE_HELLO] = L"안녕하세요";
	m_mapSubTitleText[SUBTITLE_WELCOME] = L"환영합니다";
	// 추가 자막 설정

	return S_OK;
}

void CSubTitle_Manager::Camera_Update(_float fTimeDelta)
{
}


void CSubTitle_Manager::Update(_float fTimeDelta)
{
	if (m_bIsDisplaying)
	{
		m_CurrentSubTitle.fRemainingTime -= fTimeDelta;
		if (m_CurrentSubTitle.fRemainingTime <= 0.f)
		{
			m_bIsDisplaying = false;
		}
	}
}

void CSubTitle_Manager::Late_Update(_float fTimeDelta)
{
}

HRESULT CSubTitle_Manager::Render(_float fTimeDelta)
{
	if (m_bIsDisplaying)
	{
		// 현재 표시 중인 자막 텍스트
		std::wstring wsSubTitle = m_mapSubTitleText[m_CurrentSubTitle.eID];

		// 기본 자막 위치 (가로: 중앙, 세로: 하단에서 약간 위)
		_float2 basePos(1920.f * 0.5f, 1080.f * 0.85f); // 중심과 화면 하단 기준
		_vector textColor = XMVectorSet(1.f, 1.f, 1.f, 1.f); // 흰색

		// 폰트 크기
		_float adjustedFontSize = 30.f; // 폰트 크기

		// 텍스트 길이 계산 (폰트 객체를 통해 길이를 얻는다고 가정)
		_float textWidth = wsSubTitle.length();

		// 중심 정렬: 텍스트의 절반 길이만큼 왼쪽으로 이동
		_float2 adjustedPos = basePos;
		adjustedPos.x -= textWidth * 0.5f;

		// 자막 렌더링
		m_pGameInstance->Draw_Font(TEXT("Font_Nexon"), wsSubTitle.c_str(), adjustedPos, textColor, 0.f, { 0.f, 0.f }, adjustedFontSize);
	}

	return S_OK;
}

HRESULT CSubTitle_Manager::Play(SUBTITLE_ID eID, _float duration)
{
	// 현재 자막이 표시되고 있다면 멈춥니다.
	if (m_bIsDisplaying)
	{
		Stop(m_CurrentSubTitle.eID);
	}

	// 자막 ID가 유효한지 확인
	if (m_mapSubTitleText.find(eID) == m_mapSubTitleText.end())
		return E_FAIL;

	m_CurrentSubTitle.eID = eID;
	m_CurrentSubTitle.fRemainingTime = duration;
	m_bIsDisplaying = true;

	return S_OK;
}

void CSubTitle_Manager::Stop(SUBTITLE_ID eID)
{
	// 현재 표시 중인 자막과 동일한지 확인
	if (m_bIsDisplaying && m_CurrentSubTitle.eID == eID)
	{
		m_bIsDisplaying = false;
		m_CurrentSubTitle.fRemainingTime = 0.f;
	}
}



void CSubTitle_Manager::Free()
{
	Safe_Release(m_pGameInstance);

	__super::Free();
}
