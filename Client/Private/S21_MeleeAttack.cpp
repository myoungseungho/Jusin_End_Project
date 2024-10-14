#include "stdafx.h"
#include "..\Public\S21_MeleeAttack.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "Input.h"

#include "Play_21.h"






void CS21_MeleeAttack::Initalize(CPlay_21* pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPlayerAnimationIndex = m_pPlayer->Get_pAnimationIndex();


	m_pbAttackCount = m_pPlayer->Get_pbAttackCount();
	m_piCountGroundSpecial = m_pPlayer->Get_piSpecialCount();
}

void CS21_MeleeAttack::Attack_Light()
{

	this;
	_bool bDebug = false;


	if (m_pPlayer->Check_bCurAnimationisGroundMove())
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_LIGHT1);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_LIGHT2,1.f);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_LIGHT3, 1.f);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_AIR1);
	}
	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR1)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_AIR2,0.5f);
	}
	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR2)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_AIR3,0.5f);
	}

	
	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_LIGHT)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_LIGHT1);
	}
	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_LIGHT1);
	}
}

void CS21_MeleeAttack::Attack_Medium()
{


	

	//if(m_pPlayer->Check_bCurAnimationisGroundMove(*m_pPlayerAnimationIndex) ||
	//	*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2
	//	|| *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_LIGHT || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_MEDUIM)
	//{
	//
	//	//���� �߰��� Ƚ���� ���������� �ϴ� ���
	//	if (m_pbAttackCount[CPlay_21::COUNT_ATTACK_MEDIUM] == true)
	//	{
	//		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_MEDIUM);
	//		m_pbAttackCount[CPlay_21::COUNT_ATTACK_MEDIUM] = false;
	//
	//		m_pbAttackCount[CPlay_21::COUNT_ATTACK_MEDIUM];
	//		m_pbAttackCount[CPlay_21::COUNT_ATTACK_CROUCH_MEDUIM];
	//		m_pbAttackCount;
	//
	//		_bool bDebug = true;
	//
	//
	//	}
	//
	//}


	if (m_pPlayer->Check_bCurAnimationisGroundMove(*m_pPlayerAnimationIndex))
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_MEDIUM);
		m_pbAttackCount[CPlay_21::COUNT_ATTACK_MEDIUM] = false;

		m_pbAttackCount[CPlay_21::COUNT_ATTACK_MEDIUM];
		m_pbAttackCount[CPlay_21::COUNT_ATTACK_CROUCH_MEDUIM];
		m_pbAttackCount;

		_bool bDebug = true;
	}
	
	else if(*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2
		|| *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_LIGHT || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_MEDUIM)
	{

		//���� �߰��� Ƚ���� ���������� �ϴ� ���
		if (m_pbAttackCount[CPlay_21::COUNT_ATTACK_MEDIUM] == true)
		{
			m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_MEDIUM, 0.5f);
			m_pbAttackCount[CPlay_21::COUNT_ATTACK_MEDIUM] = false;

			m_pbAttackCount[CPlay_21::COUNT_ATTACK_MEDIUM];
			m_pbAttackCount[CPlay_21::COUNT_ATTACK_CROUCH_MEDUIM];
			m_pbAttackCount;

			_bool bDebug = true;


		}

	}

	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_MEDIUM)
	{
		//���� �߰��� �߿� �� ���� �� Ƚ���� ������  �ɾ� �߰��� �ڵ� ���
		if (m_pbAttackCount[CPlay_21::COUNT_ATTACK_CROUCH_MEDUIM] == true)
		{
			m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_CROUCH_MEDUIM,0.5f);
			m_pbAttackCount[CPlay_21::COUNT_ATTACK_CROUCH_MEDUIM] = false;
		}
	}

	
	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR1)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_AIR2, 0.5f);
	}
	
	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_AIR2);
	}


	//�����ִϸ��̼����� �����ϴ� ��������� ������� ���� �̻���
	//if (m_pPlayer->Check_bCurAnimationisGroundMove(*m_pPlayerAnimationIndex))
	//{
	//	m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_MEDIUM);
	//}
	//else if(*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2
	//	|| *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_LIGHT || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_MEDUIM)
	//{
	//	m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_MEDIUM,0.5f);
	//}
	//
	//else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR1)
	//{
	//	m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_AIR2, 0.5f);
	//}

	
	
}

