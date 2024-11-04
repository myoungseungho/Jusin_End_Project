#include "stdafx.h"

#include "UI_CharaSelectIcon.h"
#include "UI_SelectArrow.h"
#include "RenderInstance.h"

CUI_CharaSelectIcon::CUI_CharaSelectIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_CharaSelectIcon::CUI_CharaSelectIcon(const CUI_CharaSelectIcon& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_CharaSelectIcon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_CharaSelectIcon::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	UI_DESC* Desc = static_cast<UI_DESC*>(pArg);

	m_iTexIndex = Desc->iNumUI;
	m_fPosX = 150.f + (m_iTexIndex * 320), m_fPosY = 620.f;
	m_fSizeX = 150.f, m_fSizeY = 150.f;

	m_pArrowTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_CHARACTER, TEXT("Layer_MarkArrow"), TEXT("Com_Transform")));
	Safe_AddRef(m_pArrowTransform);

	__super::Set_UI_Setting(m_fSizeX, m_fSizeY, m_fPosX, m_fPosY, 0.8f);

	return S_OK;
}

void CUI_CharaSelectIcon::Camera_Update(_float fTimeDelta)
{
	__super::Camera_Update(fTimeDelta);
}

void CUI_CharaSelectIcon::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	_vector vArrowPos = m_pArrowTransform->Get_State(CTransform::STATE_POSITION);

	_float fPosX = XMVectorGetX(vArrowPos) + g_iWinSizeX * 0.5f;
	_float fPosY = g_iWinSizeY * 0.5f - XMVectorGetY(vArrowPos);
	
	SelectIcon(fPosX, fPosY);
}

void CUI_CharaSelectIcon::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_CharaSelectIcon::Render(_float fTimeDelta)
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTexIndex)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_CharaSelectIcon::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_CHARACTER, TEXT("Prototype_Component_Texture_CharaSelect_Icon"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;


	return S_OK;
}

void CUI_CharaSelectIcon::SelectIcon(_float fPosX, _float fPosY)
{
	switch (m_iTexIndex)
	{
		case CUI_Define::GOKU:

			ClickRange(fPosX, fPosY) ? m_fPosY = 600.f , InputEvent(DIK_RETURN) : m_fPosY = 620.f;
			break;

		case CUI_Define::ANDROID21:
			ClickRange(fPosX, fPosY) ? m_fPosY = 600.f , InputEvent(DIK_RETURN) : m_fPosY = 620.f;
			break;

		case CUI_Define::BUU:
			ClickRange(fPosX, fPosY) ? m_fPosY = 600.f, InputEvent(DIK_RETURN) : m_fPosY = 620.f;
			break;

		case CUI_Define::HIT:
			ClickRange(fPosX, fPosY) ? m_fPosY = 600.f, InputEvent(DIK_RETURN) : m_fPosY = 620.f;
			break;

		default:
			break;
	}
		
	__super::Set_UI_Setting(m_fSizeX, m_fSizeY, m_fPosX, m_fPosY, 0.8f);
}

void CUI_CharaSelectIcon::InputEvent(_uint iKey)
{
	if (m_pGameInstance->Key_Down(iKey))
		dynamic_cast<CUI_SelectArrow*>(m_pGameInstance->Get_GameObject(LEVEL_CHARACTER, TEXT("Layer_MarkArrow")))->SelectChoice();	
}

void CUI_CharaSelectIcon::CreateChoiceMark()
{

}

CUI_CharaSelectIcon* CUI_CharaSelectIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_CharaSelectIcon* pInstatnce = new CUI_CharaSelectIcon(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_CharaSelectIcon"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_CharaSelectIcon::Clone(void* pArg)
{
	CUI_CharaSelectIcon* pInstatnce = new CUI_CharaSelectIcon(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_CharaSelectIcon"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_CharaSelectIcon::Free()
{
	Safe_Release(m_pArrowTransform);

	__super::Free();
}
