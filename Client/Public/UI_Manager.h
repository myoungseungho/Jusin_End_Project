#pragma once

#include "Base.h"
#include "Client_Defines.h"
#include "Pawn.h"

BEGIN(Engine)

class CGameInstance;

END

BEGIN(Client)

class CUI_Manager final : public CBase
{
	DECLARE_SINGLETON(CUI_Manager)

public:
	typedef struct
	{
		_bool		bAttBuf = { FALSE };
		_int		iSKillPoint = { 0 };
		_int		iSKillCount = { 0 };
	}TEAM_DESC;

private:
	CUI_Manager();
	virtual ~CUI_Manager() = default;

public:
	void UsingStunCheck(_bool bStun) { m_bStun = bStun; }
	void UsingAttckBuff(_float fAttBufDuration, CPawn::PLAYER_SLOT eSlotID);
	void UsingSkillPoint(_int iSkillPoint) { m_iSkillPoint += iSkillPoint; }
	void UsingComboCount(_uint iComboCnt);
	void UsingSelectCharacher(CPawn* pPawn, CPawn::PLAYER_SLOT eSlotID) { m_pPawnArray[eSlotID] = pPawn; }
	void UsingChangeCharacher(CPawn::PLAYER_SLOT eCurrSlotID);
	void UsingCreateStartUI();
	void UsingCreateEndUI();

	void SKillCount();

public:
	void UI_Setting_Debug(_float& fSizeX , _float& fSizeY , _float& fPosX , _float& fPosY , _float fSizeRadio);


public:
	_bool m_bStun = { FALSE };
	_bool m_bHit = { FALSE };
	_uint m_iHp = {0};

	TEAM_DESC	m_tTeamDesc[2] = {};
	
public:

	//UsingAttckBuff
	_float m_fDuration = { 0.f };	

	//Skill
	_int m_iSkillPoint = { 0 };
	_int m_iSkillCount = { 0 };

	//Combo
	_uint m_iComboCount = { 0 };

	CPawn* m_pPawnArray[CPawn::SLOT_END] = {};

	//Anim
	_float m_fTotalDuration = { 0.f };
private:
	CGameInstance* m_pGameInstance = { nullptr };

public:
	virtual void Free() override;
};

END