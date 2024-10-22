#pragma once
#include "stdafx.h"
#include "Effect_Manager.h"
#include "Effect_Layer.h"
#include "Effect.h"
#include "GameInstance.h"
#include "Imgui_Manager.h"
#include <Effect_Single.h>
#include <string>
#include <locale>
#include <codecvt>
#include "IMGUI_Shader_Tab.h"
#include "Effect_Multi.h"
#include "Effect_MoveTex.h"

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

	for (auto& Pair : m_UsingEffect)
		Pair->Priority_Update(fTimeDelta);

}

void CEffect_Manager::Update(_float fTimeDelta)
{
	for (auto& Pair : m_FinalEffects)
		Pair.second->Update(fTimeDelta);

	for (auto& Pair : m_TestEffect)
		Pair->Update(fTimeDelta);

	for (auto& Pair : m_UsingEffect)
		Pair->Update(fTimeDelta);
}

void CEffect_Manager::Late_Update(_float fTimeDelta)
{
	for (auto& Pair : m_FinalEffects)
		if (Pair.second->m_bIsRender)
		{
			Pair.second->Late_Update(fTimeDelta);
		}

	for (auto& Pair : m_TestEffect)
		Pair->Late_Update(fTimeDelta);

	for (auto& Pair : m_UsingEffect)
		Pair->Late_Update(fTimeDelta);
}

void CEffect_Manager::Render(_float fTimeDelta)
{
	for (auto& Pair : m_FinalEffects)
		Pair.second->Render(fTimeDelta);
		
	for (auto& Pair : m_TestEffect)
		Pair->Render(fTimeDelta);

	for (auto& Pair : m_UsingEffect)
		Pair->Render(fTimeDelta);
}

HRESULT CEffect_Manager::Copy_Layer(const wstring& strEffectLayerTag)
{
	CEffect_Layer* pLayer = Find_Effect_Layer(strEffectLayerTag);

	if (pLayer == nullptr)
		return E_FAIL;

	m_UsingEffect.push_back(pLayer->Clone());

	return S_OK;
}

