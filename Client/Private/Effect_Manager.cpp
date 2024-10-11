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

HRESULT CEffect_Manager::Add_Effect_To_Layer(_int iCurTestEffectIndex, const wstring& strEffectLayerTag, void* pArg)
{
	CEffect_Layer* pLayer = Find_Effect_Layer(strEffectLayerTag);

	if (nullptr == pLayer)
	{
		CEffect_Layer* pLayer = CEffect_Layer::Create();

		CEffect::EFFECT_DESC EffectDesc {};
		EffectDesc.ModelName = m_TestEffect[iCurTestEffectIndex]->m_ModelName;
		EffectDesc.EffectName = m_TestEffect[iCurTestEffectIndex]->m_EffectName;
		EffectDesc.MaskTextureName = m_TestEffect[iCurTestEffectIndex]->m_MaskTextureName;
		EffectDesc.DiffuseTextureName = m_TestEffect[iCurTestEffectIndex]->m_DiffuseTextureName;
		EffectDesc.vPosition = m_TestEffect[iCurTestEffectIndex]->Get_Effect_Position();
		EffectDesc.vScaled = m_TestEffect[iCurTestEffectIndex]->Get_Effect_Scaled();
		EffectDesc.vRotation= m_TestEffect[iCurTestEffectIndex]->Get_Effect_Rotation();
		EffectDesc.iUnique_Index = m_TestEffect[iCurTestEffectIndex]->m_iUnique_Index;
		EffectDesc.SRV_Ptr = static_cast<CTexture*>(m_TestEffect[iCurTestEffectIndex]->Get_Component(TEXT("Com_DiffuseTexture")))->Get_SRV(0);
		EffectDesc.iRenderIndex = 2;
		pLayer->Add_Effect(static_cast<CEffect*>(m_TestEffect[iCurTestEffectIndex]->Clone(&EffectDesc)));

		m_FinalEffects.emplace(strEffectLayerTag, pLayer);
	}
	else
	{
		CEffect::EFFECT_DESC EffectDesc{};
		EffectDesc.ModelName = m_TestEffect[iCurTestEffectIndex]->m_ModelName;
		EffectDesc.EffectName = m_TestEffect[iCurTestEffectIndex]->m_EffectName;
		EffectDesc.MaskTextureName = m_TestEffect[iCurTestEffectIndex]->m_MaskTextureName;
		EffectDesc.DiffuseTextureName = m_TestEffect[iCurTestEffectIndex]->m_DiffuseTextureName;
		EffectDesc.vPosition = m_TestEffect[iCurTestEffectIndex]->Get_Effect_Position();
		EffectDesc.vScaled = m_TestEffect[iCurTestEffectIndex]->Get_Effect_Scaled();
		EffectDesc.vRotation = m_TestEffect[iCurTestEffectIndex]->Get_Effect_Rotation();
		EffectDesc.iUnique_Index = m_TestEffect[iCurTestEffectIndex]->m_iUnique_Index;
		EffectDesc.SRV_Ptr = static_cast<CTexture*>(m_TestEffect[iCurTestEffectIndex]->Get_Component(TEXT("Com_DiffuseTexture")))->Get_SRV(0);
		EffectDesc.iRenderIndex = 2;
		pLayer->Add_Effect(static_cast<CEffect*>(m_TestEffect[iCurTestEffectIndex]->Clone(&EffectDesc)));
	}

	return S_OK;
}

HRESULT CEffect_Manager::Add_All_Effect_To_Layer(const wstring& strEffectLayerTag, void* pArg)
{
	CEffect_Layer* pLayer = Find_Effect_Layer(strEffectLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CEffect_Layer::Create();
		m_FinalEffects.emplace(strEffectLayerTag, pLayer);
	}

	for (auto* pEffect : m_TestEffect)
	{
		if (pEffect != nullptr)
		{
			CEffect::EFFECT_DESC EffectDesc{};
			EffectDesc.ModelName = pEffect->m_ModelName;
			EffectDesc.MaskTextureName = pEffect->m_MaskTextureName;
			EffectDesc.DiffuseTextureName = pEffect->m_DiffuseTextureName;

			pLayer->Add_Effect(static_cast<CEffect*>(pEffect->Clone(&EffectDesc)));
		}
	}

	return S_OK;
}

