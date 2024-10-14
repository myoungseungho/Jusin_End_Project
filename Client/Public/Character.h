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

class CCharacter  : public CGameObject
{
public:
	const int BUFFER_SIZE = 30;
	enum AttackGrade {Attack_light =0, Attack_Medium, Attack_Heavy=2, Attack_Special=2, Attack_Command, Attack_Skill, Attack_FinalSkill};
	static const _float fGroundHeight;  //0

	typedef struct: CGameObject::GAMEOBJECT_DESC
	{
		_wstring strModelName;
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
	virtual void InputCommand();
	virtual void InputedCommandUpdate(_float fTimeDelta) ;
	virtual void UpdateInputBuffer(CInput newInput) 
	{
		if (inputBuffer.size() >= BUFFER_SIZE) {
			inputBuffer.erase(inputBuffer.begin());  // 오래된 입력 삭제
		}
		inputBuffer.push_back(newInput);
	}
	virtual bool CheckCommandSkippingExtras(const vector<CInput>& pattern, int timeWindow);
	bool CheckCommandWithStartCondition(const vector<CInput>& pattern, int timeWindow);
	_uint CheckAllCommands();


	//애니메이션
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

	//키프레임 이벤트
	virtual void ProcessEventsFramesZero(_uint characterIndex, _uint animationIndex);
	virtual void ProcessEventsBetweenFrames2(int characterIndex, int animationIndex, float prevFrame, float currentFrame);

	//키프레임 이벤트 파생
	void Create_Effect(_int iEffectIndex);


	_float Get_fHeight();

	//디버그용 코드 
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
	//커맨드 만들면 스택형으로 넣어두고 0.3초안에 지우기

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
	
	//index,시간
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