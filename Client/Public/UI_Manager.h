#pragma once

#include "Base.h"
#include "Client_Defines.h"
#include "Character.h"
#include "input.h"

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
	void Update(_float fTimeDelta);
	void UsingStunCheck(_bool bStun) { m_bStun = bStun; }
	void UsingAttckBuff(_float fAttBufDuration, CCharacter::PLAYER_SLOT eSlotID);
	void UsingSkillPoint(_int iSkillPoint) { m_iSkillPoint += iSkillPoint; }
	void UsingComboCount(_uint iComboCnt);
	void UsingSelectCharacher(CCharacter* pPawn, CCharacter::PLAYER_SLOT eSlotID) { m_pPawnArray[eSlotID] = pPawn; }
	void UsingChangeCharacher(CCharacter::PLAYER_SLOT eCurrSlotID);
	void UsingCreateStartUI();
	void UsingCreateEndUI();

	void SKillCount();
	void Set_Hp(_uint iHp) { m_iHp = iHp; }


public:
	void UI_Setting_Debug(_float& fSizeX , _float& fSizeY , _float& fPosX , _float& fPosY , _float fSizeRadio);
	void InitUIObject();

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

	//CCharacter* m_pPawnArray[CCharacter::CCharacter::PLAYER_SLOT::SLOT_END] = {nullptr,nullptr ,nullptr ,nullptr };
	CCharacter* m_pPawnArray[CCharacter::PLAYER_SLOT::SLOT_END] = { nullptr,nullptr ,nullptr ,nullptr };

	//Anim
	_float m_fTotalDuration = { 0.f };

	DirectionInput m_eDirInput = {};
	ButtonInput m_eBtnInput = {};

	_uint m_iNumCommandList = { 0 };

	//Rneder
	_bool m_isRender = { TRUE };

	//Input
	_bool m_isInput = { FALSE };

	//RedBlueTimer
	_float m_fColorValue = { 0.f };

	//CUIObject::UI_LRPOS m_eTeam = { };
	 _uint m_iTeam = { 2 };
	 _uint m_iAnimEndCnt = { 0 };
private:
	CGameInstance* m_pGameInstance = { nullptr };

public:
	virtual void Free() override;
};

END