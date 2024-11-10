#include "stdafx.h"
#include "..\Public\QTE_Hit_UI_Effect.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "QTE_Hit_Situation.h"

CQTE_Hit_UI_Effect::CQTE_Hit_UI_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CQTE_Hit_UI_Effect::CQTE_Hit_UI_Effect(const CQTE_Hit_UI_Effect& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CQTE_Hit_UI_Effect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CQTE_Hit_UI_Effect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Hit_EFFECT_DESC* Desc = static_cast<Hit_EFFECT_DESC*>(pArg);
	m_fX = Desc->fX;
	m_fY = Desc->fY;
	m_fSizeX = Desc->fSizeX;
	m_fSizeY = Desc->fSizeY;
	m_fLifeTime = Desc->fTimer;
	m_fTimer = m_fLifeTime;

	m_pTransformCom->Set_Scaled(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.9f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	SetActive(true);

	return S_OK;
}

void CQTE_Hit_UI_Effect::Camera_Update(_float fTimeDelta)
{

}

void CQTE_Hit_UI_Effect::Update(_float fTimeDelta)
{
	if (!m_bIsActive)
		return;

	// 남은 시간을 감소시키고 경과 시간을 누적
	m_fTimer -= fTimeDelta;
	m_fElapsedTime += fTimeDelta;

	// 로그 함수를 통한 크기 비율 계산
	_float maxTime = 1.0f; // 총 시간을 1초로 가정, 필요 시 조정 가능
	_float ratio = Clamp(m_fElapsedTime / maxTime, 0.f, 1.f);

	// 비율을 로그 함수로 조정 (0.1을 더해줘서 초기 상태에서 무한대 방지)
	_float growthFactor = log(1.0f + 9.0f * ratio) / log(10.0f); // 0에서 1로 점진적 증가

	// 초기 크기와 목표 크기 설정
	_float startX = m_fSizeX * 0.7f; // 초기 크기
	_float endX = m_fSizeX * 1.5f;   // 목표 크기
	_float startY = m_fSizeY * 0.7f;
	_float endY = m_fSizeY * 1.5f;

	// 로그 함수에 따른 크기 보간
	_float X = Lerp(startX, endX, growthFactor);
	_float Y = Lerp(startY, endY, growthFactor);

	// 변환 컴포넌트에 적용
	m_pTransformCom->Set_Scaled(X, Y, 1.f);

	// 타이머가 끝나면 이펙트를 비활성화
	if (m_fTimer <= 0)
	{
		m_fTimer = 0;
		SetActive(false);
	}
}

void CQTE_Hit_UI_Effect::Late_Update(_float fTimeDelta)
{
	if (!m_bIsActive)
		return;

	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CQTE_Hit_UI_Effect::Render(_float fTimeDelta)
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(27)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}


HRESULT CQTE_Hit_UI_Effect::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI_VtxRect"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_QTE_Effect"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CQTE_Hit_UI_Effect::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	// 시간 값을 셰이더로 전달
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fElapsedTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_MaxTime", &m_fLifeTime, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CQTE_Hit_UI_Effect* CQTE_Hit_UI_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CQTE_Hit_UI_Effect* pInstance = new CQTE_Hit_UI_Effect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CQTE_Hit_UI_Effect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CQTE_Hit_UI_Effect::Clone(void* pArg)
{
	CQTE_Hit_UI_Effect* pInstance = new CQTE_Hit_UI_Effect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CQTE_Hit_UI_Effect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQTE_Hit_UI_Effect::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);

	__super::Free();
}
