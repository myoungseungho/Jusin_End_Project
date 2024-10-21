#pragma once

#include "GameObject.h"
#include "Client_Defines.h"
#include "AnimationEvent_Defines.h"
#include "FrameEvent_Manager.h"

#include "Input.h"
#include <functional>

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)



/*
vector<CInput> Command_236Attack =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_NEUTRAL, ATTACK_LIGHT}
};
vector<CInput> Command_236Attack_Extra =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_LIGHT}
};


vector<CInput> Command_214Attack =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_NONE},
	{MOVEKEY_NEUTRAL, ATTACK_LIGHT}
};
vector<CInput> Command_214Attack_Extra =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_LIGHT}
};

vector<CInput> Command_236Special =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_SPECIAL}
};

vector<CInput> Command_236Special_Side =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_DOWN, ATTACK_SPECIAL}
};


vector<CInput> Command_214FinalAttack =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_SPECIAL}
};


vector<CInput> Command_236UltimateAttack =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_GRAB}
};


vector<CInput> Command_236UltimateAttack_Side =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_DOWN, ATTACK_GRAB}
};



vector<CInput> Command_BackDash =
{
	{MOVEKEY_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_NONE}
};

vector<CInput> Command_Forward =
{
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{ MOVEKEY_RIGHT, ATTACK_NONE }

};

vector<CInput> Command_LightAttack = { {MOVEKEY_NEUTRAL, ATTACK_LIGHT} };
vector<CInput> Command_MediumAttack = { {MOVEKEY_NEUTRAL, ATTACK_MEDIUM} };
vector<CInput> Command_HeavyAttack = { {MOVEKEY_NEUTRAL, ATTACK_HEAVY} };
vector<CInput> Command_SpecialAttack = { {MOVEKEY_NEUTRAL, ATTACK_SPECIAL} };

vector<CInput> Command_HeavyAttack_Extra = { {MOVEKEY_RIGHT, ATTACK_HEAVY} };



vector<CInput> Command_Crouch_LightAttack = { {MOVEKEY_DOWN, ATTACK_LIGHT} };
vector<CInput> Command_Crouch_MediumAttack = { {MOVEKEY_DOWN, ATTACK_MEDIUM} };
vector<CInput> Command_Crouch_HeavyAttack = { {MOVEKEY_DOWN, ATTACK_HEAVY} };
vector<CInput> Command_Crouch_SpecialAttack = { {MOVEKEY_DOWN, ATTACK_SPECIAL} };



vector<CInput> Command_Crouch_MediumAttack_Extra = { {MOVEKEY_DOWN_RIGHT, ATTACK_MEDIUM} };
vector<CInput> Command_Crouch_HeavyAttack_Extra = { {MOVEKEY_DOWN_RIGHT, ATTACK_HEAVY} };

*/

class CCharacter  : public CGameObject
{
public:
	static vector<CInput> Command_236Attack;
	static vector<CInput> Command_236Attack_Extra;
	static vector<CInput> Command_214Attack;
	static vector<CInput> Command_214Attack_Extra;
	static vector<CInput> Command_236Special;
	static vector<CInput> Command_236Special_Side;
	static vector<CInput> Command_214Special;
	static vector<CInput> Command_214Special_Extra;


	static vector<CInput> Command_214FinalAttack;
	static vector<CInput> Command_236UltimateAttack;
	static vector<CInput> Command_236UltimateAttack_Side;

	static vector<CInput> Command_BackDash;
	static vector<CInput> Command_Forward;

	static vector<CInput> Command_LightAttack;
	static vector<CInput> Command_MediumAttack;
	static vector<CInput> Command_MediumAttack_Extra;

	static vector<CInput> Command_HeavyAttack;
	static vector<CInput> Command_SpecialAttack;
	static vector<CInput> Command_HeavyAttack_Extra;

