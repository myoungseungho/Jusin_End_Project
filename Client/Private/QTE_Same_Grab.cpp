#include "stdafx.h"
#include "..\Public\QTE_Same_Grab.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "QTE_UI_Icon.h"

CQTE_Same_Grab::CQTE_Same_Grab(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject { pDevice, pContext }
{

}

CQTE_Same_Grab::CQTE_Same_Grab(const CQTE_Same_Grab & Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CQTE_Same_Grab::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CQTE_Same_Grab::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	srand(static_cast<_uint>(time(nullptr))); // 랜덤 시드 초기화

	return S_OK;
}

void CQTE_Same_Grab::Camera_Update(_float fTimeDelta)
{
	
}

void CQTE_Same_Grab::Update(_float fTimeDelta)
{
	// F5 키 입력 감지
	if (m_pGameInstance->Key_Down(DIK_F5))
	{
		if (m_bIsQTEActive)
		{
			// QTE가 활성화되어 있으면 즉시 종료
			EndQTE();
		}
		else
		{
			// QTE가 비활성화되어 있으면 시작
			StartQTE();
		}
	}

	// QTE가 활성화된 경우 타이머 감소 및 입력 처리
	if (m_bIsQTEActive)
	{
		// 타이머 감소
		m_fTimer -= fTimeDelta;
		if (m_fTimer <= 0.0f)
		{
			EndQTE();
		}

		// 사용자 입력 처리
		HandleQTEInput();

		// UI 아이콘 업데이트
		for (auto& iter : m_UIIcons_P1)
			iter->Update(fTimeDelta);
		for (auto& iter : m_UIIcons_P2)
			iter->Update(fTimeDelta);
	}
}

void CQTE_Same_Grab::Late_Update(_float fTimeDelta)
{
	if (m_bIsQTEActive)
	{
		for (auto& iter : m_UIIcons_P1)
			iter->Late_Update(fTimeDelta);
		for (auto& iter : m_UIIcons_P2)
			iter->Late_Update(fTimeDelta);
	}
}


void CQTE_Same_Grab::StartQTE()
{
	if (m_bIsQTEActive)
		return; // 이미 QTE가 활성화되어 있으면 무시

	m_bIsQTEActive = true;
	m_fTimer = static_cast<_float>(m_iTotalTime);

	// 1P 초기화
	// Queue 다 뽑아내기
	while (!m_CommandQueue_P1.empty()) m_CommandQueue_P1.pop();
	// vector 다 뽑아내기
	m_CurrentSequence_P1.clear();
	// 1P가 얼마나 맞췄는지 Input 초기화
	m_iCorrectInputs_P1 = 0;

	vector<UI_COMMAND> sequence_P1;
	for (_int i = 0; i < m_iSequenceLength; ++i)
	{
		UI_COMMAND cmd = static_cast<UI_COMMAND>(rand() % UI_COMMAND_END);
		m_CommandQueue_P1.push(cmd);
		m_CurrentSequence_P1.push_back(cmd);
		sequence_P1.push_back(cmd);
	}

	// 2P 초기화
	while (!m_CommandQueue_P2.empty()) m_CommandQueue_P2.pop();
	m_CurrentSequence_P2.clear();
	m_iCorrectInputs_P2 = 0;

	std::vector<UI_COMMAND> sequence_P2;
	for (_int i = 0; i < m_iSequenceLength; ++i)
	{
		UI_COMMAND cmd = static_cast<UI_COMMAND>(rand() % UI_COMMAND_END);
		m_CommandQueue_P2.push(cmd);
		m_CurrentSequence_P2.push_back(cmd);
		sequence_P2.push_back(cmd);
	}

	// UI 아이콘 생성
	CreateUIIcons(1, sequence_P1);
	CreateUIIcons(2, sequence_P2);

	// 첫 번째 아이콘 선택 상태로 설정
	if (!m_UIIcons_P1.empty())
		m_UIIcons_P1[0]->Set_State(CQTE_UI_Icon::SELECTED);

	if (!m_UIIcons_P2.empty())
		m_UIIcons_P2[0]->Set_State(CQTE_UI_Icon::SELECTED);
}

void CQTE_Same_Grab::EndQTE()
{
	m_bIsQTEActive = false;
	m_fTimer = 0.0f;

	// 큐와 시퀀스 초기화
	while (!m_CommandQueue_P1.empty()) m_CommandQueue_P1.pop();
	m_CurrentSequence_P1.clear();
	m_iCorrectInputs_P1 = 0;
	m_CurrentIndex_P1 = 0;

	while (!m_CommandQueue_P2.empty()) m_CommandQueue_P2.pop();
	m_CurrentSequence_P2.clear();
	m_iCorrectInputs_P2 = 0;
	m_CurrentIndex_P2 = 0;

	// UI 아이콘 제거
	ClearUIIcons();

	// QTE 종료 후 처리 로직 추가 (우승자 결정)
	if (m_iCorrectInputs_P1 > m_iCorrectInputs_P2)
	{
		// 1P 승리 처리
	}
	else if (m_iCorrectInputs_P2 > m_iCorrectInputs_P1)
	{
		// 2P 승리 처리
	}
	else
	{
		// 무승부 처리
	}
}

void CQTE_Same_Grab::HandleQTEInput()
{
	// 1P 입력 처리 (예: A, S, D, F 키)
	if (m_pGameInstance->Key_Down(DIK_U))
	{
		ProcessCommand(UI_COMMAND_LIGHT, 1);
	}
	else if (m_pGameInstance->Key_Down(DIK_I))
	{
		ProcessCommand(UI_COMMAND_MIDDLE, 1);
	}
	else if (m_pGameInstance->Key_Down(DIK_J))
	{
		ProcessCommand(UI_COMMAND_ULTIMATE, 1);
	}
	else if (m_pGameInstance->Key_Down(DIK_K))
	{
		ProcessCommand(UI_COMMAND_HEAVY, 1);
	}

	// 2P 입력 처리 (예: J, K, L, ; 키)
	if (m_pGameInstance->Key_Down(DIK_NUMPAD7))
	{
		ProcessCommand(UI_COMMAND_LIGHT, 2);
	}
	else if (m_pGameInstance->Key_Down(DIK_NUMPAD8))
	{
		ProcessCommand(UI_COMMAND_MIDDLE, 2);
	}
	else if (m_pGameInstance->Key_Down(DIK_NUMPAD4))
	{
		ProcessCommand(UI_COMMAND_ULTIMATE, 2);
	}
	else if (m_pGameInstance->Key_Down(DIK_NUMPAD5))
	{
		ProcessCommand(UI_COMMAND_HEAVY, 2);
	}
}

void CQTE_Same_Grab::ProcessCommand(UI_COMMAND input, _int playerID)
{
	if (playerID == 1)
	{
		if (m_CommandQueue_P1.empty())
			return; // 모든 명령을 이미 처리한 경우

		UI_COMMAND expected = m_CommandQueue_P1.front();
		if (input == expected)
		{
			m_CommandQueue_P1.pop();
			m_iCorrectInputs_P1++;

			// 현재 선택된 아이콘의 선택 상태 해제
			if (m_CurrentIndex_P1 < m_UIIcons_P1.size())
			{
				m_UIIcons_P1[m_CurrentIndex_P1]->Set_State(CQTE_UI_Icon::ALREADY_PRESSED);
				m_CurrentIndex_P1++;
			}

			// 다음 아이콘을 선택 상태로 설정
			if (m_CurrentIndex_P1 < m_UIIcons_P1.size())
			{
				m_UIIcons_P1[m_CurrentIndex_P1]->Set_State(CQTE_UI_Icon::SELECTED);
			}

			//// 모든 명령을 완료한 경우 QTE 종료 (선택 사항)
			//if (m_CommandQueue_P1.empty())
			//{
			//	EndQTE();
			//}
		}
		else
		{
			// 잘못된 입력 시 QTE 실패 처리
			return;
		}
	}
	else if (playerID == 2)
	{
		if (m_CommandQueue_P2.empty())
			return; // 모든 명령을 이미 처리한 경우

		UI_COMMAND expected = m_CommandQueue_P2.front();
		if (input == expected)
		{
			m_CommandQueue_P2.pop();
			m_iCorrectInputs_P2++;

			// 현재 선택된 아이콘의 선택 상태 해제
			if (m_CurrentIndex_P2 < m_UIIcons_P2.size())
			{
				m_UIIcons_P2[m_CurrentIndex_P2]->Set_State(CQTE_UI_Icon::ALREADY_PRESSED);
				m_CurrentIndex_P2++;
			}

			// 다음 아이콘을 선택 상태로 설정
			if (m_CurrentIndex_P2 < m_UIIcons_P2.size())
			{
				m_UIIcons_P2[m_CurrentIndex_P2]->Set_State(CQTE_UI_Icon::SELECTED);
			}

			//// 모든 명령을 완료한 경우 QTE 종료 (선택 사항)
			//if (m_CommandQueue_P2.empty())
			//{
			//	EndQTE();
			//}
		}
		else
		{
			// 잘못된 입력 시 QTE 실패 처리
			return;
		}
	}

	// 타이머가 아직 남아있는 경우, 큐가 모두 비었는지 확인
	if (playerID == 1 && m_CommandQueue_P1.empty())
	{
		// 1P가 모든 명령을 완료했을 때 (선택 사항)
	}
	if (playerID == 2 && m_CommandQueue_P2.empty())
	{
		// 2P가 모든 명령을 완료했을 때 (선택 사항)
	}
}

void CQTE_Same_Grab::CreateUIIcons(_int playerID, const vector<UI_COMMAND>& sequence)
{
	// 각 플레이어의 UI 아이콘 벡터에 추가
	vector<CQTE_UI_Icon*>& targetIcons = (playerID == 1) ? m_UIIcons_P1 : m_UIIcons_P2;

	// 플레이어별 중앙 x 위치 설정
	_float centerX = (playerID == 1) ? 480.f : 1440.f;

	// y 위치 고정
	_float centerY = 300.f;

	// 아이콘 간격 설정 (필요에 따라 조정)
	_float spacing = 50.f;

	// 아이콘 수
	_int numIcons = static_cast<_int>(sequence.size());

	// 아이콘의 x 위치를 저장할 벡터
	vector<_float> iconPositionsX;

	// 홀수 개의 아이콘 배치
	if (numIcons % 2 == 1)
	{
		_int mid = numIcons / 2;
		for (_int i = 0; i < numIcons; ++i)
		{
			_float offset = spacing * (i - mid);
			iconPositionsX.push_back(centerX + offset);
		}
	}
	// 짝수 개의 아이콘 배치
	else
	{
		_int mid = numIcons / 2;
		for (_int i = 0; i < numIcons; ++i)
		{
			// 짝수일 경우 중앙 두 아이콘을 기준으로 좌우 배치
			_float offset = spacing * (i - mid + 0.5f);
			iconPositionsX.push_back(centerX + offset);
		}
	}

	// 각 아이콘을 생성하고 위치 설정
	for (_int i = 0; i < numIcons; ++i)
	{
		CQTE_UI_Icon::QTE_UI_ICON_DESC Desc{};
		Desc.iTextureNumber = static_cast<_int>(sequence[i]);
		Desc.fSizeX = 50.f; // 필요에 따라 크기 조정
		Desc.fSizeY = 50.f;
		Desc.fX = iconPositionsX[i];
		Desc.fY = centerY;
		Desc.fAlpha = 1.f;

		// UI 아이콘 클론
		CQTE_UI_Icon* pIcon = dynamic_cast<CQTE_UI_Icon*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_QTE_UI_Icon"), &Desc));
		if (pIcon)
		{
			// 벡터에 저장
			targetIcons.push_back(pIcon);
		}
	}
}

void CQTE_Same_Grab::ClearUIIcons()
{
	// 1P UI 아이콘 제거
	for (auto& icon : m_UIIcons_P1)
	{
		if (icon)
		{
			Safe_Release(icon);
		}
	}
	m_UIIcons_P1.clear();

	// 2P UI 아이콘 제거
	for (auto& icon : m_UIIcons_P2)
	{
		if (icon)
		{
			Safe_Release(icon);
		}
	}
	m_UIIcons_P2.clear();
}


HRESULT CQTE_Same_Grab::Render(_float fTimeDelta)
{

	return S_OK;
}

CQTE_Same_Grab * CQTE_Same_Grab::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CQTE_Same_Grab*		pInstance = new CQTE_Same_Grab(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CQTE_Same_Grab"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CQTE_Same_Grab::Clone(void * pArg)
{
	CQTE_Same_Grab*		pInstance = new CQTE_Same_Grab(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CQTE_Same_Grab"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQTE_Same_Grab::Free()
{
	for (auto& iter : m_UIIcons_P1)
		Safe_Release(iter);

	for (auto& iter : m_UIIcons_P2)
		Safe_Release(iter);

	__super::Free();
}
