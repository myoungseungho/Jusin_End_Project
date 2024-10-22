#include "stdafx.h"

#include "UIObject.h"
#include "RenderInstance.h"


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


	pDesc = static_cast<UI_DESC*>(pArg);

	if (pDesc != nullptr)
	{
		m_eLRPos = pDesc->eLRPos;
	
		switch (m_eLRPos)
		{
		case LEFT:
			m_pMainPawn = m_pUI_Manager->m_pPawnArray[CCharacter::LPLAYER1];
			m_pSubPawn = m_pUI_Manager->m_pPawnArray[CCharacter::LPLAYER2];
			break;
	
		case RIGHT:
			m_fPosX = g_iWinSizeX - m_fPosX;
			m_fSizeX *= -1;
			m_pMainPawn = m_pUI_Manager->m_pPawnArray[CCharacter::RPLAYER1];
			m_pSubPawn = m_pUI_Manager->m_pPawnArray[CCharacter::RPLAYER2];
			break;
		}
	}

	m_bIsActive = FALSE;

	return S_OK;
}

void CUIObject::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	if (m_pMainPawn != nullptr)
		m_bCharaStun = m_pMainPawn->Get_PawnDesc().bStun;

	if (pDesc != nullptr)
	{
		switch (m_eLRPos)
		{
		case LEFT:
			m_pMainPawn = m_pUI_Manager->m_pPawnArray[CCharacter::LPLAYER1];
			m_pSubPawn = m_pUI_Manager->m_pPawnArray[CCharacter::LPLAYER2];
			break;

		case RIGHT:
			m_pMainPawn = m_pUI_Manager->m_pPawnArray[CCharacter::RPLAYER1];
			m_pSubPawn = m_pUI_Manager->m_pPawnArray[CCharacter::RPLAYER2];
			break;
		}
	}

	if (m_pGameInstance->Key_Down(DIK_F4))
		m_bIsActive = true;
	if (m_pGameInstance->Key_Down(DIK_F5))
		m_bIsActive = FALSE;
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

void CUIObject::InitPlayer()
{
	switch (pDesc->eLRPos)
	{
	case LEFT:
		m_pMainPawn = m_pUI_Manager->m_pPawnArray[CCharacter::LPLAYER1];
		m_pSubPawn = m_pUI_Manager->m_pPawnArray[CCharacter::LPLAYER2];
		break;

	case RIGHT:
		m_fPosX = g_iWinSizeX - m_fPosX;
		m_fSizeX *= -1;
		m_pMainPawn = m_pUI_Manager->m_pPawnArray[CCharacter::RPLAYER1];
		m_pSubPawn = m_pUI_Manager->m_pPawnArray[CCharacter::RPLAYER2];
		break;
	}
}

void CUIObject::MoveAnimUI(_vector vTargetPos, _float fSpeed, _float fDepth ,  _float fTimeDelta)
{
	vTargetPos = XMVectorSetX(vTargetPos, XMVectorGetX(vTargetPos) - g_iWinSizeX * 0.5f);
	vTargetPos = XMVectorSetY(vTargetPos, -XMVectorGetY(vTargetPos) + g_iWinSizeY * 0.5f);

	_vector vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vMoveDir = XMVector2Normalize(vTargetPos - vOriginPos);

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + vMoveDir * fSpeed * fTimeDelta;

	vPos = XMVectorSetW(vPos, 1.f);
	vPos = XMVectorSetZ(vPos, fDepth);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	_float fDistance = XMVectorGetX(XMVector3Length(vTargetPos - vOriginPos));

	if (fDistance <= 5.f)
	{
		vTargetPos = XMVectorSetW(vTargetPos, 1.f);
		vTargetPos = XMVectorSetZ(vTargetPos, fDepth);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTargetPos);

		m_bCheck = TRUE;
	}
}

_vector CUIObject::GetOffsetPostion(_vector vPosition)
{
	//직교의 0,0좌표는 뷰포트 상에 중앙으로 설정되어 있기에 왼쪽위를 따로 설정

	vPosition = XMVectorSetX(vPosition, XMVectorGetX(vPosition) - g_iWinSizeX * 0.5f );
	vPosition = XMVectorSetY(vPosition,  - XMVectorGetY(vPosition) + g_iWinSizeY * 0.5f);

	return vPosition;
}

void CUIObject::Animation(_vector vStartPos ,_vector vTargetPos, _float fSpeed, _float fDepth,_float fTimeDelta)
{
	if (m_bCheck)
	{
		m_fAnimDelayTiemr += fTimeDelta;

		if (m_fAnimDelayTiemr >= 0.25f)
		{
			m_bCheck = FALSE;
			m_fAnimDelayTiemr = 0.f;
			m_bStart = FALSE;
			m_pUI_Manager->m_iTeam = POS_END;
		}
	}

	if (m_pUI_Manager->m_iTeam == m_eLRPos )
	{
		if (m_eLRPos == LEFT)
		{
			m_vAnimStartPos = GetOffsetPostion(vStartPos);
			m_vAnimTargetPos = vTargetPos;
		}
		else if (m_eLRPos == RIGHT)
		{
			vStartPos= XMVectorSetX(vStartPos, g_iWinSizeX  - XMVectorGetX(vStartPos));
			m_vAnimStartPos = GetOffsetPostion(vStartPos);
			m_vAnimTargetPos = vTargetPos; 
		}

		if (m_bStart == FALSE && m_bCheck== FALSE)
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vAnimStartPos);
			m_bStart = TRUE;
		}
		
		if(m_bStart)
			MoveAnimUI(vTargetPos, 300.f, fDepth , fTimeDelta);
	}
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
