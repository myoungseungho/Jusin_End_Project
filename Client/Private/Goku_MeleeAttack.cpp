#include "stdafx.h"
#include "..\Public\Goku_MeleeAttack.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "Input.h"

#include "Play_Goku.h"




//vector<CInput> Command_236Attack =
//{
//	{MOVEKEY_DOWN, ATTACK_NONE},
//	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
//	{MOVEKEY_RIGHT, ATTACK_NONE},
//	{MOVEKEY_NEUTRAL, ATTACK_LIGHT}
//};
//vector<CInput> Command_236Attack_Extra =
//{
//	{MOVEKEY_DOWN, ATTACK_NONE},
//	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
//	{MOVEKEY_RIGHT, ATTACK_NONE},
//	{MOVEKEY_RIGHT, ATTACK_LIGHT}
//};
//vector<CInput> Command_LightAttack ={	{MOVEKEY_NEUTRAL, ATTACK_LIGHT}};
//vector<CInput> Command_MediumAttack ={	{MOVEKEY_NEUTRAL, ATTACK_MEDIUM}};
//vector<CInput> Command_HeavyAttack ={	{MOVEKEY_NEUTRAL, ATTACK_HEAVY}};
//vector<CInput> Command_SpecialAttack ={	{MOVEKEY_NEUTRAL, ATTACK_SPECIAL}};




void CGoku_MeleeAttack::Initalize(CPlay_Goku* pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPlayerAnimationIndex = m_pPlayer->Get_pAnimationIndex();


	m_pbAttackCount = m_pPlayer->Get_pbAttackCount();
	m_piCountGroundSpecial = m_pPlayer->Get_piSpecialCount();
}

void CGoku_MeleeAttack::Attack_Light()
{

	this;
	_bool bDebug = false;


	if (m_pPlayer->Check_bCurAnimationisGroundMove())
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_LIGHT1);
	
	}

	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT1)
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_LIGHT2,1.f);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT2)
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_LIGHT3, 1.f);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_AIR1);
	}
	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR1)
	{
		
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_AIR2,0.5f);
		
		if (m_pPlayer->Get_fGravityTime() > 0.135)
		{
			m_pPlayer->Set_bNextAnimationGravityEvent();
		}
	}
	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR2)
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_AIR3,0.5f);
		if (m_pPlayer->Get_fGravityTime() > 0.135)
		{
			//m_pPlayer->Set_ForcedGravityTime_LittleUp();
			m_pPlayer->Set_bNextAnimationGravityEvent();
		}
	}

	
	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_CROUCH_LIGHT)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_LIGHT1);
	}
	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_LIGHT1);
	}
}

void CGoku_MeleeAttack::Attack_Medium()
{


	if (m_pPlayer->Check_bCurAnimationisGroundMove(*m_pPlayerAnimationIndex))
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_MEDIUM);
		m_pbAttackCount[CPlay_Goku::COUNT_ATTACK_MEDIUM] = false;

		m_pbAttackCount[CPlay_Goku::COUNT_ATTACK_MEDIUM];
		m_pbAttackCount[CPlay_Goku::COUNT_ATTACK_CROUCH_MEDUIM];
		m_pbAttackCount;

		_bool bDebug = true;
	}
	
	else if(*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT2
		|| *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_CROUCH_LIGHT || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_CROUCH_MEDUIM)
	{

		//서서 중공격 횟수가 남아있으면 일단 사용
		if (m_pbAttackCount[CPlay_Goku::COUNT_ATTACK_MEDIUM] == true)
		{
			m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_MEDIUM, 0.5f);
			m_pbAttackCount[CPlay_Goku::COUNT_ATTACK_MEDIUM] = false;
		}

	}

	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_MEDIUM)
	{
		//서서 중공격 중에 또 누를 시 횟수가 있으면  앉아 중공격 자동 사용
		if (m_pbAttackCount[CPlay_Goku::COUNT_ATTACK_CROUCH_MEDUIM] == true)
		{
			m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_CROUCH_MEDUIM,0.5f);
			m_pbAttackCount[CPlay_Goku::COUNT_ATTACK_CROUCH_MEDUIM] = false;
		}
	}

	
	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR1)
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_AIR2, 0.5f);
	}
	
	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_AIR2);
	}


	//다음애니메이션으로 설정하는 방식이지만 약공격이 빨라서 이상함
	//if (m_pPlayer->Check_bCurAnimationisGroundMove(*m_pPlayerAnimationIndex))
	//{
	//	m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_MEDIUM);
	//}
	//else if(*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT2
	//	|| *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_CROUCH_LIGHT || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_CROUCH_MEDUIM)
	//{
	//	m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_MEDIUM,0.5f);
	//}
	//
	//else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR1)
	//{
	//	m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_AIR2, 0.5f);
	//}

	
	
}

