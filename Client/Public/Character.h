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
	static vector<CInput> Command_214FinalAttack;
	static vector<CInput> Command_236UltimateAttack;
	static vector<CInput> Command_236UltimateAttack_Side;
	static vector<CInput> Command_BackDash;
	static vector<CInput> Command_Forward;
	static vector<CInput> Command_LightAttack;
	static vector<CInput> Command_MediumAttack;
	static vector<CInput> Command_HeavyAttack;
	static vector<CInput> Command_SpecialAttack;
	static vector<CInput> Command_HeavyAttack_Extra;
	static vector<CInput> Command_Crouch_LightAttack;
	static vector<CInput> Command_Crouch_MediumAttack;
	static vector<CInput> Command_Crouch_HeavyAttack;
	static vector<CInput> Command_Crouch_SpecialAttack;
	static vector<CInput> Command_Crouch_MediumAttack_Extra;
	static vector<CInput> Command_Crouch_HeavyAttack_Extra;


public:
	const int BUFFER_SIZE = 30;
	enum AttackGrade {Attack_light =0, Attack_Medium, Attack_Heavy=2, Attack_Special=2, Attack_Command, Attack_Skill, Attack_FinalSkill};
	static const _float fGroundHeight;  //0

	typedef struct: CGameObject::GAMEOBJECT_DESC
	{
		_wstring strModelName;
	}Character_DESC;

	struct CommandPattern {
		vector<CInput> pattern;  // �Է� ����
		//std::function<void()> action;  // �ش� ������ �������� �� ������ �Լ�
		_uint AnimationIndex;
	};

	struct CommandPatternFunction {
		vector<CInput> pattern;  // �Է� ����
		std::function<void()> action;  // �ش� ������ �������� �� ������ �Լ�
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

	
	//Ŀ�ǵ� �Է�
	virtual void InputCommand();
	virtual void InputedCommandUpdate(_float fTimeDelta) ;
	virtual void UpdateInputBuffer(CInput newInput) 
	{
		if (inputBuffer.size() >= BUFFER_SIZE) {
			inputBuffer.erase(inputBuffer.begin());  // ������ �Է� ����
		}
		inputBuffer.push_back(newInput);
	}
	virtual bool CheckCommandSkippingExtras(const vector<CInput>& pattern, int timeWindow);
	bool CheckCommandWithStartCondition(const vector<CInput>& pattern, int timeWindow);
	_uint CheckAllCommands();


	//�ִϸ��̼�
	virtual _bool Character_Play_Animation(_float fTimeDelta);
	virtual _bool Check_bCurAnimationisGroundMove(_uint iAnimation = 1000) { return false; };
	virtual _bool Check_bCurAnimationisAttack(_uint iAnimation = 1000) { return false; };
	void Set_NextAnimation(_uint iAnimationIndex, _float fLifeTime, _float fAnimationPosition =0);
	//void Set_NextAnimation(_uint iAnimationIndex, _float fLifeTime);
	virtual void AttackNextMoveCheck() {};
	virtual void AnimeEndNextMoveCheck() {};
	//virtual void Set_Animation(_uint iAnimationIndex) {};
	virtual void Set_Animation(_uint iAnimationIndex, _bool bloof =false) {};

	_bool		CompareNextAnimation(_uint iAnimationIndex, _float fNextPosition = 0);

	//Ű������ �̺�Ʈ
	virtual void ProcessEventsFramesZero(_uint characterIndex, _uint animationIndex);
	virtual void ProcessEventsBetweenFrames2(int characterIndex, int animationIndex, float prevFrame, float currentFrame);

	//Ű������ �̺�Ʈ �Ļ�
	void Create_Effect(_int iEffectIndex);


	_float Get_fHeight();

	//����׿� �ڵ� 
	virtual void ShowInputBuffer();
	virtual void DebugPositionReset();

	_int Get_iDirection() { return m_iLookDirection; };
	void FlipDirection(_int iDirection = 0);
	_uint* Get_pAnimationIndex();





	virtual void Gravity(_float fTimeDelta);

protected:
	CShader*				m_pShaderCom = { nullptr };	
	CModel*					m_pModelCom = { nullptr };

	_float					m_fRandom = {};
	_wstring				m_strModelName{};

	//_uint					m_iMoveGrade = {};
	//_uint					m_iCurMoveIndex{};
	//Ŀ�ǵ� ����� ���������� �־�ΰ� 0.3�ʾȿ� �����

	_float					m_fAccAnimationLock{};
	_float					m_fMaxAnimationLock{};
	_bool					m_bAnimationLock{};

	CHARACTER_INDEX			m_eCharacterIndex={ PLAY_GOKU };
	
	FrameEventMap* m_pFrameEvent = { nullptr };
	_bool					m_bMotionPlaying = false;

	vector<CInput> inputBuffer;

	_ushort m_iPlayerTeam = { 1 };
	_int	m_iLookDirection = { 1 };
	vector<CommandPattern> MoveCommandPatterns;
	vector<CommandPatternFunction> MoveCommandPatternsFunction;

	//_uint					m_iNextAnimationIndex = { 0 };
	
	//index,�ð�
	pair<_uint, _float>		m_iNextAnimation{0,0 };
	_float					m_fNextAnimationCurrentPosition = {};

	_ushort m_iFallAnimationIndex = {7};
	_ushort m_iIdleAnimationIndex = { 0 };


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CCharacter* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};



END