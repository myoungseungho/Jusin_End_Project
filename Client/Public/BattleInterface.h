#pragma once

#include "GameObject.h"
#include "Client_Defines.h"

#include "AnimationEvent_Defines.h"
#include "AnimationNameMap.h"


BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)


class CBattleInterface_Manager final : public CBase
{
	DECLARE_SINGLETON(CBattleInterface_Manager)
private:
	CBattleInterface_Manager();
	virtual ~CBattleInterface_Manager() = default;

public:
	void Gain_HitCount(_ushort iHitCount, _ushort iTeam);
	void Reset_HitCount(_ushort iTeam);
	_ushort Get_HitCount(_ushort iTeam);


	void Gain_HitAttackStep(_ushort iHitCount, _ushort iTeam);
	void Reset_HitAttackStep(_ushort iTeam);
	_ushort Get_HitAttackStep(_ushort iTeam);


	_bool Get_bSparkingEnable(_ushort iTeam);
	void Set_bSparkingEnable(_bool bSparkingEnable, _ushort iTeam);

	void Gain_KiGuage(_ushort iKi, _ushort iTeam);
	_bool Use_KiGuage(_ushort  irequirementKi, _ushort iTeam);
	_ushort Get_KiGuage(_ushort iTeam);

	_ushort Get_KiNumber(_ushort iTeam);

	void Stop_CharacterWithoutMe(_ushort iTeam, _ubyte iSlot, _float fTime);


	//이거 하나만 호출하게
	//	void Tag_CharacterAIO(_ubyte iTeam, _ubyte NewCharacterslot);
	_bool Tag_CharacterAIO(_ubyte iTeam, _ubyte NewCharacterslot, _vector vPos);

	void Regist_Character(_ubyte iTeam, class CCharacter* pCharacter, _ubyte iSlot);

	//캐릭터 변경시 호출해서 상대팀에게 enemy를 바꿔줘야됨
	class CCharacter* EnemyInitalize(_ubyte iTeam);

private:
	
public:
	

private:
	class CCharacter* m_p1TeamCharacter[3] = { nullptr };
	class CCharacter* m_p2TeamCharacter[3] = { nullptr };


	_bool m_bSparkingEnable[2] = { true };
	_uint m_iHitCount[2] = {}; //화면에 뜰 히트수
	
	_uint m_iKiGuage[2] = {};
	_uint m_iKiNumber[2] = {};

	_uint m_iHitAttackStep[2] = {};

	_ubyte m_i1TeamPlayingCharacterIndex = {};
	_ubyte m_i2TeamPlayingCharacterIndex = {};


private:
	CGameInstance* m_pGameInstance = { nullptr };

public:

	virtual void Free() override;
};

END