void CGoku_MeleeAttack::Attack_Heavy()
{
	//if (m_pPlayer->Check_bCurAnimationisGroundMove() || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_MEDIUM ||
	//	*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT2 ||
	//	*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_CROUCH_LIGHT || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_CROUCH_MEDUIM ||
	//	*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_SPECIAL)
	//{
	//	m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_HEAVY);
	//	//m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_HEAVY, 0.5f);
	//}
	//
	//else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR2)
	//{
	//	m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_AIR3, 0.5f);
	//}
	

	if (m_pPlayer->Check_bCurAnimationisGroundMove() || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_HEAVY);
		//m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_HEAVY, 0.5f);
	}
	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_MEDIUM ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT2 ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_CROUCH_LIGHT || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_CROUCH_MEDUIM ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_SPECIAL)
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_HEAVY, 0.5f);
	}
	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR2)
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_AIR3, 0.5f);
	}
	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_UP  || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_AIR3);
	}
}

void CGoku_MeleeAttack::Attack_Special()
{
	if (m_pPlayer->Check_bCurAnimationisGroundMove(*m_pPlayerAnimationIndex))
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_SPECIAL);
		//(*m_piCountGroundSpecial)++;
	} 
	else if(*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_MEDIUM ||
			*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_CROUCH_LIGHT || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_CROUCH_MEDUIM)
	{
		//m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_SPECIAL, 0.5f);
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_SPECIAL, 0.5f);
		//(*m_piCountGroundSpecial)++;

	}

	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_SPECIAL)
	{
		_ushort iCount = *m_piCountGroundSpecial;

		
		//0에선 더할 수 없으니 5대신 4,    4발째일때까지 다음공격 가능하니 이하 대신 미만.
		if (iCount < 4)
		{

			//m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_SPECIAL, 0.5f);
			
			//홀짝 판별 비트연산 (최적화)  true일때 홀수
			if (iCount & 1)  //홀수만큼 썻으니 왼손
			{

				//if (m_pPlayer->CompareNextAnimation(CPlay_Goku::ANIME_ATTACK_SPECIAL, 10.f) == false)

				if (m_pPlayer->CompareNextAnimation(CPlay_Goku::ANIME_IDLE))
				{
					m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_SPECIAL, 0.5f, 10.f);
					(*m_piCountGroundSpecial)++;
				}
				
			}
			else
			{
				//if (m_pPlayer->CompareNextAnimation(CPlay_Goku::ANIME_ATTACK_SPECIAL, 55.f) == false)
				if (m_pPlayer->CompareNextAnimation(CPlay_Goku::ANIME_IDLE))
				{
					m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_SPECIAL, 0.5f, 55);
					(*m_piCountGroundSpecial)++;
				}

			}


		}
		else
		{
			iCount;
			_bool bDebugf = true;
		}
	}


	//else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_DOWN ||
	//	*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR2)
	//{
	//	m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_SPECIAL_AIR);
	//}


	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_SPECIAL_AIR);
	

	}
	else if(*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR2)
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_SPECIAL_AIR,0.5f);


	}
	

}

