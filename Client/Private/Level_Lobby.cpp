#include "stdafx.h"
#include "..\Public\Level_Lobby.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "RenderInstance.h"


CLevel_Lobby::CLevel_Lobby(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{
}

HRESULT CLevel_Lobby::Initialize()
{
	m_iLevelIndex = LEVEL_LOBBY;

	//로비 카메라
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOBBY, TEXT("Prototype_GameObject_Main_Camera_Lobby"), TEXT("Layer_Main_Camera_Lobby"))))
		return E_FAIL;

	//로비맵
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOBBY, TEXT("Prototype_GameObject_Lobby_Center_Map"), TEXT("Layer_Lobby_Center_Map"))))
		return E_FAIL;

	//로비_배틀_빌딩
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOBBY, TEXT("Prototype_GameObject_Lobby_Battle_Building"), TEXT("Layer_Lobby_Battle_Building"))))
		return E_FAIL;

	LIGHT_DESC			LightDesc{};
	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, 1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.0f, 1.0f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(0.f, 0.f, 0.f, 1.f);

	if (FAILED(m_pRenderInstance->Add_Light(LightDesc)))
		return E_FAIL;


	//사운드 준비
	if (FAILED(Ready_Sound()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Lobby::Update(_float fTimeDelta)
{

}

HRESULT CLevel_Lobby::Render(_float fTimeDelta)
{
	SetWindowText(g_hWnd, TEXT("로비레벨"));

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Sound()
{
	return S_OK;
}

CLevel_Lobby* CLevel_Lobby::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Lobby* pInstance = new CLevel_Lobby(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Lobby"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Lobby::Free()
{
	__super::Free();
}
