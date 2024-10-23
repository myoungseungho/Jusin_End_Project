#include "..\Public\Layer.h"

#include "GameObject.h"

CLayer::CLayer()
{
}

CComponent* CLayer::Get_Component(const _wstring& strComponentTag, _uint iIndex)
{
	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	if (iter == m_GameObjects.end())
		return nullptr;

	return (*iter)->Get_Component(strComponentTag);
}

HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
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

	//��������
	*pList = m_GameObjects;

	return S_OK;
}


CGameObject* CLayer::Get_GameObject(_uint iIndex)
{
	auto iter = m_GameObjects.begin();

	// ��ȿ�� ������Ʈ�� ���� ���� ī����
	size_t validIndex = 0;

	while (iter != m_GameObjects.end())
	{
		// ���� ������Ʈ�� ��ȿ���� �ʴٸ� �ǳʶٱ�
		if ((*iter)->m_bDead)
		{
			++iter;
			continue;
		}

		// ��ȿ�� ������Ʈ�� ã�� ���
		if (validIndex == iIndex)
		{
			return (*iter);  // ��ȿ�� ������Ʈ ��ȯ
		}

		// ��ȿ�� ������Ʈ�� ���� �ε����� ������Ŵ
		++validIndex;
		++iter;
	}

	// ��ȿ�� ������Ʈ�� ã�� ���ϸ� nullptr ��ȯ
	return nullptr;
}

void CLayer::Priority_Update(_float fTimeDelta)
{
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); )
	{
		if ((*it)->m_bDead)  // ��ü�� ��� ���¶��
			it = m_GameObjects.erase(it);  // ��Ͽ��� ���� �� iterator ������Ʈ
		else
		{
			(*it)->Priority_Update(fTimeDelta);  // ������Ʈ ȣ��
			++it;  // ���� ��ü�� �̵�
		}
	}
}

void CLayer::Update(_float fTimeDelta)
{
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); )
	{
		if ((*it)->m_bDead)
		{
			Safe_Release(*it);
			it = m_GameObjects.erase(it);
		}
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
		if ((*it)->m_bDead)
			it = m_GameObjects.erase(it);
		else
		{
			(*it)->Late_Update(fTimeDelta);
			++it;
		}
	}
}

CLayer* CLayer::Create()
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

