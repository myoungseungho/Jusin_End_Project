#include "stdafx.h"
#include "..\Public\Character.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "iostream"
#include "AttackObject.h"
#include "UI_Manager.h"


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
		
vector<CInput> CCharacter::Command_Grab = { {MOVEKEY_NEUTRAL, ATTACK_GRAB} };


vector<CInput> CCharacter::Command_Crouch_LightAttack = { {MOVEKEY_DOWN, ATTACK_LIGHT} };
vector<CInput> CCharacter::Command_Crouch_MediumAttack = { {MOVEKEY_DOWN, ATTACK_MEDIUM} };
vector<CInput> CCharacter::Command_Crouch_HeavyAttack = { {MOVEKEY_DOWN, ATTACK_HEAVY} };
vector<CInput> CCharacter::Command_Crouch_SpecialAttack = { {MOVEKEY_DOWN, ATTACK_SPECIAL} };
			  
vector<CInput> CCharacter::Command_Crouch_MediumAttack_Extra = { {MOVEKEY_DOWN_RIGHT, ATTACK_MEDIUM} };
vector<CInput> CCharacter::Command_Crouch_HeavyAttack_Extra = { {MOVEKEY_DOWN_RIGHT, ATTACK_HEAVY} };




CCharacter::CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
	, m_pUI_Manager{ CUI_Manager::Get_Instance() }
{
	Safe_AddRef(m_pUI_Manager);
}

CCharacter::CCharacter(const CCharacter& Prototype)
	: CGameObject{ Prototype }
	, m_pFrameEvent{Prototype.m_pFrameEvent }
	, m_pUI_Manager{ CUI_Manager::Get_Instance() }
{
	Safe_AddRef(m_pUI_Manager);
}

HRESULT CCharacter::Initialize_Prototype()
{
	//m_pFrameEvent = CFrameEvent_Manager::Get_Instance()->Get_pFrameEventMap();
	  //첫 실행일때, 루프돌았을때 ZeroFrame을 어떻게 구분하는가


	return S_OK;
}

HRESULT CCharacter::Initialize(void* pArg)
{
	Character_DESC* pDesc = static_cast<Character_DESC*>(pArg);
	//m_strModelName = pDesc->strModelName;

	m_pFrameEvent = CFrameEvent_Manager::Get_Instance()->Get_pFrameEventMap();

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Character_DESC* pSlotDesc = static_cast<Character_DESC*>(pArg);
	m_ePlayerSlot = pSlotDesc->ePlayerSlot;
	m_tCharacterDesc.ePlayer_Slot = m_ePlayerSlot;

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


	//모델 로드를 하위 클래스로 옮겼으니 각자 처리하기 
	//m_pModelCom->SetUp_Animation(0, false);
	//m_pModelCom->Play_Animation(0.f);


	inputBuffer.push_back(CInput(MOVEKEY_NEUTRAL, ATTACK_NONE));

	return S_OK;
}

