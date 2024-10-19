#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "Main_Camera.h"
#include "GameInstance.h"
#include "RenderInstance.h"
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
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Main_Camera"))))
		return E_FAIL;

	//Light 준비
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	// 해당 레벨에 맞는 파일 경로를 설정
	wstring filePath = L"../Bin/Level_GamePlay_Objects.txt";

	if (FAILED(ParseInitialize(filePath))) {
		return E_FAIL;
	}

	// 카메라 포인트 로드
	_wstring cameraFilePath = L"../Bin/CameraPoints.txt";
	CameraSaveData cameraSaveData;

	HRESULT hr = m_pGameInstance->Load_All_CameraPoints(cameraFilePath, &cameraSaveData);
	if (FAILED(hr)) {
		// 로드 실패 처리
		// 예: 로그 남기기 또는 기본 설정 적용
		return S_OK;
	}

	if (cameraSaveData.models.empty()) {
		// 로드된 데이터가 없으면 기본 설정 적용
		return S_OK;
	}

	// 로드된 데이터를 mainCamera에 적용
	CMain_Camera* mainCamera = static_cast<CMain_Camera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, L"Layer_Main_Camera"));
	if (mainCamera) {
		mainCamera->ApplyCameraData(cameraSaveData);
	}

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

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Main_Camera"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};
	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, 1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(0.8f, 0.85f, 1.0f, 1.0f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = _float4(0.f, 0.f, 0.f, 1.f);

	if (FAILED(m_pRenderInstance->Add_Light(LightDesc)))
		return E_FAIL;
}

CLevel_GamePlay* CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pDevice, pContext);

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
