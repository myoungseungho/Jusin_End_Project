#include "stdafx.h"

#include "UI_GameState.h"
#include "RenderInstance.h"

CUI_GameState::CUI_GameState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice , pContext }
{

}

CUI_GameState::CUI_GameState(const CUI_GameState& Prototype)
	:CUIObject{ Prototype }
{

}

HRESULT CUI_GameState::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_GameState::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	UI_DESC* pDesc = static_cast<UI_DESC*>(pArg);
	if(pDesc != nullptr)
		m_eAnimType = pDesc->eType;

	return S_OK;
}

void CUI_GameState::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUI_GameState::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CUI_GameState::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CUI_GameState::Render(_float fTimeDelta)
{
	return S_OK;
}

void CUI_GameState::Set_AnimPosition(_uint iPosX, _float fAnimSpeed, _bool bStop, _float fStopDuration)
{
	if(m_eAnimType == UI_ANIM)
		m_QueueAnimPos.push({ iPosX ,fAnimSpeed ,bStop  ,fStopDuration });
}

void CUI_GameState::Action_ScaleAnim(_float fOffsetScaleY ,_float fTimeDelta)
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
			//m_fSizeY += fScaleAmount * fOffsetScaleY;
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

	m_pTransformCom->Set_Scaled(m_fSizeX, m_fSizeX * fOffsetScaleY, 1.f);
}

HRESULT CUI_GameState::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_GameState::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_GameState::Free()
{

	__super::Free();
}