void CS21_MeleeAttack::Attack_Heavy()
{
	//if (m_pPlayer->Check_bCurAnimationisGroundMove() || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_MEDIUM ||
	//	*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2 ||
	//	*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_LIGHT || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_MEDUIM ||
	//	*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL)
	//{
	//	m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_HEAVY);
	//	//m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_HEAVY, 0.5f);
	//}
	//
	//else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR2)
	//{
	//	m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_AIR3, 0.5f);
	//}
	

	if (m_pPlayer->Check_bCurAnimationisGroundMove() || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_HEAVY);
		//m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_HEAVY, 0.5f);
	}
	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_MEDIUM ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2 ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_LIGHT || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_MEDUIM ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_HEAVY, 0.5f);
	}
	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR2)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_AIR3, 0.5f);
	}
	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_UP  || *m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_AIR3);
	}
}

void CS21_MeleeAttack::Attack_Special()
{
	if (m_pPlayer->Check_bCurAnimationisGroundMove(*m_pPlayerAnimationIndex))
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_SPECIAL);
		//(*m_piCountGroundSpecial)++;
	} 
	else if(*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_MEDIUM ||
			*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_LIGHT || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_MEDUIM)
	{
		//m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_SPECIAL, 0.5f);
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_SPECIAL, 0.5f);
		//(*m_piCountGroundSpecial)++;

	}

	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL)
	{
		_ushort iCount = *m_piCountGroundSpecial;

		
		//0���� ���� �� ������ 5��� 4,    4��°�϶����� �������� �����ϴ� ���� ��� �̸�.
		if (iCount < 4)
		{

			//m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_SPECIAL, 0.5f);
			
			//Ȧ¦ �Ǻ� ��Ʈ���� (����ȭ)  true�϶� Ȧ��
			if (iCount & 1)  //Ȧ����ŭ ������ �޼�
			{

				//if (m_pPlayer->CompareNextAnimation(CPlay_21::ANIME_ATTACK_SPECIAL, 10.f) == false)

				if (m_pPlayer->CompareNextAnimation(CPlay_21::ANIME_IDLE))
				{
					m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_SPECIAL, 0.5f, 10.f);
					(*m_piCountGroundSpecial)++;
				}
				
			}
			else
			{
				//if (m_pPlayer->CompareNextAnimation(CPlay_21::ANIME_ATTACK_SPECIAL, 55.f) == false)
				if (m_pPlayer->CompareNextAnimation(CPlay_21::ANIME_IDLE))
				{
					m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_SPECIAL, 0.5f, 55);
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


	//else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_DOWN ||
	//	*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR2)
	//{
	//	m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_SPECIAL_AIR);
	//}


	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_SPECIAL_AIR);
	}
	else if(*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR2)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_SPECIAL_AIR,0.5f);
	}
	

}

void CS21_MeleeAttack::Attack_236()
{

	//�������� �ƴ��� ���� �ް�,  ������ΰ� �ʻ�Ⱑ �ƴ��� ����.  �ǰ����� Ű lock�� �ع�����

	if (m_pPlayer->Check_bCurAnimationisGroundMove() || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236);
	} 
	
	else if(*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_MEDIUM ||  *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_HEAVY ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2  || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT3 ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_236,0.5f);
	}
	
	
	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_AIR);
	}
	else if(*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR1  || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR2 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL_AIR)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_236_AIR, 0.5f);
		
	}


	


}

