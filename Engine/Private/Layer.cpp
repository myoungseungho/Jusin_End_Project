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

HRESULT CLayer::Add_List(list<CGameObject*>* pList)
{
	if (pList == nullptr)
		return E_FAIL;

	//얕은복사
	*pList = m_GameObjects;

	return S_OK;
}

void CLayer::Priority_Update(_float fTimeDelta)
{
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); )
	{
		if ((*it)->m_bDied)  // 객체가 사망 상태라면
			it = m_GameObjects.erase(it);  // 목록에서 삭제 후 iterator 업데이트
		else
		{
			(*it)->Priority_Update(fTimeDelta);  // 업데이트 호출
			++it;  // 다음 객체로 이동
		}
	}
}

void CLayer::Update(_float fTimeDelta)
{
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); )
	{
		if ((*it)->m_bDied)
			it = m_GameObjects.erase(it);
		else
		{
			(*it)->Update(fTimeDelta);
			++it;
		}
	}
}

void CLayer::Late_Update(_float fTimeDelta)
{
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); )
	{
		if ((*it)->m_bDied)
			it = m_GameObjects.erase(it);
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

