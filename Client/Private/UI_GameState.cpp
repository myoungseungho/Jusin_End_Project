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

	m_fAnimPos = m_fSizeX;

	return S_OK;
}

void CUI_GameState::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	m_fTotalAnimDuration += fTimeDelta;
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

void CUI_GameState::Set_AnimPosition(_int iNextPosX, _float fAnimSpeed)
{
	if(m_eAnimType == UI_ANIM)
		m_DequeAnim.push_back({ iNextPosX ,fAnimSpeed});
}

void CUI_GameState::Action_Anim(_float fSizeOffSet, _float fTimeDelta)
{
	if (m_DequeAnim.empty() == FALSE )
	{
		_float fVelocity = (m_DequeAnim.at(0).iPos - m_fAnimPos) / (m_DequeAnim.at(0).fEventFrame - m_fAnimFrame);

		if (m_DequeAnim.at(0).fEventFrame >= m_fTotalAnimDuration)
		{
			m_fSizeX +=  fVelocity* fTimeDelta;
		}
		else
		{
			m_fAnimFrame = m_DequeAnim.at(0).fEventFrame;
			m_fAnimPos = m_DequeAnim.at(0).iPos;

			m_DequeAnim.pop_front();
		}

		m_pTransformCom->Set_Scaled(m_fSizeX, m_fSizeX  * fSizeOffSet, 1.f);
	}
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
