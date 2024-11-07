#include "stdafx.h"
#include "..\Public\QTE_Hit_Situation.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "QTE_Hit_UI_Icon.h"
CQTE_Hit_Situation::CQTE_Hit_Situation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CQTE_Hit_Situation::CQTE_Hit_Situation(const CQTE_Hit_Situation& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CQTE_Hit_Situation::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CQTE_Hit_Situation::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//랜덤시드 발생
	srand(static_cast<unsigned>(std::time(0)));

	QTE_HIT_SITUATION_DESC* Desc = static_cast<QTE_HIT_SITUATION_DESC*>(pArg);
	//해당 상황의 전체 라이프타임
	m_fLifeTime = Desc->lifeTime;
	m_fTimer = m_fLifeTime;

	//해당 상황의 만들어낼 숫자
	m_iCreate_Num = Desc->create_Num;
	//해당 상황 ID
	m_currentSituationID = Desc->ID;

	//초기화
	m_fElapsedTime = 0.0f;
	m_iNextIconIndex = 0;

	switch (m_currentSituationID)
	{
	case Client::CQTE_Hit::Hit_Situation_ID_A:
		//3개라서 2개요소가 있음
		m_vecIconCreationTimes = { 1.f, 2.f };
		break;
	case Client::CQTE_Hit::Hit_Situation_ID_B:
		break;
	case Client::CQTE_Hit::Hit_Situation_ID_C:
		break;
	case Client::CQTE_Hit::Hit_Situation_ID_END:
		break;
	default:
		break;
	}

	return S_OK;
}

void CQTE_Hit_Situation::Camera_Update(_float fTimeDelta)
{

}

void CQTE_Hit_Situation::Update(_float fTimeDelta)
{
#pragma region 디버그
	// F5 키 입력 감지
	if (m_pGameInstance->Key_Down(DIK_F5))
	{
		if (m_bIsQTEActive)
		{
			// QTE가 활성화되어 있으면 즉시 종료
			End_QTE();
		}
		else
		{
			// QTE가 비활성화되어 있으면 시작
			Start_QTE();
		}
	}
#pragma endregion

#pragma region 활성화

	if (m_bIsQTEActive)
	{
		// 경과 시간 업데이트
		m_fElapsedTime += fTimeDelta;

		// 다음 아이콘 생성 시간인지 확인
		if (m_iNextIconIndex < m_vecIconCreationTimes.size())
		{
			if (m_fElapsedTime >= m_vecIconCreationTimes[m_iNextIconIndex])
			{
				// 아이콘 생성
				Create_UIIcon();
				m_iNextIconIndex++;
			}
		}

		// 각 Hit_UI_Icon 업데이트
		for (auto& iter : m_vecHitUIIcon)
			iter->Update(fTimeDelta);

		// 사용자 입력 처리
		Handle_QTEInput();

		// 타이머 업데이트
		m_fTimer -= fTimeDelta;

		if (m_fTimer <= 0.0f)
		{
			// QTE 종료
			End_QTE();
		}
	}
#pragma endregion

}

void CQTE_Hit_Situation::Late_Update(_float fTimeDelta)
{
	if (m_bIsQTEActive)
	{
		for (auto& iter : m_vecHitUIIcon)
			iter->Late_Update(fTimeDelta);
	}
}

HRESULT CQTE_Hit_Situation::Render(_float fTimeDelta)
{
	return S_OK;
}

void CQTE_Hit_Situation::Start_QTE()
{
	if (m_bIsQTEActive)
		return; // 이미 QTE가 활성화되어 있으면 무시

	m_bIsQTEActive = true;

	// 첫 번째 아이콘 즉시 생성
	Create_UIIcon();
	m_iNextIconIndex++;
}

void CQTE_Hit_Situation::End_QTE()
{
	m_bIsQTEActive = false;
	m_fElapsedTime = 0.0f;
	m_iNextIconIndex = 0;
	m_fTimer = 0.f;

	for (auto& iter : m_vecHitUIIcon)
		Safe_Release(iter);

	m_vecHitUIIcon.clear();
}

void CQTE_Hit_Situation::Handle_QTEInput()
{
}

void CQTE_Hit_Situation::Create_UIIcon()
{
	CQTE_Hit_UI_Icon::QTE_Hit_UI_ICON_DESC Desc{};
	Desc.fSizeX = { 100.f };
	Desc.fSizeY = { 100.f };
	Desc.iTextureNumber = { 0 };

	// 위치 범위 설정
	_float minX = 100.f;
	_float maxX = 1920.f - Desc.fSizeX; // 화면 너비 - 아이콘 너비를 고려
	_float minY = 100.f;
	_float maxY = 1080.f - Desc.fSizeY; // 화면 높이 - 아이콘 높이를 고려

	// 겹치지 않는 위치를 찾기 위한 최대 시도 횟수
	const _int maxAttempts = 100;
	_int attempts = 0;
	_bool positionFound = false;

	while (!positionFound && attempts < maxAttempts)
	{
		// 랜덤 위치 생성
		Desc.fX = minX + static_cast<_float>(rand()) / RAND_MAX * (maxX - minX);
		Desc.fY = minY + static_cast<_float>(rand()) / RAND_MAX * (maxY - minY);

		// 아이콘이 겹치는지 검사
		positionFound = true;
		for (auto& existingIcon : m_vecHitUIIcon)
		{
			// 기존 아이콘의 위치와 크기 가져오기
			_float existingX = existingIcon->m_fX;
			_float existingY = existingIcon->m_fY;
			_float existingSizeX = existingIcon->m_fSizeX;
			_float existingSizeY = existingIcon->m_fSizeY;

			// AABB 충돌 검사
			if (Desc.fX < existingX + existingSizeX &&
				Desc.fX + Desc.fSizeX > existingX &&
				Desc.fY < existingY + existingSizeY &&
				Desc.fY + Desc.fSizeY > existingY)
			{
				// 겹침 발생
				positionFound = false;
				break;
			}
		}
		attempts++;
	}

	if (!positionFound)
	{
		// 위치를 찾지 못한 경우 기본 위치 설정 또는 처리
		Desc.fX = 960.f - Desc.fSizeX / 2; // 화면 중앙 등
		Desc.fY = 540.f - Desc.fSizeY / 2;
	}

	// fTimer를 최소 및 최대 값 사이에서 랜덤하게 설정
	_float minTimer = 0.5f; // 최소 시간
	_float maxTimer = 2.0f; // 최대 시간
	Desc.fTimer = minTimer + static_cast<_float>(rand()) / RAND_MAX * (maxTimer - minTimer);

	// 랜덤하게 키 하나 생성
	Desc.key = static_cast<CQTE_Hit_UI_Icon::KEY_ID>(rand() % 4);

	// 아이콘 생성
	CQTE_Hit_UI_Icon* ui_Icon = static_cast<CQTE_Hit_UI_Icon*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_QTE_Hit_UI_Icon"), &Desc));

	m_vecHitUIIcon.push_back(ui_Icon);
}

CQTE_Hit_Situation* CQTE_Hit_Situation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CQTE_Hit_Situation* pInstance = new CQTE_Hit_Situation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CQTE_Hit_Situation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CQTE_Hit_Situation::Clone(void* pArg)
{
	CQTE_Hit_Situation* pInstance = new CQTE_Hit_Situation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CQTE_Hit_Situation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQTE_Hit_Situation::Free()
{
	for (auto& iter : m_vecHitUIIcon)
		Safe_Release(iter);

	m_vecHitUIIcon.clear();

	__super::Free();
}
