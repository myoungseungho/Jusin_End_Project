#include "stdafx.h"
#include "..\Public\QTE_UI_Icon.h"

#include "RenderInstance.h"
#include "GameInstance.h"

CQTE_UI_Icon::CQTE_UI_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CQTE_UI_Icon::CQTE_UI_Icon(const CQTE_UI_Icon& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CQTE_UI_Icon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CQTE_UI_Icon::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	CQTE_UI_Icon::QTE_UI_ICON_DESC* desc = reinterpret_cast<CQTE_UI_Icon::QTE_UI_ICON_DESC*>(pArg);
	m_fSizeX = desc->fSizeX;
	m_fSizeY = desc->fSizeY;
	m_fX = desc->fX;
	m_fY = desc->fY;
	m_fAlpha = desc->fAlpha;
	m_iTextureNumber = desc->iTextureNumber;

	// 초기 Y 위치 설정
	m_fCurrentY = m_fTargetY = m_fDefault_Y;
	m_pTransformCom->Set_Scaled(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fCurrentY + g_iWinSizeY * 0.5f, 0.9f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CQTE_UI_Icon::Camera_Update(_float fTimeDelta)
{

}

void CQTE_UI_Icon::Update(_float fTimeDelta)
{
	// Y 위치 업데이트
	_float omega = 2.0f / m_fSmoothing_Time;
	_float x = omega * fTimeDelta;
	_float exp_factor = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);

	_float change = m_fCurrentY - m_fTargetY;
	_float temp = (m_fVelocityY + omega * change) * fTimeDelta;
	m_fVelocityY = (m_fVelocityY - omega * temp) * exp_factor;
	_float newY = m_fTargetY + (change + temp) * exp_factor;

	m_fCurrentY = newY;

	// 흔들림 애니메이션 처리
	if (m_bIsShaking)
	{
		m_fShakeTime += fTimeDelta;

		if (m_fShakeTime >= m_fShakeDuration)
		{
			// 흔들림 종료 후 상태를 SELECTED로 변경
			m_bIsShaking = false;
			Set_State(SELECTED);
		}
		else
		{
			// 흔들림 계산
			_float shakeOffsetX = m_fShakeAmplitude * sinf(2.0f * XM_PI * m_fShakeFrequency * m_fShakeTime);
			// 감쇠 적용
			_float damping = 1.0f - (m_fShakeTime / m_fShakeDuration);
			shakeOffsetX *= damping;

			// 흔들림이 적용된 X 위치 계산
			_float shakenX = m_fX + shakeOffsetX;

			// 흔들림이 적용된 위치 설정
			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(shakenX - g_iWinSizeX * 0.5f, -m_fCurrentY + g_iWinSizeY * 0.5f, 0.9f, 1.f));
		}
	}
	else
	{
		// 흔들림이 아닐 때 기본 위치 설정
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fCurrentY + g_iWinSizeY * 0.5f, 0.9f, 1.f));
	}
}

void CQTE_UI_Icon::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CQTE_UI_Icon::Render(_float fTimeDelta)
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

void CQTE_UI_Icon::Set_State(IconState state)
{
	m_State = state;

	if (m_State == SELECTED)
	{
		m_fTargetY = m_fSelected_Y;
		m_bIsShaking = false;
	}
	else if (m_State == ALREADY_PRESSED)
	{
		m_fTargetY = m_fDefault_Y;
		m_bIsShaking = false;
	}
	else if (m_State == WRONG_PRESSED)
	{
		m_fTargetY = m_fSelected_Y;
		m_fShakeTime = 0.0f;        // 초기 흔들림 시간
		m_bIsShaking = true;         // 흔들림 시작
	}
	else // NOT_SELECTED
	{
		m_fTargetY = m_fDefault_Y;
		m_bIsShaking = false;
	}
}

HRESULT CQTE_UI_Icon::Ready_Components()
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

HRESULT CQTE_UI_Icon::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureNumber)))
		return E_FAIL;

	// IconState를 셰이더에 바인딩
	if (FAILED(m_pShaderCom->Bind_RawValue("g_IconState", &m_State, sizeof(_int))))
		return E_FAIL;

	return S_OK;
}

CQTE_UI_Icon* CQTE_UI_Icon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CQTE_UI_Icon* pInstance = new CQTE_UI_Icon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CQTE_UI_Icon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CQTE_UI_Icon::Clone(void* pArg)
{
	CQTE_UI_Icon* pInstance = new CQTE_UI_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CQTE_UI_Icon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQTE_UI_Icon::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);

	__super::Free();
}
