#include "stdafx.h"

#include "UI_Opt_Sound.h"
#include "RenderInstance.h"

CUI_Opt_Sound::CUI_Opt_Sound(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_Option{ pDevice , pContext }
{

}

CUI_Opt_Sound::CUI_Opt_Sound(const CUI_Opt_Sound& Prototype)
	:CUI_Option{ Prototype }
{

}

HRESULT CUI_Opt_Sound::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Opt_Sound::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUI_Opt_Sound::Camera_Update(_float fTimeDelta)
{
	__super::Camera_Update(fTimeDelta);
}

void CUI_Opt_Sound::Update(_float fTimeDelta)
{
	MenuChange();
}

void CUI_Opt_Sound::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CUI_Opt_Sound::Render(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CUI_Opt_Sound::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Opt_Sound::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_Opt_Sound::MenuChange()
{
	if(m_pGameInstance->Key_Down(DIK_UP))
	{
		m_eMenuValue = (SOUND_MENU)(m_eMenuValue - 1);

		if (m_eMenuValue < 0)
			m_eMenuValue = VOICE;
	}

	if (m_pGameInstance->Key_Down(DIK_DOWN))
	{
		m_eMenuValue = (SOUND_MENU)(m_eMenuValue + 1);

		if (m_eMenuValue >= 3)
			m_eMenuValue = BGM;
	}
}

void CUI_Opt_Sound::Free()
{

	__super::Free();
}