HRESULT CEffect_Manager::Set_Saved_Effects(vector<EFFECT_LAYER_DATA>* pSavedEffect)
{
	// pSavedEffect가 nullptr인 경우 실패 반환
	if (!pSavedEffect) return E_FAIL;

	//// 기존에 존재하는 레이어들을 제거
	//for (auto& layerPair : m_FinalEffects)
	//{
	//	Safe_Release(layerPair.second);
	//}
	//m_FinalEffects.clear();

	// pSavedEffect에 있는 각 레이어 데이터를 순회
	for (const auto& layerData : *pSavedEffect)
	{
		// 새로운 레이어 생성
		CEffect_Layer* pLayer = CEffect_Layer::Create();
		if (!pLayer) continue;  // 레이어 생성에 실패한 경우 건너뜀

		// 레이어에 필요한 설정 값을 할당
		pLayer->m_fDuration = layerData.duration;
		pLayer->m_fTickPerSecond = layerData.tickPerSecond;
		pLayer->m_iNumKeyFrames = layerData.keyFramesCount;

		// 각 이펙트 데이터를 순회하며 레이어에 추가
		for (const auto& effectData : layerData.effects)
		{
			// 이펙트 생성 정보 초기화
			CEffect::EFFECT_DESC EffectDesc;
			EffectDesc.ModelName = effectData.modelName;
			EffectDesc.EffectName = effectData.effectName;
			EffectDesc.MaskTextureName = effectData.maskTextureName;
			EffectDesc.DiffuseTextureName = effectData.diffuseTextureName;
			EffectDesc.EffectType = static_cast<EFFECT_TYPE>(effectData.effectType);
			EffectDesc.vPosition = effectData.position;
			EffectDesc.vScaled = effectData.scale;
			EffectDesc.vRotation = effectData.rotation;
			EffectDesc.iUnique_Index = m_TestEffect_Count++;
			EffectDesc.iRenderIndex = effectData.renderIndex;
			EffectDesc.iPassIndex = effectData.passIndex;
			EffectDesc.SRV_Ptr = nullptr;  // SRV는 nullptr로 초기화; 필요한 경우 적절히 설정

			CEffect_Single* pSingleEffect = { nullptr };
			CEffect_Multi* pMultiEffect = { nullptr };
			CEffect_MoveTex* pMoveTexEffect = { nullptr };

			wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
			string sMaskTextureName = converter.to_bytes(EffectDesc.MaskTextureName);

			// 이펙트를 클론하여 레이어에 추가
			switch (EffectDesc.EffectType)
			{
			case EFFECT_SINGLE:
				pSingleEffect = static_cast<CEffect_Single*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Effect_Single"), &EffectDesc));

				if (!pSingleEffect) {
					Safe_Release(pLayer);
					return E_FAIL;
				}

				CImgui_Manager::Get_Instance()->Load_Shader_Tab(static_cast<CTexture*>(pSingleEffect->Get_Component(TEXT("Com_DiffuseTexture"))), sMaskTextureName, EffectDesc.iUnique_Index);

				// 이펙트에 기본 설정값 적용
				pSingleEffect->m_bIsNotPlaying = effectData.isNotPlaying;
				pSingleEffect->m_bIsLoop = effectData.isLoop;

				// 이펙트의 각 키프레임 설정
				for (const auto& keyFrameData : effectData.keyframes)
				{
					EFFECT_KEYFRAME keyFrame;
					keyFrame.vPosition = keyFrameData.position;
					keyFrame.vScale = keyFrameData.scale;
					keyFrame.vRotation = keyFrameData.rotation;
					keyFrame.fCurTime = keyFrameData.curTime;
					keyFrame.fDuration = keyFrameData.duration;
					keyFrame.bIsNotPlaying = keyFrameData.bIsNotPlaying;

					// 키프레임 추가
					pSingleEffect->Add_KeyFrame(keyFrameData.keyFrameNumber, keyFrame);
				}

				// 레이어에 이펙트를 추가
				if (FAILED(pLayer->Add_Effect(pSingleEffect))) {
					Safe_Release(pSingleEffect);  // 실패 시 이펙트를 해제
					Safe_Release(pLayer);
					return E_FAIL;
				}
				break;
			case EFFECT_MULTI:
				pMultiEffect = static_cast<CEffect_Multi*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Effect_Multi"), &EffectDesc));

				if (!pMultiEffect) {
					Safe_Release(pLayer);
					return E_FAIL;
				}


				CImgui_Manager::Get_Instance()->Load_Shader_Tab(static_cast<CTexture*>(pMultiEffect->Get_Component(TEXT("Com_DiffuseTexture"))), sMaskTextureName, EffectDesc.iUnique_Index);

				// 이펙트에 기본 설정값 적용
				pMultiEffect->m_bIsNotPlaying = effectData.isNotPlaying;
				pMultiEffect->m_bIsLoop = effectData.isLoop;

				// 이펙트의 각 키프레임 설정
				for (const auto& keyFrameData : effectData.keyframes)
				{
					EFFECT_KEYFRAME keyFrame;
					keyFrame.vPosition = keyFrameData.position;
					keyFrame.vScale = keyFrameData.scale;
					keyFrame.vRotation = keyFrameData.rotation;
					keyFrame.fCurTime = keyFrameData.curTime;
					keyFrame.fDuration = keyFrameData.duration;
					keyFrame.bIsNotPlaying = keyFrameData.bIsNotPlaying;

					// 키프레임 추가
					pMultiEffect->Add_KeyFrame(keyFrameData.keyFrameNumber, keyFrame);
				}

				// 레이어에 이펙트를 추가
				if (FAILED(pLayer->Add_Effect(pMultiEffect))) {
					Safe_Release(pMultiEffect);  // 실패 시 이펙트를 해제
					Safe_Release(pLayer);
					return E_FAIL;
				}
				break;
			case EFFECT_MOVETEX:
				pMoveTexEffect = static_cast<CEffect_MoveTex*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Effect_MoveTex"), &EffectDesc));

				if (!pMoveTexEffect) {
					Safe_Release(pLayer);
					return E_FAIL;
				}

				CImgui_Manager::Get_Instance()->Load_Shader_Tab(static_cast<CTexture*>(pMoveTexEffect->Get_Component(TEXT("Com_DiffuseTexture"))), sMaskTextureName, EffectDesc.iUnique_Index);

				// 이펙트에 기본 설정값 적용
				pMoveTexEffect->m_bIsNotPlaying = effectData.isNotPlaying;
				pMoveTexEffect->m_bIsLoop = effectData.isLoop;

				// 이펙트의 각 키프레임 설정
				for (const auto& keyFrameData : effectData.keyframes)
				{
					EFFECT_KEYFRAME keyFrame;
					keyFrame.vPosition = keyFrameData.position;
					keyFrame.vScale = keyFrameData.scale;
					keyFrame.vRotation = keyFrameData.rotation;
					keyFrame.fCurTime = keyFrameData.curTime;
					keyFrame.fDuration = keyFrameData.duration;
					keyFrame.bIsNotPlaying = keyFrameData.bIsNotPlaying;

					// 키프레임 추가
					pMoveTexEffect->Add_KeyFrame(keyFrameData.keyFrameNumber, keyFrame);
				}

				// 레이어에 이펙트를 추가
				if (FAILED(pLayer->Add_Effect(pMoveTexEffect))) {
					Safe_Release(pMoveTexEffect);  // 실패 시 이펙트를 해제
					Safe_Release(pLayer);
					return E_FAIL;
				}
				break;
			}
		}

		// 생성된 레이어를 m_FinalEffects에 추가
		m_FinalEffects.emplace(layerData.layerName, pLayer);
	}

	return S_OK;
}