void CGoku_MeleeAttack::Attack_Grab()
{
	
	if (m_pPlayer->Check_bCurAnimationisGroundMove())
	{
		m_pPlayer->Set_Grab(false);
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_GRAB_READY);
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_GRAB, 3.f, 5.f);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_DOWN || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_UP)
	{
		m_pPlayer->Set_Grab(true);
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_GRAB_READY);
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_GRAB, 3.f, 5.f);
	}

}

void CGoku_MeleeAttack::Attack_236()
{

	//공중인지 아닌지 상태 받고,  사용중인게 필살기가 아닌지 구분.  피격중은 키 lock을 해버릴것

	if (m_pPlayer->Check_bCurAnimationisGroundMove() || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_236);
		m_pPlayer->Set_fImpulse(2.f *m_pPlayer->Get_iDirection());
		m_pPlayer->Set_bAttackGravity(false);
	} 
	
	else if(*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_MEDIUM ||  *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_HEAVY ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT2  || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT3 ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_SPECIAL || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_CROUCH_LIGHT || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_CROUCH_MEDUIM)
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_236,0.5f);
		m_pPlayer->Set_fImpulse(2.f * m_pPlayer->Get_iDirection());
		m_pPlayer->Set_bAttackGravity(false);

	}
	
	
	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_236_AIR);
		m_pPlayer->Set_fImpulse(2.f * m_pPlayer->Get_iDirection());
		m_pPlayer->Set_bAttackGravity(false);

	}
	else if(*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR1  || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR2 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_SPECIAL_AIR)
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_236_AIR, 0.5f);
		m_pPlayer->Set_fImpulse(2.f * m_pPlayer->Get_iDirection());
		m_pPlayer->Set_bAttackGravity(false);

	}


	


}

void CGoku_MeleeAttack::Attack_214()
{
	


	if (m_pPlayer->Check_bCurAnimationisGroundMove() || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_214);
		m_pPlayer->Set_fImpulse(2.5f * m_pPlayer->Get_iDirection());
		m_pPlayer->Set_bAttackGravity(false);
		m_pPlayer->Set_ForcedGravityDown();

	}
	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_MEDIUM ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT3 ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_HEAVY || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_SPECIAL ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_CROUCH_LIGHT || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_CROUCH_MEDUIM)
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_214, 0.5f);
		m_pPlayer->Set_fImpulse(2.5f * m_pPlayer->Get_iDirection());
		m_pPlayer->Set_bAttackGravity(false);
		m_pPlayer->Set_ForcedGravityDown();
	}


	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_214);
		m_pPlayer->Set_fImpulse(2.5f * m_pPlayer->Get_iDirection());
		m_pPlayer->Set_bAttackGravity(false);
		m_pPlayer->Set_CurrentAnimationPositionJump(20.f);
		m_pPlayer->Set_ForcedGravityDown();

	}
	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR2 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_SPECIAL_AIR)
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_214, 0.5f, 20.f);
		m_pPlayer->Set_fImpulse(2.5f * m_pPlayer->Get_iDirection());
		m_pPlayer->Set_bAttackGravity(false);
		m_pPlayer->Set_ForcedGravityDown();


	}


}

void CGoku_MeleeAttack::Attack_236Special()
{

	//if (m_pPlayer->Check_bCurAnimationisGroundMove() ||		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_MEDIUM || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_HEAVY ||
	//	*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT3 )
	//{
	//	m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL);
	//}
	//
	//else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_DOWN ||
	//	*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR2)
	//{
	//	m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL_AIR);
	//}


	if (m_pPlayer->Check_bCurAnimationisGroundMove() || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL);
		m_pPlayer->Set_fImpulse(2.5f * m_pPlayer->Get_iDirection());

	} 
	
	else if(*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_MEDIUM || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_HEAVY ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT3 ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_SPECIAL)
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL,3.5f);
		m_pPlayer->Set_fImpulse(2.5f * m_pPlayer->Get_iDirection());

	}

	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL_AIR);
		m_pPlayer->Set_fImpulse(2.5f * m_pPlayer->Get_iDirection());

	} 
	else if(*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR2 ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_SPECIAL_AIR)
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL_AIR, 3.5f);
		m_pPlayer->Set_fImpulse(2.5f * m_pPlayer->Get_iDirection());

	}

}