	static vector<CInput> Command_Crouch_LightAttack;
	static vector<CInput> Command_Crouch_MediumAttack;
	static vector<CInput> Command_Crouch_MediumAttack_Extra;
	static vector<CInput> Command_Crouch_HeavyAttack;
	static vector<CInput> Command_Crouch_HeavyAttack_Extra;
	static vector<CInput> Command_Crouch_SpecialAttack;


public:
	const int BUFFER_SIZE = 30;
	//enum AttackGrade {Attack_light =0, Attack_Medium, Attack_Heavy=2, Attack_Special=2, Attack_Command, Attack_Skill, Attack_FinalSkill};
	//enum HitMotion { HIT_LIGHT, HIT_MEDIUM, HIT_HEAVY, HIT_CROUCH_MEDIUM, HIT_KNOCK_AWAY_LEFT, HIT_KNOCK_AWAY_UP };
	static const _float fGroundHeight;  //0
	static const _float fJumpPower;
	//const enum LOOK{ LOOK_LEFT = -1,  LOOK_RIGHT = 1};

	typedef struct: CGameObject::GAMEOBJECT_DESC
	{
		//_wstring strModelName;
		_ushort iTeam = 1;
	}Character_DESC;

	struct CommandPattern {
		vector<CInput> pattern;  // 입력 패턴
		//std::function<void()> action;  // 해당 패턴이 성공했을 때 실행할 함수
		_uint AnimationIndex;
	};

	struct CommandPatternFunction {
		vector<CInput> pattern;  // 입력 패턴
		std::function<void()> action;  // 해당 패턴이 성공했을 때 실행할 함수
		//_uint AnimationIndex;
	};

protected:
	CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharacter(const CCharacter& Prototype);
	virtual ~CCharacter() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

	
	//커맨드 입력
	//virtual void InputCommand();
	virtual _bool InputCommand();
	virtual void InputedCommandUpdate(_float fTimeDelta) ;
	virtual void UpdateInputBuffer(CInput newInput) 
	{
		if (inputBuffer.size() >= BUFFER_SIZE) {
			inputBuffer.erase(inputBuffer.begin());  // 오래된 입력 삭제
		}
		inputBuffer.push_back(newInput);
	}
	virtual bool CheckCommandSkippingExtras(const vector<CInput>& pattern, int timeWindow);
	bool CheckCommand_Exactly(const std::vector<CInput>& pattern, int timeWindow);
	_uint CheckAllCommands();


	//애니메이션
	virtual _bool Character_Play_Animation(_float fTimeDelta);
	virtual _bool Check_bCurAnimationisGroundMove(_uint iAnimation = 1000) { return false; };
	virtual _bool Check_bCurAnimationisAttack(_uint iAnimation = 1000) { return false; };
	virtual _bool Check_bCurAnimationisAirAttack(_uint iAnimation = 1000) { return false; };
	virtual _bool Check_bCurAnimationisAirHit(_uint iAnimation = 1000);
	virtual _bool Check_bCurAnimationisHitAway(_uint iAnimation = 1000);

	void Set_NextAnimation(_uint iAnimationIndex, _float fLifeTime, _float fAnimationPosition =0);
	//void Set_NextAnimation(_uint iAnimationIndex, _float fLifeTime);
	virtual void AttackNextMoveCheck();
	virtual void AnimeEndNextMoveCheck();
	//virtual void Set_Animation(_uint iAnimationIndex) {};
	virtual void Set_Animation(_uint iAnimationIndex, _bool bloof =false);

	_bool		CompareNextAnimation(_uint iAnimationIndex, _float fNextPosition = 0);

	virtual void Set_CurrentAnimationPositionJump(_float fAnimationPosition);

	//키프레임 이벤트
	virtual void ProcessEventsFramesZero(_uint characterIndex, _uint animationIndex);
	virtual void ProcessEventsBetweenFrames2(int characterIndex, int animationIndex, float prevFrame, float currentFrame);

	//키프레임 이벤트 파생
	void Create_Effect(_int iEffectIndex);



	void FlipDirection(_int iDirection = 0);
	_int Get_iDirection() { return m_iLookDirection; };
	_uint* Get_pAnimationIndex();

	_uint Get_iPlayerTeam() { return m_iPlayerTeam; };

	//virtual void AttackEvent(_int iAttackEventEnum, _int AddEvent = 0) {};
	virtual void AttackEvent(_int iAttackEvent, _int AddEvent = 0) {};




