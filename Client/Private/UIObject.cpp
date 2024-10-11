#include "stdafx.h"

#include "UIObject.h"


CUIObject::CUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{ pDevice , pContext }
	, m_pUI_Manager{ CUI_Manager::Get_Instance() }
{
	Safe_AddRef(m_pUI_Manager);
}

CUIObject::CUIObject(const CUIObject& Prototype)
	:CGameObject{ Prototype }
	, m_pUI_Manager{ CUI_Manager::Get_Instance() }
{
	Safe_AddRef(m_pUI_Manager);
}

HRESULT CUIObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	UI_DESC* pUIDesc = static_cast<UI_DESC*>(pArg);

	if (pUIDesc->eLRPos == RIGHT)
		m_fPosX = g_iWinSizeX - m_fPosX;

	switch (pUIDesc->eLRPos)
	{
	case LEFT:
		m_pMainPawn = m_pUI_Manager->m_pPawnArray[CPawn::LPLAYER1];
		m_pSubPawn = m_pUI_Manager->m_pPawnArray[CPawn::LPLAYER2];
		break;

	case RIGHT:
		m_fPosX = g_iWinSizeX - m_fPosX;
		m_pMainPawn = m_pUI_Manager->m_pPawnArray[CPawn::RPLAYER1];
		m_pSubPawn = m_pUI_Manager->m_pPawnArray[CPawn::RPLAYER2];
		break;
	}


	return S_OK;
}

void CUIObject::Priority_Update(_float fTimeDelta)
{
	m_bCharaStun = m_pUI_Manager->m_bStun;
}

void CUIObject::Update(_float fTimeDelta)
{
}

void CUIObject::Late_Update(_float fTimeDelta)
{
	
}

HRESULT CUIObject::Render(_float fTimeDelta)
{
	return S_OK;
}

void CUIObject::Set_UI_Setting(_float fSizeX, _float fSizeY, _float fPosX, _float fPosY,  _float fDepth)
{
	m_pTransformCom->Set_Scaled(fSizeX, fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fPosX - g_iWinSizeX * 0.5f, -fPosY + g_iWinSizeY * 0.5f, fDepth, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f));
}

void CUIObject::DebugTesting(_float fSizeOffset, _float fDepth)
{
	m_pUI_Manager->UI_Setting_Debug(m_fSizeX, m_fSizeY, m_fPosX, m_fPosY, fSizeOffset);
	Set_UI_Setting(m_fSizeX, m_fSizeY, m_fPosX, m_fPosY , fDepth);
}

_bool CUIObject::ClickRange()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	
	_float MouseX = (_float)(pt.x);
	_float MouseY = (_float)(pt.y);

	if (m_fPosX - m_fSizeX * 0.5f < MouseX && m_fPosX + m_fSizeX * 0.5f > MouseX && m_fPosY - m_fSizeY * 0.5f < MouseY && m_fPosY + m_fSizeY * 0.5f > MouseY)
	{
		return true;
	}

	return false;
}

_bool CUIObject::HitCheck()
{
	if (m_bCharaStun == TRUE)
	{
		if (m_bHit == FALSE)
		{
			m_bHit = TRUE;
			return m_bHit;
		}
	}
	else
		m_bHit = FALSE;

}

HRESULT CUIObject::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIObject::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI_VtxRect"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

void CUIObject::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pUI_Manager);

	__super::Free();
}