HRESULT CEffect_Manager::Delete_Layer(const wstring& strEffectLayerTag)
{
	CEffect_Layer* pLayer = Find_Effect_Layer(strEffectLayerTag);

	if (pLayer == nullptr)
	{
		return E_FAIL;
	}

	Safe_Release(pLayer);

	m_FinalEffects.erase(strEffectLayerTag);

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

HRESULT CEffect_Manager::Add_Test_Effect(EFFECT_TYPE eEffectType, wstring* EffectName, wstring* ModelName)
{
	CEffect::EFFECT_DESC EffectDesc{};

	EffectDesc.ModelName = *ModelName;
	EffectDesc.EffectName = *EffectName;
	EffectDesc.MaskTextureName = TEXT("Texture_Effect_Default_Mask");
	EffectDesc.DiffuseTextureName = TEXT("Texture_Effect_Default_Diffuse");
	EffectDesc.iUnique_Index = m_TestEffect_Count++;
	EffectDesc.vPosition = { 0.f, 0.f, 0.f };
	EffectDesc.vScaled = { 1.f, 1.f, 1.f };
	EffectDesc.vRotation = { 0.f, 0.f, 0.f };
	EffectDesc.iRenderIndex = 1;

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

HRESULT CEffect_Manager::Delete_Test_Effect(_uint iCurTestEffectID)
{
	for (auto it = m_TestEffect.begin(); it != m_TestEffect.end(); ++it)
	{
		CEffect* pEffect = *it;

		if (pEffect && pEffect->m_iUnique_Index == iCurTestEffectID)
		{
			Safe_Release(pEffect);
			m_TestEffect.erase(it);
			return S_OK; 
		}
	}

	return E_FAIL;
}

HRESULT CEffect_Manager::Delete_All_Test_Effect()
{
	for (auto& pEffect : m_TestEffect)
	{
		if (pEffect)
		{
			Safe_Release(pEffect);
		}
	}

	m_TestEffect.clear();

	return S_OK;
}

HRESULT CEffect_Manager::Set_Effect_Scaled(_int EffectId, _float3 ChangeScaled)
{
	for (auto it = m_TestEffect.begin(); it != m_TestEffect.end(); ++it)
	{
		CEffect* pEffect = *it;

		if (pEffect && pEffect->m_iUnique_Index == EffectId)
		{
			pEffect->Set_Effect_Scaled(ChangeScaled);

			return S_OK;
		}
	}

	return E_FAIL;
}

HRESULT CEffect_Manager::Set_Effect_Position(_int EffectId, _float3 ChangePosition)
{
	for (auto it = m_TestEffect.begin(); it != m_TestEffect.end(); ++it)
	{
		CEffect* pEffect = *it;

		if (pEffect && pEffect->m_iUnique_Index == EffectId)
		{
			pEffect->Set_Effect_Position(ChangePosition);

			return S_OK;
		}
	}

	return E_FAIL;
}

HRESULT CEffect_Manager::Set_Effect_Rotation(_int EffectId, _float3 ChangeRotation)
{
	for (auto it = m_TestEffect.begin(); it != m_TestEffect.end(); ++it)
	{
		CEffect* pEffect = *it;

		if (pEffect && pEffect->m_iUnique_Index == EffectId)
		{
			pEffect->Set_Effect_Rotation(ChangeRotation);

			return S_OK;
		}
	}

	return E_FAIL;
}

_float3 CEffect_Manager::Get_Effect_Scaled(_int EffectId)
{
	for (auto it = m_TestEffect.begin(); it != m_TestEffect.end(); ++it)
	{
		CEffect* pEffect = *it;

		if (pEffect && pEffect->m_iUnique_Index == EffectId)
		{
			return pEffect->Get_Effect_Scaled();
			
		}
	}

	return _float3(0.f, 0.f, 0.f);
}

_float3 CEffect_Manager::Get_Effect_Position(_int EffectId)
{
	for (auto it = m_TestEffect.begin(); it != m_TestEffect.end(); ++it)
	{
		CEffect* pEffect = *it;

		if (pEffect && pEffect->m_iUnique_Index == EffectId)
		{
			return pEffect->Get_Effect_Position();

		}
	}

	return _float3(0.f, 0.f, 0.f);
}

_float3 CEffect_Manager::Get_Effect_Rotation(_int EffectId)
{
	for (auto it = m_TestEffect.begin(); it != m_TestEffect.end(); ++it)
	{
		CEffect* pEffect = *it;

		if (pEffect && pEffect->m_iUnique_Index == EffectId)
		{
			return pEffect->Get_Effect_Rotation();

		}
	}

	return _float3(0.f, 0.f, 0.f);
}

void CEffect_Manager::Add_KeyFrame(_int EffectId, EFFECT_KEYFRAME NewKeyFrame)
{
	for (auto it = m_TestEffect.begin(); it != m_TestEffect.end(); ++it)
	{
		CEffect* pEffect = *it;

		if (pEffect && pEffect->m_iUnique_Index == EffectId)
		{
			pEffect->Add_KeyFrame(NewKeyFrame);
			return;
		}
	}
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
