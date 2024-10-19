#include "stdafx.h"
#include "..\Public\Character.h"

#include "RenderInstance.h"
#include "GameInstance.h"

#include "iostream"

#include "AttackObject.h"


const _float CCharacter::fGroundHeight = 0.f; //0
const _float CCharacter::fJumpPower	 = 3.f; //0


vector<CInput> CCharacter::Command_236Attack =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_NEUTRAL, ATTACK_LIGHT}
};
vector<CInput> CCharacter::Command_236Attack_Extra =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_LIGHT}
};


vector<CInput> CCharacter::Command_214Attack =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_NONE},
	{MOVEKEY_NEUTRAL, ATTACK_LIGHT}
};
vector<CInput> CCharacter::Command_214Attack_Extra =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_LIGHT}
};

vector<CInput> CCharacter::Command_236Special =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_SPECIAL}
};

vector<CInput> CCharacter::Command_236Special_Side =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_DOWN, ATTACK_SPECIAL}
};

vector<CInput> CCharacter::Command_214Special =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_NONE},
	{MOVEKEY_NEUTRAL, ATTACK_SPECIAL}
};

vector<CInput> CCharacter::Command_214Special_Extra =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_SPECIAL}
};




vector<CInput> CCharacter::Command_214FinalAttack =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_GRAB}
};


vector<CInput> CCharacter::Command_236UltimateAttack =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_GRAB}
};


vector<CInput> CCharacter::Command_236UltimateAttack_Side =
{
	{MOVEKEY_DOWN, ATTACK_NONE},
	{MOVEKEY_DOWN_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_DOWN, ATTACK_GRAB}
};



vector<CInput> CCharacter::Command_BackDash =
{
	{MOVEKEY_LEFT, ATTACK_NONE},
	{MOVEKEY_NEUTRAL, ATTACK_NONE},
	{MOVEKEY_LEFT, ATTACK_NONE}
};

vector<CInput> CCharacter::Command_Forward =
{ 
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_NEUTRAL, ATTACK_NONE},
	{ MOVEKEY_RIGHT, ATTACK_NONE }

};

vector<CInput> CCharacter::Command_LightAttack = { {MOVEKEY_NEUTRAL, ATTACK_LIGHT} };
vector<CInput> CCharacter::Command_MediumAttack = { {MOVEKEY_NEUTRAL, ATTACK_MEDIUM} };

vector<CInput> CCharacter::Command_MediumAttack_Extra = { {MOVEKEY_RIGHT, ATTACK_MEDIUM} };

vector<CInput> CCharacter::Command_HeavyAttack = { {MOVEKEY_NEUTRAL, ATTACK_HEAVY} };
vector<CInput> CCharacter::Command_SpecialAttack = { {MOVEKEY_NEUTRAL, ATTACK_SPECIAL} };
			
vector<CInput> CCharacter::Command_HeavyAttack_Extra = { {MOVEKEY_RIGHT, ATTACK_HEAVY} };
			 
vector<CInput> CCharacter::Command_Crouch_LightAttack = { {MOVEKEY_DOWN, ATTACK_LIGHT} };
vector<CInput> CCharacter::Command_Crouch_MediumAttack = { {MOVEKEY_DOWN, ATTACK_MEDIUM} };
vector<CInput> CCharacter::Command_Crouch_HeavyAttack = { {MOVEKEY_DOWN, ATTACK_HEAVY} };
vector<CInput> CCharacter::Command_Crouch_SpecialAttack = { {MOVEKEY_DOWN, ATTACK_SPECIAL} };
			  
vector<CInput> CCharacter::Command_Crouch_MediumAttack_Extra = { {MOVEKEY_DOWN_RIGHT, ATTACK_MEDIUM} };
vector<CInput> CCharacter::Command_Crouch_HeavyAttack_Extra = { {MOVEKEY_DOWN_RIGHT, ATTACK_HEAVY} };




CCharacter::CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CCharacter::CCharacter(const CCharacter& Prototype)
	: CGameObject{ Prototype }
	, m_pFrameEvent{Prototype.m_pFrameEvent }
{

}

HRESULT CCharacter::Initialize_Prototype()
{
	//m_pFrameEvent = CFrameEvent_Manager::Get_Instance()->Get_pFrameEventMap();
	  //ù �����϶�, ������������ ZeroFrame�� ��� �����ϴ°�


	return S_OK;
}

HRESULT CCharacter::Initialize(void* pArg)
{

	Character_DESC* pDesc = static_cast<Character_DESC*>(pArg);
	//m_strModelName = pDesc->strModelName;

	m_pFrameEvent = CFrameEvent_Manager::Get_Instance()->Get_pFrameEventMap();


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	if(pDesc->iTeam == 1)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
		m_iPlayerTeam = 1;
		FlipDirection(1);
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(2.f, 0.f, 0.f, 1.f));
		m_iPlayerTeam = 2;
		FlipDirection(-1);
	}


	//�� �ε带 ���� Ŭ������ �Ű����� ���� ó���ϱ� 
	//m_pModelCom->SetUp_Animation(0, false);
	//m_pModelCom->Play_Animation(0.f);


	inputBuffer.push_back(CInput(MOVEKEY_NEUTRAL, ATTACK_NONE));


	CTransform* pCameraTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), m_strTransformTag, 0));
	pCameraTransform->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _vector{ 0.f, 1.f, -5.f });
	pCameraTransform->LookAt(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	
	return S_OK;
}

void CCharacter::Priority_Update(_float fTimeDelta)
{

}

void CCharacter::Update(_float fTimeDelta)
{
	
	if (m_bAnimationLock == false)
	{

		


	}
	else
	{
		m_fAccAnimationLock += fTimeDelta;
		if (m_fAccAnimationLock > m_fMaxAnimationLock)
		{
			m_fAccAnimationLock = 0.f;
			m_bAnimationLock = false;
		}
	}

}

void CCharacter::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CCharacter::Render(_float fTimeDelta)
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		/* ���� ������ �ִ� ���׸��� �� i��° �޽ð� ����ؾ��ϴ� ���׸�����ü�� aiTextureType_DIFFUSE��° �ؽ��ĸ� */
		/* m_pShaderCom�� �ִ� g_DiffuseTexture������ ����. */
		if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;
		// m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i);

		/* ���� ������ �ִ� ���� �߿��� ���� �������ҷ��� �ߴ� i��°�� �޽ð� ����ϴ� ������ �迭�� ���� ���̴��� �����ش�.  */
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}



