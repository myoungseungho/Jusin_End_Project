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

	typedef struct: CGameObject::GAMEOBJECT_DESC
	{
		_wstring strModelName;
	}Character_DESC;

	struct CommandPattern {
		vector<CInput> pattern;  // �Է� ����
		//std::function<void()> action;  // �ش� ������ �������� �� ������ �Լ�
		_uint AnimationIndex;
	};

	//struct CommandPatternFunction {
	//	vector<CInput> pattern;  // �Է� ����
	//	std::function<void()> action;  // �ش� ������ �������� �� ������ �Լ�
	//	//_uint AnimationIndex;
	//};

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

	virtual void NextMoveCheck();
	virtual void ProcessEventsFramesZero(_uint characterIndex, _uint animationIndex);
	virtual void ProcessEventsBetweenFrames2(int characterIndex, int animationIndex, float prevFrame, float currentFrame);
	virtual void InputCommand();

	virtual void UpdateInputBuffer(CInput newInput) 
	{
		if (inputBuffer.size() >= BUFFER_SIZE) {
			inputBuffer.erase(inputBuffer.begin());  // ������ �Է� ����
		}
		inputBuffer.push_back(newInput);
	}

	virtual _bool Character_Play_Animation(_float fTimeDelta);
	virtual void Set_Animation(_uint iAnimationIndex) {};
	virtual bool CheckCommandSkippingExtras(const vector<CInput>& pattern, int timeWindow);

	bool CheckCommandWithStartCondition(const vector<CInput>& pattern, int timeWindow);

	_int Get_iDirection() { return m_iLookDirection; };
	_uint CheckAllCommands() 
	{
		for (const auto& command : MoveCommandPatterns) 
		{
			if (CheckCommandSkippingExtras(command.pattern,0)) {
				//command.action();  // �ش� ������ ��Ī�Ǹ� �ش� ��� ����
				
				return command.AnimationIndex;
			}
		}
		return 0;
	}

	virtual void ShowInputBuffer();

	virtual void DebugPositionReset();

protected:
	CShader*				m_pShaderCom = { nullptr };	
	CModel*					m_pModelCom = { nullptr };

	_float					m_fRandom = {};
	_wstring				m_strModelName{};

	_uint					m_iMoveGrade = {};
	_uint					m_iCurMoveIndex{};
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
	//vector<CommandPatternFunction> MoveCommandPatternsFunction;

	//_uint					m_iNextAnimationIndex = { 0 };
	
	//index,�ð�
	pair<_uint, _float>		m_iNextAnimation{ 0,0 };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CCharacter* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END