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
			// 첫번째 아이콘을 즉각 만들어버림
			Start_QTE();
		}
	}
#pragma endregion

#pragma region 활성화
	if (m_bIsQTEActive)
	{
		//전체 타이머가 전부 시간 소요되거나
		//마지막 UI 객체 처리가 완료됬거나
		if (m_fTimer <= 0.0f || m_bUI_Final_Complate)
		{
			// QTE 종료
			End_QTE();
		}

		// 각 Hit_UI_Icon 업데이트
		for (auto& iter : m_vecHitUIIcon)
			iter->Update(fTimeDelta);

		// 다음 아이콘 생성할게 남아있는지 체크
		if (m_iNextIconIndex < m_vecIconCreationTimes.size())
		{
			//경과시간이 딜레이 시간보다 늘어났다면 아이콘 생성
			if (m_fElapsedTime >= m_vecIconCreationTimes[m_iNextIconIndex])
			{
				// 아이콘 생성
				Create_UIIcon();
				m_iNextIconIndex++;
			}
		}

		// 사용자 입력 처리
		Handle_QTEInput();

		// 경과 시간 업데이트
		m_fElapsedTime += fTimeDelta;

		// 타이머 업데이트
		m_fTimer -= fTimeDelta;
	}
#pragma endregion

}

void CQTE_Hit_Situation::Late_Update(_float fTimeDelta)
{
	//QTE가 활성화되었다면
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

	//활성화
	m_bIsQTEActive = true;
	//마지막 객체 완료 처리 여부 초기화
	m_bUI_Final_Complate = false;
	// 첫 번째 아이콘 즉시 생성
	Create_UIIcon();
}

void CQTE_Hit_Situation::End_QTE()
{
#pragma region 초기화
	//활성화 여부 초기화
	m_bIsQTEActive = false;
	//경과시간 초기화
	m_fElapsedTime = 0.0f;
	//다음 객체를 만드는데 필요한 변수 초기화
	m_iNextIconIndex = 0;
	//타이머는 원래 LifeTime으로 초기화
	m_fTimer = m_fLifeTime;
	//마지막 객체 완료 처리 여부 초기화
	m_bUI_Final_Complate = false;

	//모든 아이콘 싹다 삭제
	for (auto& iter : m_vecHitUIIcon)
		Safe_Release(iter);

	m_vecHitUIIcon.clear();
#pragma endregion

#pragma region 점수 산정
	_bool isSuccess = true;

	for (auto& iter : m_vecHitUIIcon)
	{
		//FAIL이거나 결정되지 않았거나(예외처리) 하나라도 있으면 성공 실패
		if (iter->m_currentResult_ID == CQTE_Hit_UI_Icon::RESULT_ID::HIT_RESULT_FAILED ||
			iter->m_currentResult_ID == CQTE_Hit_UI_Icon::RESULT_ID::HIT_RESULT_NOT_YET_DECIDED)
		{
			isSuccess = false;
			break;
		}
	}

	//이제 여기에 결과를 전달해줄 객체에 isSuccess 전달
#pragma endregion

}