CEffect_Layer* CEffect_Manager::Find_Effect_Layer(const wstring& strEffectLayerTag)
{
	auto	iter = m_FinalEffects.find(strEffectLayerTag);

	if (iter == m_FinalEffects.end())
		return nullptr;

	return iter->second;
}

CEffect* CEffect_Manager::Find_In_Layer_Effect(wstring& layerName, wstring& effectName)
{
	CEffect_Layer* pLayer = Find_Effect_Layer(layerName);

	if (pLayer == nullptr)
		return nullptr;

	return pLayer->Find_Effect(effectName);
}

_bool CEffect_Manager::Find_KeyFrame(wstring& layerName, wstring& effectName, _uint frameNumber)
{
	CEffect_Layer* pLayer = Find_Effect_Layer(layerName);
	if (!pLayer)
		return false;

	CEffect* pEffect = pLayer->Find_Effect(effectName);
	if (!pEffect)
		return false;

	return pEffect->Find_KeyFrame(frameNumber);
}

void CEffect_Manager::Set_Render_Layer(const wstring& strEffectLayerTag)
{
	for (auto& Pair : m_FinalEffects)
		Pair.second->m_bIsRender = { false };

	Find_Effect_Layer(strEffectLayerTag)->m_bIsRender = { true };
}

EFFECT_KEYFRAME CEffect_Manager::Get_KeyFrame(wstring& layerName, wstring& effectName, _uint frameNumber)
{
	CEffect_Layer* pLayer = Find_Effect_Layer(layerName);

	CEffect* pEffect = pLayer->Find_Effect(effectName);

	return pEffect->Get_KeyFrame(frameNumber);
}

EFFECT_KEYFRAME CEffect_Manager::Get_Near_Front_KeyFrame(wstring& layerName, wstring& effectName, _uint frameNumber)
{
	CEffect_Layer* pLayer = Find_Effect_Layer(layerName);

	CEffect* pEffect = pLayer->Find_Effect(effectName);

	return pEffect->Get_Near_Front_KeyFrame(frameNumber);
}

