#include "stdafx.h"

#include "UI_Input.h"
#include "RenderInstance.h"

CUI_Input::CUI_Input(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice , pContext }
{

}

CUI_Input::CUI_Input(const CUI_Input& Prototype)
	:CUIObject{ Prototype }
{

}

HRESULT CUI_Input::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Input::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUI_Input::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUI_Input::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

}

void CUI_Input::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CUI_Input::Render(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CUI_Input::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Input::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_Input::CreateList()
{
	//m_bInitCommand 
	if (m_bInitCommand  == FALSE && DirectionInput::MOVEKEY_NEUTRAL != m_pUI_Manager->m_eDirInput)
	{
		UI_INPUT_DESC tDirInput = {};
		tDirInput.eDirInput = MOVEKEY_NEUTRAL;
		tDirInput.eActionInput = ATTACK_NONE;

		tDirInput.eDirInput = m_pUI_Manager->m_eDirInput;

		 
		tDirInput.eActionInput = m_pUI_Manager->m_eBtnInput;

		m_eDir = m_pUI_Manager->m_eDirInput;
		m_eAction = m_pUI_Manager->m_eBtnInput;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_DirInputIcon"), TEXT("Layer_DirInput"), &tDirInput);
		
		m_bInitCommand = TRUE;
	}

	if(m_pUI_Manager->m_eDirInput != m_eDir || m_pUI_Manager->m_eBtnInput != m_eAction)
		m_bInitCommand = FALSE;

	if (m_bInitCommand == FALSE && (ButtonInput::ATTACK_NONE != m_pUI_Manager->m_eBtnInput))
	{
		UI_INPUT_DESC tActionInput = {};
		tActionInput.eActionInput = m_pUI_Manager->m_eBtnInput;
	
		m_eAction = m_pUI_Manager->m_eBtnInput;
	
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_ActionInputIcon"), TEXT("Layer_DirInput"), &tActionInput);
		//m_bInitCommand = TRUE;
	}
}

void CUI_Input::Free()
{

	__super::Free();
}
