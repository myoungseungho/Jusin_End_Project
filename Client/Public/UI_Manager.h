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
	enum PLAYER_SLOT {LPLAYER1  ,LPLAYER2 , RPLAYER1, RPLAYER2 , SLOT_END};

private:
	CUI_Manager();
	virtual ~CUI_Manager() = default;

public:
	void UsingStunCheck(_bool bStun) { m_bStun = bStun; }
	void UsingAttckBuff(_float fAttBufDuration);
	void UsingSkillPoint(_int iSkillPoint) { m_iSkillPoint += iSkillPoint; }
	void UsingComboCount(_uint iComboCnt);

	void SKillCount();

public:
	void UI_Setting_Debug(_float& fSizeX , _float& fSizeY , _float& fPosX , _float& fPosY , _float fSizeRadio);
	


public:
	_bool m_bStun = { FALSE };
	_bool m_bHit = { FALSE };
	_uint m_iHp = {0};
	
public:
	//UsingAttckBuff
	_float m_fDuration = { 0.f };	

	//Skill
	_int m_iSkillPoint = { 0 };
	_int m_iSkillCount = { 0 };

	//Combo
	_uint m_iComboCount = { 0 };

	CPawn* m_tPawnDesc[SLOT_END] = {};
private:
	CGameInstance* m_pGameInstance = { nullptr };

public:
	virtual void Free() override;
};

END