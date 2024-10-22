#include "stdafx.h"

#include "UI_Chara_SubIcon.h"
#include "RenderInstance.h"

CUI_Chara_SubIcon::CUI_Chara_SubIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Chara_SubIcon::CUI_Chara_SubIcon(const CUI_Chara_SubIcon& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Chara_SubIcon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Chara_SubIcon::Initialize(void* pArg)
{
	m_fPosX = 72.f;
	m_fPosY = 112.f;
	m_fSizeX = 115.f;
	m_fSizeY = 115.f;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	__super::Set_UI_Setting(-m_fSizeX, m_fSizeY, m_fPosX, m_fPosY, 0.5f);

	return S_OK;
}

void CUI_Chara_SubIcon::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	(m_pSubPawn != nullptr) ? m_iCharaID = m_pSubPawn->Get_PawnDesc().ePlayerID : m_bDead = TRUE;
}

void CUI_Chara_SubIcon::Update(_float fTimeDelta)
{
	Animation({ 50.f ,40.f ,0.5f, 1.f }, { m_fPosX, m_fPosY, 0.5f, 1.f }, 100.f, 0.5f, fTimeDelta);
}

void CUI_Chara_SubIcon::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Chara_SubIcon::Render(_float fTimeDelta)
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;;

	//m_iCharaID → 캐릭터 ID가 만들어주면 0대신에 넣어주는 값 (캐릭터에 따라서 이미지가 바뀌도록)
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iCharaID)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(9)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Chara_SubIcon::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_CharaIcon"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Chara_SubIcon* CUI_Chara_SubIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Chara_SubIcon* pInstatnce = new CUI_Chara_SubIcon(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Chara_SubIcon"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Chara_SubIcon::Clone(void* pArg)
{
	CUI_Chara_SubIcon* pInstatnce = new CUI_Chara_SubIcon(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Chara_SubIcon"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Chara_SubIcon::Free()
{
	__super::Free();
}