void CS21_MeleeAttack::Attack_214()
{
	//if (m_pPlayer->Check_bCurAnimationisGroundMove() || *m_pPlayerAnimationIndex ==  CPlay_21::ANIME_JUMP_UP  || *m_pPlayerAnimationIndex ==  CPlay_21::ANIME_JUMP_DOWN ||
	//	*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_MEDIUM ||
	//	*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT3
	//	|| *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_HEAVY )
	//{
	//	m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_214);
	//}


	//���߿��� ����ص� �� �ڼ� ������ �ڵ�
	//if (m_pPlayer->Check_bCurAnimationisGroundMove() || *m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_DOWN)
	//{
	//	m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_214);
	//}
	//else if(*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_MEDIUM || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_HEAVY ||
	//	*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT3||
	//	*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR2 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL_AIR	)
	//{
	//	m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_214, 1.f);
	//}



	//���߿��� ����ص� �ߵ�� �ִ� �ڵ�
	//if (m_pPlayer->Check_bCurAnimationisGroundMove())
	//{
	//	m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_214);
	//}
	//else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_MEDIUM || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_HEAVY ||
	//	*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT3)
	//{
	//	m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_214, 1.f);
	//}
	//
	//
	//if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_DOWN)
	//{
	//	m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_214);
	//
	//}
	//else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR2 || 
	//		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL_AIR)
	//{
	//	m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_214, 1.f);
	//}




	if (m_pPlayer->Check_bCurAnimationisGroundMove() || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_214);
	}
	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_MEDIUM ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT3 ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_HEAVY || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_214, 0.5f);
	}


	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_214);
	}
	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR2 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL_AIR)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_214, 0.5f, 20.f);

	}


}

void CS21_MeleeAttack::Attack_236Special()
{

	//if (m_pPlayer->Check_bCurAnimationisGroundMove() ||		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_MEDIUM || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_HEAVY ||
	//	*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT3 )
	//{
	//	m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_SPECIAL);
	//}
	//
	//else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_DOWN ||
	//	*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR2)
	//{
	//	m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_SPECIAL_AIR);
	//}


	if (m_pPlayer->Check_bCurAnimationisGroundMove() || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_SPECIAL);
	} 
	
	else if(*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_MEDIUM || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_HEAVY ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT3 ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_236_SPECIAL,3.5f);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_SPECIAL_AIR);
	} 
	else if(*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR2 ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL_AIR)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_236_SPECIAL_AIR, 3.5f);
	}

}

void CS21_MeleeAttack::Attack_236Special_Side()
{


	if (m_pPlayer->Check_bCurAnimationisGroundMove())
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_SPECIAL_UPSIDE);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_MEDIUM || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_HEAVY ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT3 ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_236_SPECIAL || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_236_SPECIAL_UPSIDE)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_236_SPECIAL_UPSIDE, 3.5f);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_SPECIAL_AIR_DOWNSIDE);
	}
	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR2 ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL_AIR )
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_236_SPECIAL_AIR_DOWNSIDE, 3.f);
	}
	




}

void CS21_MeleeAttack::Attack_236Ultimate()
{

	//�ʻ��� ����� ���� ���°�ó�� ���̴µ� �ڼ��� ���� ������� �ٸ�
	// �������Ĵ� ���� ����,   ������ Ÿ������ �� ����? 

	if (m_pPlayer->Check_bCurAnimationisGroundMove() || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_SPECIAL);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_MEDIUM || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_HEAVY ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT3 ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_236  ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_236_SPECIAL || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_236_SPECIAL_UPSIDE)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_236_SPECIAL, 3.5f);
		//m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_SPECIAL);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_SPECIAL_AIR);
	}
	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR2 ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL_AIR || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_236_AIR ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_236_SPECIAL_AIR || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_236_SPECIAL_AIR_DOWNSIDE)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_236_SPECIAL_AIR, 3.5f);
		//m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_SPECIAL_AIR);
	}


	//214 �ִϸ��̼��� ����,���� ������ ���µ� ��� ó���Ұ��ΰ�
	//���� ������ �ϴ� 0. �ٲ���� ����

	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_214)
	{
		if(m_pPlayer->Get_fHeight() > CPlay_21::fGroundHeight)
		{
			m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_236_SPECIAL_AIR, 3.5f);
			//m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_SPECIAL_AIR);
			
		}
		else
		{
			m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_236_SPECIAL, 3.5f);
			//m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_SPECIAL);

		}
	}


}

