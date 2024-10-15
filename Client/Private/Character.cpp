#include "stdafx.h"
#include "..\Public\Character.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "UI_Manager.h"


const _float CCharacter::fGroundHeight = 0.f; //0


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
	{MOVEKEY_LEFT, ATTACK_NONE}
};

vector<CInput> CCharacter::Command_Forward =
{
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{MOVEKEY_RIGHT, ATTACK_NONE},
	{ MOVEKEY_RIGHT, ATTACK_NONE }

};

vector<CInput> CCharacter::Command_LightAttack = { {MOVEKEY_NEUTRAL, ATTACK_LIGHT} };
vector<CInput> CCharacter::Command_MediumAttack = { {MOVEKEY_NEUTRAL, ATTACK_MEDIUM} };
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

	//Character_DESC* pDesc = static_cast<Character_DESC*>(pArg);
	//m_strModelName = pDesc->strModelName;

	m_pFrameEvent = CFrameEvent_Manager::Get_Instance()->Get_pFrameEventMap();


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Character_DESC* pSlotDesc = static_cast<Character_DESC*>(pArg);
	m_ePlayerSlot = pSlotDesc->ePlayerSlot;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	
	
	//SLOT_DESC* pSlotDesc = static_cast<SLOT_DESC*>(pArg);
	//m_ePlayerSlot = pSlotDesc->ePlayerSlot;

	//모델 로드를 하위 클래스로 옮겼으니 각자 처리하기 
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
	__super::Priority_Update(fTimeDelta);

	m_tCharacterDesc.bStun = m_bStun;
	m_tCharacterDesc.bHit = m_bHit;
	m_tCharacterDesc.bAttBuf = m_bAttBuf;
	m_tCharacterDesc.iHp = m_iHp;
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

void CCharacter::InputCommand()
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


		 if (m_pGameInstance->Key_Pressing(DIK_O))
		 {
			 iAttackkey = ATTACK_GRAB;
		
		 }
		// if (m_pGameInstance->Key_Pressing(DIK_Y))
		// {
		//	 iAttackkey = ATTACK_LIGHT;
		//
		// }

		 m_pUI_Manager->m_eDirInput = (DirectionInput)DirectionX;
		 m_pUI_Manager->m_eBtnInput = (ButtonInput)iAttackkey;

	}



	CInput newInput(iMoveKey, iAttackkey);

	if(inputBuffer.size()>0)
	{
		//마지막 상태와 키 입력 상태가 똑같으면 갱신하지 않음
		if (inputBuffer.back() == newInput)
		{

		}
		else
			UpdateInputBuffer(CInput(iMoveKey, iAttackkey));
	}
	else
	{
		UpdateInputBuffer(CInput(iMoveKey, iAttackkey));
	}
}

void CCharacter::InputedCommandUpdate(_float fTimeDelta)
{

	//for (auto tinput : inputBuffer)
	//{
	//	tinput.frameTime += fTimeDelta;
	//	
	//	if(tinput.frameTime>0.2)
	//
	//}

	inputBuffer.erase(
		remove_if(inputBuffer.begin(), inputBuffer.end(), [fTimeDelta](CInput& input) {
			input.frameTime += fTimeDelta;  // frameTime에 fTimeDelta 더하기
			return input.frameTime >= 0.35f; // 0.3 이상인 경우 삭제
			}),
		inputBuffer.end()
	);

	//inputBuffer.erase(remove_if(inputBuffer.begin(), inputBuffer.end(), [fTimeDelta](CInput& input) 
	//	{
	//		input.frameTime += fTimeDelta;
	//		return input.frameTime > 0.2f;
	//
	//	};

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

bool CCharacter::CheckCommandWithStartCondition(const vector<CInput>& pattern, int timeWindow)
{
	return false;
}

_uint CCharacter::CheckAllCommands()
{

	{
		//for (const auto& command : MoveCommandPatterns) 
		//{
		//	if (CheckCommandSkippingExtras(command.pattern,0)) {
		//		//command.action();  // 해당 패턴이 매칭되면 해당 기술 실행
		//		
		//		return command.AnimationIndex;
		//	}
		//}
		//return 0;

		for (const auto& command : MoveCommandPatternsFunction)
		{
			if (CheckCommandSkippingExtras(command.pattern, 0)) {
				command.action();  // 해당 패턴이 매칭되면 해당 기술 실행

				return 0;
			}
		}
		return 0;

	}
}

void CCharacter::ShowInputBuffer()
{
	inputBuffer;

	_bool bDebug = true;
}

void CCharacter::DebugPositionReset()
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 0,0,0,1 });
}

void CCharacter::FlipDirection(_int iDirection)
{
	if (iDirection == 0)
	{
		m_iLookDirection = -m_iLookDirection;
	}
	else
	{
		m_iLookDirection = iDirection;
	}

	m_pTransformCom->Set_Scaled(-1, 1, 1);
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
{
	
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float fHeight = XMVectorGetY(vPos);


	if (fHeight > 0)
	{
		
		if (Check_bCurAnimationisGroundMove())
		{
			//공중 하강 모션으로 변경
			//Set_Animation(m_iFallAnimationIndex);
			m_pModelCom->SetUp_Animation(m_iFallAnimationIndex, false);
		}

		//하강 모션중이면 점점 추락
		if(m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex)
		{
			//fHeight -= fTimeDelta;
			//m_pTransformCom->
			m_pTransformCom->Add_Move({ 0,-fTimeDelta,0 });
		}

	}
	else
	{
		if (m_pModelCom->m_iCurrentAnimationIndex == m_iFallAnimationIndex)
		{
			m_pModelCom->SetUp_Animation(m_iIdleAnimationIndex, true);
		}
	}

}

void CCharacter::Action_AttBuf(_ubyte byKeyID, PLAYER_SLOT eSlot, _float fTimeDelta)
{
	if (m_iNumAttBuf <= 1 && m_pGameInstance->Get_DIKeyState(byKeyID))
	{
		m_bAttBuf = TRUE;
	//	m_pUI_Manager->UsingAttckBuff(5.f, eSlot);
		m_iNumAttBuf--;
	}
	if (m_bAttBuf == TRUE)
	{
		m_fAttBufTimer += fTimeDelta;

		if (m_fAttBufTimer >= 5.f)
		{
			m_bAttBuf = FALSE;
			m_fAttBufTimer = 0.f;
		}
	}
}



void CCharacter::Action_Hit(_ubyte byKeyID, _float fStunDuration, _float fTimeDelta)
{
	if (m_pGameInstance->Get_DIKeyState(byKeyID))
	{
		m_iComboCount++;
		m_iHp--;

		m_fStunTImer = fStunDuration;
		m_bStun = TRUE;
		m_bHit = TRUE;

		m_iSKillPoint += 3;
	}
	else
		m_bHit = FALSE;

	SkillGaugeLimit();
	StunRecover(fTimeDelta);
}

void CCharacter::SkillGaugeLimit()
{
	if (m_iSKillPoint > 100)
	{
		m_iSKillPoint -= 100;
		m_iSKillCount++;
	}
	else if (m_iSKillPoint < 0)
	{
		m_iSKillPoint += 100;
		m_iSKillCount--;
	}
}

void CCharacter::StunRecover(_float fTimeDelta)
{
	if (m_bStun == TRUE)
	{
		m_fStunTImer -= fTimeDelta;

		if (m_fStunTImer <= 0.f)
		{
			m_fStunTImer = 0.f;
			m_bStun = FALSE;
			m_iComboCount = 0;
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
	Safe_Release(m_pUI_Manager);
}
