#pragma once

#include "Base.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CQTE_Manager : public CBase
{
	DECLARE_SINGLETON(CQTE_Manager)

public:
	enum UI_COMMAND
	{
		UI_COMMAND_LIGHT, //약공
		UI_COMMAND_MIDDLE, //중공
		UI_COMMAND_ULTIMATE, //특수공격
		UI_COMMAND_HEAVY, //강공
		UI_COMMAND_END
	};

private:
	CQTE_Manager();
	virtual ~CQTE_Manager() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Camera_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

	CGameInstance* m_pGameInstance = { nullptr };

private:
	void StartQTE();
	void EndQTE();
	void HandleQTEInput();
	void ProcessCommand(UI_COMMAND input, _int playerID);

private:
	_bool m_bIsQTEActive = { false }; // QTE 활성화 여부
	_float m_fTimer = { 0.f }; // 타이머
	_int m_iTotalTime = { 10 }; // 총 시간 (예: 5초)
	_int m_iSequenceLength = { 10 }; // 시퀀스 길이 (N)

	// 1P 관련
	queue<UI_COMMAND> m_CommandQueue_P1;
	vector<UI_COMMAND> m_CurrentSequence_P1;
	_int m_iCorrectInputs_P1;

	// 2P 관련
	queue<UI_COMMAND> m_CommandQueue_P2;
	vector<UI_COMMAND> m_CurrentSequence_P2;
	_int m_iCorrectInputs_P2;

public:
	virtual void Free() override;
};

END