void CS21_MeleeAttack::Attack_236Ultimate_Side()
{


	if (m_pPlayer->Check_bCurAnimationisGroundMove() || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_SPECIAL_UPSIDE);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_MEDIUM || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_HEAVY ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT3 ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_236 ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_236_SPECIAL || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_236_SPECIAL_UPSIDE)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_236_SPECIAL_UPSIDE, 3.5f);
		//m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_SPECIAL);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_SPECIAL_AIR_DOWNSIDE);
	}
	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR2 ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL_AIR || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_236_AIR ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_236_SPECIAL_AIR || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_236_SPECIAL_AIR_DOWNSIDE)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_236_SPECIAL_AIR_DOWNSIDE, 3.5f);
		//m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_SPECIAL_AIR);
	}


	//214 �ִϸ��̼��� ����,���� ������ ���µ� ��� ó���Ұ��ΰ�
	//���� ������ �ϴ� 0. �ٲ���� ����

	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_214)
	{
		if (m_pPlayer->Get_fHeight() > CPlay_21::fGroundHeight)
		{
			m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_236_SPECIAL_AIR_DOWNSIDE, 3.5f);
			//m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_SPECIAL_AIR);

		}
		else
		{
			m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_236_SPECIAL_UPSIDE, 3.5f);
			//m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_SPECIAL);

		}
	}

}

void CS21_MeleeAttack::Attack_214Final()
{
	if (m_pPlayer->Check_bCurAnimationisGroundMove())
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_FINAL_START);
	}
}



void CS21_MeleeAttack::Attack_Crouch_Light()
{
	if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_CROUCHING  || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_CROUCH_LIGHT);
	}
}

void CS21_MeleeAttack::Attack_Crouch_Medium()
{

	//Ƚ�� ������ ��
	if (m_pbAttackCount[CPlay_21::COUNT_ATTACK_CROUCH_MEDUIM] == false)
		return;




	if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_CROUCHING || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_CROUCH_MEDUIM);
		m_pbAttackCount[CPlay_21::COUNT_ATTACK_CROUCH_MEDUIM] = false;
	}

	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_LIGHT ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2 ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_MEDIUM)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_CROUCH_MEDUIM, 1.f);
		m_pbAttackCount[CPlay_21::COUNT_ATTACK_CROUCH_MEDUIM] = false;
	}



	//������ ���ð��� ������̴ٺ��� nextanimation�� ����ϸ� Ÿ�̹��� ��ħ
	//if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_CROUCHING)
	//{
	//	m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_CROUCH_MEDUIM);
	//} 
	//else if(*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_LIGHT ||
	//	*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2 )
	//{
	//	m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_CROUCH_MEDUIM,0.4f);
	//}

	
}

void CS21_MeleeAttack::Attack_Crouch_Heavy()
{



	//if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_CROUCHING || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_MEDIUM || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_HEAVY ||
	//	*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT3 ||
	//	*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_LIGHT || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_MEDUIM)
	//{
	//	m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_CROUCH_HEAVY);
	//}
	//
	//else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_DOWN ||
	//	*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR2)
	//{
	//	m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_UPPER_AIR);
	//}



	if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_CROUCHING || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_CROUCH_HEAVY);
	}
	
	else if(*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_MEDIUM || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_HEAVY ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT3 ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_LIGHT || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_MEDUIM || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_CROUCH_HEAVY, 0.5f);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_UPPER_AIR);
	} 
	else if(*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR2 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL_AIR)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_UPPER_AIR, 0.5f);
	}




}

void CS21_MeleeAttack::BackDash()
{

	if (m_pPlayer->Check_bCurAnimationisGroundMove())
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_BACK_DASH);
	}
}

void CS21_MeleeAttack::ForwardDash()
{
	//if (m_pPlayer->Check_bCurAnimationisGroundMove())
	if(*m_pPlayerAnimationIndex == CPlay_21::ANIME_IDLE || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_WALK || *m_pPlayerAnimationIndex == CPlay_21::ANIME_BACK_WALK)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_FORWARD_DASH);
	}
}