_bool CCharacter::CompareNextAnimation(_uint iAnimationIndex, _float fNextPosition)
{
	_bool bCompare = true;

	if (fNextPosition != 0)
	{
		if (m_fNextAnimationCurrentPosition != fNextPosition)
			bCompare = false;
	}

	if (iAnimationIndex != m_iNextAnimation.first)
		bCompare = false;



	return bCompare;

}

void CCharacter::Set_CurrentAnimationPositionJump(_float fAnimationPosition)
{
	m_pModelCom->CurrentAnimationPositionJump(fAnimationPosition);
}

void CCharacter::ProcessEventsFramesZero(_uint characterIndex, _uint animationIndex)
{

	//�ִϸ��̼� ����� 0��° �������̺�Ʈ�� �����ϱ� ����

	auto characterIt = m_pFrameEvent->find(m_eCharacterIndex);   //�׽�Ʈ��
	if (characterIt != m_pFrameEvent->end())
	{
		auto& animationMap = characterIt->second;
		auto animationIt = animationMap.find(animationIndex);
		if (animationIt != animationMap.end())
		{
			auto& frameMap = animationIt->second;

			auto frameIt = frameMap[0];

			for (auto event : frameIt)
			{

				CFrameEvent_Manager::Get_Instance()->UseEvent(event, this);
			}

		}
	}

	

}

void CCharacter::ProcessEventsBetweenFrames2(int characterIndex, int animationIndex, float prevFrame, float currentFrame)
{

	//Frame�� ���缭 �ߺ�ó���Ǵ°���?
   // �������� 0�ΰ�츦 �����ϰ� �������� ����µ� ��Ȯ�ϰ� �̺�Ʈ�� ���� �� �ֳ�?
	   // 0.001�ʴ����� ���� �Ұ����Ѱ� �ƴ�

   //�ذ���
   // 1. �� �Լ��� play animation �ֺ��� �־, �ִϸ��̼��� �����Ѱ�� �� �Լ� ȣ�� ��ü�� ����.  << ����
   // 2. bool���� ���� 1ȸ�� ó��

   //�ϴ� ����.




   //frame�� 0�ΰ��� �ִϸ��̼� ���� ����, �ִϸ��̼� ����� 0�� �̺�Ʈ�� ã�Ƽ� �ϴ°ɷ� ó��.   (ProcessEventsFramesZero)
	if (prevFrame == 0)
	{

		//����,���� �������� ��� 0���� �ִϸ��̼� ��ü�� ������ ��� �Լ� ��
		if (currentFrame == 0)
			return;

		//�ƴϸ� 0������ �̺�Ʈ �����ϰ� �����ϱ� ����  ���������� ���� ����
		else
			prevFrame += 0.001;
	}





	// ĳ���� �ε��� Ž��

	auto characterIt = m_pFrameEvent->find(m_eCharacterIndex);   //�׽�Ʈ��
	if (characterIt != m_pFrameEvent->end())
	{
		auto& animationMap = characterIt->second;
		auto animationIt = animationMap.find(animationIndex);
		if (animationIt != animationMap.end())
		{
			auto& frameMap = animationIt->second;


			//0�϶��� ���� ��������� �����ؾ��ϳ�?
			for (auto frameIt = frameMap.lower_bound(prevFrame); frameIt != frameMap.end() && frameIt->first <= currentFrame; ++frameIt)
			{
				// �ش� �����ӿ����� �̺�Ʈ ����Ʈ�� ���
				for (string event : frameIt->second)
				{
					//MSG_BOX(TEXT("������ " + frameIt->first + "���� �̺�Ʈ �߻�: " + event));

					_bool bdebug1 = false;

					//m_pSelectedModelCom->m_Animations[m_pSelectedModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 100.f;
					//���� 25���� ���ƿ��� �ʴ� ������ ����...?


					CFrameEvent_Manager::Get_Instance()->UseEvent(event, this);

				}
			}

		}
	}

	
}