void CGoku_MeleeAttack::Attack_236Special_Side()
{


	if (m_pPlayer->Check_bCurAnimationisGroundMove())
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL_UPSIDE);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_MEDIUM || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_HEAVY ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT3 ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_SPECIAL || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_236_SPECIAL || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_236_SPECIAL_UPSIDE)
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL_UPSIDE, 3.5f);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL_AIR_DOWNSIDE);
	}
	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR2 ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_SPECIAL_AIR )
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL_AIR_DOWNSIDE, 3.f);
	}
	




}

void CGoku_MeleeAttack::Attack_236Ultimate()
{

	//필살기는 모션을 끊고 쓰는것처럼 보이는데 자세히 보면 기술마다 다름
	// 에너지파는 끊고 쓰고,   공격은 타격판정 난 이후? 

	if (m_pPlayer->Check_bCurAnimationisGroundMove() || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_MEDIUM || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_HEAVY ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT3 ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_SPECIAL || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_236  ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_236_SPECIAL || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_236_SPECIAL_UPSIDE)
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL, 3.5f);
		//m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL_AIR);
	}
	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR2 ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_SPECIAL_AIR || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_236_AIR ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_236_SPECIAL_AIR || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_236_SPECIAL_AIR_DOWNSIDE)
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL_AIR, 3.5f);
		//m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL_AIR);
	}


	//214 애니메이션은 지상,공중 구분이 없는데 어떻게 처리할것인가
	//높이 기준이 일단 0. 바뀔수도 있음

	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_214)
	{
		if(m_pPlayer->Get_fHeight() > CPlay_Goku::fGroundHeight)
		{
			m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL_AIR, 3.5f);
			//m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL_AIR);
			
		}
		else
		{
			m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL, 3.5f);
			//m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL);

		}
	}


}

void CGoku_MeleeAttack::Attack_236Ultimate_Side()
{


	if (m_pPlayer->Check_bCurAnimationisGroundMove() || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL_UPSIDE);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_MEDIUM || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_HEAVY ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT3 ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_SPECIAL || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_236 ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_236_SPECIAL || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_236_SPECIAL_UPSIDE)
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL_UPSIDE, 3.5f);
		//m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL_AIR_DOWNSIDE);
	}
	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR2 ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_SPECIAL_AIR || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_236_AIR ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_236_SPECIAL_AIR || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_236_SPECIAL_AIR_DOWNSIDE)
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL_AIR_DOWNSIDE, 3.5f);
		//m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL_AIR);
	}


	//214 애니메이션은 지상,공중 구분이 없는데 어떻게 처리할것인가
	//높이 기준이 일단 0. 바뀔수도 있음

	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_214)
	{
		if (m_pPlayer->Get_fHeight() > CPlay_Goku::fGroundHeight)
		{
			m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL_AIR_DOWNSIDE, 3.5f);
			//m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL_AIR);

		}
		else
		{
			m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL_UPSIDE, 3.5f);
			//m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_236_SPECIAL);

		}
	}

}

void CGoku_MeleeAttack::Attack_214Final()
{
	if (m_pPlayer->Check_bCurAnimationisGroundMove())
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_FINAL_START);
	}
}



void CGoku_MeleeAttack::Attack_Crouch_Light()
{
	if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_CROUCHING  || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_CROUCH_LIGHT);
	}
}