void CCharacter::Priority_Update(_float fTimeDelta)
{
	m_tCharacterDesc.bStun = m_bRedHp;
	m_tCharacterDesc.bHit = m_bHit;
	m_tCharacterDesc.bAttBuf = m_bAttBuf;
	m_tCharacterDesc.iHp = m_iHP;
	m_tCharacterDesc.iComboCount = m_iComboCount;
	m_tCharacterDesc.iSKillCount = m_iSKillCount;
	m_tCharacterDesc.iSKillPoint = m_iSKillPoint;
	m_tCharacterDesc.ePlayer_Slot = m_ePlayerSlot;
	m_tCharacterDesc.ePlayerID = m_eCharacterID;
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
		/* 모델이 가지고 있는 머테리얼 중 i번째 메시가 사용해야하는 머테리얼구조체의 aiTextureType_DIFFUSE번째 텍스쳐를 */
		/* m_pShaderCom에 있는 g_DiffuseTexture변수에 던져. */
		if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;
		// m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i);

		/* 모델이 가지고 있는 뼈들 중에서 현재 렌더링할려고 했던 i번째ㅑ 메시가 사용하는 뼈들을 배열로 만들어서 쉐이더로 던져준다.  */
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

	//애니메이션 변경시 0번째 프레임이벤트를 실행하기 위함

	auto characterIt = m_pFrameEvent->find(m_eCharacterIndex);   //테스트중
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

	//Frame이 멈춰서 중복처리되는경우는?
   // 프레임이 0인경우를 제외하고 프레임이 멈췄는데 정확하게 이벤트가 있을 수 있나?
	   // 0.001초단위로 보면 불가능한건 아님

   //해결방법
   // 1. 이 함수를 play animation 주변에 넣어서, 애니메이션이 정지한경우 이 함수 호출 자체를 안함.  << 정석
   // 2. bool값을 만들어서 1회만 처리

   //일단 보류.




   //frame이 0인경우는 애니메이션 실행 말고, 애니메이션 변경시 0의 이벤트만 찾아서 하는걸로 처리.   (ProcessEventsFramesZero)
	if (prevFrame == 0)
	{

		//과거,현재 프레임이 모두 0으로 애니메이션 자체가 정지된 경우 함수 끝
		if (currentFrame == 0)
			return;

		//아니면 0프레임 이벤트 제외하고 실행하기 위해  과거프레임 조금 증가
		else
			prevFrame += 0.001;
	}





	// 캐릭터 인덱스 탐색

	auto characterIt = m_pFrameEvent->find(m_eCharacterIndex);   //테스트중
	if (characterIt != m_pFrameEvent->end())
	{
		auto& animationMap = characterIt->second;
		auto animationIt = animationMap.find(animationIndex);
		if (animationIt != animationMap.end())
		{
			auto& frameMap = animationIt->second;


			//0일때를 따로 만들었으니 제외해야하나?
			for (auto frameIt = frameMap.lower_bound(prevFrame); frameIt != frameMap.end() && frameIt->first <= currentFrame; ++frameIt)
			{
				// 해당 프레임에서의 이벤트 리스트를 출력
				for (string event : frameIt->second)
				{
					//MSG_BOX(TEXT("프레임 " + frameIt->first + "에서 이벤트 발생: " + event));

					_bool bdebug1 = false;

					//m_pSelectedModelCom->m_Animations[m_pSelectedModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 100.f;
					//현재 25에서 돌아오지 않는 문제가 있음...?


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

		 else if (DirectionX == 0 && DirectionY == 1)
			 iMoveKey = MOVEKEY_UP;

		 else if (DirectionX == -1 && DirectionY == -1)
			 iMoveKey = MOVEKEY_DOWN_LEFT;

		 else if (DirectionX == 0 && DirectionY == -1)
			 iMoveKey = MOVEKEY_DOWN;

		 else if (DirectionX == 1 && DirectionY == -1)
			 iMoveKey = MOVEKEY_DOWN_RIGHT;

		 else if (DirectionX == 1 && DirectionY == 0)
			 iMoveKey = MOVEKEY_RIGHT;

		 else if (DirectionX == -1 && DirectionY == 1)
			 iMoveKey = MOVEKEY_UP_LEFT;

		 else if (DirectionX == 1 && DirectionY == 1)
			 iMoveKey = MOVEKEY_UP_RIGHT;

		 //둘 다 0인경우는 기본값이므로 지정하지 않음 대각선 위는 쓰이는 커맨드가 없으므로 지정하지 않음



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


		 //if (m_pGameInstance->Key_Pressing(DIK_O))
		 if (m_pGameInstance->Key_Down(DIK_O))
		 {
			 iAttackkey = ATTACK_GRAB;
		
		 }
		// if (m_pGameInstance->Key_Pressing(DIK_Y))
		// {
		//	 iAttackkey = ATTACK_LIGHT;
		//
		// }

		 GetUI_Input(DirectionX, DirectionY, iMoveKey, iAttackkey);

	}
	else  //2팀
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

		else if (DirectionX == 0 && DirectionY == 1)
			iMoveKey = MOVEKEY_UP;

		else if (DirectionX == -1 && DirectionY == -1)
			iMoveKey = MOVEKEY_DOWN_LEFT;

		else if (DirectionX == 0 && DirectionY == -1)
			iMoveKey = MOVEKEY_DOWN;

		else if (DirectionX == 1 && DirectionY == -1)
			iMoveKey = MOVEKEY_DOWN_RIGHT;

		else if (DirectionX == 1 && DirectionY == 0)
			iMoveKey = MOVEKEY_RIGHT;

		else if (DirectionX == -1 && DirectionY == 1)
			iMoveKey = MOVEKEY_UP_LEFT;

		else if (DirectionX == 1 && DirectionY == 1)
			iMoveKey = MOVEKEY_UP_RIGHT;


		//둘 다 0인경우는 기본값이므로 지정하지 않음 대각선 위는 쓰이는 커맨드가 없으므로 지정하지 않음



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


		if (m_pGameInstance->Key_Down(DIK_NUMPAD9))
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
		//마지막 상태와 키 입력 상태가 똑같으면 갱신하지 않음
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
			input.frameTime += fTimeDelta;  // frameTime에 fTimeDelta 더하기
			return input.frameTime >= 0.35f; // 0.3 이상인 경우 삭제
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
		//모션이 끝났으면, 루프면    (아까까진 루프가 아니였는데 이번에 루프면 어쩌지?)
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

	// 입력 버퍼에서 패턴을 찾기 시작
	for (int i = 0; i < inputBuffer.size(); ++i) {
		const CInput& input = inputBuffer[i];
		const CInput& expected = pattern[patternIndex];

		// 불필요한 입력은 건너뜀
		if (input.direction == expected.direction && input.button == expected.button) {
			// 패턴이 일치하면 다음 패턴으로 넘어감
			patternIndex++;

			// 모든 패턴이 일치하면 성공
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
	if (inputBuffer.size() < pattern.size()) return false;  // 입력 버퍼가 패턴보다 짧으면 실패


	// 입력 버퍼 전체를 순회하면서 패턴을 찾음
	for (int i = 0; i <= inputBuffer.size() - pattern.size(); ++i)
	{
		bool isPatternMatched = true;

		// 현재 위치부터 패턴을 비교
		for (int j = 0; j < pattern.size(); ++j) {
			const CInput& inputCheck = inputBuffer[i + j];
			const CInput& expected = pattern[j];

			// 패턴과 일치하지 않으면 실패
			if (inputCheck.direction != expected.direction || inputCheck.button != expected.button) {
				isPatternMatched = false;
				break;
			}

			
		}

		// 패턴이 일치했으면 true 반환
		if (isPatternMatched) 
		{
			inputBuffer.clear();
			return true;
		}
	}

	return false;  // 패턴을 찾지 못하면 실패
}


_uint CCharacter::CheckAllCommands()
{

	
	for (const auto& command : MoveCommandPatternsFunction)
	{
		if (CheckCommandSkippingExtras(command.pattern, 0)) {
			command.action();  // 해당 패턴이 매칭되면 해당 기술 실행

			return 0;
		}
	}


	//정확해야하만 하는 패턴 따로 구분
	for (const auto& command : MoveCommandPatternsFunction_Exactly)
	{
		if (CheckCommand_Exactly(command.pattern, 0)) {
			command.action();  // 해당 패턴이 매칭되면 해당 기술 실행

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
	_float fHeight = Get_fHeight();
   	_bool bDebug = true;
}

void CCharacter::DebugPositionReset()
{
	_float fHegiht = Get_fHeight();
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { (_float)m_iPlayerTeam,fHegiht,0,1 });

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
	//중력 최고점
	//if (m_fGravityTime < 0.305)
	//	m_fGravityTime = 0.305f;

	//공중 앞대시 
	m_fGravityTime = 0.255f;


}

void CCharacter::Set_ForcedGravityTime_LittleUp()
{
	m_fGravityTime = 0.135f;

	if (m_fJumpPower == 0)
	{
		m_fJumpPower = fJumpPower;
	}
}

void CCharacter::Set_ForcveGravityTime(_float fGravityTime)
{
	m_fGravityTime = fGravityTime;
}

void CCharacter::AttckCancleJump()
{
	
	if (m_pModelCom->m_iCurrentAnimationIndex == m_iStandingMidAttackAnimationIndex )
	{
		
		//1팀
		if (m_iPlayerTeam == 1 && m_pGameInstance->Key_Down(DIK_W))
		{

			//Set_fJumpPower(4.f); //중력Ver1 기준
			Set_fJumpPower(3.f); //중력Ver2 기준


			//if (m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex && m_bDoubleJumpEnable)
			//{
			//	//Set_NextAnimation(m_iJumpAnimationIndex, 0.5f);
			//}
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

		//2팀
		else if (m_iPlayerTeam == 2 && m_pGameInstance->Key_Down(DIK_UP))
		{
			//Set_fJumpPower(4.f); //중력Ver1 기준
			Set_fJumpPower(3.f); //중력Ver2 기준

			//if(m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex && m_bDoubleJumpEnable)
			//{
			//	//Set_NextAnimation(m_iJumpAnimationIndex, 0.5f);
			//}
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
	

	else if (m_bDoubleJumpEnable && (
		m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex ||
		m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_Air1  || m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_Air2 || m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_AirUpper))
	{
		
		//1팀
		if (m_iPlayerTeam == 1 && m_pGameInstance->Key_Down(DIK_W))
		{

			//Set_fJumpPower(4.f); //중력Ver1 기준
			Set_fJumpPower(3.f); //중력Ver2 기준
			//Set_ForcveGravityTime(0.08f); //묘하게 낮은듯
			Set_ForcveGravityTime(0.03f);


			//Set_ForcedGravityTime_LittleUp() //0.135f

			//if (m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex && m_bDoubleJumpEnable)
			//{
			//	//Set_NextAnimation(m_iJumpAnimationIndex, 0.5f);
			//}
			Set_NextAnimation(m_iJumpAnimationIndex, 0.5f);



			if (m_pGameInstance->Key_Pressing(DIK_A))
			{
				Set_fImpulse(-5.f);
			}

			else if (m_pGameInstance->Key_Pressing(DIK_D))
			{
				Set_fImpulse(5.f);
			}
			else
				Set_fImpulse(0.f);


			m_bDoubleJumpEnable = false;

		}

		//2팀
		else if (m_iPlayerTeam == 2 && m_pGameInstance->Key_Down(DIK_UP))
		{
			//Set_fJumpPower(4.f); //중력Ver1 기준
			Set_fJumpPower(3.f); //중력Ver2 기준
			Set_ForcveGravityTime(0.03f);

			Set_NextAnimation(m_iJumpAnimationIndex, 0.5f);

			if (m_pGameInstance->Key_Pressing(DIK_LEFT))
			{
				Set_fImpulse(-5.f);
			}

			else if (m_pGameInstance->Key_Pressing(DIK_RIGHT))
			{
				Set_fImpulse(5.f);
			}
			else
				Set_fImpulse(0.f);


			m_bDoubleJumpEnable = false;

		}

		
	}

}

/*

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



	if (0 < angle && angle < 90)  //적이 오른쪽에 있는 경우
	{
		FlipDirection(1);
	}

	else if (angle > 90)   //적이 왼쪽 위에 있는 경우 
	{
		//110의 경우 70으로 바꿔야 한다.    초과값 20.   90으로부터 초과값 만큼 빼면 됨
		// angle = 90 - (90 - angle);    =  180-angle;

		FlipDirection(-1);
		angle = 180 - angle;
	}
	else if (angle < 0)   //적이 왼쪽에 아래에 있는 경우 
	{
		FlipDirection(-1);
		angle = -angle;
	}


	//추적 속도를 점점 빠르게
	vDir = vDir * m_fAccChaseTime * 0.5f;

	m_pTransformCom->Add_MoveVector(vDir);
	Set_CurrentAnimationPositionJump(angle);
}
*/
void CCharacter::Chase2(_float fTimeDelta)
{


	//디버그용 예외처리.  멈춰버리면 지랄남
	if (fTimeDelta > 1)
	{
		return;
	}


	m_fAccChaseTime += fTimeDelta;


	//준비자세면 이렇게 한다.
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

	//돌진중이면 이렇게 한다
	else if (m_pModelCom->m_iCurrentAnimationIndex == m_iChaseAnimationIndex)
	{
		if (m_fAccChaseTime > 5.f)
		{
			m_bChase = false;
			return;
		}
	}
	else   //돌진중도 준비자세도 아니면
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



		//테스트
		m_fAccChaseTime = 0.f;
		m_fGravityTime = 0.185f;
		m_pModelCom->SetUp_Animation(m_iFallAnimationIndex, false);


		return;
	}


	m_vChaseDir = XMVector4Normalize(vTargetPos - vMyPos);
	Set_fImpulse(XMVectorGetX(m_vChaseDir) * 2.f);




	//애니메이션 이용을 위해 각도값을 특수 처리 할 필요가 있음
	_float angle = atan2(XMVectorGetY(m_vChaseDir), XMVectorGetX(m_vChaseDir)) * (180.0 / 3.14);
	angle = (angle + 90) * 0.5f;

	cout << angle << endl;

	if (0 < angle && angle < 90)  //적이 오른쪽에 있는 경우
	{
		//캐릭터 보는 방향 오른쪽으로 변경
		FlipDirection(1);
	}

	else if (angle > 90)   //적이 왼쪽 위에 있는 경우 
	{
		//110의 경우 70으로 바꿔야 한다.    초과값 20.   90으로부터 초과값 만큼 빼면 됨
		// angle = 90 - (90 - angle);    =  180-angle;

		FlipDirection(-1);
		angle = 180 - angle;
	}
	else if (angle < 0)   //적이 왼쪽에 아래에 있는 경우 
	{
		FlipDirection(-1);
		angle = -angle;
	}


	//추적 속도를 점점 빠르게
	//m_pTransformCom->Add_MoveVector(m_vChaseDir * m_fAccChaseTime * 0.5f);
	m_pTransformCom->Add_MoveVector(m_vChaseDir * m_fAccChaseTime * m_fAccChaseTime * 0.7f);



	//애니메이션의 position이 각도를 의미함 (1:1은 아니고 특수처리되어있음)
	Set_CurrentAnimationPositionJump(angle);



	//반드시 Set이던 시절 코드 백업용.
	/*
	//m_bChase 가 true일 때만 들어올것


	
	//디버그용 예외처리.  멈춰버리면 지랄남
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



		//테스트
		m_fAccChaseTime = 0.f;
		m_fGravityTime = 0.185f;
		m_pModelCom->SetUp_Animation(m_iFallAnimationIndex, false);


		return;
	}


	m_vChaseDir = XMVector4Normalize(vTargetPos - vMyPos);
	Set_fImpulse(XMVectorGetX(m_vChaseDir) * 2.f);




	//애니메이션 이용을 위해 각도값을 특수 처리 할 필요가 있음
	_float angle = atan2(XMVectorGetY(m_vChaseDir), XMVectorGetX(m_vChaseDir)) * (180.0 / 3.14);
	angle = (angle + 90) * 0.5f;

	cout << angle << endl;

	if (0 < angle && angle < 90)  //적이 오른쪽에 있는 경우
	{
		//캐릭터 보는 방향 오른쪽으로 변경
		FlipDirection(1);
	}

	else if (angle > 90)   //적이 왼쪽 위에 있는 경우 
	{
		//110의 경우 70으로 바꿔야 한다.    초과값 20.   90으로부터 초과값 만큼 빼면 됨
		// angle = 90 - (90 - angle);    =  180-angle;

		FlipDirection(-1);
		angle = 180 - angle;
	}
	else if (angle < 0)   //적이 왼쪽에 아래에 있는 경우 
	{
		FlipDirection(-1);
		angle = -angle;
	}


	//추적 속도를 점점 빠르게
	//m_pTransformCom->Add_MoveVector(m_vChaseDir * m_fAccChaseTime * 0.5f);
	m_pTransformCom->Add_MoveVector(m_vChaseDir * m_fAccChaseTime * m_fAccChaseTime );



	//애니메이션의 position이 각도를 의미함 (1:1은 아니고 특수처리되어있음)
	Set_CurrentAnimationPositionJump(angle);

	*/
}

void CCharacter::Chase_Ready(_float fTimeDelta)
{
	
	


	/*
	//반드시 Set에서 Next로 변경하고싶은데
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


		//공격판정 테스트
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
			Desc.ihitCharacter_Motion = { HitMotion::HIT_LIGHT };
			Desc.iTeam = m_iPlayerTeam;
			Desc.fAnimationLockTime = 0.1f;
			Desc.pOwner = this;

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);
		}
	}

	*/



	//반드시 Set에서 Next로 변경하고싶은데
	

	if (Check_bCurAnimationisGroundMove() || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex)
	{
		m_pModelCom->SetUp_Animation(m_iFallAnimationIndex, false);

		m_bChase = true;

		if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) < 0.5)
		{
			m_pTransformCom->Add_Move({ 0.f,0.6f,0.f });

		}


		
	}
	else
	{
		Set_NextAnimation(m_iFallAnimationIndex, 1.f);
		m_bChase = true;
	}
	

	//공격판정 테스트
	{
		CAttacKObject::ATTACK_DESC Desc{};
	/*	Desc.ColliderDesc.fSizeX = 2.0;
		Desc.ColliderDesc.fSizeY = 2.0f;
		Desc.ColliderDesc.Offset = { 0.f,0.6f,0.f };
		Desc.ColliderDesc.pTransform = m_pTransformCom;*/
		Desc.fhitCharacter_Impus = { 0.7f * m_iLookDirection,0.3f };

		Desc.fhitCharacter_StunTime = 0.6f;

		Desc.iDamage = 300 * Get_DamageScale();
		Desc.fLifeTime = 5.f;
		Desc.ihitCharacter_Motion = { HitMotion::HIT_CHASE };

		Desc.iTeam = m_iPlayerTeam;
		Desc.fAnimationLockTime = 0.1f;
		Desc.pOwner = this;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Attack"), TEXT("Layer_AttackObject"), &Desc);

	}






}

void CCharacter::Chase_Grab(_float fTimeDelta)
{

	//디버그용 예외처리.  멈춰버리면 지랄남
	if (fTimeDelta > 1)
	{
		return;
	}


	

	//준비자세면 이렇게 한다.
	if (m_pModelCom->m_iCurrentAnimationIndex == m_iGrabReadyAnimationIndex)
	{
		//이펙트생성 1회?
		//if (m_fAccGrabTime == fTimeDelta)
		if(m_fAccGrabTime == 0.f)
		{
			;
		}
		return;
	}

	//준비자세도 잡기자세도 아닌데 이 함수에 들어온 경우 초기화 후 종료.
	else if (m_pModelCom->m_iCurrentAnimationIndex != m_iGrabAnimationIndex)
	{
		m_fAccGrabTime = 0.f;
		m_bGrab = false;
		return;
	}
	else if ( m_fAccChaseTime >1.2) //시간제한으로 푸는게 아니라  1.2초 이상 지속될 수 없으니 강제해제.
	{
		m_fAccGrabTime = 0.f;
		m_bGrab = false;
		return;
	}
	m_fAccGrabTime += fTimeDelta;



	
	
 


	CTransform* pTarget = static_cast<CTransform*>(m_pDebugEnemy->Get_Component(TEXT("Com_Transform")));

	_vector vTargetPos = pTarget->Get_State(CTransform::STATE_POSITION);
	_vector vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


	//_float vLength = GetVectorLength((vTargetPos - vMyPos));
	/*
	if (vLength < 0.5f) //0.3
	{
		m_bChase = false;



		//테스트
		m_fAccChaseTime = 0.f;
		m_fGravityTime = 0.185f;
		m_pModelCom->SetUp_Animation(m_iFallAnimationIndex, false);


		return;
	}
	*/

	m_vChaseDir = XMVector4Normalize(vTargetPos - vMyPos);
	Set_fImpulse(XMVectorGetX(m_vChaseDir) * 2.f);




	//m_pTransformCom->Add_MoveVector(m_vChaseDir * (15 - m_fAccChaseTime*m_fAccChaseTime*3.f) * fTimeDelta);
	m_pTransformCom->Add_MoveVector(m_vChaseDir * (15 - m_fAccChaseTime * m_fAccChaseTime * 10.f) * fTimeDelta);



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

		//Set_fJumpPower(4.f); //중력Ver1 기준
		Set_fJumpPower(3.f); //중력Ver2 기준


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

		//Set_fJumpPower(4.f); //중력Ver1 기준
		Set_fJumpPower(3.f); //중력Ver2 기준


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



/*
_bool CCharacter::Set_Hit(_uint eAnimation, _float fStunTime, _uint iDamage, _float fStopTime, _float2 Impus)
{

	if (m_bStun == false)
	{

		//Guard_Update();

		//if (m_bGuard == true)
		if (Guard_Check())
		{
			if (m_pModelCom->m_iCurrentAnimationIndex == m_iCrouchAnimationIndex)
				Set_Animation(m_iGuard_CrouchAnimationIndex);

			else if (m_pModelCom->m_iCurrentAnimationIndex == m_iIdleAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iBackWalkAnimationIndex)
				Set_Animation(m_iGuard_GroundAnimationIndex);

			else if (m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex)
				Set_Animation(m_iGuard_AirAnimationIndex);

			return false;

		}

		else if (m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Air || m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Ground)
			return false;

		//else if()  smash 상태이고 hit air이고   공격타입이 필살기면 세이프
	}


	m_bStun = true;

	m_fMaxStunTime = fStunTime;

	Set_HitAnimation(eAnimation, Impus);
	Set_AnimationStop(fStopTime);

	Set_bRedHP(true);


	m_iHP -= iDamage;  // 여기에 콤보계수 곱할것
	
	m_iDebugComoboDamage += iDamage;
	cout << "Dagage : " << iDamage  <<"  ,  Total : " << m_iDebugComoboDamage << endl;


	if (m_iHP < 0)
	{
		m_iHP = 0;
	}
	
	return true;
}
*/

/*
_bool CCharacter::Set_Hit2(_uint eAnimation, AttackGrade eAttackGrade, AttackType eAttackType, _float fStunTime, _uint iDamage, _float fStopTime, _float2 Impus)
{

	if (m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Air || m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Ground || m_pModelCom->m_iCurrentAnimationIndex == m_iBound_Ground)
		return false;

	//스턴상태, 땅바닥에 꽂혔을때 검사
	else if (m_bHitGroundSmashed && Get_fHeight() == 0)
	{
		if (eAttackGrade != GRADE_ULTIMATE)
			return false;
	}

	//스턴 상태가 아니면 가드 체크
	if (m_bStun == false)
	{

		if (Guard_Check2(eAttackType))
		{
			if (m_pModelCom->m_iCurrentAnimationIndex == m_iCrouchAnimationIndex)
				Set_Animation(m_iGuard_CrouchAnimationIndex);

			else if (m_pModelCom->m_iCurrentAnimationIndex == m_iIdleAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iBackWalkAnimationIndex)
				Set_Animation(m_iGuard_GroundAnimationIndex);

			else if (m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex)
				Set_Animation(m_iGuard_AirAnimationIndex);

			return false;

		}

		

	

	

		//else if()  smash 상태이고 hit air이고   공격타입이 필살기면 세이프
		//else if (eAttackGrade == GRADE_ULTIMATE && m_bHitGroundSmashed && m_iHit_Air_LightAnimationIndex)
		//{
		//
		//}
		//else
		//	return false;


		//거꾸로 체크하자.
		// 바닥에 질질 끌리고있을때  필살기 아니면 false

		//else if (m_bHitGroundSmashed && m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_LightAnimationIndex )
		



	}

	

	m_bStun = true;

	m_fMaxStunTime = fStunTime;

	Set_HitAnimation(eAnimation, Impus);
	Set_AnimationStop(fStopTime);


	m_iHP -= iDamage;  // 여기에 콤보계수 곱할것

	m_iDebugComoboDamage += iDamage;
	cout << "Dagage : " << iDamage << "  ,  Total : " << m_iDebugComoboDamage << endl;


	if (m_iHP < 0)
	{
		m_iHP = 0;
	}

	return true;
}
*/
AttackColliderResult CCharacter::Set_Hit3(_uint eAnimation, AttackGrade eAttackGrade, AttackType eAttackType, _float fStunTime, _uint iDamage, _float fStopTime, _float2 Impus)
{
	if (m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Air || m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Ground || m_pModelCom->m_iCurrentAnimationIndex == m_iBound_Ground)
		return RESULT_MISS;

	//스턴상태, 땅바닥에 꽂혔을때 검사
	else if (m_bHitGroundSmashed && Get_fHeight() == 0)
	{
		if (eAttackGrade != GRADE_ULTIMATE)
			return RESULT_MISS;
	}

	//스턴 상태가 아니면 가드 체크
	if (m_bStun == false)
	{

		AttackColliderResult eResult = Guard_Check3(eAttackType);

		if (eResult == RESULT_GUARD)
		{
			if (m_pModelCom->m_iCurrentAnimationIndex == m_iCrouchAnimationIndex)
				Set_Animation(m_iGuard_CrouchAnimationIndex);

			else if (m_pModelCom->m_iCurrentAnimationIndex == m_iIdleAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iBackWalkAnimationIndex)
				Set_Animation(m_iGuard_GroundAnimationIndex);

			else if (m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex)
				Set_Animation(m_iGuard_AirAnimationIndex);

			return RESULT_GUARD;

		}
		else if (eResult != RESULT_HIT)    //회피나 비긴 경우 아니면 일단 속행.   저 경우는 나중에 따로 처리
			return eResult;


	}



	m_bStun = true;

	m_fMaxStunTime = fStunTime;

	Set_HitAnimation(eAnimation, Impus);
	Set_AnimationStop(fStopTime);


	m_iHP -= iDamage;  // 여기에 콤보계수 곱할것

	m_iDebugComoboDamage += iDamage;
	cout << "Dagage : " << iDamage << "  ,  Total : " << m_iDebugComoboDamage << endl;


	if (m_iHP < 0)
	{
		m_iHP = 0;
	}

	return RESULT_HIT;
}



void CCharacter::Set_HitAnimation(_uint eAnimation, _float2 Impus)
{

	//하나라도 0이 아니면 적용
	if (Impus.x != 0 || Impus.y != 0)
	{
		m_fImpuse = Impus;
	}


	//if (eAnimation == HIT_LIGHT)
	//{
	//	if (m_pModelCom->m_iCurrentAnimationIndex == m_iCrouchAnimationIndex)
	//	{
	//		Set_Animation(m_iHit_Crouch_AnimationIndex, false);
	//	}
	//	else if (Get_fHeight() >0)
	//	{
	//		Set_Animation(m_iHit_Air_LightAnimationIndex, false);
	//		Set_ForcedGravityTime_LittleUp();
	//
	//	}
	//	else
	//		Set_Animation(m_iHit_Stand_LightAnimationIndex, false);
	//}
	//else if (eAnimation == HIT_MEDUIUM)
	//{
	//
	//}
	//else if (eAnimation == HIT_HEAVY)
	//{
	//
	//}
	//else if (eAnimation == HIT_CROUCH_MEDIUM)
	//{
	//	Set_Animation(m_iHit_Air_FallAnimationIndex);
	//	m_pTransformCom->Add_Move({ 0.f,0.3f,0.f });
	//	Set_ForcedGravityTime_LittleUp();
	//}
	//
	//else if (eAnimation == HIT_KNOCK_AWAY_LEFT)
	//{
	//	Set_Animation(m_iHit_Away_LeftAnimationIndex, false);
	//	//m_pModelCom->CurrentAnimationPositionJump()
	//
	//}
	//else if (eAnimation == HIT_KNOCK_AWAY_UP)
	//{
	//	Set_Animation(m_iHit_Away_UpAnimationIndex, false);
	//	Set_ForcedGravityTime_LittleUp();
	//}


	switch (eAnimation)
	{
	case Client::HitMotion::HIT_CHASE:  //break 없음. 고의임
	{
		if (Get_fHeight() == 0)
			m_pTransformCom->Add_Move({ 0.f,0.3f,0.f });
	}

	case Client::HitMotion::HIT_LIGHT:
	{
		if (m_pModelCom->m_iCurrentAnimationIndex == m_iCrouchAnimationIndex)
		{
			Set_Animation(m_iHit_Crouch_AnimationIndex, false);
		}
		else if (Get_fHeight() > 0)
		{
			Set_Animation(m_iHit_Air_LightAnimationIndex, false);
			Set_ForcedGravityTime_LittleUp();

		}
		else
			Set_Animation(m_iHit_Stand_LightAnimationIndex, false);
	}
		break;


	case Client::HitMotion::HIT_MEDIUM:
	{
		if (m_pModelCom->m_iCurrentAnimationIndex == m_iCrouchAnimationIndex)
		{
			Set_Animation(m_iHit_Crouch_AnimationIndex, false);
		}
		else if (Get_fHeight() > 0)
		{
			Set_Animation(m_iHit_Air_LightAnimationIndex, false);
			//Set_ForcedGravityTime_LittleUp();
			Set_ForcveGravityTime(0.05f);

		}
		else
			Set_Animation(m_iHit_Stand_LightAnimationIndex, false);
	}
		break;
	case Client::HitMotion::HIT_HEAVY:
		break;
	case Client::HitMotion::HIT_CROUCH_MEDIUM:
	{
		Set_Animation(m_iHit_Air_FallAnimationIndex);
		m_pTransformCom->Add_Move({ 0.f,0.3f,0.f });
		Set_ForcedGravityTime_LittleUp();
	}
		break;
	case Client::HitMotion::HIT_KNOCK_AWAY_LEFT:
	{
		Set_Animation(m_iHit_Away_LeftAnimationIndex, false);
		//m_pModelCom->CurrentAnimationPositionJump()
	}
		break;
	case Client::HitMotion::HIT_KNOCK_AWAY_UP:
	{
		Set_Animation(m_iHit_Away_UpAnimationIndex, false);
		Set_ForcedGravityTime_LittleUp();
	}
		break;
	default:
		break;
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

	//공중에서 맞고 낙하중에는 중력 비슷하게 적용
	//if (m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_LightAnimationIndex)
	if (m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_LightAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_FallAnimationIndex)
	{
		;
	}

	//땅에서 약하게 맞았을경우 가속도 최대값을 제한함
	else if (m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Stand_LightAnimationIndex)
	{
		if (m_fImpuse.x > 0.5f)
		{
			m_fImpuse.x = 0.5f;
		}
		m_pTransformCom->Add_Move({ m_fImpuse.x * fTimeDelta, m_fImpuse.y * fTimeDelta, 0 });

	}

	//그 외에 맞고서 강하게 날라가는 중에는 전용 가속도를 받음
	else
		m_pTransformCom->Add_Move({ m_fImpuse.x *fTimeDelta, m_fImpuse.y * fTimeDelta, 0 });



}

void CCharacter::Set_BreakFall_Ground()
{
	Set_Animation(m_iBreakFall_Ground, 2.f);
	Set_NextAnimation(m_iIdleAnimationIndex, 2.f);

	Set_bRedHP(false);


	DirectionInput iMoveKey = inputBuffer.back().direction;

	if (iMoveKey == MOVEKEY_UP  || iMoveKey == MOVEKEY_UP_LEFT)
	{
		Set_fImpulse({ -3.f * m_iLookDirection,1.f });
		Set_ForcveGravityTime(0.f);
	}
	else if (iMoveKey == MOVEKEY_RIGHT)
	{
		//Set_fImpulse({ 0.f , 0.3f });
		Set_fImpulse({ 0.f , 0.1f });

		Set_ForcedGravityTime_LittleUp();
	}


	else //if (iMoveKey == MOVEKEY_LEFT)
	{
		Set_fImpulse({ -5.f * m_iLookDirection, 0.f });
		Set_ForcedGravityTime_LittleUp();
	}

	



}

void CCharacter::BreakFall_Air()
{
	//땅바닥에서 질질 끌리는 모션도 공중피격이라 조건 추가해야함
	if (Check_bCurAnimationisAirHit() && m_bHitGroundSmashed == false)
	{
		InputCommand();

		CInput InputKey = inputBuffer.back();

		if (InputKey.button != ATTACK_NONE)
		{
			Set_Animation(m_iBreakFall_Air);
			Set_ForcedGravityDown();

			if (InputKey.direction == MOVEKEY_UP)
			{
				Set_fImpulse({ 0.f, 5.f });
			}
			else if (InputKey.direction == MOVEKEY_DOWN)
			{
				Set_fImpulse({ 0.f, -15.f });
			}

			else if (InputKey.direction == MOVEKEY_LEFT || InputKey.direction == MOVEKEY_UP_LEFT)
			{
				Set_fImpulse({ -10.f * m_iLookDirection, 1.f });
			}

			else //(InputKey.direction == MOVEKEY_RIGHT || InputKey.direction == MOVEKEY_UP_RIGHT)
			{
				Set_fImpulse({ 10.f * m_iLookDirection, 1.f });
			}
			Set_bRedHP(false);


		}

	}
}

_float CCharacter::Get_DamageScale()
{


	//깡으로 더하는건 쉬운데 1히트당 1이 아닌데 따로 더해도 되나  //스파킹도 있는데 받는쪽에서 더하는게 아니라 때리는쪽에 더해야하는거 아님?
	//뎀감비율
	//Step Count	0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17 +
	//Next Hit		0%	10%	20% 30% 40% 50% 60% 70% 70% 70% 70% 75% 75% 75% 80% 80% 80% 85%
	//데미지비율    1.0 0.9 0.8 0              0.3   


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
		//fDamageScale += 0.2f;   //합연산. 너무 큰가?  15%->35%
		fDamageScale *= 1.2f;	  //곱연산 .  15%->16%   너무 작은가 싶지만 원작반영.
	}


	return fDamageScale;
}

void CCharacter::Set_GroundSmash(_bool bSmash)
{
	m_bHitGroundSmashed = bSmash;
}

/*
void CCharacter::Guard_Update()
{

	if(Check_bCurAnimationisGroundMove() || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex)
	{
		if (m_iPlayerTeam == 1)
		{
			if (m_iLookDirection == 1 && m_pGameInstance->Key_Pressing(DIK_A))
			{
				m_bGuard = true;
			}

			else if (m_iLookDirection == -1 && m_pGameInstance->Key_Pressing(DIK_D))
			{
				m_bGuard = true;
			}
			else
				m_bGuard = false;
		}

		else
		{
			if (m_iLookDirection == 1 && m_pGameInstance->Key_Pressing(DIK_LEFT))
			{
				m_bGuard = true;
			}

			else if (m_iLookDirection == -1 && m_pGameInstance->Key_Pressing(DIK_RIGHT))
			{
				m_bGuard = true;
			}
			else
				m_bGuard = false;
		}
	}
	else
	{
		m_bGuard = false;
	}

	
}
*/


/*
_bool CCharacter::Guard_Check()
{

	if (Check_bCurAnimationisGroundMove() || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex)
	{
		if (m_iPlayerTeam == 1)
		{
			if (m_iLookDirection == 1 && m_pGameInstance->Key_Pressing(DIK_A))
			{
				return true;
			}

			else if (m_iLookDirection == -1 && m_pGameInstance->Key_Pressing(DIK_D))
			{
				return true;
			}
			else
				return false;
		}

		else
		{
			if (m_iLookDirection == 1 && m_pGameInstance->Key_Pressing(DIK_LEFT))
			{
				return true;
			}

			else if (m_iLookDirection == -1 && m_pGameInstance->Key_Pressing(DIK_RIGHT))
			{
				return true;
			}
			else
				return false;
		}
	}
	else
	{
		return false;
	}
}

<<<<<<< HEAD
_bool CCharacter::Guard_Check2(AttackType eAttackType)
{
	//가드 중에는 어떤 공격 들어와도 무조건 가드 성공
	if (Check_bCurAnimationisGuard())
	{
		return true;
	}


	// 공격이나 행동을 하지 않고 있을때만 가드 체크.  뭔가 하고있었으면 무조건 가드실패.
	if (Check_bCurAnimationisGroundMove() || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex)
	{
		if (m_iPlayerTeam == 1)
		{

			//보고 있는 방향의 반대를 누르고 있을 시,   상중하단 체크 (잡기 포함)
			if (m_iLookDirection == 1 && m_pGameInstance->Key_Pressing(DIK_A))
			{
				return CompareGuardType(eAttackType);

			}

			else if (m_iLookDirection == -1 && m_pGameInstance->Key_Pressing(DIK_D))
			{
				return CompareGuardType(eAttackType);
			}

			//안눌렀으면 가드실패
			else   
				return false;
		}

		else
		{
			//보고 있는 방향의 반대를 누르고 있을 시,   상중하단 체크 (잡기 포함)
			if (m_iLookDirection == 1 && m_pGameInstance->Key_Pressing(DIK_LEFT))
			{
				return CompareGuardType(eAttackType);
			}

			else if (m_iLookDirection == -1 && m_pGameInstance->Key_Pressing(DIK_RIGHT))
			{
				return CompareGuardType(eAttackType);
			}

			//안눌렀으면 가드실패
			else
				return false;
		}
	}
	else
	{
		return false;
	}
}
*/

AttackColliderResult CCharacter::Guard_Check3(AttackType eAttackType)
{
	//가드 중에는 어떤 공격 들어와도 무조건 가드 성공
	if (Check_bCurAnimationisGuard())
	{
		return RESULT_GUARD;
	}


	if (eAttackType == ATTACKTYPE_GRAB_GROUND || eAttackType == ATTACKTYPE_GRAB_AIR || eAttackType == ATTACKTYPE_COMMANDGRAB)
	{
		return CompareGrabType3(eAttackType);
	}

	// 공격이나 행동을 하지 않고 있을때만 가드 체크.  뭔가 하고있었으면 무조건 가드실패.
	if (Check_bCurAnimationisGroundMove() || m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex)
	{

		


		if (m_iPlayerTeam == 1)
		{
			//보고 있는 방향의 반대를 누르고 있을 시,   상중하단 체크 (잡기 포함)
			if (m_iLookDirection == 1 && m_pGameInstance->Key_Pressing(DIK_A))
			{
				return CompareGuardType3(eAttackType);

			}

			else if (m_iLookDirection == -1 && m_pGameInstance->Key_Pressing(DIK_D))
			{
				return CompareGuardType3(eAttackType);
			}

			//안눌렀으면 가드실패
			else
				return RESULT_HIT;
		}

		else
		{
			//보고 있는 방향의 반대를 누르고 있을 시,   상중하단 체크 (잡기 포함)
			if (m_iLookDirection == 1 && m_pGameInstance->Key_Pressing(DIK_LEFT))
			{
				return CompareGuardType3(eAttackType);
			}

			else if (m_iLookDirection == -1 && m_pGameInstance->Key_Pressing(DIK_RIGHT))
			{
				return CompareGuardType3(eAttackType);
			}

			//안눌렀으면 가드실패
			else
				return RESULT_HIT;
		}
	}
	else
	{
		return RESULT_HIT;
	}
}

_bool CCharacter::CompareGuardType(AttackType eAttackType)
{
	//상단은 무조건 가드 가능
	if (eAttackType == ATTACKTYPE_HIGH)
		return true;

	//중단은 앉아있으면 가드 실패
	else if (eAttackType == ATTACKTYPE_MIDDLE)
	{
		if (m_pModelCom->m_iCurrentAnimationIndex == m_iCrouchAnimationIndex)
			return false;

		else
			return true;
	}

	//하단은 서있으면 가드 실패.  걷는중도 포함되는데  바꿔야한다.      앉아있거나 공중이면 성공?
	else if (eAttackType == ATTACKTYPE_LOW)
	{
		//if (m_pModelCom->m_iCurrentAnimationIndex == m_iIdleAnimationIndex)
		//	return false;
		//
		//else
		//	return true;

		if (m_pModelCom->m_iCurrentAnimationIndex == m_iCrouchAnimationIndex || Get_fHeight() > 0 )
			return true;

		else
			return false;



	}

	//잡기는 잡기버튼으로 상쇄 가능
	else if (eAttackType == ATTACKTYPE_GRAB_GROUND)
	{

		//피격된 상대의 높이가 0이 아니거나 잡기 버튼으로 상쇄했으면 실패.
		//if (Get_fHeight != 0 || m_pGameInstance->Key_Pressing(DIK_O))
		//	return false;

		//상대의 높이가 0이 아니면 실패
		if (Get_fHeight() != 0)
			return false;

		//잡기 버튼으로 상쇄했으면 실패.
		else if (m_iPlayerTeam == 1 && m_pGameInstance->Key_Pressing(DIK_O))
			return false;

		else if (m_iPlayerTeam == 2 && m_pGameInstance->Key_Pressing(DIK_NUMPAD9))
			return false;

		else
			return true;


		//대치상태가 아닌 피격중에는 높이 구분 없이 맞는게 정상임

	}

	//공중에서는  공중에 있는 상대한테만 통함.
	else if (eAttackType == ATTACKTYPE_GRAB_AIR)
	{
		//피격된 상대의 높이가 0이거나 잡기 버튼으로 상쇄했으면 실패.
		//if (Get_fHeight == 0 || m_pGameInstance->Key_Pressing(DIK_O))


		//상대가 공중이 아니면 실패
		if (Get_fHeight() == 0)
			return false;

		//상대가 잡기 버튼으로 상쇄했으면 실패.
		else if (m_iPlayerTeam == 1 && m_pGameInstance->Key_Pressing(DIK_O))
			return false;

		else if (m_iPlayerTeam == 2 && m_pGameInstance->Key_Pressing(DIK_NUMPAD9))
			return false;

		else
			return true;
	}

	//기술에 의한 잡기는  점프로만 가능.
	else if (eAttackType == ATTACKTYPE_COMMANDGRAB)
	{
		//높이조건 대신 애니메이션으로 해야하나? 
		if (Get_fHeight() > 0)
			return false;
		else
			return true;
	}

	

}

AttackColliderResult CCharacter::CompareGuardType3(AttackType eAttackType)
{
	//상단은 무조건 가드 가능
	if (eAttackType == ATTACKTYPE_HIGH)
		return RESULT_GUARD;

	//중단은 앉아있으면 가드 실패
	else if (eAttackType == ATTACKTYPE_MIDDLE)
	{
		if (m_pModelCom->m_iCurrentAnimationIndex == m_iCrouchAnimationIndex)
			return RESULT_HIT;

		else
			return RESULT_GUARD;
	}

	//하단은 서있으면 가드 실패.  걷는중도 포함되는데  바꿔야한다.      앉아있거나 공중이면 성공?
	else if (eAttackType == ATTACKTYPE_LOW)
	{
		//if (m_pModelCom->m_iCurrentAnimationIndex == m_iIdleAnimationIndex)
		//	return false;
		//
		//else
		//	return true;

		if (m_pModelCom->m_iCurrentAnimationIndex == m_iCrouchAnimationIndex || Get_fHeight() > 0)
			return RESULT_GUARD;

		else
			return RESULT_HIT;



	}

	



}

AttackColliderResult CCharacter::CompareGrabType3(AttackType eAttackType)
{
	//잡기는 잡기버튼으로 상쇄 가능
	if (eAttackType == ATTACKTYPE_GRAB_GROUND)
	{

		//피격된 상대의 높이가 0이 아니거나 잡기 버튼으로 상쇄했으면 실패.
		//if (Get_fHeight != 0 || m_pGameInstance->Key_Pressing(DIK_O))
		//	return false;

		//상대의 높이가 0이 아니면 실패
		if (Get_fHeight() != 0)
			return RESULT_MISS;

		//잡기 버튼으로 상쇄했으면 실패.
		else if (m_iPlayerTeam == 1 && m_pGameInstance->Key_Pressing(DIK_O))
			return RESULT_DRAW;

		else if (m_iPlayerTeam == 2 && m_pGameInstance->Key_Pressing(DIK_NUMPAD9))
			return RESULT_DRAW;

		else
			return RESULT_HIT;


		//대치상태가 아닌 피격중에는 높이 구분 없이 맞는게 정상임

	}

	//공중에서는  공중에 있는 상대한테만 통함.
	else if (eAttackType == ATTACKTYPE_GRAB_AIR)
	{
		//피격된 상대의 높이가 0이거나 잡기 버튼으로 상쇄했으면 실패.
		//if (Get_fHeight == 0 || m_pGameInstance->Key_Pressing(DIK_O))


		//상대가 공중이 아니면 실패
		if (Get_fHeight() == 0)
			return RESULT_MISS;

		//상대가 잡기 버튼으로 상쇄했으면 실패.
		else if (m_iPlayerTeam == 1 && m_pGameInstance->Key_Pressing(DIK_O))
			return RESULT_DRAW;

		else if (m_iPlayerTeam == 2 && m_pGameInstance->Key_Pressing(DIK_NUMPAD9))
			return RESULT_DRAW;

		else
			return RESULT_HIT;
	}

	//기술에 의한 잡기는  점프로만 가능.
	else if (eAttackType == ATTACKTYPE_COMMANDGRAB)
	{
		//높이조건 대신 애니메이션으로 해야하나? 
		if (Get_fHeight() > 0)
			return RESULT_MISS;
		else
			return RESULT_HIT;
			}
}

void CCharacter::Teleport_ToEnemy(_float OffsetX, _float OffsetY)
{
	
	_vector vTargetPos = static_cast<CTransform*>(m_pDebugEnemy->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

	vTargetPos += {OffsetX, OffsetY, 0, 0};

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTargetPos);


}

void CCharacter::Set_Grab(_bool bAir)
{
	m_bGrab = true;
	m_bGrab_Air = bAir;
}






void CCharacter::Set_bRedHP(_bool bRedHP)
{
	if (bRedHP == true)
	{
		//m_pUI_Manager->Set_Hp(m_iHP);
		m_tCharacterDesc.iHp = m_iHP;

	}

	 m_bRedHp = bRedHP; 
	 m_tCharacterDesc.bStun = m_bRedHp;
}


_uint* CCharacter::Get_pAnimationIndex()
{
	return &(m_pModelCom->m_iCurrentAnimationIndex);
}



_bool CCharacter::Check_bCurAnimationisAirHit(_uint iAnimation)
{
	_uint iModelIndex = iAnimation;

	if (iAnimation == 1000)
		iModelIndex = m_pModelCom->m_iCurrentAnimationIndex;



	//if (iModelIndex == m_iHit_Air_LightAnimationIndex || iModelIndex == m_iHit_Away_LeftAnimationIndex || iModelIndex == m_iHit_Away_UpAnimationIndex || iModelIndex == m_iHit_Air_FallAnimationIndex)
	if (iModelIndex == m_iHit_Air_LightAnimationIndex || iModelIndex == m_iHit_Air_FallAnimationIndex)
	{
		return true;
	}

	return false;
}

_bool CCharacter::Check_bCurAnimationisHitAway(_uint iAnimation)
{
	_uint iModelIndex = iAnimation;

	if (iAnimation == 1000)
		iModelIndex = m_pModelCom->m_iCurrentAnimationIndex;



	if (iModelIndex == m_iHit_Away_LeftAnimationIndex || iModelIndex == m_iHit_Away_UpAnimationIndex )
	{
		return true;
	}

	return false;
}

_bool CCharacter::Check_bCurAnimationisGuard(_uint iAnimation)
{
	//_uint iModelIndex = iAnimation;
	//if (iAnimation == 1000)

	_uint iModelIndex = iModelIndex = m_pModelCom->m_iCurrentAnimationIndex;


	if (iModelIndex == m_iGuard_AirAnimationIndex || iModelIndex == m_iGuard_CrouchAnimationIndex || iModelIndex == m_iGuard_GroundAnimationIndex)
	{
		return true;
	}

	return false;
}

void CCharacter::Set_NextAnimation(_uint iAnimationIndex, _float fLifeTime, _float fAnimationPosition)
{
	m_iNextAnimation.first = iAnimationIndex;
	m_iNextAnimation.second = fLifeTime;


	m_fNextAnimationCurrentPosition = fAnimationPosition;

}

void CCharacter::AttackNextMoveCheck()
{
	if (m_iNextAnimation.first != m_iIdleAnimationIndex)
	{

	
		if (m_bNextAnimationGravityEvent) 	//Set_Animation보다 위에 있어야함
		{
			m_bNextAnimationGravityEvent = false;
			Set_ForcedGravityTime_LittleUp();
		}

		Set_Animation(m_iNextAnimation.first);

		m_iNextAnimation.first = m_iIdleAnimationIndex;
		m_iNextAnimation.second = 1000.f;

		if (m_fNextAnimationCurrentPosition != 0)
		{
			m_pModelCom->CurrentAnimationPositionJump(m_fNextAnimationCurrentPosition);
			m_fNextAnimationCurrentPosition = 0.f;
		}
		
	}
}

void CCharacter::AnimeEndNextMoveCheck()
{

	Set_Animation(m_iNextAnimation.first);

	m_iNextAnimation.first = m_iIdleAnimationIndex;
	m_iNextAnimation.second = 1000.f;

}

void CCharacter::Set_Animation(_uint iAnimationIndex, _bool bloof)
{



	if (iAnimationIndex == m_iIdleAnimationIndex)
		m_pModelCom->SetUp_Animation(iAnimationIndex, true);
	else
		m_pModelCom->SetUp_Animation(iAnimationIndex, bloof);

	if (iAnimationIndex == m_iHit_Air_LightAnimationIndex)
	{
		m_pModelCom->CurrentAnimationPositionJump(0.f);
	}

}

void CCharacter::Gravity(_float fTimeDelta)
{
	


	if (m_bChase == true)
	{
		return;
	}

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float fHeight = XMVectorGetY(vPos);


	

	if (fHeight > 0)
	{


		

		// IDLE이면 공중 하강모션으로 변경


		if (m_bStun)
		{

		}


		else if (Check_bCurAnimationisGroundMove())
			m_pModelCom->SetUp_Animation(m_iFallAnimationIndex, false);
		
		
		// if (Check_bCurAnimationisGroundMove())
		//m_pModelCom->SetUp_Animation(m_iFallAnimationIndex, false);

		//중력 ver1 일때는 항상 더해야 자연스러운데 2에서는 아님
		//if (m_fGravityTime < m_fJumpPower)
		//{
		//	m_fGravityTime += fTimeDelta;
		//}


		//ver1  괜찮은데 너무 둥실둥실함  점프력4 베이스.
		//_float fGravity = ( - 0.3f * (m_fGravityTime - m_fJumpPower) * (m_fGravityTime - m_fJumpPower) + 2) *0.03f;

		//ver  점프에 속도감이 좀 느껴지고 전체적으로 낮음, 점프력 3 베이스.
		//_float fGravity = (-0.7f * (m_fGravityTime - m_fJumpPower) * (m_fGravityTime - m_fJumpPower) + 4) * 0.03f;

		//Ver3  점프력 3
		//_float fGravity = (-0.7f * (m_fGravityTime - m_fJumpPower) * (m_fGravityTime - m_fJumpPower) + 4) * 0.05f;


		//1~3짜리 전부 실수로 Gravity 두번씩 호출함.  

		//Ver4 점프력3.   마지막 값을 *0.1 대신 *0.08해도 자연스러움
		//_float fGravity = (-0.7f * (m_fGravityTime - m_fJumpPower) * (m_fGravityTime - m_fJumpPower) + 4) * 0.1;
		_float fGravity = (-0.7f * (2*m_fGravityTime - m_fJumpPower) * (2*m_fGravityTime - m_fJumpPower) + 4) * 0.1;



		//점프시작, 점프중에 중력 따름 
		//if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex)

		//공중 약공격으로 맞는중 추가
		//if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_LightAnimationIndex)

		//공중 기본공격중 추가( 기탄제외)
		//if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex ||
		//	m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_Air1 || m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_Air2 || m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_Air3 ||
		//	m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_LightAnimationIndex)
		if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex ||
			m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_Air1 || m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_Air2 || m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_Air3 ||
			m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_AirUpper || m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Ground ||
			Check_bCurAnimationisAirHit() || Check_bCurAnimationisHitAway() || m_pModelCom->m_iCurrentAnimationIndex == m_iGuard_AirAnimationIndex)
		{

			

			//스매시 당했으면 시간 더하지 않음.   공중 아래강 중에도 더하지 않음
			if (Check_bCurAnimationisHitAway() || m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_AirUpper)
			{
				;
			}

			//중력ver2 용
			//if (m_fGravityTime < m_fJumpPower)


			else if (m_fGravityTime*2.f < m_fJumpPower)
			{
				m_fGravityTime += fTimeDelta;

				//일시정지해서 여기서 넘어가면 값이 확 높아질 수 있음.  예외처리해서 시간값 한번 더?
			}
			else if (m_fGravityTime*2.f > m_fJumpPower)
			{
				m_fGravityTime = m_fJumpPower * 0.5f;
			}
			


			//m_pModelCom->m_iCurrentAnimationIndex == m_iAttack_AirUpper ||
			//HitAway가 아니고, Upper도 아니여야됨

			//if(Check_bCurAnimationisHitAway() == false )
			if (m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Ground)
			{
				m_pTransformCom->Add_Move({ m_fImpuse.x * fTimeDelta,-fGravity + m_fImpuse.y *fTimeDelta,0 });
			}
			else if (Check_bCurAnimationisHitAway() == false && m_pModelCom->m_iCurrentAnimationIndex != m_iAttack_AirUpper)
				m_pTransformCom->Add_Move({ m_fImpuse.x * fTimeDelta,-fGravity,0 });


		}
		else
		{
		
			//중력Ver2 전용 처리.  올라가다가 공격때문에 멈췄는데  공격 끝나고 다시 올라가는거 이상해서 처리
			//if (fGravity < 0 && m_fGravityTime < m_fJumpPower)


			if (m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Air)
			{
				m_pTransformCom->Add_Move({ m_fImpuse.x * fTimeDelta, m_fImpuse.y * fTimeDelta,0.f });
			}


			if(m_bAttackGravity == true)
			{ 

				//if(m_iPlayerTeam ==1)
				//{
				//	if ((m_pGameInstance->Key_Pressing(DIK_W) || (fGravity < 0 && m_fGravityTime * 2 < m_fJumpPower)))
				//	{
				//		m_fGravityTime += fTimeDelta;
				//	}
				//
				//
				//	//모든 공격중에 중력적용.  특정 모션만 하려면 각 클래스에서 override 필요
				//
				//	//if (m_pGameInstance->Key_Pressing(DIK_W))
				//	//	m_pTransformCom->Add_Move({ m_fImpuse.x * fTimeDelta,-fGravity,0 });
				//
				//}
				//else
				{
					//if ((m_pGameInstance->Key_Pressing(DIK_UP) || (fGravity < 0 && m_fGravityTime * 2 < m_fJumpPower)))
					//{
					//	m_fGravityTime += fTimeDelta;
					//}

					if (fGravity < 0 && m_fGravityTime * 2 < m_fJumpPower)
					{
						m_fGravityTime += fTimeDelta;
					}
					//모든 공격중에 중력적용.  특정 모션만 하려면 각 클래스에서 override 필요

					//if (m_pGameInstance->Key_Pressing(DIK_UP))
					//	m_pTransformCom->Add_Move({ m_fImpuse.x * fTimeDelta,-fGravity,0 });
				}

			}
			//가속만 받고 중력은 냅두는 코드. 모든 모션에 가속도 적용할꺼 아니면 굉장히 이상하게 보임.
			//m_pTransformCom->Add_Move({ m_fImpuse * fTimeDelta,0,0 });


		}



	}




	//이부분은 Gravity랑 다르게 분리해도될지도 

	//	else if (fHeight <0)   //왜 else if 였는지 기억이 안남. 실수였을지도
	//if (fHeight <0)
	if (fHeight < 0)
	{

		//if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || Check_bCurAnimationisAirAttack())
		if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || Check_bCurAnimationisAirAttack() || m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Air)
		{
			m_pModelCom->SetUp_Animation(m_iIdleAnimationIndex, true);

			Set_fGravityTime(0.f);
			//Set_fJumpPower(0.f);
			Set_fImpulse(0.f);

			Set_NextAnimation(m_iIdleAnimationIndex, 2.f);
			m_bAriDashEnable = true;
			Set_bAttackGravity(true);

			if (m_bJumpLock == false)
			{
				m_bJumpLock = true;
			}

		}
		else if (Check_bCurAnimationisAirHit() || Check_bCurAnimationisHitAway())
		{
			//m_pModelCom->SetUp_Animation(m_iIdleAnimationIndex, true);

			if (m_bHitGroundSmashed)
			{
				Set_Animation(m_iHit_Air_LightAnimationIndex);
				m_pModelCom->CurrentAnimationPositionJump(55.f);
				Set_NextAnimation(m_iBound_Ground, 5.f);

				//m_bHitGroundSmashed = false;
				//m_pTransformCom->Add_Move({ -1 * fTimeDelta * m_iLookDirection, 0, 0 });
			}
			else 
			{
				//m_pModelCom->SetUp_Animation(m_iBound_Ground, false);
				
				//백업
				//Set_Animation(m_iBreakFall_Ground, 2.f);
				//Set_fImpulse({ 5.f,0.f });
				Set_BreakFall_Ground();

				Set_NextAnimation(m_iIdleAnimationIndex, 2.f);
			}
			

			if (m_pModelCom->m_iCurrentAnimationIndex != m_iBreakFall_Ground)
			{
				Set_fGravityTime(0.f);
				Set_fImpulse(0.f);
			}


			m_bAriDashEnable = true;
			Set_bAttackGravity(true);

			if (m_bJumpLock == false)
			{
				m_bJumpLock = true;
			}

			
		}
		if (m_bHitGroundSmashed == false)
		{
			m_bStun = false;
		}
		//m_pTransformCom->Add_Move({ 0,-fHeight,0 });

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { XMVectorGetX(vPos),0.f,XMVectorGetZ(vPos),1.f });
		
	}

	else if (fHeight == 0)
	{
		if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex || m_pModelCom->m_iCurrentAnimationIndex == m_iJumpAnimationIndex)
		{
			m_pModelCom->SetUp_Animation(m_iIdleAnimationIndex, true);

			Set_fGravityTime(0.f);
			//Set_fJumpPower(0.f);
			Set_fImpulse(0.f);
			m_bAriDashEnable = true;
			Set_bAttackGravity(true);
		}
		
		//else if (m_bHitGroundSmashed == true && m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_LightAnimationIndex)
		//{
		//	if (m_bMotionPlaying == false)
		//	{
		//		Set_Animation(m_iBreakFall_Ground, 2.f);
		//		Set_NextAnimation(m_iIdleAnimationIndex, 2.f);
		//		m_bHitGroundSmashed = false;
		//	}
		//
		//}
		//else if (m_bHitGroundSmashed == true && m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_LightAnimationIndex)
		else if (m_bHitGroundSmashed == true)
		{
			if(m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_LightAnimationIndex)
			{
				//바닥에 꼬라박혔을때 질질 끌림.  보기에 이상하다면 맞았을 시에 FlipDirection 해야한다
				m_pTransformCom->Add_Move({ -2 * fTimeDelta * m_iLookDirection, 0, 0 });

				if (m_bMotionPlaying == false)
				{
					Set_Animation(m_iBound_Ground, 2.f);

					//Set_NextAnimation(m_iBreakFall_Ground, 2.f);
					//
					//if (inputBuffer.back() == CInput{ MOVEKEY_UP, ATTACK_LIGHT })
					//{
					//	m_fImpuse = { -3.f * m_iLookDirection, 2.f };
					//}
					//m_bHitGroundSmashed = false;



					m_bHitGroundSmashed = false;
					Set_NextAnimation(m_iBreakFall_Ground, 2.f);
					DirectionInput iMoveKey = inputBuffer.back().direction;

					if (iMoveKey == MOVEKEY_UP || iMoveKey == MOVEKEY_UP_LEFT)
					{
						Set_fImpulse({ -3.f * m_iLookDirection,1.f });
						Set_ForcveGravityTime(0.f);
					}
					else if (iMoveKey == MOVEKEY_RIGHT)
					{
						//Set_fImpulse({ 0.f , 0.3f });
						Set_fImpulse({ 0.f , 0.1f });

						Set_ForcedGravityTime_LittleUp();
					}


					else //if (iMoveKey == MOVEKEY_LEFT)
					{
						Set_fImpulse({ -5.f * m_iLookDirection, 0.f });
						Set_ForcedGravityTime_LittleUp();
					}
				}

			}
		}
		else if (Check_bCurAnimationisHitAway() || Check_bCurAnimationisAirHit())
		{
			//낙법 또는 스매시 
			if (m_bHitGroundSmashed)
			{
				//Set_Animation(m_iHit_Air_LightAnimationIndex);
				//m_pModelCom->CurrentAnimationPositionJump(55.f);
				//Set_NextAnimation(m_iBound_Ground, 5.f);

				//m_bHitGroundSmashed = false;
			}
			else
			{
				//Set_Animation(m_iBound_Ground);

				Set_Animation(m_iBreakFall_Ground, 2.f);
				if (inputBuffer.back() == CInput{ MOVEKEY_UP, ATTACK_LIGHT })
				{
					m_fImpuse = { -3.f * m_iLookDirection, 2.f };
				}
				Set_NextAnimation(m_iIdleAnimationIndex, 2.f);
			}
		}
		//else if (m_bHitGroundSmashed == true && m_pModelCom->m_iCurrentAnimationIndex == m_iHit_Air_LightAnimationIndex)
		//{
		//	if (m_bMotionPlaying == false)
		//	{
		//		Set_Animation(m_iBreakFall_Ground, 2.f);
		//		Set_NextAnimation(m_iIdleAnimationIndex, 2.f);
		//		m_bHitGroundSmashed = false;
		//	}
		//
		//}
		else if (m_pModelCom->m_iCurrentAnimationIndex == m_iBound_Ground)
		{
			if (m_bMotionPlaying == false)
			{
				Set_Animation(m_iBreakFall_Ground);
				
			}

		}
		if (m_pModelCom->m_iCurrentAnimationIndex == m_iBreakFall_Ground)
		{

			//낙법체크
			//1회만 실행될것
			m_fGravityTime += fTimeDelta;
			if (m_fGravityTime * 2.f > m_fJumpPower)
			{
				m_fGravityTime = m_fJumpPower * 0.5f;
			}
			//_float fGravity = (-0.7f * (2 * m_fGravityTime - m_fJumpPower) * (2 * m_fGravityTime - m_fJumpPower) + 4) * 0.1;
			//m_pTransformCom->Add_Move({ m_fImpuse.x * fTimeDelta,-fGravity + m_fImpuse.y * fTimeDelta,0 });
			m_pTransformCom->Add_Move({ m_fImpuse.x * fTimeDelta,0.3f,0 });

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

		m_bDoubleJumpEnable = true;
		
	}



}



HRESULT CCharacter::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
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


	////Com_Collider
	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
	//	TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
	//	return E_FAIL;
	
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

void CCharacter::GetUI_Input(_uint iInputDirX, _uint iInputDirY, DirectionInput eDirInput, ButtonInput eBtnInput)
{
	m_pUI_Manager->m_eDirInput = eDirInput;
	m_pUI_Manager->m_eBtnInput = eBtnInput;
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
	Safe_Release(m_pUI_Manager);

	//Safe_Release(m_pColliderCom);

}
