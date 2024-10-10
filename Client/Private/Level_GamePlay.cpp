#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "Camera_Free.h"
#include "GameInstance.h"
#include "RenderInstance.h"
//#include "LandObject.h"
#include "Monster.h"
#include "UI_ComboNumber.h"

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

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_UIObjects()))
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

HRESULT CLevel_GamePlay::Ready_UIObjects()
{
	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Cursor"), TEXT("Layer_UI_Cursor"))))
	//	return E_FAIL;

	//HP

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_HpPanel"), TEXT("Layer_UI_HpGauge"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_HpGauge"), TEXT("Layer_UI_HpGauge"))))
		return E_FAIL;

	//캐릭터 아이콘
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Chara_Icon_Panel"), TEXT("Layer_UI_Chara_Icon"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Chara_Icon"), TEXT("Layer_UI_Chara_Icon"))))
		return E_FAIL;

	//공격력 버프

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_AttBufMark"), TEXT("Layer_UI_AttBufMark"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_AttBuf"), TEXT("Layer_UI_AttBuf"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_AttBufNone"), TEXT("Layer_UI_AttBuf"))))
		return E_FAIL;

	// 스킬

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SkillGauge"), TEXT("Layer_UI_SkillGauge"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SkillGaugeBar"), TEXT("Layer_UI_SkillGaugeBar"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SkillNumber"), TEXT("Layer_UI_SkillNumber"))))
		return E_FAIL;

	//타이머
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Timer"), TEXT("Layer_UI_Timer"))))
		return E_FAIL;

	//콤보

	CUI_ComboNumber::UI_COMBO_DESC ComboDesc = {};
	for (int i = 0; i < 3; ++i)
	{
		ComboDesc.iNumUI = i;
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_ComboNumber"), TEXT("Layer_UI_Combo_Number"),&ComboDesc);
	}

	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_ComboFont"), TEXT("Layer_UI_Combo_Font"));

	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_ComboEffect"), TEXT("Layer_UI_Combo_Effect"));


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
