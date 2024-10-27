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
	_ushort Get_KiGuage(_ushort iKi, _ushort iTeam);




private:
	
public:
	

private:
	class CCharacter* m_p1TeamCharacter[3] = { nullptr };
	class CCharacter* m_p2TeamCharacter[3] = { nullptr };


	_bool m_bSparkingEnable[2] = { true };
	_uint m_iHitCount[2] = {};
	_uint m_iKiGuage[2] = {};
	_uint m_iHitAttackStep[2] = {};


private:
	CGameInstance* m_pGameInstance = { nullptr };

public:

	virtual void Free() override;
};

END