_bool CCharacter::InputCommand()
{

	ButtonInput iAttackkey = ATTACK_NONE;
	DirectionInput iMoveKey = MOVEKEY_NEUTRAL;

	_int DirectionX=0;
	_int DirectionY=0;


	if (m_iPlayerTeam == 1)
	{

		if (m_pGameInstance->Key_Pressing(DIK_W))
		{
			DirectionY = 1;
		}

		else if (m_pGameInstance->Key_Pressing(DIK_S))
		{
			DirectionY =-1;
		}
		
		if (m_pGameInstance->Key_Pressing(DIK_A))
		{
			DirectionX -= m_iLookDirection;
		}

		else if (m_pGameInstance->Key_Pressing(DIK_D))
		{
			DirectionX += m_iLookDirection;
		}

		 //if(DirectionX==0 && DirectionY ==0)

		 if (DirectionX == -1 && DirectionY == 0)
			 iMoveKey = MOVEKEY_LEFT;

		 else if (DirectionX == -1 && DirectionY == -1)
			 iMoveKey = MOVEKEY_DOWN_LEFT;

		 else if (DirectionX == 0 && DirectionY == -1)
			 iMoveKey = MOVEKEY_DOWN;

		 else if (DirectionX == 0 && DirectionY == -1)
			 iMoveKey = MOVEKEY_DOWN;

		 else if (DirectionX == 1 && DirectionY == -1)
			 iMoveKey = MOVEKEY_DOWN_RIGHT;

		 else if (DirectionX == 1 && DirectionY == 0)
			 iMoveKey = MOVEKEY_RIGHT;


		 //�� �� 0�ΰ��� �⺻���̹Ƿ� �������� ���� �밢�� ���� ���̴� Ŀ�ǵ尡 �����Ƿ� �������� ����



		 if (m_pGameInstance->Key_Down(DIK_U))
		 {
			 iAttackkey = ATTACK_LIGHT;
		 }

		 if (m_pGameInstance->Key_Down(DIK_I))
		 {
			 iAttackkey = ATTACK_MEDIUM;

		 }
		 if (m_pGameInstance->Key_Down(DIK_J))
		 {
			 iAttackkey = ATTACK_SPECIAL;

		 }
		 if (m_pGameInstance->Key_Down(DIK_K))
		 {
			 iAttackkey = ATTACK_HEAVY;

		 }


		 if (m_pGameInstance->Key_Pressing(DIK_O))
		 {
			 iAttackkey = ATTACK_GRAB;
		
		 }
		// if (m_pGameInstance->Key_Pressing(DIK_Y))
		// {
		//	 iAttackkey = ATTACK_LIGHT;
		//
		// }

	}
	else  //2��
	{
		if (m_pGameInstance->Key_Pressing(DIK_UP))
		{
			DirectionY = 1;
		}

		else if (m_pGameInstance->Key_Pressing(DIK_DOWN))
		{
			DirectionY = -1;
		}

		if (m_pGameInstance->Key_Pressing(DIK_LEFT))
		{
			DirectionX -= m_iLookDirection;
		}

		else if (m_pGameInstance->Key_Pressing(DIK_RIGHT))
		{
			DirectionX += m_iLookDirection;
		}

		//if(DirectionX==0 && DirectionY ==0)

		if (DirectionX == -1 && DirectionY == 0)
			iMoveKey = MOVEKEY_LEFT;

		else if (DirectionX == -1 && DirectionY == -1)
			iMoveKey = MOVEKEY_DOWN_LEFT;

		else if (DirectionX == 0 && DirectionY == -1)
			iMoveKey = MOVEKEY_DOWN;

		else if (DirectionX == 1 && DirectionY == -1)
			iMoveKey = MOVEKEY_DOWN_RIGHT;

		else if (DirectionX == 1 && DirectionY == 0)
			iMoveKey = MOVEKEY_RIGHT;


		//�� �� 0�ΰ��� �⺻���̹Ƿ� �������� ���� �밢�� ���� ���̴� Ŀ�ǵ尡 �����Ƿ� �������� ����



		if (m_pGameInstance->Key_Down(DIK_NUMPAD7))
		{
			iAttackkey = ATTACK_LIGHT;
		}

		if (m_pGameInstance->Key_Down(DIK_NUMPAD8))
		{
			iAttackkey = ATTACK_MEDIUM;

		}
		if (m_pGameInstance->Key_Down(DIK_NUMPAD4))
		{
			iAttackkey = ATTACK_SPECIAL;

		}
		if (m_pGameInstance->Key_Down(DIK_NUMPAD5))
		{
			iAttackkey = ATTACK_HEAVY;

		}


		if (m_pGameInstance->Key_Pressing(DIK_NUMPAD9))
		{
			iAttackkey = ATTACK_GRAB;

		}
		// if (m_pGameInstance->Key_Pressing(DIK_Y))
		// {
		//	 iAttackkey = ATTACK_LIGHT;
		//
		// }
	}


	_bool bNewKey = false;

	CInput newInput(iMoveKey, iAttackkey);

	if(inputBuffer.size()>0)
	{
		//������ ���¿� Ű �Է� ���°� �Ȱ����� �������� ����
		if (inputBuffer.back() == newInput)
		{

		}
		else
		{
			UpdateInputBuffer(CInput(iMoveKey, iAttackkey));
			bNewKey = true;
		}
	}
	else
	{
		UpdateInputBuffer(CInput(iMoveKey, iAttackkey));
		bNewKey = true;
	}




	return bNewKey;
}

void CCharacter::InputedCommandUpdate(_float fTimeDelta)
{

	

	inputBuffer.erase(
		remove_if(inputBuffer.begin(), inputBuffer.end(), [fTimeDelta](CInput& input) {
			input.frameTime += fTimeDelta;  // frameTime�� fTimeDelta ���ϱ�
			return input.frameTime >= 0.35f; // 0.3 �̻��� ��� ����
			}),
		inputBuffer.end()
	);

	

}


_bool CCharacter::Character_Play_Animation(_float fTimeDelta)
{

	_bool bAnimationEnd = false;

	_float fPrePosition = m_pModelCom->m_fCurrentAnimPosition;

	_int iOneFrameTeest = 0;

	if (fPrePosition == 0)
	{
		ProcessEventsFramesZero(m_eCharacterIndex, m_pModelCom->m_iCurrentAnimationIndex);
		fPrePosition += 0.001;

		iOneFrameTeest++;
	}

	if (m_pModelCom->Play_Animation_Lick(fTimeDelta))
	{
		//����� ��������, ������    (�Ʊ���� ������ �ƴϿ��µ� �̹��� ������ ��¼��?)
		if (m_pModelCom->m_isLoopAnim)
		{
			fPrePosition = 0.001;
			ProcessEventsFramesZero(m_eCharacterIndex, m_pModelCom->m_iCurrentAnimationIndex);
			iOneFrameTeest++;
		}
		bAnimationEnd = true;
		m_bMotionPlaying = false;
	}
	else
		m_bMotionPlaying = true;

	
	_float fCurPosition = m_pModelCom->m_fCurrentAnimPosition;


	ProcessEventsBetweenFrames2(0, m_pModelCom->m_iCurrentAnimationIndex, fPrePosition, fCurPosition);

	return bAnimationEnd;
}

bool CCharacter::CheckCommandSkippingExtras(const vector<CInput>& pattern, int timeWindow)
{
	if (inputBuffer.size() < pattern.size()) return false;

	int patternIndex = 0;

	// �Է� ���ۿ��� ������ ã�� ����
	for (int i = 0; i < inputBuffer.size(); ++i) {
		const CInput& input = inputBuffer[i];
		const CInput& expected = pattern[patternIndex];

		// ���ʿ��� �Է��� �ǳʶ�
		if (input.direction == expected.direction && input.button == expected.button) {
			// ������ ��ġ�ϸ� ���� �������� �Ѿ
			patternIndex++;

			// ��� ������ ��ġ�ϸ� ����
			if (patternIndex >= pattern.size()) 
			{
				inputBuffer.clear();
				return true;
			}
		}

		
	}

	return false;
}

bool CCharacter::CheckCommand_Exactly(const std::vector<CInput>& pattern, int timeWindow)
{
	if (inputBuffer.size() < pattern.size()) return false;  // �Է� ���۰� ���Ϻ��� ª���� ����


	// �Է� ���� ��ü�� ��ȸ�ϸ鼭 ������ ã��
	for (int i = 0; i <= inputBuffer.size() - pattern.size(); ++i)
	{
		bool isPatternMatched = true;

		// ���� ��ġ���� ������ ��
		for (int j = 0; j < pattern.size(); ++j) {
			const CInput& inputCheck = inputBuffer[i + j];
			const CInput& expected = pattern[j];

			// ���ϰ� ��ġ���� ������ ����
			if (inputCheck.direction != expected.direction || inputCheck.button != expected.button) {
				isPatternMatched = false;
				break;
			}

			
		}

		// ������ ��ġ������ true ��ȯ
		if (isPatternMatched) 
		{
			inputBuffer.clear();
			return true;
		}
	}

	return false;  // ������ ã�� ���ϸ� ����
}


