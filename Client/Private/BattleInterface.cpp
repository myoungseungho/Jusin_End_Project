#include "stdafx.h"
#include "..\Public\BattleInterface.h"

#include "RenderInstance.h"
#include "GameInstance.h"


#include <fstream>  
#include <iostream> 
#include <vector>   
#include <string>   
#include <sstream>

#include "Animation.h"
#include "Character.h"

#include "Model_Preview.h"
#include "BattleInterface.h"

IMPLEMENT_SINGLETON(CBattleInterface_Manager)



CBattleInterface_Manager::CBattleInterface_Manager()
{
}




void CBattleInterface_Manager::Gain_HitCount(_ushort iHitCount, _ushort iTeam)
{
    m_iHitCount[iTeam-1] += iHitCount;
}

void CBattleInterface_Manager::Reset_HitCount(_ushort iTeam)
{
    m_iHitCount[iTeam-1] = 0;

}

_ushort CBattleInterface_Manager::Get_HitCount(_ushort iTeam)
{
    return  m_iHitCount[iTeam - 1];
}

_bool CBattleInterface_Manager::Get_bSparkingEnable(_ushort iTeam)
{
    return  m_bSparkingEnable[iTeam-1];
}

void CBattleInterface_Manager::Set_bSparkingEnable(_bool bSparkingEnable, _ushort iTeam)
{
    m_bSparkingEnable[iTeam - 1] = bSparkingEnable;
}

void CBattleInterface_Manager::Gain_KiGuage(_ushort iKi, _ushort iTeam)
{
    m_iKiGuage[iTeam - 1] += iKi;
}

_bool CBattleInterface_Manager::Use_KiGuage(_ushort irequirementKi, _ushort iTeam)
{
    if (m_iKiGuage[iTeam - 1] > irequirementKi)
    {
        m_iKiGuage[iTeam - 1] -= irequirementKi;
        return true;
    }

    return false;
}

_ushort CBattleInterface_Manager::Get_KiGuage(_ushort iKi, _ushort iTeam)
{
    return  m_iKiGuage[iTeam - 1];
}



void CBattleInterface_Manager::Gain_HitAttackStep(_ushort iHitCount, _ushort iTeam)
{
    m_iHitAttackStep[iTeam - 1] += iHitCount;
}

void CBattleInterface_Manager::Reset_HitAttackStep(_ushort iTeam)
{
    m_iHitAttackStep[iTeam - 1] = 0;
}

_ushort CBattleInterface_Manager::Get_HitAttackStep(_ushort iTeam)
{
    return m_iHitAttackStep[iTeam - 1];
}

void CBattleInterface_Manager::Free()
{
    __super::Free();
}

