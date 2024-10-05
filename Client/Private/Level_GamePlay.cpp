#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "Camera_Free.h"
#include "GameInstance.h"
//#include "LandObject.h"
#include "Monster.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext } 
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	m_iLevelIndex = LEVEL_GAMEPLAY;


	//카메라 생성
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	//몬스터 생성
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster"), TEXT("Layer_Monster"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨"));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring & strLayerTag)
{
	CCamera_Free::CAMERA_FREE_DESC			CameraDesc{};

	CameraDesc.vEye = _float3(0.f, 10.f, -10.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1000.f;
	CameraDesc.fSpeedPerSec = 10.f;
	CameraDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	CameraDesc.fSensor = 0.1f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"), strLayerTag, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
}