_uint CCharacter::CheckAllCommands()
{

	
	for (const auto& command : MoveCommandPatternsFunction)
	{
		if (CheckCommandSkippingExtras(command.pattern, 0)) {
			command.action();  // �ش� ������ ��Ī�Ǹ� �ش� ��� ����

			return 0;
		}
	}


	//��Ȯ�ؾ��ϸ� �ϴ� ���� ���� ����
	for (const auto& command : MoveCommandPatternsFunction_Exactly)
	{
		if (CheckCommand_Exactly(command.pattern, 0)) {
			command.action();  // �ش� ������ ��Ī�Ǹ� �ش� ��� ����

			return 0;
		}
	}
	return 0;

	
}

void CCharacter::ShowInputBuffer()
{
	inputBuffer;
	m_fGravityTime;
	m_pModelCom->m_iCurrentAnimationIndex;
	m_pModelCom->m_fCurrentAnimPosition;
   	_bool bDebug = true;
}

void CCharacter::DebugPositionReset()
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { (_float)m_iPlayerTeam,0,0,1 });
}

void CCharacter::FlipDirection(_int iDirection)
{
	if (iDirection == 0)
	{
		m_iLookDirection = -m_iLookDirection;

		m_pTransformCom->Set_Scaled(-1, 1, 1);
	}
	else if(m_iLookDirection != iDirection)
	{

		m_iLookDirection = iDirection;

		m_pTransformCom->Set_Scaled(-1, 1, 1);
	}

	//m_pTransformCom->Set_Scaled(-1, 1, 1);

}

void CCharacter::Create_Effect(_int iEffectIndex)
{
	switch (iEffectIndex)
	{
	case 0:
		//Create_kamehameha();
		break;

	case 1:
		//Create_Smash();
		break;

	default:
		break;
	}
}

_float CCharacter::Get_fHeight()
{
	return 	XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

}

void CCharacter::Set_ForcedGravityDown()
{
	//�߷� �ְ���
	//if (m_fGravityTime < 0.305)
	//	m_fGravityTime = 0.305f;

	//���ߴ�ÿ�?
	m_fGravityTime = 0.255f;


}

void CCharacter::Set_ForcveGravityTime(_float fGravityTime)
{
	m_fGravityTime = fGravityTime;
}

void CCharacter::AttckCancleJump()
{

	if (m_pModelCom->m_iCurrentAnimationIndex == m_iStandingMidAttackAnimationIndex)
	{
		
		//1��
		if (m_iPlayerTeam == 1 && m_pGameInstance->Key_Down(DIK_W))
		{

			//Set_fJumpPower(4.f); //�߷�Ver1 ����
			Set_fJumpPower(3.f); //�߷�Ver2 ����

			Set_NextAnimation(m_iJumpAnimationIndex, 0.5f);

			if (m_pGameInstance->Key_Pressing(DIK_A))
			{
				Set_fImpulse(-5.f);
			}

			else if (m_pGameInstance->Key_Pressing(DIK_D))
			{
				Set_fImpulse(5.f);
			}
		}

		//2��
		else if (m_iPlayerTeam == 2 && m_pGameInstance->Key_Down(DIK_UP))
		{
			//Set_fJumpPower(4.f); //�߷�Ver1 ����
			Set_fJumpPower(3.f); //�߷�Ver2 ����

			Set_NextAnimation(m_iJumpAnimationIndex, 0.5f);

			if (m_pGameInstance->Key_Pressing(DIK_LEFT))
			{
				Set_fImpulse(-5.f);
			}

			else if (m_pGameInstance->Key_Pressing(DIK_RIGHT))
			{
				Set_fImpulse(5.f);
			}
		}

	}



}

void CCharacter::Chase(_float fTimeDelta)
{

	m_pModelCom->SetUp_Animation(m_iChaseAnimationIndex, false);
	m_pModelCom->Play_Animation(0.f);


	CTransform* pTarget = static_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Target"), TEXT("Com_Transform")));
	_vector vTargetPos = pTarget->Get_State(CTransform::STATE_POSITION);

	_vector vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	m_fAccChaseTime += fTimeDelta;

	_vector vDir = XMVector4Normalize(vTargetPos - vMyPos);
	Set_fImpulse(XMVectorGetX(vDir) * 2.f);

	//_float angle = atan2(XMVectorGetY(vDir), XMVectorGetX(vDir));
	_float angle = atan2(XMVectorGetY(vDir), XMVectorGetX(vDir)) * (180.0 / 3.14);

	angle = (angle + 90) * 0.5f;
	//cout << angle << endl;



	if (0 < angle && angle < 90)  //���� �����ʿ� �ִ� ���
	{
		FlipDirection(1);
	}

	else if (angle > 90)   //���� ���� ���� �ִ� ��� 
	{
		//110�� ��� 70���� �ٲ�� �Ѵ�.    �ʰ��� 20.   90���κ��� �ʰ��� ��ŭ ���� ��
		// angle = 90 - (90 - angle);    =  180-angle;

		FlipDirection(-1);
		angle = 180 - angle;
	}
	else if (angle < 0)   //���� ���ʿ� �Ʒ��� �ִ� ��� 
	{
		FlipDirection(-1);
		angle = -angle;
	}


	//���� �ӵ��� ���� ������
	vDir = vDir * m_fAccChaseTime * 0.5f;

	m_pTransformCom->Add_MoveVector(vDir);
	Set_CurrentAnimationPositionJump(angle);
}

