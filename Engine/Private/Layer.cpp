#include "..\Public\Layer.h"

#include "GameObject.h"

CLayer::CLayer()
{
}

CComponent * CLayer::Get_Component(const _wstring & strComponentTag, _uint iIndex)
{
	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	if (iter == m_GameObjects.end())
		return nullptr;

	return (*iter)->Get_Component(strComponentTag);
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.emplace_back(pGameObject);

	return S_OK;
}

void CLayer::Priority_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Priority_Update(fTimeDelta);
}

void CLayer::Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Update(fTimeDelta);
}

void CLayer::Late_Update(_float fTimeDelta)
{
	//최진영 객체 삭제
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); )
	{
		if ((*it)->Get_Dead())
		{
			Safe_Release(*it);
			it = m_GameObjects.erase(it); 
			continue; 
		}
		else
		{
			(*it)->Late_Update(fTimeDelta);
			++it; 
		}
	}
}

CLayer * CLayer::Create()
{
	return new CLayer();
}

void CLayer::Free()
{
	__super::Free();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();

}

