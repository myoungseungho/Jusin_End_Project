#pragma once

#include "Base.h"
#include "UI_Define.h"
#include "Client_Defines.h"
#include "input.h"

BEGIN(Engine)

class CGameInstance;

END

BEGIN(Client)


class CUI_Manager final : public CBase
{
public:

	DECLARE_SINGLETON(CUI_Manager)

private:
	CUI_Manager();
	virtual ~CUI_Manager() = default;

	//구조
public:
	void Add_UIOjbect(CUI_Define::UI_LISTPOS ePos ,class CUIObject* pObject);

	//기능
public:
	void GamePlayUpdate(_float fTimeDelta);
	void UsingAttckBuff(_float fAttBufDuration, CUI_Define::PLAYER_SLOT eSlotID);
	void UsingSelectCharacher(class CCharacter* pPawn, CUI_Define::PLAYER_SLOT eSlotID) { m_pPawnArray[eSlotID] = pPawn; }
	void UsingChangeCharacher(CUI_Define::PLAYER_SLOT eCurrSlotID);
	void UsingCreateStartUI();
	void UsingCreateEndUI();

public:
	//디버깅 위치 설정
	void UI_Setting_Debug(_float& fSizeX , _float& fSizeY , _float& fPosX , _float& fPosY , _float fSizeRadio);
	//값 초기화
	void InitUIObject();

public:
	_bool m_bStun = { FALSE };
	_bool m_bHit = { FALSE };
	_uint m_iHp = {0};
	
public:

	//UsingAttckBuff
	_float m_fDuration = { 0.f };	


	class CCharacter* m_pPawnArray[CUI_Define::SLOT_END] = {nullptr,nullptr ,nullptr ,nullptr };

	//Anim
	_float m_fTotalDuration = { 0.f };

	//Input
	DirectionInput m_eDirInput = {};
	ButtonInput m_eBtnInput = {};

	_uint m_iNumCommandList = { 0 };

	//RedBlueTimer
	_float m_fColorValue = { 0.f };
	 _uint m_iTeam = { 2 };



public:
	list<class CUIObject*> m_ListTopUI = {};
	list<class CUIObject*> m_ListMidUI = {};
	list<class CUIObject*> m_ListBotUI = {};


private:
	CGameInstance* m_pGameInstance = { nullptr };

public:
	virtual void Free() override;
};

END