#include "stdafx.h"
#include "..\Public\QTE_Continuous_Attack.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "QTE_Hit_Situation.h"


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

	return S_OK;
}

void CQTE_Continuous_Attack::Camera_Update(_float fTimeDelta)
{

}

void CQTE_Continuous_Attack::Update(_float fTimeDelta)
{
#pragma region 디버그
	// F5 키 입력 감지
	if (m_pGameInstance->Key_Down(DIK_F4))
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

		//인풋

	}

#pragma endregion
}

void CQTE_Continuous_Attack::Start_QTE()
{
	if (m_bIsQTEActive)
		return; // 이미 QTE가 활성화되어 있으면 무시

	//활성화
	m_bIsQTEActive = true;
	m_fTimer = m_fLifeTime;
}

void CQTE_Continuous_Attack::End_QTE()
{
#pragma region 초기화

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

}

void CQTE_Continuous_Attack::Late_Update(_float fTimeDelta)
{
	//활성화
	if (m_bIsQTEActive)
		m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CQTE_Continuous_Attack::Render(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CQTE_Continuous_Attack::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI_VtxRect"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ActionInput"),
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

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureNumber)))
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

	__super::Free();
}