	//디버그용 코드 
	virtual void ShowInputBuffer();
	virtual void DebugPositionReset();

	

	//중력관련
	//virtual void Gravity(_float fTimeDelta);
	virtual void Gravity(_float fTimeDelta);

	virtual void Set_fJumpPower(_float fJumpPower) { m_fJumpPower = fJumpPower; };
	virtual void Set_fGravityTime(_float fGravityTime) {	m_fGravityTime = fGravityTime;	};
	//virtual void Set_fImpulse(_float fImpulse) { m_fImpuse = fImpulse; };
	virtual void Set_fImpulse(_float2 fImpulse) { m_fImpuse = fImpulse; };
	virtual void Set_fImpulse(_float fImpulseX) { m_fImpuse.x = fImpulseX; };
	//virtual void Set_fImpulse(_float fImpulseX, _float fImpusY=0) { m_fImpuse.x = fImpulseX;   m_fImpuseImpus.y=fImpusY };  //반드시 0으로 초기화되는거니 패스


	_float Get_fHeight();
	_bool Get_bAirDashEnable() { return m_bAriDashEnable; };
	void Set_bAirDashEnable(_bool bAirDashEnable) { m_bAriDashEnable = bAirDashEnable; };

	void Set_ForcedGravityDown();
	void Set_ForcedGravityTime_LittleUp();

	void Set_ForcveGravityTime(_float fGravityTime);

	void Set_bNextAnimationGravityEvent() { m_bNextAnimationGravityEvent = true; };

	void Set_bAttackGravity(_bool bAttackGravity) { m_bAttackGravity = bAttackGravity; };

	_float Get_fGravityTime() { return m_fGravityTime; };



	void AttckCancleJump();

	void Chase(_float fTimeDelta);
	void Chase2(_float fTimeDelta);
	void Chase_Ready(_float fTimeDelta);

	void Move(_float fTimeDelta);
	void MoveKey1Team(_float fTimeDelta);
	void MoveKey2Team(_float fTimeDelta);
	virtual void Reset_AttackCount() {};


	//피격 관련
	//void Set_Hit(_uint eAnimation, _float fStunTime, _float fStopTime, _float2 Impus = { 0,0 });
	_bool Set_Hit(_uint eAnimation, _float fStunTime,_uint iDamage, _float fStopTime, _float2 Impus = { 0,0 });

	void Set_HitAnimation(_uint eAnimation, _float2 Impus = { 0,0 });
	void Set_AnimationStop(_float fStopTime);

	void Check_StunEnd();

	void Stun_Shake();

	void Update_AnimationLock(_float fTimeDelta);

	void Update_StunImpus(_float fTimeDelta);

	void Set_BreakFall_Ground();
	void BreakFall_Air();

	//공격 관련
	void Gain_AttackStep(_ushort iStep) { m_iAttackStepCount += iStep; };
	_float Get_DamageScale();



	//1020 추가
	void Set_GroundSmash(_bool bSmash);
	//void Guard_Update();   //서브캐릭터용도로 써야하나?
	_bool Guard_Check();
	
protected:
	CShader*				m_pShaderCom = { nullptr };	
	CModel*					m_pModelCom = { nullptr };

	_float					m_fRandom = {};
	_wstring				m_strModelName{};

	//_uint					m_iMoveGrade = {};
	//_uint					m_iCurMoveIndex{};
	//커맨드 만들면 스택형으로 넣어두고 0.3초안에 지우기

	//피격 스턴과 다름
	_float					m_fAccAnimationLock{};
	_float					m_fMaxAnimationLock{};
	_bool					m_bAnimationLock{};

	CHARACTER_INDEX			m_eCharacterIndex={ PLAY_GOKU };
	
	FrameEventMap*			m_pFrameEvent = { nullptr };
	_bool					m_bMotionPlaying = false;

	vector<CInput> inputBuffer;

	_ushort m_iPlayerTeam = { 1 };
	_int	m_iLookDirection = { 1 };
	vector<CommandPattern> MoveCommandPatterns;
	vector<CommandPatternFunction> MoveCommandPatternsFunction;

	vector<CommandPatternFunction> MoveCommandPatternsFunction_Exactly;