void CCharacter::Chase2(_float fTimeDelta)
{


	//����׿� ����ó��.  ��������� ������
	if (fTimeDelta > 1)
	{
		return;
	}


	m_fAccChaseTime += fTimeDelta;


	//�غ��ڼ��� �̷��� �Ѵ�.
	if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex)
	{

		if (m_fAccChaseTime > 0.2f)
		{
			m_pModelCom->SetUp_Animation(m_iChaseAnimationIndex, false);
			m_fJumpPower = fJumpPower;
		}
		else
		{
			return;
		}
	}

	//�������̸� �̷��� �Ѵ�
	else if (m_pModelCom->m_iCurrentAnimationIndex == m_iChaseAnimationIndex)
	{
		if (m_fAccChaseTime > 5.f)
		{
			m_bChase = false;
			return;
		}
	}
	else   //�����ߵ� �غ��ڼ��� �ƴϸ�
	{
		return;
		AttackNextMoveCheck();

	}


	//CTransform* pTarget = static_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Target"), TEXT("Com_Transform")));
	CTransform* pTarget = static_cast<CTransform*>(m_pDebugEnemy->Get_Component(TEXT("Com_Transform")));

	_vector vTargetPos = pTarget->Get_State(CTransform::STATE_POSITION);

	_vector vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);



	_float vLength = GetVectorLength((vTargetPos - vMyPos));
	if (vLength < 0.5f) //0.3
	{
		m_bChase = false;



		//�׽�Ʈ
		m_fAccChaseTime = 0.f;
		m_fGravityTime = 0.185f;
		m_pModelCom->SetUp_Animation(m_iFallAnimationIndex, false);


		return;
	}


	m_vChaseDir = XMVector4Normalize(vTargetPos - vMyPos);
	Set_fImpulse(XMVectorGetX(m_vChaseDir) * 2.f);




	//�ִϸ��̼� �̿��� ���� �������� Ư�� ó�� �� �ʿ䰡 ����
	_float angle = atan2(XMVectorGetY(m_vChaseDir), XMVectorGetX(m_vChaseDir)) * (180.0 / 3.14);
	angle = (angle + 90) * 0.5f;

	cout << angle << endl;

	if (0 < angle && angle < 90)  //���� �����ʿ� �ִ� ���
	{
		//ĳ���� ���� ���� ���������� ����
		FlipDirection(1);
	}

	else if (angle > 90)   //���� ���� ���� �ִ� ��� 
	{
		//110�� ��� 70���� �ٲ�� �Ѵ�.    �ʰ��� 20.   90���κ��� �ʰ��� ��ŭ ���� ��
		// angle = 90 - (90 - angle);    =  180-angle;

		FlipDirection(-1);
		angle = 180 - angle;
	}
	else if (angle < 0)   //���� ���ʿ� �Ʒ��� �ִ� ��� 
	{
		FlipDirection(-1);
		angle = -angle;
	}


	//���� �ӵ��� ���� ������
	//m_pTransformCom->Add_MoveVector(m_vChaseDir * m_fAccChaseTime * 0.5f);
	m_pTransformCom->Add_MoveVector(m_vChaseDir * m_fAccChaseTime * m_fAccChaseTime);



	//�ִϸ��̼��� position�� ������ �ǹ��� (1:1�� �ƴϰ� Ư��ó���Ǿ�����)
	Set_CurrentAnimationPositionJump(angle);



	//�ݵ�� Set�̴� ���� �ڵ� �����.
	/*
	//m_bChase �� true�� ���� ���ð�


	
	//����׿� ����ó��.  ��������� ������
	if (fTimeDelta > 1)
	{
		return;
	}

	

	m_fAccChaseTime += fTimeDelta;
	//if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex && m_fAccChaseTime > 0.3f)
	if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex)
	{

		if (m_fAccChaseTime > 0.2f)
		{
			m_pModelCom->SetUp_Animation(m_iChaseAnimationIndex, false);
			m_fJumpPower = fJumpPower;
		}
		else
		{
			return;
		}
	}

	else if (m_fAccChaseTime > 5.f)
	{
		m_bChase = false;
		return;
	}


	//CTransform* pTarget = static_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Target"), TEXT("Com_Transform")));
	CTransform* pTarget = static_cast<CTransform*>(m_pDebugEnemy->Get_Component(TEXT("Com_Transform")));

	_vector vTargetPos = pTarget->Get_State(CTransform::STATE_POSITION);

	_vector vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


	
	_float vLength = GetVectorLength((vTargetPos - vMyPos));
	if (vLength < 0.5f) //0.3
	{
		m_bChase = false;



		//�׽�Ʈ
		m_fAccChaseTime = 0.f;
		m_fGravityTime = 0.185f;
		m_pModelCom->SetUp_Animation(m_iFallAnimationIndex, false);


		return;
	}


	m_vChaseDir = XMVector4Normalize(vTargetPos - vMyPos);
	Set_fImpulse(XMVectorGetX(m_vChaseDir) * 2.f);




	//�ִϸ��̼� �̿��� ���� �������� Ư�� ó�� �� �ʿ䰡 ����
	_float angle = atan2(XMVectorGetY(m_vChaseDir), XMVectorGetX(m_vChaseDir)) * (180.0 / 3.14);
	angle = (angle + 90) * 0.5f;

	cout << angle << endl;

	if (0 < angle && angle < 90)  //���� �����ʿ� �ִ� ���
	{
		//ĳ���� ���� ���� ���������� ����
		FlipDirection(1);
	}

	else if (angle > 90)   //���� ���� ���� �ִ� ��� 
	{
		//110�� ��� 70���� �ٲ�� �Ѵ�.    �ʰ��� 20.   90���κ��� �ʰ��� ��ŭ ���� ��
		// angle = 90 - (90 - angle);    =  180-angle;

		FlipDirection(-1);
		angle = 180 - angle;
	}
	else if (angle < 0)   //���� ���ʿ� �Ʒ��� �ִ� ��� 
	{
		FlipDirection(-1);
		angle = -angle;
	}


	//���� �ӵ��� ���� ������
	//m_pTransformCom->Add_MoveVector(m_vChaseDir * m_fAccChaseTime * 0.5f);
	m_pTransformCom->Add_MoveVector(m_vChaseDir * m_fAccChaseTime * m_fAccChaseTime );



	//�ִϸ��̼��� position�� ������ �ǹ��� (1:1�� �ƴϰ� Ư��ó���Ǿ�����)
	Set_CurrentAnimationPositionJump(angle);

	*/
}