HRESULT CEffect_Manager::Add_Effect_To_Layer(_int iCurTestEffectIndex, const wstring& strEffectLayerTag, void* pArg)
{
	CEffect_Layer* pLayer = Find_Effect_Layer(strEffectLayerTag);

	if (nullptr == pLayer)
	{
		CEffect_Layer* pLayer = CEffect_Layer::Create();

		CEffect::EFFECT_DESC EffectDesc {};
		for (auto& iter : m_TestEffect)
		{
			if (iter->m_iUnique_Index == iCurTestEffectIndex)
			{
				EffectDesc.ModelName =iter->m_ModelName;
				EffectDesc.EffectName =iter->m_EffectName;
				EffectDesc.MaskTextureName =iter->m_MaskTextureName;
				EffectDesc.DiffuseTextureName =iter->m_DiffuseTextureName;
				EffectDesc.vPosition =iter->Get_Effect_Position();
				EffectDesc.vScaled =iter->Get_Effect_Scaled();
				EffectDesc.vRotation =iter->Get_Effect_Rotation();
				EffectDesc.iUnique_Index =iter->m_iUnique_Index;
				EffectDesc.SRV_Ptr = static_cast<CTexture*>(iter->Get_Component(TEXT("Com_DiffuseTexture")))->Get_SRV(0);
				EffectDesc.iRenderIndex = 2;

				CEffect* pClone = static_cast<CEffect*>(iter->Clone(&EffectDesc));

				CImgui_Manager::Get_Instance()->Access_Shader_Tab(EffectDesc.iUnique_Index)->Update_TestToLayer_TextureCom(static_cast<CTexture*>(pClone->Get_Component(TEXT("Com_DiffuseTexture"))));

				pLayer->Add_Effect(pClone);

				m_FinalEffects.emplace(strEffectLayerTag, pLayer);

				EFFECT_KEYFRAME FirstKeyFrame{};

				FirstKeyFrame.vPosition = EffectDesc.vPosition;
				FirstKeyFrame.vRotation = EffectDesc.vRotation;
				FirstKeyFrame.vScale = EffectDesc.vScaled;

				pLayer->Find_Effect(EffectDesc.EffectName)->Add_KeyFrame(0, FirstKeyFrame);

				_int iCount = 0;
				for (auto& iter : m_TestEffect)
				{
					if (iter->m_iUnique_Index == iCurTestEffectIndex)
					{
						Safe_Release(iter);
						m_TestEffect.erase(m_TestEffect.begin() + iCount);
						break;
					}
					iCount++;
				}

			}
		}
	}
	else
	{
		CEffect::EFFECT_DESC EffectDesc{};
		for (auto& iter : m_TestEffect)
		{
			if (iter->m_iUnique_Index == iCurTestEffectIndex)
			{
				EffectDesc.ModelName =iter->m_ModelName;
				EffectDesc.EffectName =iter->m_EffectName;
				EffectDesc.MaskTextureName =iter->m_MaskTextureName;
				EffectDesc.DiffuseTextureName =iter->m_DiffuseTextureName;
				EffectDesc.vPosition =iter->Get_Effect_Position();
				EffectDesc.vScaled =iter->Get_Effect_Scaled();
				EffectDesc.vRotation =iter->Get_Effect_Rotation();
				EffectDesc.iUnique_Index =iter->m_iUnique_Index;
				EffectDesc.SRV_Ptr = static_cast<CTexture*>(iter->Get_Component(TEXT("Com_DiffuseTexture")))->Get_SRV(0);
				EffectDesc.iRenderIndex = 2;
				CEffect* pClone = static_cast<CEffect*>(iter->Clone(&EffectDesc));
				
				CImgui_Manager::Get_Instance()->Access_Shader_Tab(EffectDesc.iUnique_Index)->Update_TestToLayer_TextureCom(static_cast<CTexture*>(pClone->Get_Component(TEXT("Com_DiffuseTexture"))));
					
				pLayer->Add_Effect(pClone);

				EFFECT_KEYFRAME FirstKeyFrame{};

				FirstKeyFrame.vPosition = EffectDesc.vPosition;
				FirstKeyFrame.vRotation = EffectDesc.vRotation;
				FirstKeyFrame.vScale = EffectDesc.vScaled;

				pLayer->Find_Effect(EffectDesc.EffectName)->Add_KeyFrame(0, FirstKeyFrame);

				_int iCount = 0;
				for (auto& iter : m_TestEffect)
				{
					if (iter->m_iUnique_Index == iCurTestEffectIndex)
					{
						Safe_Release(iter);
						m_TestEffect.erase(m_TestEffect.begin() + iCount);
						break;
					}
					iCount++;
				}
			}
		}
	
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

	for (auto& pEffect : m_TestEffect)
	{
		if (pEffect != nullptr)
		{
			CEffect::EFFECT_DESC EffectDesc{};
			EffectDesc.ModelName = pEffect->m_ModelName;
			EffectDesc.EffectName = pEffect->m_EffectName;
			EffectDesc.MaskTextureName = pEffect->m_MaskTextureName;
			EffectDesc.DiffuseTextureName = pEffect->m_DiffuseTextureName;
			EffectDesc.vPosition = pEffect->Get_Effect_Position();
			EffectDesc.vScaled = pEffect->Get_Effect_Scaled();
			EffectDesc.vRotation = pEffect->Get_Effect_Rotation();
			EffectDesc.iUnique_Index = pEffect->m_iUnique_Index;
			EffectDesc.SRV_Ptr = static_cast<CTexture*>(pEffect->Get_Component(TEXT("Com_DiffuseTexture")))->Get_SRV(0);
			EffectDesc.iRenderIndex = 2;

			CEffect* pClone = static_cast<CEffect*>(pEffect->Clone(&EffectDesc));

			CImgui_Manager::Get_Instance()->Access_Shader_Tab(EffectDesc.iUnique_Index)->Update_TestToLayer_TextureCom(static_cast<CTexture*>(pClone->Get_Component(TEXT("Com_DiffuseTexture"))));

			pLayer->Add_Effect(pClone);

			EFFECT_KEYFRAME FirstKeyFrame{};

			FirstKeyFrame.vPosition = EffectDesc.vPosition;
			FirstKeyFrame.vRotation = EffectDesc.vRotation;
			FirstKeyFrame.vScale = EffectDesc.vScaled;

			pLayer->Find_Effect(EffectDesc.EffectName)->Add_KeyFrame(0, FirstKeyFrame);
		}
	}

	for (auto& pEffect : m_TestEffect)
	{
		Safe_Release(pEffect);
	}
	m_TestEffect.clear();

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

vector<wstring> CEffect_Manager::Get_In_Layer_Effect_List(wstring* LayerName)
{
	vector<wstring> EffectNames;

	auto it = m_FinalEffects.find(*LayerName);
	if (it != m_FinalEffects.end())
	{
		CEffect_Layer* pLayer = it->second;
		const auto& effects = pLayer->Get_Effects();

		for (const auto& effect : effects)
		{
			if (effect)
			{
				EffectNames.push_back(effect->m_EffectName);
			}
		}
	}

	return EffectNames;
}

HRESULT CEffect_Manager::Add_Test_Effect(EFFECT_TYPE eEffectType, wstring* EffectName, wstring* ModelName)
{
	CEffect::EFFECT_DESC EffectDesc{};

	EffectDesc.ModelName = *ModelName;
	EffectDesc.EffectName = *EffectName;
	EffectDesc.MaskTextureName = TEXT("Texture_Effect_Default_Mask");
	EffectDesc.DiffuseTextureName = TEXT("Texture_Effect_cmn_test");
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

vector<_int> CEffect_Manager::Delete_All_Test_Effect()
{
	vector<_int> UniqueIDs;

	for (auto& pEffect : m_TestEffect)
	{
		if (pEffect)
		{
			UniqueIDs.push_back(pEffect->m_iUnique_Index);
			Safe_Release(pEffect);
		}
	}

	m_TestEffect.clear();

	return UniqueIDs;
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

HRESULT CEffect_Manager::Set_Layer_Effect_Scaled(wstring& layerName, wstring& effectName, _float3 ChangeScaled)
{
	CEffect* pEffect = Find_In_Layer_Effect(layerName, effectName);

	if (pEffect) {
		pEffect->Set_Effect_Scaled(ChangeScaled);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CEffect_Manager::Set_Layer_Effect_Position(wstring& layerName, wstring& effectName, _float3 ChangePosition)
{
	CEffect* pEffect = Find_In_Layer_Effect(layerName, effectName);

	if (pEffect) {
		pEffect->Set_Effect_Position(ChangePosition);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CEffect_Manager::Set_Layer_Effect_Rotation(wstring& layerName, wstring& effectName, _float3 ChangeRotation)
{
	CEffect* pEffect = Find_In_Layer_Effect(layerName, effectName);

	if (pEffect) {
		pEffect->Set_Effect_Rotation(ChangeRotation);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CEffect_Manager::Set_Layer_Effect_IsNotPlaying(wstring& layerName, wstring& effectName, _bool bIsNotPlaying)
{
	CEffect* pEffect = Find_In_Layer_Effect(layerName, effectName);

	if (pEffect) {
		pEffect->Set_Effect_IsNotPlaying(bIsNotPlaying);
		return S_OK;
	}
	return E_FAIL;
}

_float3 CEffect_Manager::Get_Layer_Effect_Scaled(wstring& layerName, wstring& effectName)
{
	CEffect* pEffect = Find_In_Layer_Effect(layerName, effectName);

	if (pEffect) {
		return pEffect->Get_Effect_Scaled();
	}
	return _float3(0.f, 0.f, 0.f); // 기본값 반환
}

_float3 CEffect_Manager::Get_Layer_Effect_Position(wstring& layerName, wstring& effectName)
{
	CEffect* pEffect = Find_In_Layer_Effect(layerName, effectName);

	if (pEffect) {
		return pEffect->Get_Effect_Position();
	}
	return _float3(0.f, 0.f, 0.f); // 기본값 반환
}

_float3 CEffect_Manager::Get_Layer_Effect_Rotation(wstring& layerName, wstring& effectName)
{
	CEffect* pEffect = Find_In_Layer_Effect(layerName, effectName);

	if (pEffect) {
		return pEffect->Get_Effect_Rotation();
	}
	return _float3(0.f, 0.f, 0.f); // 기본값 반환
}

_bool CEffect_Manager::Get_Layer_Effect_IsPlaying(wstring& layerName, wstring& effectName)
{
	CEffect* pEffect = Find_In_Layer_Effect(layerName, effectName);

	if (pEffect) {
		return pEffect->m_bIsNotPlaying;
	}

	return false;
}

void CEffect_Manager::Delete_KeyFrame(const wstring& LayerName, const wstring& EffectName, _uint KeyFrameNumber)
{
	auto layerIt = m_FinalEffects.find(LayerName);
	if (layerIt != m_FinalEffects.end())
	{
		CEffect_Layer* pLayer = layerIt->second;
		if (pLayer)
		{
			CEffect* pEffect = pLayer->Find_Effect(EffectName);
			if (pEffect)
			{
				pEffect->Delete_KeyFrame(KeyFrameNumber);
				return;
			}
		}
	}
}

void CEffect_Manager::Add_KeyFrame(const wstring& LayerName, const wstring& EffectName, _uint KeyFrameNumber, EFFECT_KEYFRAME NewKeyFrame)
{
	auto layerIt = m_FinalEffects.find(LayerName);
	if (layerIt != m_FinalEffects.end())
	{
		CEffect_Layer* pLayer = layerIt->second;
		if (pLayer)
		{
			CEffect* pEffect = pLayer->Find_Effect(EffectName);
			if (pEffect)
			{
				pEffect->Add_KeyFrame(KeyFrameNumber, NewKeyFrame);
				return;
			}
		}
	}
}

EFFECT_KEYFRAME CEffect_Manager::Get_Layer_Effect_KeyFrame(wstring& layerName, wstring& effectName, _uint KeyFrameNumber)
{
	CEffect* pEffect = Find_In_Layer_Effect(layerName, effectName);

	if (pEffect) {
		return pEffect->Get_KeyFrame(KeyFrameNumber);
	}

	return EFFECT_KEYFRAME();
}

HRESULT CEffect_Manager::Play_Layer_Animation(_float fTimeDelta, const wstring& LayerName)
{
	CEffect_Layer* pLayer = Find_Effect_Layer(LayerName);

	if (pLayer)
	{
		pLayer->Play_Effect_Animation(fTimeDelta);
	}
	else
		return E_FAIL;
	

	return S_OK;
}

HRESULT CEffect_Manager::Set_Layer_Animation_Position(const wstring& LayerName, _float CurAnimPos)
{
	CEffect_Layer* pLayer = Find_Effect_Layer(LayerName);

	if (pLayer)
	{
		pLayer->Set_Animation_Position(CurAnimPos);
	}
	else
		return E_FAIL;


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

	CEffect_Manager::Get_Instance()->Destroy_Instance();
}