void CGoku_MeleeAttack::Attack_Crouch_Medium()
{

	//횟수 없으면 컷
	if (m_pbAttackCount[CPlay_Goku::COUNT_ATTACK_CROUCH_MEDUIM] == false)
		return;




	if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_CROUCHING || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_CROUCH_MEDUIM);
		m_pbAttackCount[CPlay_Goku::COUNT_ATTACK_CROUCH_MEDUIM] = false;
	}

	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_CROUCH_LIGHT ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT2 ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_MEDIUM)
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_CROUCH_MEDUIM, 1.f);
		m_pbAttackCount[CPlay_Goku::COUNT_ATTACK_CROUCH_MEDUIM] = false;
	}



	//이전에 나올것이 약공격이다보니 nextanimation을 사용하면 타이밍을 놓침
	//if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_CROUCHING)
	//{
	//	m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_CROUCH_MEDUIM);
	//} 
	//else if(*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_CROUCH_LIGHT ||
	//	*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT2 )
	//{
	//	m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_CROUCH_MEDUIM,0.4f);
	//}

	
}

void CGoku_MeleeAttack::Attack_Crouch_Heavy()
{



	//if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_CROUCHING || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_MEDIUM || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_HEAVY ||
	//	*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT3 ||
	//	*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_CROUCH_LIGHT || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_CROUCH_MEDUIM)
	//{
	//	m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_CROUCH_HEAVY);
	//}
	//
	//else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_DOWN ||
	//	*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR2)
	//{
	//	m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_UPPER_AIR);
	//}



	if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_CROUCHING || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_CROUCH_HEAVY);
	}
	
	else if(*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_MEDIUM || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_HEAVY ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_LIGHT3 ||
		*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_CROUCH_LIGHT || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_CROUCH_MEDUIM || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_SPECIAL)
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_CROUCH_HEAVY, 0.5f);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_ATTACK_UPPER_AIR);
		m_pPlayer->Set_fImpulse(1.6f * m_pPlayer->Get_iDirection());

	} 
	else if(*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_AIR2 || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_ATTACK_SPECIAL_AIR)
	{
		m_pPlayer->Set_NextAnimation(CPlay_Goku::ANIME_ATTACK_UPPER_AIR, 0.5f);
		m_pPlayer->Set_fImpulse(1.6f * m_pPlayer->Get_iDirection());

	}




}

void CGoku_MeleeAttack::BackDash()
{

	if (m_pPlayer->Check_bCurAnimationisGroundMove())
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_BACK_DASH);
	}

	else if (m_pPlayer->Get_bAirDashEnable() && (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_DOWN || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_UP))
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_BACK_DASH);
		m_pPlayer->Set_CurrentAnimationPositionJump(12.f);
		m_pPlayer->Set_fImpulse(m_pPlayer->Get_iDirection() *(-10.f));

		m_pPlayer->Set_bAirDashEnable(false);
		m_pPlayer->Set_ForcedGravityDown();
		m_pPlayer->Set_ForcveGravityTime(0.135f);
	}
}

void CGoku_MeleeAttack::ForwardDash()
{
	//if (m_pPlayer->Check_bCurAnimationisGroundMove())
	if(*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_IDLE || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_FORWARD_WALK || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_BACK_WALK)
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_FORWARD_DASH);
	}

	else if (m_pPlayer->Get_bAirDashEnable()  && (*m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_DOWN || *m_pPlayerAnimationIndex == CPlay_Goku::ANIME_JUMP_UP ))
	{
		m_pPlayer->Set_Animation(CPlay_Goku::ANIME_FORWARD_DASH);
		//m_pPlayer->Set_CurrentAnimationPositionJump(4.f);
		m_pPlayer->Set_fImpulse(m_pPlayer->Get_iDirection() * (10.f));
		m_pPlayer->Set_bAirDashEnable(false);
		//m_pPlayer->Set_ForcedGravityDown();

		m_pPlayer->Set_ForcveGravityTime(0.255f);

	}


}

