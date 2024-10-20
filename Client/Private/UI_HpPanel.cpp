
#include "stdafx.h"

#include "UI_HpPanel.h"
#include "RenderInstance.h"
#include "RenderInstance.h"

CUI_HpPanel::CUI_HpPanel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_HpPanel::CUI_HpPanel(const CUI_HpPanel& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_HpPanel::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_HpPanel::Initialize(void* pArg)
{
	m_fPosX = 330.f;
	m_fSizeX = 464.f;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	__super::Set_UI_Setting(m_fSizeX , 116.f, m_fPosX, 87.f,0.8f);


	return S_OK;
}

void CUI_HpPanel::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUI_HpPanel::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	_vector vTarget = { 330.f, 87.f, 0, 0 };
	
	if (m_pUI_Manager->m_iTeam == m_eLRPos)
	{
		if (m_bStart == FALSE)
		{
			_vector vStartPos = { 271 + g_iWinSizeX * 0.5f, 147 + g_iWinSizeY * -0.5f ,0.8f ,1.f };
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vStartPos);
			m_bStart = TRUE;
		}
		UI_Anim(vTarget, 300.f, fTimeDelta);
	}
	



}

void CUI_HpPanel::Late_Update(_float fTimeDelta)
{

	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_HpPanel::Render(_float fTimeDelta)
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition_Float4(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_HpPanel::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HpPanel"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

void CUI_HpPanel::UI_Anim(_vector vTargetPos, _float fSpeed,_float fTimeDelta)
{
	vTargetPos = XMVectorSetX(vTargetPos, XMVectorGetX(vTargetPos) - g_iWinSizeX * 0.5f);
	vTargetPos = XMVectorSetY(vTargetPos, -XMVectorGetY(vTargetPos) + g_iWinSizeY * 0.5f);

	//_vector vPos = { 271 + g_iWinSizeX * -0.5f , -528 + g_iWinSizeY * 0.5f, 0.8f, 1.f };
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	//_vector vSubtract = { g_iWinSizeX * -0.5f,g_iWinSizeY * 0.5f , 0.f,0.f };
	//vTargetPos = vTargetPos - vSubtract;

	_vector vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vMoveDir = XMVector2Normalize(vTargetPos - vOriginPos);

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + vMoveDir * fSpeed * fTimeDelta;

	vPos = XMVectorSetW(vPos, 1.f);
	vPos = XMVectorSetZ(vPos, 0.8f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	_float fDistance = XMVectorGetX( XMVector3Length(vTargetPos - vOriginPos));

	if (fDistance <= 5.f)
	{
		m_pUI_Manager->m_iTeam = POS_END;
		m_bStart = FALSE;
	}
}

CUI_HpPanel* CUI_HpPanel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_HpPanel* pInstatnce = new CUI_HpPanel(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_HpPanel"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_HpPanel::Clone(void* pArg)
{
	CUI_HpPanel* pInstatnce = new CUI_HpPanel(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_HpPanel"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_HpPanel::Free()
{
	__super::Free();
}
