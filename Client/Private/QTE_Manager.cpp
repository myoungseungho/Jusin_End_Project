#include "stdafx.h"
#include "QTE_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CQTE_Manager)

CQTE_Manager::CQTE_Manager()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CQTE_Manager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    srand(static_cast<_uint>(time(nullptr))); // 랜덤 시드 초기화
	return S_OK;
}

void CQTE_Manager::Camera_Update(_float fTimeDelta)
{
}


void CQTE_Manager::Update(_float fTimeDelta)
{
    // F5 키 입력 감지
    if (m_pGameInstance->Key_Down(DIK_F5))
    {
        StartQTE();
    }

    //QTE가 시작되면 그제서야 타이머가 돌아감
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
    }
}

void CQTE_Manager::Late_Update(_float fTimeDelta)
{

}

HRESULT CQTE_Manager::Render(_float fTimeDelta)
{
	return S_OK;
}

void CQTE_Manager::StartQTE()
{
    if (m_bIsQTEActive)
        return; // 이미 QTE가 활성화되어 있으면 무시

    m_bIsQTEActive = true;
    m_fTimer = static_cast<float>(m_iTotalTime);

    // 1P 초기화
    m_CommandQueue_P1 = std::queue<UI_COMMAND>();
    m_CurrentSequence_P1.clear();
    m_iCorrectInputs_P1 = 0;

    for (int i = 0; i < m_iSequenceLength; ++i)
    {
        UI_COMMAND cmd = static_cast<UI_COMMAND>(rand() % UI_COMMAND_END);
        m_CommandQueue_P1.push(cmd);
        m_CurrentSequence_P1.push_back(cmd);
    }

    // 2P 초기화
    m_CommandQueue_P2 = std::queue<UI_COMMAND>();
    m_CurrentSequence_P2.clear();
    m_iCorrectInputs_P2 = 0;

    for (int i = 0; i < m_iSequenceLength; ++i)
    {
        UI_COMMAND cmd = static_cast<UI_COMMAND>(rand() % UI_COMMAND_END);
        m_CommandQueue_P2.push(cmd);
        m_CurrentSequence_P2.push_back(cmd);
    }
}

void CQTE_Manager::EndQTE()
{
    m_bIsQTEActive = false;
    m_fTimer = 0.0f;

    // 큐와 시퀀스 초기화
    m_CommandQueue_P1 = std::queue<UI_COMMAND>();
    m_CurrentSequence_P1.clear();
    m_iCorrectInputs_P1 = 0;

    m_CommandQueue_P2 = std::queue<UI_COMMAND>();
    m_CurrentSequence_P2.clear();
    m_iCorrectInputs_P2 = 0;

    // QTE 종료 후 처리 로직 추가 (우승자 결정)
    if (m_iCorrectInputs_P1 > m_iCorrectInputs_P2)
    {
        // 1P 승리 처리
        // 예: DisplayWinner(1);
    }
    else if (m_iCorrectInputs_P2 > m_iCorrectInputs_P1)
    {
        // 2P 승리 처리
        // 예: DisplayWinner(2);
    }
    else
    {
        // 무승부 처리
        // 예: DisplayDraw();
    }

}

void CQTE_Manager::HandleQTEInput()
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

void CQTE_Manager::ProcessCommand(UI_COMMAND input, _int playerID)
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
            // 1P의 올바른 입력 처리 (예: 피드백 제공)
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
            // 2P의 올바른 입력 처리 (예: 피드백 제공)
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

void CQTE_Manager::Free()
{
	Safe_Release(m_pGameInstance);

	__super::Free();
}
