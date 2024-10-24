#pragma once

#include "Base.h"
#include "Client_Defines.h"
#include "input.h"
#include "Character.h"

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
	void UsingAttckBuff(_float fAttBufDuration, CCharacter::PLAYER_SLOT eSlotID);
	//void UsingSkillPoint(_int iSkillPoint) { m_iSkillPoint += iSkillPoint; }
	void UsingSelectCharacher(CCharacter* pPawn, CCharacter::PLAYER_SLOT eSlotID) { m_pPawnArray[eSlotID] = pPawn; }
	void UsingChangeCharacher(CCharacter::PLAYER_SLOT eCurrSlotID);
	void UsingCreateStartUI();
	void UsingCreateEndUI();

	//void SKillCount();
	//void Set_Hp(_uint iHp) { m_iHp = iHp; }


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
	//_int m_iSkillPoint = { 0 };
	//_int m_iSkillCount = { 0 };

	//Combo
	//_uint m_iComboCount = { 0 };

	class CCharacter* m_pPawnArray[CCharacter::SLOT_END] = {nullptr,nullptr ,nullptr ,nullptr };

	//Anim
	_float m_fTotalDuration = { 0.f };

	//Input
	DirectionInput m_eDirInput = {};
	ButtonInput m_eBtnInput = {};

	_uint m_iNumCommandList = { 0 };

	//RedBlueTimer
	_float m_fColorValue = { 0.f };

	//CUIObject::UI_LRPOS m_eTeam = { };
	 _uint m_iTeam = { 2 };

private:
	CGameInstance* m_pGameInstance = { nullptr };

public:
	virtual void Free() override;
};

END