void CCharacter::Chase_Ready(_float fTimeDelta)
{
	
	


	/*
	//�ݵ�� Set���� Next�� �����ϰ������
	//Ready
	m_pModelCom->SetUp_Animation(m_iFallAnimationIndex, false);


	//if (Check_bCurAnimationisGroundMove() || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex)
	//{
	//
	//}
	//else
	//{
	//	Set_NextAnimation(m_iFallAnimationIndex, 1.f);
	//}
	 
	{
		m_bChase = true;

		if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) < 0.5)
		{
			m_pTransformCom->Add_Move({ 0.f,0.6f,0.f });

		}


		//�������� �׽�Ʈ
		{
			CAttacKObject::ATTACK_DESC Desc{};
			Desc.ColliderDesc.fSizeX = 2.0;
			Desc.ColliderDesc.fSizeY = 2.0f;
			Desc.ColliderDesc.Offset = { 0.f,0.6f,0.f };
			Desc.ColliderDesc.pTransform = m_pTransformCom;
			Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0.3f };
			Desc.fhitCharacter_StunTime = 0.3f;
			Desc.iDamage = 400 * Get_DamageScale();
			Desc.fLifeTime = 5.f;
			Desc.ihitCharacter_Motion = { CAttacKObject::HIT_LIGHT };
			Desc.iTeam = m_iPlayerTeam;
			Desc.fAnimationLockTime = 0.1f;
			Desc.pOwner = this;

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
		}
	}

	*/



	//�ݵ�� Set���� Next�� �����ϰ������
	

	if (Check_bCurAnimationisGroundMove() || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex)
	{
		m_pModelCom->SetUp_Animation(m_iFallAnimationIndex, false);

		m_bChase = true;

		if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) < 0.5)
		{
			m_pTransformCom->Add_Move({ 0.f,0.6f,0.f });

		}


		//�������� �׽�Ʈ
		{
			CAttacKObject::ATTACK_DESC Desc{};
			Desc.ColliderDesc.fSizeX = 2.0;
			Desc.ColliderDesc.fSizeY = 2.0f;
			Desc.ColliderDesc.Offset = { 0.f,0.6f,0.f };
			Desc.ColliderDesc.pTransform = m_pTransformCom;
			Desc.fhitCharacter_Impus = { 0.3f * m_iLookDirection,0.3f };
			Desc.fhitCharacter_StunTime = 0.3f;
			Desc.iDamage = 400 * Get_DamageScale();
			Desc.fLifeTime = 5.f;
			Desc.ihitCharacter_Motion = { CAttacKObject::HIT_LIGHT };
			Desc.iTeam = m_iPlayerTeam;
			Desc.fAnimationLockTime = 0.1f;
			Desc.pOwner = this;

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
		}
	}
	else
	{
		Set_NextAnimation(m_iFallAnimationIndex, 1.f);
		m_bChase = true;
	}
	









}

void CCharacter::Move(_float fTimeDelta)
{


	if (Check_bCurAnimationisGroundMove() || m_pModelCom->m_iCurrentAnimationIndex == m_iForwardDashAnimationIndex)
	{
		if (Check_bCurAnimationisGroundMove(m_iNextAnimation.first))
		{



			Reset_AttackCount();

			if (m_iPlayerTeam == 1)
				MoveKey1Team(fTimeDelta);
			else
				MoveKey2Team(fTimeDelta);


		}
	}
}

void CCharacter::MoveKey1Team(_float fTimeDelta)
{
	_short MoveKey = 0;
	if (m_pGameInstance->Key_Pressing(DIK_W) && m_bJumpLock == false)
	{
		m_pTransformCom->Add_Move({ 0,0.3f,0 });

		//Set_fJumpPower(4.f); //�߷�Ver1 ����
		Set_fJumpPower(3.f); //�߷�Ver2 ����


		Set_Animation(m_iJumpAnimationIndex);


		if (m_pGameInstance->Key_Pressing(DIK_A))
		{
			Set_fImpulse(-5.f);
		}

		else if (m_pGameInstance->Key_Pressing(DIK_D))
		{
			Set_fImpulse(5.f);
		}


	}

	else if (m_pGameInstance->Key_Pressing(DIK_S))
	{
		if (m_pModelCom->m_iCurrentAnimationIndex != m_iForwardDashAnimationIndex)
		{
			m_pModelCom->SetUp_Animation(m_iCrouchAnimationIndex, true);
		}
	}

	else
	{
		if (m_pGameInstance->Key_Pressing(DIK_A))
		{
			MoveKey -= m_iLookDirection;
		}

		else if (m_pGameInstance->Key_Pressing(DIK_D))
		{
			MoveKey += m_iLookDirection;
		}


		if (MoveKey == -1)
		{
			m_pModelCom->SetUp_Animation(m_iBackWalkAnimationIndex, false);

			m_iNextAnimation.first = m_iIdleAnimationIndex;

			m_iNextAnimation.second = 100.f;

		}
		else if (MoveKey == 1)
		{
			if (m_pModelCom->m_iCurrentAnimationIndex == m_iForwardDashAnimationIndex)
			{
				m_pModelCom->SetUp_Animation(m_iForwardDashAnimationIndex, true);
			}
			else
				m_pModelCom->SetUp_Animation(m_iForwardWalkAnimationIndex, false);

			m_iNextAnimation.first = m_iIdleAnimationIndex;
			m_iNextAnimation.second = 100.f;
		}
		else
		{
			if (m_pModelCom->m_iCurrentAnimationIndex == m_iForwardDashAnimationIndex)
			{
				m_pModelCom->SetUp_Animation(m_iForwardDashEndAnimationIndex, false);
			}
			else
				m_pModelCom->SetUp_Animation(m_iIdleAnimationIndex, true);

			m_iNextAnimation.first = m_iIdleAnimationIndex;
			m_iNextAnimation.second = 100.f;
		}
	}
}