void CQTE_Hit_Situation::Create_UIIcon()
{
	CQTE_Hit_UI_Icon::QTE_Hit_UI_ICON_DESC Desc{};
	Desc.fSizeX = { 100.f };
	Desc.fSizeY = { 100.f };

	// 위치 범위 설정
	// 이 범위 내에서 확률적으로 뜰 것
	_float minX = 600.f;
	_float maxX = 1300.f; // 화면 너비 - 아이콘 너비를 고려
	_float minY = 400.f;
	_float maxY = 680.f; // 화면 높이 - 아이콘 높이를 고려

#pragma region 겹치지 않는 위치를 찾기 위한 최대 시도 횟수

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
			_float existingSizeX = existingIcon->m_fSizeX * 2.f;
			_float existingSizeY = existingIcon->m_fSizeY * 2.f;

			// AABB 충돌 검사
			if (Desc.fX < existingX + existingSizeX &&
				Desc.fX + Desc.fSizeX * 2.f > existingX &&
				Desc.fY < existingY + existingSizeY &&
				Desc.fY + Desc.fSizeY * 2.f > existingY)
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

#pragma endregion


	// fTimer를 최소 및 최대 값 사이에서 랜덤하게 설정
	_float minTimer = 1.f; // 최소 시간
	_float maxTimer = 2.5f; // 최대 시간
	Desc.fTimer = minTimer + static_cast<_float>(rand()) / RAND_MAX * (maxTimer - minTimer);


#pragma region 키설정

	vector<CQTE_Hit_UI_Icon::KEY_ID> possibleKeys = {
		CQTE_Hit_UI_Icon::KEY_ID::HIT_KEY_LIGHT,
		CQTE_Hit_UI_Icon::KEY_ID::HIT_KEY_MEDIUM,
		CQTE_Hit_UI_Icon::KEY_ID::HIT_KEY_ULTIMATE,
		CQTE_Hit_UI_Icon::KEY_ID::HIT_KEY_HEAVY
	};


	// 랜덤하게 키 하나 생성
	for (auto& iter : m_vecHitUIIcon)
	{
		_bool isActive = iter->IsActive();
		if (!isActive)
			continue;

		CQTE_Hit_UI_Icon::KEY_ID key = iter->m_Key;
		\
			auto it = std::find(possibleKeys.begin(), possibleKeys.end(), key);
		if (it != possibleKeys.end())
			possibleKeys.erase(it);
	}

	// 가능한 키 목록이 비어있는지 확인
	if (possibleKeys.empty())
		return;

	// 남은 키 중에서 랜덤하게 선택
	_int randomIndex = rand() % possibleKeys.size();
	Desc.key = possibleKeys[randomIndex];
	Desc.iTextureNumber = (_int)possibleKeys[randomIndex];

#pragma endregion

	Desc.Hit_Situation = this;

	//마지막 객체 생성이라면
	if (m_vecHitUIIcon.size() == m_iCreate_Num - 1)
		Desc.bFinal = true;

	// 아이콘 생성
	CQTE_Hit_UI_Icon* ui_Icon = static_cast<CQTE_Hit_UI_Icon*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_QTE_Hit_UI_Icon"), &Desc));

	m_vecHitUIIcon.push_back(ui_Icon);
}


void CQTE_Hit_Situation::Handle_QTEInput()
{
	//해당 기술을 시전하는 캐릭터
	//이건 나중에 가져와야함

	if (m_iCharacterSide == 1)
	{
		if (m_pGameInstance->Key_Down(DIK_U))
		{
			Process_Command(CQTE_Hit_UI_Icon::HIT_KEY_LIGHT);
		}
		else if (m_pGameInstance->Key_Down(DIK_I))
		{
			Process_Command(CQTE_Hit_UI_Icon::HIT_KEY_MEDIUM);
		}
		else if (m_pGameInstance->Key_Down(DIK_J))
		{
			Process_Command(CQTE_Hit_UI_Icon::HIT_KEY_ULTIMATE);
		}
		else if (m_pGameInstance->Key_Down(DIK_K))
		{
			Process_Command(CQTE_Hit_UI_Icon::HIT_KEY_HEAVY);
		}
	}
	else if (m_iCharacterSide == 2)
	{
		if (m_pGameInstance->Key_Down(DIK_NUMPAD7))
		{
			Process_Command(CQTE_Hit_UI_Icon::HIT_KEY_LIGHT);
		}
		else if (m_pGameInstance->Key_Down(DIK_NUMPAD8))
		{
			Process_Command(CQTE_Hit_UI_Icon::HIT_KEY_MEDIUM);
		}
		else if (m_pGameInstance->Key_Down(DIK_NUMPAD4))
		{
			Process_Command(CQTE_Hit_UI_Icon::HIT_KEY_ULTIMATE);
		}
		else if (m_pGameInstance->Key_Down(DIK_NUMPAD5))
		{
			Process_Command(CQTE_Hit_UI_Icon::HIT_KEY_HEAVY);
		}
	}
}


void CQTE_Hit_Situation::Process_Command(CQTE_Hit_UI_Icon::KEY_ID input)
{
	//키를 입력할 때, 현재 살아있는 QTE에 한해 작동해야 한다
	for (auto& iter : m_vecHitUIIcon)
	{
		_bool isActive = iter->IsActive();
		if (!isActive)
			continue;

		//살아있는 객체의 키
		CQTE_Hit_UI_Icon::KEY_ID key = iter->m_Key;

		//for문을 돌리면서 Input과 안맞으면 패스
		if (input != key)
			continue;
		//Input을 보내서 결과 판단하라고 함
		else
			iter->Send_Input(input);
	}
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
