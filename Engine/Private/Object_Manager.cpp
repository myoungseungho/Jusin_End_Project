#include "..\Public\Object_Manager.h"

#include "Layer.h"
#include "GameObject.h"
#include <locale>
#include <codecvt>

CObject_Manager::CObject_Manager()
{
}

CComponent* CObject_Manager::Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex)
{
	if (nullptr == m_pLayers ||
		iLevelIndex >= m_iNumLevels)
		return nullptr;

	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Component(strComponentTag, iIndex);
}

CGameObject* CObject_Manager::Get_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, _uint iIndex)
{
	if (nullptr == m_pLayers ||
		iLevelIndex >= m_iNumLevels)
		return nullptr;

	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_GameObject(iIndex);
}

HRESULT CObject_Manager::Initialize(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;

	m_pLayers = new map<const _wstring, class CLayer*>[iNumLevels];

	return S_OK;
}

HRESULT CObject_Manager::Add_Prototype(const _wstring& strPrototypeTag, CGameObject* pPrototype)
{
	if (nullptr == Find_Prototype(strPrototypeTag))
		m_Prototypes.emplace(strPrototypeTag, pPrototype);
	else
		Safe_Release(pPrototype);

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject_ToLayer(_uint iLevelIndex, const _wstring& strPrototypeTag, const _wstring& strLayerTag, void* pArg)
{
	/* ������ ������ü�� ã��. */
	CGameObject* pPrototype = Find_Prototype(strPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	/* ã�Ƴ� ������ü�� �����Ͽ� �纻��ü�� �����ؿ���. */
	CGameObject* pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	/* ������ �纻��ü�� ���̾ �߰��Ѵ�.. */
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
	{
		CLayer* pLayer = CLayer::Create();

		pLayer->Add_GameObject(pGameObject);

		m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
	}
	else
		pLayer->Add_GameObject(pGameObject);

	return S_OK;
}


CGameObject* CObject_Manager::Clone_GameObject(const _wstring& strPrototypeTag, void* pArg)
{
	/* ������ ������ü�� ã��. */
	CGameObject* pPrototype = Find_Prototype(strPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;

	/* ã�Ƴ� ������ü�� �����Ͽ� �纻��ü�� �����ؿ���. */
	CGameObject* pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return nullptr;

	return pGameObject;
}

HRESULT CObject_Manager::Get_Prototype_Names(vector<string>* pVector)
{
	if (pVector == nullptr)
		return E_FAIL;

	wstring_convert<codecvt_utf8<wchar_t>> converter;

	for (auto& iter : m_Prototypes)
	{
		// Convert _wstring (iter.first) to string
		string converted = converter.to_bytes(iter.first);
		pVector->push_back(converted);
	}

	return S_OK;
}

HRESULT CObject_Manager::Add_Object_Layers_Vector(_uint _level, vector<pair<string, list<CGameObject*>>>* pVector)
{
	if (pVector == nullptr)
		return E_FAIL;

	// Create a wstring_convert object to perform conversion
	wstring_convert<codecvt_utf8<wchar_t>> converter;
	list<CGameObject*> tempList;
	for (auto& iter : m_pLayers[_level])
	{
		// Convert wstring (iter.first) to string
		string converted = converter.to_bytes(iter.first);
		iter.second->Add_List(&tempList);

		pair<string, list<CGameObject*>> tempPair;
		tempPair = { converted ,tempList };
		pVector->push_back(tempPair);
	}

	return S_OK;
}

HRESULT CObject_Manager::Add_Object_Layers_Vector(_uint _level, vector<pair<_wstring, list<CGameObject*>>>* pVector)
{
	if (pVector == nullptr)
		return E_FAIL;

	list<CGameObject*> tempList;
	for (auto& iter : m_pLayers[_level])
	{
		iter.second->Add_List(&tempList);

		pair<wstring, list<CGameObject*>> tempPair;
		tempPair = { iter.first ,tempList };
		pVector->push_back(tempPair);
	}

	return S_OK;
}

void CObject_Manager::Destory_Update()
{
	for (auto& iter : m_DestoryObjects)
		Safe_Release(iter);
}

void CObject_Manager::Priority_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Priority_Update(fTimeDelta);
	}
}

void CObject_Manager::Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Update(fTimeDelta);
	}
}

void CObject_Manager::Late_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Late_Update(fTimeDelta);
	}
}


void CObject_Manager::Destory_Reserve(CGameObject* gameObject)
{
	m_DestoryObjects.push_back(gameObject);
}

HRESULT CObject_Manager::Clear_Resources(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return E_FAIL;

	for (auto& Pair : m_pLayers[iLevelIndex])
		Safe_Release(Pair.second);

	m_pLayers[iLevelIndex].clear();

	return S_OK;
}

CGameObject* CObject_Manager::Find_Prototype(const _wstring& strPrototypeTag)
{
	auto	iter = m_Prototypes.find(strPrototypeTag);

	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;
}

CLayer* CObject_Manager::Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag)
{
	auto	iter = m_pLayers[iLevelIndex].find(strLayerTag);

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

CObject_Manager* CObject_Manager::Create(_uint iNumLevels)
{
	CObject_Manager* pInstance = new CObject_Manager();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX(TEXT("Failed to Created : CObject_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObject_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);

		m_pLayers[i].clear();
	}

	Safe_Delete_Array(m_pLayers);


	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);

	m_Prototypes.clear();

}
