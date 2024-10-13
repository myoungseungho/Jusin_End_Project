#include "stdafx.h"

#include "UI_GameStart.h"
#include "RenderInstance.h"

CUI_GameStart::CUI_GameStart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice , pContext }
{

}

CUI_GameStart::CUI_GameStart(const CUI_GameStart& Prototype)
	:CUIObject{ Prototype }
{

}

HRESULT CUI_GameStart::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_GameStart::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUI_GameStart::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUI_GameStart::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CUI_GameStart::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CUI_GameStart::Render(_float fTimeDelta)
{
	return S_OK;
}

void CUI_GameStart::Set_AnimPosition(_uint iPos, _float fAnimSpeed, _bool bStop, _float fStopDuration)
{
	m_QueueAnimPos.push({ iPos ,fAnimSpeed ,bStop  ,fStopDuration });
}

void CUI_GameStart::Action_ScaleAnim(_float fTimeDelta)
{
	if (!m_QueueAnimPos.empty())
	{
		auto& frontAnim = m_QueueAnimPos.front();
		_bool isGrowing = frontAnim.iPos > m_fSizeX;

		if ((m_QueueAnimPos.front().bStop == FALSE) || (m_QueueAnimPos.front().fStopDuration <= m_fStopTimer))
		{
			isGrowing ? m_fScaleAnimTimer += fTimeDelta : m_fScaleAnimTimer -= fTimeDelta;
			_float fScaleAmount = m_fScaleAnimTimer * frontAnim.m_fSpeed;
			m_fSizeX += fScaleAmount;
			m_fSizeY += fScaleAmount;
		}
		else
			m_fStopTimer += fTimeDelta;

		if ((isGrowing && frontAnim.iPos <= m_fSizeX) ||
			(!isGrowing && frontAnim.iPos >= m_fSizeX))
		{
			m_QueueAnimPos.pop();
			m_fStopTimer = 0.f;
			m_fScaleAnimTimer = 0.f;
		}
	}
	//else
	//	m_bDead = true;

	m_pTransformCom->Set_Scaled(m_fSizeX, m_fSizeY, 1.f);
}

HRESULT CUI_GameStart::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_GameStart::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_GameStart::Free()
{

	__super::Free();
}
