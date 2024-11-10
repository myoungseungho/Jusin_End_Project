#include "stdafx.h"
#include "..\Public\Level_VS.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Define.h"
#include "UIObject.h"
#include "GameObject.h"

#include "CharaSelectCamera.h"
#include "RenderInstance.h" 


CLevel_VS::CLevel_VS(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{
}

HRESULT CLevel_VS::Initialize()
{
	m_iLevelIndex = LEVEL_VS;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Sound()))
		return E_FAIL;

	return S_OK;
}

void CLevel_VS::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_RETURN))
	{
		if (FAILED(m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY))))
			return;
	}
}

HRESULT CLevel_VS::Render(_float fTimeDelta)
{
	SetWindowText(g_hWnd, TEXT("로고레벨"));

	return S_OK;
}

HRESULT CLevel_VS::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_VS, TEXT("Prototype_GameObject_VS_bG"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_VS::Ready_Sound()
{
	return S_OK;
}

CLevel_VS* CLevel_VS::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_VS* pInstance = new CLevel_VS(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_VS"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_VS::Free()
{
	__super::Free();
}