	//_uint					m_iNextAnimationIndex = { 0 };
	
	//index,시간
	pair<_uint, _float>		m_iNextAnimation{0,0 };
	_float					m_fNextAnimationCurrentPosition = {};




	_ushort m_iJumpAnimationIndex = { 6 };
	_ushort m_iFallAnimationIndex = {7};

	_ushort m_iIdleAnimationIndex = { 0 };
	_ushort m_iCrouchAnimationIndex = {4};
	_ushort m_iBackWalkAnimationIndex = { 10 };
	_ushort m_iForwardWalkAnimationIndex = { 9 };
	_ushort m_iForwardDashAnimationIndex = {11};
	_ushort m_iForwardDashEndAnimationIndex = { 14 };

	_ushort m_iStandingMidAttackAnimationIndex = { 46 };
	_ushort m_iChaseAnimationIndex = { 13 };


	_ushort m_iHit_Stand_LightAnimationIndex = { 21 };		//050
	//_ushort m_iHit_Stand_LightFrontAnimationIndex = { 21 };		//050

	_ushort m_iHit_Stand_MediumAnimationIndex = { 22 };		//051
	_ushort m_iHit_Crouch_AnimationIndex = { 23 };			//052

	_ushort m_iHit_Away_LeftAnimationIndex = {33};
	_ushort m_iHit_Away_UpAnimationIndex = { 35 };

	_ushort m_iHit_Air_LightAnimationIndex = { 24 };		//050
	_ushort m_iHit_Air_FallAnimationIndex = { 26 };	

	_ushort m_iHit_Air_Spin_LeftUp = {31};


	//기상
	_ushort m_iBreakFall_Ground = { 41 };   //100
	_ushort m_iBreakFall_Air = { 42 };   //101
	_ushort m_iBound_Ground = { 25 };



	_ushort m_iAttack_Air1 = { 52 };		
	_ushort m_iAttack_Air2 = { 53 };		
	_ushort m_iAttack_Air3 = { 54 };		
	_ushort m_iAttack_AirUpper = { 55 };


	//가드
	_ushort m_iGuard_GroundAnimationIndex = { 18 };
	_ushort m_iGuard_CrouchAnimationIndex = { 19 };
	_ushort m_iGuard_AirAnimationIndex = { 20 };



	_float m_fGravityTime = {0.f}; 
	_float m_fJumpPower = 3;// { 0.f };

	//가속도
	//_float m_fImpuse = { 0.f };


	//피격시 Y축 이동을 대비해 xy로 만듦.  각도별로 되어있으니 그냥 애니메이션으로 처리할까 생각도 듦
	_float2 m_fImpuse = { 0.f,0.f };


	_bool m_bDoubleJumpEnable = { true };
	_bool m_bAriDashEnable = { true };
	
	_bool m_bJumpLock = { false };
	_float m_fAccJumpLockTime= { 0.f };

	_bool m_bAttackGravity = { true };


	_bool m_bChase = { false };
	_float m_fAccChaseTime = { 0.f };
	_vector m_vChaseDir{ 0 };



	//스턴 관련
	_bool m_bStun = { false };
	_float m_fAccStunTime = {};
	_float m_fMaxStunTime = {};

	_bool m_bStunShakeDirection = { false };

	//디버그용 임시 collider
	CCollider_Test* m_pColliderCom = { nullptr };
	CCharacter* m_pDebugEnemy = { nullptr };

	_short		 m_iHP = 10000;   //맞는순간 음수가 될 수 있으니 ushort 대신 sohrt.  범위가   -32,768 ~ 32,767 니까 주의 


	_ushort		m_iAttackStepCount = { 0 };  //콤보수 아님.

	_bool		m_bSparking = false;
	_bool		m_bNextAnimationGravityEvent = false;




	//1020추가분량

	//스턴관련
	_bool m_bHitGroundSmashed = { false };

	//멤버변수에 넣는 대신 함수로 체크?   이거 없으면  서브캐릭터들도 가드를 해버림.  교체할때 그냥 사라지게?
	//_bool m_bGuard = { false };


	//디버그용
	_uint m_iDebugComoboDamage = { 0 };


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CCharacter* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};



END