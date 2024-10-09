#pragma once
#include "stdafx.h"
#include "Effect_Manager.h"
#include "Effect_Layer.h"
#include "Effect.h"
#include "GameInstance.h"
#include "Imgui_Manager.h"
IMPLEMENT_SINGLETON(CEffect_Manager)

CEffect_Manager::CEffect_Manager()
	:m_pGameInstance{CGameInstance::Get_Instance()}
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CEffect_Manager::Initialize()
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CEffect_Manager::Priority_Update(_float fTimeDelta)
{
	for (auto& Pair : m_FinalEffects)
		Pair.second->Priority_Update(fTimeDelta);

	for (auto& Pair : m_TestEffect)
		Pair->Priority_Update(fTimeDelta);
}

void CEffect_Manager::Update(_float fTimeDelta)
{
	for (auto& Pair : m_FinalEffects)
		Pair.second->Update(fTimeDelta);


	for (auto& Pair : m_TestEffect)
		Pair->Update(fTimeDelta);
}

void CEffect_Manager::Late_Update(_float fTimeDelta)
{
	for (auto& Pair : m_FinalEffects)
		Pair.second->Late_Update(fTimeDelta);


	for (auto& Pair : m_TestEffect)
		Pair->Late_Update(fTimeDelta);
}

void CEffect_Manager::Render(_float fTimeDelta)
{
	for (auto& Pair : m_FinalEffects)
		Pair.second->Render(fTimeDelta);


	for (auto& Pair : m_TestEffect)
		Pair->Render(fTimeDelta);
}

CEffect_Layer* CEffect_Manager::Find_Effect_Layer(const wstring& strEffectLayerTag)
{
	auto	iter = m_FinalEffects.find(strEffectLayerTag);

	if (iter == m_FinalEffects.end())
		return nullptr;

	return iter->second;
}

HRESULT CEffect_Manager::Add_Effect_To_Layer(_uint iCurTestEffectIndex, const wstring& strEffectLayerTag, void* pArg)
{
	CEffect_Layer* pLayer = Find_Effect_Layer(strEffectLayerTag);

	if (nullptr == pLayer)
	{
		CEffect_Layer* pLayer = CEffect_Layer::Create();

		pLayer->Add_Effect(m_TestEffect[iCurTestEffectIndex]);

		m_FinalEffects.emplace(strEffectLayerTag, pLayer);
	}
	else
		pLayer->Add_Effect(m_TestEffect[iCurTestEffectIndex]);

	return S_OK;
}

vector<wstring> CEffect_Manager::Get_Layer_List()
{
	vector<wstring> LayerList;

	// m_FinalEffects의 키 값만 벡터에 추가
	for (const auto& pair : m_FinalEffects)
	{
		LayerList.push_back(pair.first);
	}

	return LayerList;
}

HRESULT CEffect_Manager::Add_Test_Effect(EFFECT_TYPE eEffectType, wstring* ModelName)
{
	CEffect::EFFECT_DESC EffectDesc{};

	EffectDesc.ModelName = *ModelName;
	EffectDesc.MaskTextureName = TEXT("Texture_Effect_Default_Mask");
	EffectDesc.DiffuseTextureName = TEXT("Texture_Effect_Default_Diffuse");

	CGameObject* pEffect = nullptr;
	CEffect* pTestEffect = nullptr;

	switch (eEffectType)
	{
	case EFFECT_SINGLE:
		pEffect = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Effect_Single"), &EffectDesc);
		pTestEffect = static_cast<CEffect*>(pEffect);

		if (pTestEffect == nullptr)
			return E_FAIL;

		CImgui_Manager::Get_Instance()->Push_Shader_Tab(static_cast<CTexture*>(pTestEffect->Get_Component(TEXT("Com_DiffuseTexture"))));

		m_TestEffect.push_back(pTestEffect);
		break;
	case EFFECT_MOVETEX:
		pEffect = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Effect_MoveTex"), &EffectDesc);
		pTestEffect = static_cast<CEffect*>(pEffect);

		if (pTestEffect == nullptr)
			return E_FAIL;

		CImgui_Manager::Get_Instance()->Push_Shader_Tab(static_cast<CTexture*>(pTestEffect->Get_Component(TEXT("Com_DiffuseTexture"))));

		m_TestEffect.push_back(pTestEffect);
		break;
	case EFFECT_MULTI:
		pEffect = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Effect_Multi"), &EffectDesc);
		pTestEffect = static_cast<CEffect*>(pEffect);

		if (pTestEffect == nullptr)
			return E_FAIL;

		CImgui_Manager::Get_Instance()->Push_Shader_Tab(static_cast<CTexture*>(pTestEffect->Get_Component(TEXT("Com_DiffuseTexture"))));

		m_TestEffect.push_back(pTestEffect);
		break;
	}

	return S_OK;
}