void CCharacter::MoveKey2Team(_float fTimeDelta)
{
	_short MoveKey = 0;
	if (m_pGameInstance->Key_Pressing(DIK_UP) && m_bJumpLock == false)
	{
		m_pTransformCom->Add_Move({ 0,0.3f,0 });

		//Set_fJumpPower(4.f); //�߷�Ver1 ����
		Set_fJumpPower(3.f); //�߷�Ver2 ����


		Set_Animation(m_iJumpAnimationIndex);


		if (m_pGameInstance->Key_Pressing(DIK_LEFT))
		{
			Set_fImpulse(-5.f);
		}

		else if (m_pGameInstance->Key_Pressing(DIK_RIGHT))
		{
			Set_fImpulse(5.f);
		}


	}

	else if (m_pGameInstance->Key_Pressing(DIK_DOWN))
	{
		if (m_pModelCom->m_iCurrentAnimationIndex != m_iForwardDashAnimationIndex)
		{
			m_pModelCom->SetUp_Animation(m_iCrouchAnimationIndex, true);
		}
	}

	else
	{
		if (m_pGameInstance->Key_Pressing(DIK_LEFT))
		{
			MoveKey -= m_iLookDirection;
		}

		else if (m_pGameInstance->Key_Pressing(DIK_RIGHT))
		{
			MoveKey += m_iLookDirection;
		}


		if (MoveKey == -1)
		{
			m_pModelCom->SetUp_Animation(m_iBackWalkAnimationIndex, false);

			m_iNextAnimation.first = m_iIdleAnimationIndex;

			m_iNextAnimation.second = 100.f;

		}
		else if (MoveKey == 1)
		{
			if (m_pModelCom->m_iCurrentAnimationIndex == m_iForwardDashAnimationIndex)
			{
				m_pModelCom->SetUp_Animation(m_iForwardDashAnimationIndex, true);
			}
			else
				m_pModelCom->SetUp_Animation(m_iForwardWalkAnimationIndex, false);

			m_iNextAnimation.first = m_iIdleAnimationIndex;
			m_iNextAnimation.second = 100.f;
		}
		else
		{
			if (m_pModelCom->m_iCurrentAnimationIndex == m_iForwardDashAnimationIndex)
			{
				m_pModelCom->SetUp_Animation(m_iForwardDashEndAnimationIndex, false);
			}
			else
				m_pModelCom->SetUp_Animation(m_iIdleAnimationIndex, true);

			m_iNextAnimation.first = m_iIdleAnimationIndex;
			m_iNextAnimation.second = 100.f;
		}
	}

}




void CCharacter::Set_Hit(_uint eAnimation, _float fStunTime, _uint iDamage, _float fStopTime, _float2 Impus)
{
	m_bStun = true;

	m_fMaxStunTime = fStunTime;

	Set_HitAnimation(eAnimation, Impus);
	Set_AnimationStop(fStopTime);


	m_iHP -= iDamage;  // ���⿡ �޺���� ���Ұ�

	if (m_iHP < 0)
	{
		m_iHP = 0;
	}
	
}

void CCharacter::Set_HitAnimation(_uint eAnimation, _float2 Impus)
{

	//�ϳ��� 0�� �ƴϸ� ����
	if (Impus.x != 0 || Impus.y != 0)
	{
		m_fImpuse = Impus;
	}


	if (eAnimation == HIT_LIGHT)
	{
		if (m_pModelCom->m_iCurrentAnimationIndex == m_iCrouchAnimationIndex)
		{
			Set_Animation(m_iHit_Crouch_AnimationIndex, false);
		}
		else
			Set_Animation(m_iHit_Stand_LightAnimationIndex, false);
	}

	else if (eAnimation == HIT_HEAVY)
	{

	}

	else if (eAnimation == HIT_KNOCK_AWAY_LEFT)
	{
		Set_Animation(m_iHit_Away_LeftAnimationIndex, false);

	}
	else if (eAnimation == HIT_KNOCK_AWAY_UP)
	{
		Set_Animation(m_iHit_Away_UpAnimationIndex, false);

	}


}

void CCharacter::Set_AnimationStop(_float fStopTime)
{
	m_bAnimationLock = true;
	m_fMaxAnimationLock = fStopTime;
	m_fAccAnimationLock = 0.f;

	m_pModelCom->Play_Animation(0.f);
}





void CCharacter::Check_StunEnd()
{

}

void CCharacter::Stun_Shake()
{
	m_bStunShakeDirection = !m_bStunShakeDirection;

	m_pTransformCom->Add_Move({ 0.02f-m_bStunShakeDirection*(0.04f),0,0 });

}

void CCharacter::Update_AnimationLock(_float fTimeDelta)
{
	m_fAccAnimationLock += fTimeDelta;
	if (m_fAccAnimationLock > m_fMaxAnimationLock)
	{
		m_bAnimationLock = false;
		m_fAccAnimationLock = 0.f;
	}

}

void CCharacter::Update_StunImpus(_float fTimeDelta)
{
	m_pTransformCom->Add_Move({ m_fImpuse.x *fTimeDelta, m_fImpuse.y * fTimeDelta, 0 });
}

_float CCharacter::Get_DamageScale()
{


	//������ ���ϴ°� ��� 1��Ʈ�� 1�� �ƴѵ� ���� ���ص� �ǳ�  //����ŷ�� �ִµ� �޴��ʿ��� ���ϴ°� �ƴ϶� �������ʿ� ���ؾ��ϴ°� �ƴ�?
	//��������
	//Step Count	0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17 +
	//Next Hit		0%	10%	20% 30% 40% 50% 60% 70% 70% 70% 70% 75% 75% 75% 80% 80% 80% 85%
	//����������    1.0 0.9 0.8 0              0.3   


	_float fDamageScale;// = 1.f;

	if (m_iAttackStepCount <= 7) 
	{
		fDamageScale = 1.0f - m_iAttackStepCount * 0.1f;  
	}

	else if (m_iAttackStepCount <= 10) 
	{
		fDamageScale = 0.3f;  
	}

	else if (m_iAttackStepCount <= 13) 
	{
		fDamageScale = 0.25f;
	}

	else if (m_iAttackStepCount <= 16)
	{
		fDamageScale = 0.2f;  
	}
	else 
	{
		fDamageScale = 0.15f; 
	}


	if (m_bSparking)
	{
		//fDamageScale += 0.2f;   //�տ���. �ʹ� ū��?  15%->35%
		fDamageScale *= 1.2f;	  //������ .  15%->16%   �ʹ� ������ ������ ���۹ݿ�.
	}


	return fDamageScale;
}


_uint* CCharacter::Get_pAnimationIndex()
{
	return &(m_pModelCom->m_iCurrentAnimationIndex);
}



void CCharacter::Set_NextAnimation(_uint iAnimationIndex, _float fLifeTime, _float fAnimationPosition)
{
	m_iNextAnimation.first = iAnimationIndex;
	m_iNextAnimation.second = fLifeTime;


	m_fNextAnimationCurrentPosition = fAnimationPosition;

}

