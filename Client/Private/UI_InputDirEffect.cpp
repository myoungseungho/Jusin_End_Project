#include "stdafx.h"

#include "UI_InputDirEffect.h"
#include "RenderInstance.h"

CUI_InputDirEffect::CUI_InputDirEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_Input{ pDevice ,pContext }
{
}

CUI_InputDirEffect::CUI_InputDirEffect(const CUI_InputDirEffect& Prototype)
	:CUI_Input{ Prototype }
{
}

HRESULT CUI_InputDirEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_InputDirEffect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fPosX = 230.f;
	m_fPosY = 510.f;
	__super::Set_UI_Setting(30.f, 30.f, m_fPosX, m_fPosY, 1.f);

	return S_OK;
}

void CUI_InputDirEffect::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUI_InputDirEffect::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	DirectionInput eDirInput = m_pUI_Manager->m_eDirInput;

	switch (eDirInput)
	{
	case DirectionInput::MOVEKEY_NEUTRAL:

		m_fPosX = 230.f;
		m_fPosY = 510.f;
		break;

	case DirectionInput::MOVEKEY_UP:
		m_fPosX = 230.f;
		m_fPosY = 450.f;
		break;

	case DirectionInput::MOVEKEY_DOWN:
		m_fPosX = 230.f;
		m_fPosY = 570.f;
		break;

	case DirectionInput::MOVEKEY_LEFT:
		m_fPosX = 170.f;
		m_fPosY = 510.f;
		break;

	case DirectionInput::MOVEKEY_RIGHT:
		m_fPosX = 290.f;
		m_fPosY = 510.f;
		break;

	case DirectionInput::MOVEKEY_UP_LEFT:
		m_fPosX = 170.f;
		m_fPosY = 450.f;
		break;

	case DirectionInput::MOVEKEY_UP_RIGHT:
		m_fPosX = 290.f;
		m_fPosY = 450.f;
		break;

	case DirectionInput::MOVEKEY_DOWN_LEFT:
		m_fPosX = 170.f;
		m_fPosY = 570.f;
		break;

	case DirectionInput::MOVEKEY_DOWN_RIGHT:
		m_fPosX = 290.f;
		m_fPosY = 570.f;
		break;
	}

	__super::Set_UI_Setting(30.f, 30.f, m_fPosX, m_fPosY, 1.f);
}

void CUI_InputDirEffect::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_InputDirEffect::Render(_float fTimeDelta)
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;;

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

HRESULT CUI_InputDirEffect::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Cursor"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_InputDirEffect* CUI_InputDirEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_InputDirEffect* pInstatnce = new CUI_InputDirEffect(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_InputDirEffect"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_InputDirEffect::Clone(void* pArg)
{
	CUI_InputDirEffect* pInstatnce = new CUI_InputDirEffect(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_InputDirEffect"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_InputDirEffect::Free()
{
	__super::Free();
}
