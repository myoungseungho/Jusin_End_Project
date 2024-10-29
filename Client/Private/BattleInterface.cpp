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


#include "Main_Camera.h"


IMPLEMENT_SINGLETON(CBattleInterface_Manager)




CBattleInterface_Manager::CBattleInterface_Manager()
    :m_pGameInstance{ CGameInstance::Get_Instance() }
{
    Safe_AddRef(m_pGameInstance);
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

    //디버그용 코드 반드시 성공
    if (m_pGameInstance->Key_Pressing(DIK_INSERT))
    {
        return true;
    }

    if (m_iKiGuage[iTeam - 1] > irequirementKi)
    {
        m_iKiGuage[iTeam - 1] -= irequirementKi;
        return true;
    }

    return false;
}

_ushort CBattleInterface_Manager::Get_KiGuage(_ushort iTeam)
{
    return  m_iKiGuage[iTeam - 1];
}

/*
void CBattleInterface_Manager::Tag_CharacterAIO(_ubyte iTeam, _ubyte NewCharacterslot)
{

    if (iTeam == 1)
    {
        for (auto pCharcter : m_p2TeamCharacter)
        {
            pCharcter->RegisterEnemy(m_p1TeamCharacter[NewCharacterslot]);
        }

        m_p1TeamCharacter[NewCharacterslot]->Tag_Out()
    }
    else if (iTeam == 2)
    {
        for (auto pCharcter : m_p1TeamCharacter)
        {
            pCharcter->RegisterEnemy(m_p2TeamCharacter[NewCharacterslot]);
        }
    }



}
*/

void CBattleInterface_Manager::Tag_CharacterAIO(_ubyte iTeam, _ubyte NewCharacterslot, _vector vPos)
{


    CMain_Camera* pMainCamera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera")));


    if (iTeam == 1)
    {
        for (auto pCharcter : m_p2TeamCharacter)
        {
            if(pCharcter != nullptr)
                pCharcter->RegisterEnemy(m_p1TeamCharacter[NewCharacterslot]);
        }
        m_i1TeamPlayingCharacterIndex = NewCharacterslot;
        m_p1TeamCharacter[NewCharacterslot]->Tag_Out(vPos);
        pMainCamera->SetPlayer(CMain_Camera::PLAYER_1P, m_p1TeamCharacter[m_i1TeamPlayingCharacterIndex]);
    }
    else if (iTeam == 2)
    {
        for (auto pCharcter : m_p1TeamCharacter)
        {
            if (pCharcter != nullptr)
                pCharcter->RegisterEnemy(m_p2TeamCharacter[NewCharacterslot]);
        }
        m_i2TeamPlayingCharacterIndex = NewCharacterslot;
        m_p2TeamCharacter[NewCharacterslot]->Tag_Out(vPos);
        pMainCamera->SetPlayer(CMain_Camera::PLAYER_2P, m_p2TeamCharacter[m_i2TeamPlayingCharacterIndex]);
    }




}

void CBattleInterface_Manager::Regist_Character(_ubyte iTeam, class CCharacter* pCharacter, _ubyte iSlot)
{
    if (iTeam == 1)
    {
        m_p1TeamCharacter[iSlot] = pCharacter;
    }
    else if (iTeam == 2)
    {
        m_p2TeamCharacter[iSlot-2] = pCharacter;
    }
}

CCharacter* CBattleInterface_Manager::EnemyInitalize(_ubyte iTeam)
{
    if (iTeam == 1)
    {
        return m_p2TeamCharacter[0];
    }
    else if (iTeam == 2)
    {
        return m_p1TeamCharacter[0];
    }
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

