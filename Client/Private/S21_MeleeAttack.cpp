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
	//	//서서 중공격 횟수가 남아있으면 일단 사용
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

		//서서 중공격 횟수가 남아있으면 일단 사용
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
		//서서 중공격 중에 또 누를 시 횟수가 있으면  앉아 중공격 자동 사용
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


	//다음애니메이션으로 설정하는 방식이지만 약공격이 빨라서 이상함
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


	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_CROUCH_SPECIAL);
		//16부터
		CModel* pModel = static_cast<CModel*>(m_pPlayer->Get_Component(TEXT("Com_Model")));
		pModel->m_fCurrentAnimPosition = 16.f;
		pModel->Update_FrameIndex();
	}
	else if(*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR2)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_CROUCH_SPECIAL,0.5f,16.f);
	}
	

}

void CS21_MeleeAttack::Attack_236()
{

	//공중인지 아닌지 상태 받고,  사용중인게 필살기가 아닌지 구분.  피격중은 키 lock을 해버릴것

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
	
	


}

void CS21_MeleeAttack::Attack_214()
{
	


	if (m_pPlayer->Check_bCurAnimationisGroundMove() || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH_END)
	{
		CTransform* pTrasnform = static_cast<CTransform*>(m_pPlayer->Get_Component(TEXT("Com_Transform")));
		pTrasnform->Add_Move({ 0.f,1.f,0.f });
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
	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR2 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_SPECIAL)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_214, 0.5f, 20.f);

	}


}

void CS21_MeleeAttack::Attack_236Special()
{



	if (m_pPlayer->Check_bCurAnimationisGroundMove() || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_SPECIAL);

		//디버그코드 체크
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_236_SPECIAL_SUCCES, 10.5f);

	}

	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_MEDIUM || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_HEAVY ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT3 ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_236_SPECIAL, 3.5f);
	}

}


void CS21_MeleeAttack::Attack_236Ultimate()
{

	//필살기는 모션을 끊고 쓰는것처럼 보이는데 자세히 보면 기술마다 다름
	// 에너지파는 끊고 쓰고,   공격은 타격판정 난 이후? 

	if (m_pPlayer->Check_bCurAnimationisGroundMove() || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_ULTIMATE);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_MEDIUM || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_HEAVY ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT3 ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_236  ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_236_ULTIMATE)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_236_ULTIMATE, 3.5f);
		//m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_236_SPECIAL);
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

	//횟수 없으면 컷
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



	
}

void CS21_MeleeAttack::Attack_Crouch_Heavy()
{



	


	if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_CROUCHING || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_CROUCH_HEAVY);
	}
	
	else if(*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_MEDIUM || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_HEAVY ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT2 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_LIGHT3 ||
		*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_LIGHT || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_CROUCH_MEDUIM )
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_CROUCH_HEAVY, 0.5f);
	}

	else if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_UP || *m_pPlayerAnimationIndex == CPlay_21::ANIME_JUMP_DOWN)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_UPPER_AIR);
	} 
	else if(*m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR1 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_AIR2 || *m_pPlayerAnimationIndex == CPlay_21::ANIME_ATTACK_SPECIAL)
	{
		m_pPlayer->Set_NextAnimation(CPlay_21::ANIME_ATTACK_UPPER_AIR, 0.5f);
	}




}

void CS21_MeleeAttack::Attack_Crouch_Speical()
{

	if (*m_pPlayerAnimationIndex == CPlay_21::ANIME_CROUCHING || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_DASH_END)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_ATTACK_CROUCH_SPECIAL);
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
	if(*m_pPlayerAnimationIndex == CPlay_21::ANIME_IDLE || *m_pPlayerAnimationIndex == CPlay_21::ANIME_FORWARD_WALK || *m_pPlayerAnimationIndex == CPlay_21::ANIME_BACK_WALK)
	{
		m_pPlayer->Set_Animation(CPlay_21::ANIME_FORWARD_DASH);
	}
}


