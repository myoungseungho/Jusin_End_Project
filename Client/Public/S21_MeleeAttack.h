#pragma once

#include "GameObject.h"
#include "Client_Defines.h"
//#include "AnimationEvent_Defines.h"
//#include "FrameEvent_Manager.h"

#include <bitset>

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)


// Input 클래스 정의 (방향 + 버튼을 포함)
class CS21_MeleeAttack {
public:
 
	void Initalize(class CPlay_21* pPlayer);

	void Attack_Light();
	void Attack_Medium();
	void Attack_Heavy();
	void Attack_Special();

	void Attack_236();
	void Attack_214();

	void Attack_236Special();
	//void Attack_236Special_Side();
	


	void Attack_236Ultimate();
	//void Attack_236Ultimate_Side();

	void Attack_214Final();



	void Attack_Crouch_Light();
	void Attack_Crouch_Medium();
	void Attack_Crouch_Heavy();

	void Attack_Crouch_Speical();


	void BackDash();
	void ForwardDash();

	class CPlay_21* m_pPlayer = { nullptr };
	_uint* m_pPlayerAnimationIndex = { nullptr };

	_bool* m_pbAttackCount = { nullptr };
	_ushort* m_piCountGroundSpecial = { nullptr };

};

END