void CCharacter::Gravity(_float fTimeDelta)
//void CCharacter::Gravity(_float fTimeDeltaOrigin)
{
	
	//_float fTimeDelta = fTimeDeltaOrigin * 2.f;


	if (m_bChase == true)
	{
		return;
	}

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float fHeight = XMVectorGetY(vPos);


	
	//�׽�Ʈ�� �ӽ� �߷�

	if (fHeight > 0)
	{


		

		// IDLE�̸� ���� �ϰ�������� ����
		if (Check_bCurAnimationisGroundMove())
			m_pModelCom->SetUp_Animation(m_iFallAnimationIndex, false);
		
		

		//�߷� ver1 �϶��� �׻� ���ؾ� �ڿ������ 2������ �ƴ�
		//if (m_fGravityTime < m_fJumpPower)
		//{
		//	m_fGravityTime += fTimeDelta;
		//}


		//ver1  �������� �ʹ� �սǵս���  ������4 ���̽�.
		//_float fGravity = ( - 0.3f * (m_fGravityTime - m_fJumpPower) * (m_fGravityTime - m_fJumpPower) + 2) *0.03f;

		//ver  ������ �ӵ����� �� �������� ��ü������ ����, ������ 3 ���̽�.
		//_float fGravity = (-0.7f * (m_fGravityTime - m_fJumpPower) * (m_fGravityTime - m_fJumpPower) + 4) * 0.03f;

		//Ver3  ������ 3
		//_float fGravity = (-0.7f * (m_fGravityTime - m_fJumpPower) * (m_fGravityTime - m_fJumpPower) + 4) * 0.05f;


		//1~3¥�� ���� �Ǽ��� Gravity �ι��� ȣ����.  

		//Ver4 ������3.   ������ ���� *0.1 ��� *0.08�ص� �ڿ�������
		//_float fGravity = (-0.7f * (m_fGravityTime - m_fJumpPower) * (m_fGravityTime - m_fJumpPower) + 4) * 0.1;
		_float fGravity = (-0.7f * (2*m_fGravityTime - m_fJumpPower) * (2*m_fGravityTime - m_fJumpPower) + 4) * 0.1;


		//�ϰ� ������̸� ���� �߶�,  ��� �ϰ� ���ÿ� ó���ϰ������
		if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex)
		{

			//�߷�ver2 ��
			//if (m_fGravityTime < m_fJumpPower)
			if (m_fGravityTime*2 < m_fJumpPower)
			{
				m_fGravityTime += fTimeDelta;
			}

			
			m_pTransformCom->Add_Move({ m_fImpuse.x * fTimeDelta,-fGravity,0 });


		}
		else
		{
		
			//�߷�Ver2 ���� ó��.  �ö󰡴ٰ� ���ݶ����� ����µ�  ���� ������ �ٽ� �ö󰡴°� �̻��ؼ� ó��
			//if (fGravity < 0 && m_fGravityTime < m_fJumpPower)

			if(m_bAttackGravity == true)
			{ 

				if(m_iPlayerTeam ==1)
				{
					if ((m_pGameInstance->Key_Pressing(DIK_W) || (fGravity < 0 && m_fGravityTime * 2 < m_fJumpPower)))
					{
						m_fGravityTime += fTimeDelta;
					}


					//��� �����߿� �߷�����.  Ư�� ��Ǹ� �Ϸ��� �� Ŭ�������� override �ʿ�

					if (m_pGameInstance->Key_Pressing(DIK_W))
						m_pTransformCom->Add_Move({ m_fImpuse.x * fTimeDelta,-fGravity,0 });

				}
				else
				{
					if ((m_pGameInstance->Key_Pressing(DIK_UP) || (fGravity < 0 && m_fGravityTime * 2 < m_fJumpPower)))
					{
						m_fGravityTime += fTimeDelta;
					}


					//��� �����߿� �߷�����.  Ư�� ��Ǹ� �Ϸ��� �� Ŭ�������� override �ʿ�

					if (m_pGameInstance->Key_Pressing(DIK_UP))
						m_pTransformCom->Add_Move({ m_fImpuse.x * fTimeDelta,-fGravity,0 });
				}

			}
			//���Ӹ� �ް� �߷��� ���δ� �ڵ�. ��� ��ǿ� ���ӵ� �����Ҳ� �ƴϸ� ������ �̻��ϰ� ����.
			//m_pTransformCom->Add_Move({ m_fImpuse * fTimeDelta,0,0 });


		}



	}
	else if (fHeight <0)
	{

		if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || Check_bCurAnimationisAirAttack())
		{
			m_pModelCom->SetUp_Animation(m_iIdleAnimationIndex, true);

			Set_fGravityTime(0.f);
			Set_fJumpPower(0.f);
			Set_fImpulse(0.f);

			Set_NextAnimation(m_iIdleAnimationIndex, 2.f);
			m_bAriDashEnable = true;
			Set_bAttackGravity(true);

			if (m_bJumpLock == false)
			{
				m_bJumpLock = true;
			}

		}


		//m_pTransformCom->Add_Move({ 0,-fHeight,0 });

		//m_pTransformCom->Add_Move({ 0,-fHeight,0 });

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { XMVectorGetX(vPos),0.f,XMVectorGetZ(vPos),1.f });
		
	}

	else if (fHeight == 0)
	{
		if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex)
		{
			m_pModelCom->SetUp_Animation(m_iIdleAnimationIndex, true);

			Set_fGravityTime(0.f);
			Set_fJumpPower(0.f);
			Set_fImpulse(0.f);
			m_bAriDashEnable = true;
			Set_bAttackGravity(true);
		}

		if (m_bJumpLock)
		{
			m_fAccAnimationLock += fTimeDelta;

			if (m_fAccAnimationLock > 0.15f)
			{
				m_bJumpLock = false;
				m_fAccAnimationLock = 0.f;
			}
		}

	}



}



HRESULT CCharacter::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
	//	return E_FAIL;

	/* Com_Model */
	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_strModelName,
	//	TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
	//	return E_FAIL;




	CCollider_Test::COLLIDER_DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.fSizeX = 1.2f;
	ColliderDesc.fSizeY = 1.5f;
	ColliderDesc.fSizeZ = 0.7f;
	ColliderDesc.Offset = { 0.f, 0.7f, 0.f };


	//Com_Collider
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CCharacter::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CCharacter* CCharacter::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCharacter* pInstance = new CCharacter(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCharacter"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCharacter::Clone(void* pArg)
{
	CCharacter* pInstance = new CCharacter(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCharacter"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCharacter::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
