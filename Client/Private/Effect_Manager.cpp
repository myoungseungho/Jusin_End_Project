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

	int a = 0;

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

CEffect* CEffect_Manager::Find_EachEffect(const wstring& strEachEffectTag)
{
	auto	iter = m_EachEffects.find(strEachEffectTag);

	if (iter == m_EachEffects.end())
		return nullptr;

	return iter->second;
}

CEffect_Layer* CEffect_Manager::Find_Effect_Layer(const wstring& strEffectLayerTag)
{
	auto	iter = m_FinalEffects.find(strEffectLayerTag);

	if (iter == m_FinalEffects.end())
		return nullptr;

	return iter->second;
}

HRESULT CEffect_Manager::Add_EachEffect(const wstring& strEachEffectTag, CEffect* pEachEffect)
{
	if (nullptr != Find_EachEffect(strEachEffectTag))
		return E_FAIL;

	m_EachEffects.emplace(strEachEffectTag, pEachEffect);

	return S_OK;
}

HRESULT CEffect_Manager::Add_Effect_To_Layer(const wstring& strEachEffectTag, const wstring& strEffectLayerTag, void* pArg)
{
	CEffect* pEachEffect = Find_EachEffect(strEachEffectTag);
	if (nullptr == pEachEffect)
		return E_FAIL;

	CEffect_Layer* pLayer = Find_Effect_Layer(strEffectLayerTag);

	if (nullptr == pLayer)
	{
		CEffect_Layer* pLayer = CEffect_Layer::Create();

		pLayer->Add_Effect(pEachEffect);

		m_FinalEffects.emplace(strEffectLayerTag, pLayer);
	}
	else
		pLayer->Add_Effect(pEachEffect);

	return S_OK;
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

		CImgui_Manager::Get_Instance()->Push_Shader_Tab(static_cast<CTexture*>(pTestEffect->Get_Component(TEXT("Com_Texture"))));

		m_TestEffect.push_back(pTestEffect);
		break;
	case EFFECT_MOVETEX:
		pEffect = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Effect_MoveTex"), &EffectDesc);
		pTestEffect = static_cast<CEffect*>(pEffect);

		if (pTestEffect == nullptr)
			return E_FAIL;

		CImgui_Manager::Get_Instance()->Push_Shader_Tab(static_cast<CTexture*>(pTestEffect->Get_Component(TEXT("Com_Texture"))));

		m_TestEffect.push_back(pTestEffect);
		break;
	case EFFECT_MULTI:
		pEffect = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Effect_Multi"), &EffectDesc);
		pTestEffect = static_cast<CEffect*>(pEffect);

		if (pTestEffect == nullptr)
			return E_FAIL;

		CImgui_Manager::Get_Instance()->Push_Shader_Tab(static_cast<CTexture*>(pTestEffect->Get_Component(TEXT("Com_Texture"))));

		m_TestEffect.push_back(pTestEffect);
		break;
	}

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

	for (auto& Pair : m_EachEffects)
		Safe_Release(Pair.second);

	m_EachEffects.clear();

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
