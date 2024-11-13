#pragma once
#include "stdafx.h"
#include "Map_Manager.h"
#include "Effect_Layer.h"
#include "Effect.h"
#include "GameInstance.h"
#include "Imgui_Manager.h"
#include <string>
#include <locale>
#include <codecvt>


IMPLEMENT_SINGLETON(CMap_Manager)

CMap_Manager::CMap_Manager()
	:m_pGameInstance{CGameInstance::Get_Instance()}
{
	Safe_AddRef(m_pGameInstance); 
}

HRESULT CMap_Manager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CMap_Manager::Camera_Update(_float fTimeDelta)
{
}

void CMap_Manager::Update(_float fTimeDelta)
{

}

void CMap_Manager::Late_Update(_float fTimeDelta)
{

}

void CMap_Manager::Render(_float fTimeDelta)
{

}

void CMap_Manager::Map_Change(MAP_TYPE eMapType)
{

}

void CMap_Manager::Push_MapObject(MAP_TYPE eMapType, _wstring& strKey, CGameObject* pGameObject)
{
	if (pGameObject == nullptr)
		return;

	Safe_AddRef(pGameObject);

	switch (eMapType)
	{
	case MAP_SPACE:			m_SpaceModels.emplace(strKey, pGameObject);					/*pGameObject->SetActive(false);*/	break;
	case MAP_DEST_SPACE:	m_Destructive_SpaceModels.emplace(strKey, pGameObject);		/*pGameObject->SetActive(false);*/	break;
	case MAP_VOLCANO:		m_VolcanoModels.emplace(strKey, pGameObject);				pGameObject->SetActive(false);	break;
	case MAP_DEST_VOLCANO:	m_Destructive_VolcanoModels.emplace(strKey, pGameObject);	pGameObject->SetActive(false);	break;
	}
}

HRESULT CMap_Manager::Ready_Components()
{
	return S_OK;
}

void CMap_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
	for (auto& iter : m_SpaceModels)
		Safe_Release(iter.second);
	for (auto& iter : m_Destructive_SpaceModels)
		Safe_Release(iter.second);
	for (auto& iter : m_VolcanoModels)
		Safe_Release(iter.second);
	for (auto& iter : m_Destructive_VolcanoModels)
		Safe_Release(iter.second);

	m_SpaceModels.clear();
	m_Destructive_SpaceModels.clear();
	m_VolcanoModels.clear();
	m_Destructive_VolcanoModels.clear();
}
