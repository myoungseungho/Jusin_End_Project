#include "stdafx.h"
#include "..\Public\QTE_Continuous_Attack.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "QTE_Continuous_Attack_Space.h"

CQTE_Continuous_Attack::CQTE_Continuous_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CQTE_Continuous_Attack::CQTE_Continuous_Attack(const CQTE_Continuous_Attack& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CQTE_Continuous_Attack::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CQTE_Continuous_Attack::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fSizeX = 50.f;
	m_fSizeY = 50.f;
	m_fX = 960.f;
	m_fY = 700.f;

	m_fSizeX = 50.f;
	m_fSizeY = 50.f;

	// 기본 위치 설정
	m_fDefaultY = m_fY;
	// 타겟 위치 설정 (예시로 Y 좌표를 100만큼 아래로 이동)
	m_fTargetY = m_fDefaultY + 30.f;


	m_pTransformCom->Set_Scaled(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.9f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));


	

	return S_OK;
}

void CQTE_Continuous_Attack::Camera_Update(_float fTimeDelta)
{

}

void CQTE_Continuous_Attack::Update(_float fTimeDelta)
{
#pragma region 디버그
	// F5 키 입력 감지
	if (m_pGameInstance->Key_Down(DIK_F1))
	{
		if (m_bIsQTEActive)
		{
			// QTE가 활성화되어 있으면 즉시 종료
			m_eMissionState = MISSION_FAILED;
			End_QTE();
		}
		else
		{
			// QTE가 비활성화되어 있으면 시작
			Start_QTE();
		}
	}
#pragma endregion

#pragma region 활성화

	if (m_bIsQTEActive)
	{
		//인풋 관리
		Handle_QTEInput();

		// 목표 연타 수 달성 확인
		if (m_iCurrentKeyPressCount >= m_iTargetKeyPressCount)
		{
			m_eMissionState = MISSION_SUCCESS;
			End_QTE();
		}
		// 타이머 확인
		else if (m_fTimer <= 0.0f)
		{
			m_eMissionState = MISSION_FAILED;
			End_QTE();
		}
		else
		{
			// 타이머 업데이트
			m_fTimer -= fTimeDelta;
		}

		m_pContinuous_Space->Update(fTimeDelta);
	}

#pragma endregion

	// 애니메이션 업데이트
	if (m_bIsMoving)
	{
		Update_Animation(fTimeDelta);
	}
}

void CQTE_Continuous_Attack::Start_QTE()
{
	if (m_bIsQTEActive)
		return; // 이미 QTE가 활성화되어 있으면 무시

	//활성화
	m_bIsQTEActive = true;
	m_fTimer = m_fLifeTime;


	//스페이스 객체 생성
	CQTE_Continuous_Attack_Space::CONTINUOUS_ATTACK_DESC Desc{};
	Desc.fX = 960.f;
	Desc.fY = 750.f;
	Desc.fSizeX = 300.f;
	Desc.fSizeY = 200.f;
	m_pContinuous_Space = static_cast<CQTE_Continuous_Attack_Space*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_QTE_Continuous_Attack_Space"), &Desc));
	m_pContinuous_Space->SetActive(true);
}

void CQTE_Continuous_Attack::End_QTE()
{
#pragma region 초기화

	//Space 객체는 삭제
	Safe_Release(m_pContinuous_Space);

	//활성화 여부 초기화
	m_bIsQTEActive = false;
	//타이머 초기화
	m_fTimer = m_fLifeTime;
	//연타수 초기화
	m_iCurrentKeyPressCount = 0;

	// 미션 상태에 따른 처리
	if (m_eMissionState == MISSION_SUCCESS)
	{
		// 성공 시 로직 처리
	}
	else if (m_eMissionState == MISSION_FAILED)
	{
		// 실패 시 로직 처리
	}

	// 미션 상태 초기화
	m_eMissionState = MISSION_NOT_DECIDED;

#pragma endregion

}

void CQTE_Continuous_Attack::Handle_QTEInput()
{
	if (m_iCharacterSide == 1)
	{
		if (m_pGameInstance->Key_Down(DIK_SPACE))
		{
			m_iCurrentKeyPressCount++;
			Process_Command();
		}
	}
	else if (m_iCharacterSide == 2)
	{
		if (m_pGameInstance->Key_Down(DIK_NUMPAD7))
		{
			m_iCurrentKeyPressCount++;
			Process_Command();
		}
	}
}

void CQTE_Continuous_Attack::Process_Command()
{
	// 애니메이션 상태 초기화
	m_bIsMoving = true;
	m_bIsMovingDown = true;   // 먼저 내려가기 시작
	m_fCurrentTime = 0.0f;    // 진행 시간 초기화

	// 위치를 기본 위치로 초기화
	m_fY = m_fDefaultY;

	// Transform에 위치 적용
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.9f, 1.f));

	m_pContinuous_Space->Process_Command();
}

void CQTE_Continuous_Attack::Update_Animation(_float fTimeDelta)
{
	// 현재 진행 시간 업데이트
	m_fCurrentTime += fTimeDelta;

	if (m_bIsMovingDown)
	{
		// 내려가는 애니메이션 진행
		_float fProgress = m_fCurrentTime / m_fMoveDownTime;

		if (fProgress >= 1.0f)
		{
			// 내려가기 완료, 올라가기 시작
			fProgress = 1.0f;
			m_bIsMovingDown = false;
			m_fCurrentTime = 0.0f;
		}

		// 이징 함수 적용 (Ease-In-Out)
		_float fEaseProgress = EaseInOut(fProgress);

		// 위치 계산
		m_fY = Lerp(m_fDefaultY, m_fTargetY, fEaseProgress);
	}
	else
	{
		// 올라가는 애니메이션 진행
		_float fProgress = m_fCurrentTime / m_fMoveUpTime;

		if (fProgress >= 1.0f)
		{
			// 애니메이션 종료
			fProgress = 1.0f;
			m_bIsMoving = false;
		}

		// 이징 함수 적용 (Ease-In-Out)
		_float fEaseProgress = EaseInOut(fProgress);

		// 위치 계산
		m_fY = Lerp(m_fTargetY, m_fDefaultY, fEaseProgress);
	}

	// 실제 Transform에 위치 적용
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.9f, 1.f));
}

void CQTE_Continuous_Attack::Late_Update(_float fTimeDelta)
{
	//활성화
	if (m_bIsQTEActive)
	{
		m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
		m_pContinuous_Space->Late_Update(fTimeDelta);
	}
}

HRESULT CQTE_Continuous_Attack::Render(_float fTimeDelta)
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(23)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CQTE_Continuous_Attack::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI_VtxRect"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_QTE_Arrow"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CQTE_Continuous_Attack::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}



CQTE_Continuous_Attack* CQTE_Continuous_Attack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CQTE_Continuous_Attack* pInstance = new CQTE_Continuous_Attack(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CQTE_Continuous_Attack"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CQTE_Continuous_Attack::Clone(void* pArg)
{
	CQTE_Continuous_Attack* pInstance = new CQTE_Continuous_Attack(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CQTE_Continuous_Attack"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQTE_Continuous_Attack::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pContinuous_Space);

	__super::Free();
}