HRESULT CEffect_Manager::Delete_Test_Effect(_uint iCurTestEffectIndex)
{
	if (iCurTestEffectIndex >= m_TestEffect.size()) 
		return E_FAIL;
	

	if (m_TestEffect[iCurTestEffectIndex]) 
	{
		Safe_Release(m_TestEffect[iCurTestEffectIndex]);
		m_TestEffect[iCurTestEffectIndex] = nullptr;
	}

	m_TestEffect.erase(m_TestEffect.begin() + iCurTestEffectIndex);

	return S_OK;
}

HRESULT CEffect_Manager::Ready_Components()
{
	vector<const _wstring*>* pModelKeys = m_pGameInstance->Find_Prototype_Include_Key(LEVEL_GAMEPLAY, TEXT("Model_Effect"));

	if (pModelKeys == nullptr)
		return E_FAIL;

	for (size_t i = 0; i < pModelKeys->size(); i++)
	{
		const wstring&	pPrototypeTag = *pModelKeys->at(i);

		CModel* pModelComponent = reinterpret_cast<CModel*>(m_pGameInstance->Clone_Component(LEVEL_GAMEPLAY, pPrototypeTag));

		wstring prototypeTag = pPrototypeTag;

		const wstring prefixToRemove = L"Prototype_Component_";
		size_t pos = prototypeTag.find(prefixToRemove);
		if (pos != wstring::npos) {
			prototypeTag.erase(pos, prefixToRemove.length());
		}

		m_EffectModel.emplace(prototypeTag, pModelComponent);
	}
	
	vector<const _wstring*>* pTextureKeys = m_pGameInstance->Find_Prototype_Include_Key(LEVEL_GAMEPLAY, TEXT("Texture_Effect"));

	if (pTextureKeys == nullptr)
		return E_FAIL;

	for (size_t i = 0; i < pTextureKeys->size(); i++)
	{
		const wstring& pPrototypeTag = *pTextureKeys->at(i);

		CTexture* pModelComponent = reinterpret_cast<CTexture*>(m_pGameInstance->Clone_Component(LEVEL_GAMEPLAY, pPrototypeTag));

		wstring prototypeTag = pPrototypeTag;

		const wstring prefixToRemove = L"Prototype_Component_";
		size_t pos = prototypeTag.find(prefixToRemove);
		if (pos != wstring::npos) {
			prototypeTag.erase(pos, prefixToRemove.length());
		}

		m_EffectTexture.emplace(prototypeTag, pModelComponent);
	}

	return S_OK;
}

CEffect_Manager* CEffect_Manager::Create()
{
	CEffect_Manager* pInstance = new CEffect_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CEffect_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);

	for (auto& Pair : m_FinalEffects)
		Safe_Release(Pair.second);

	m_FinalEffects.clear();

	for (auto& Pair : m_EffectModel)
		Safe_Release(Pair.second);

	m_EffectModel.clear();

	for (auto& Pair : m_EffectTexture)
		Safe_Release(Pair.second);

	m_EffectTexture.clear();

	for (auto& Pair : m_TestEffect)
		Safe_Release(Pair);

	m_TestEffect.clear();
}
