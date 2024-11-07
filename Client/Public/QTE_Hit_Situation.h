#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "QTE_Hit.h"
BEGIN(Client)

class CQTE_Hit_Situation final : public CGameObject
{
public:
	struct QTE_HIT_SITUATION_DESC
	{
		_float lifeTime = {};
		_int create_Num = {};
		CQTE_Hit::Hit_Situation_ID ID = {};
	};

private:
	CQTE_Hit_Situation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CQTE_Hit_Situation(const CQTE_Hit_Situation& Prototype);
	virtual ~CQTE_Hit_Situation() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Camera_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	void Start_QTE();
	void End_QTE();
	void Handle_QTEInput();
	void Create_UIIcon(); // 아이콘 생성 함수 추가

private:
	_float m_fLifeTime = {};
	_int m_iCreate_Num = {};

	_bool m_bIsQTEActive = { false }; // QTE 활성화 여부
	_float m_fTimer = { 0.f };

	_float m_fElapsedTime = { 0.f }; // 경과 시간
	_int m_iNextIconIndex = { 0 }; // 다음 아이콘 인덱스

	vector<_float> m_vecIconCreationTimes;
	vector<class CQTE_Hit_UI_Icon*> m_vecHitUIIcon;

	CQTE_Hit::Hit_Situation_ID m_currentSituationID = {};

public:
	static CQTE_Hit_Situation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END