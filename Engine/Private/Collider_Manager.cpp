#include "..\Public\Collider_Manager.h"

#include "GameObject.h"
#include "Component.h"
#include "GameInstance.h"
#include <chrono> // �ð� ������ ���� �߰�
CCollider_Manager::CCollider_Manager()
	:m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

void CCollider_Manager::Update(_float fTimeDelta)
{
	Check_Collision(fTimeDelta);
}

HRESULT CCollider_Manager::Add_ColliderObject(COLLIDERGROUP eColliderGroup, CCollider* pColliderCom)
{
	if (pColliderCom == nullptr || eColliderGroup >= CG_END)
		return E_FAIL;

	Safe_AddRef(pColliderCom);

	m_Colliders[eColliderGroup].emplace_back(pColliderCom);

	return S_OK;
}


HRESULT CCollider_Manager::Check_Collision(_float fTimeDelta)
{
	// ���� �������� �浹 ��� �ʱ�ȭ
	m_CollisionResults.clear();

	// ��� �ݶ��̴��� m_isColl �÷��׸� false�� �ʱ�ȭ
	for (auto& group : m_Colliders)
	{
		for (auto& collider : group)
			collider->m_isColl = false;
	}


	// �浹 �� ����
	vector<pair<CCollider*, CCollider*>> collisionPairs;

	// ���� �͸��Լ��� �浹 �� ����
	auto AddCollisionPairs = [&](COLLIDERGROUP group1, COLLIDERGROUP group2) {
		for (auto& collider1 : m_Colliders[group1]) {
			for (auto& collider2 : m_Colliders[group2]) {
				collisionPairs.emplace_back(collider1, collider2);
			}
		}
		};

	// 1P_BODY vs 2P_BODY
	AddCollisionPairs(CG_1P_BODY, CG_2P_BODY);

	// 1P_BODY vs 2P_SKILL
	AddCollisionPairs(CG_1P_BODY, CG_2P_SKILL);

	// 2P_BODY vs 1P_SKILL
	AddCollisionPairs(CG_2P_BODY, CG_1P_SKILL);

	// 1P_SKILL vs 2P_SKILL
	AddCollisionPairs(CG_1P_SKILL, CG_2P_SKILL);

	// �ð� ���� ����
	auto startTime = std::chrono::high_resolution_clock::now();


	// ������Ǯ�� ����Ͽ� �۾� ���� �� �浹 �˻� ����
	size_t numThreads = m_pGameInstance->Get_ThreadNumber();
	size_t totalPairs = collisionPairs.size();
	size_t pairsPerThread = (totalPairs + numThreads - 1) / numThreads; // �ø� ���

	//future<void>�� ��ȯ���� ���� �񵿱� �۾��� ��Ÿ����
	vector<future<void>> futures;

	for (size_t t = 0; t < numThreads; ++t) {
		size_t startIdx = t * pairsPerThread;
		if (startIdx >= totalPairs) break;
		size_t endIdx = min(startIdx + pairsPerThread, totalPairs);

		// �� �����尡 ó���� �浹 �� �κ� ���� ����
		vector<pair<CCollider*, CCollider*>> taskPairs(
			collisionPairs.begin() + startIdx,
			collisionPairs.begin() + endIdx
		);

		// ������Ǯ�� �۾� ����
		futures.emplace_back(m_pGameInstance->EnqueueTask(

			//Task�� �����Լ�
			[this, taskPairs, fTimeDelta]() {
				vector<pair<CCollider*, CCollider*>> localCollisions;

				for (const auto& pair : taskPairs) {
					if (IsColliding(pair.first, pair.second)) {
						localCollisions.emplace_back(pair);
					}
				}

				// ������ �����ϰ� ����� ����
				{
					lock_guard<mutex> lock(m_ResultMutex);
					m_CollisionResults.insert(m_CollisionResults.end(), localCollisions.begin(), localCollisions.end());
				}
			}));
	}

	// ��� �۾� �Ϸ� ���
	for (auto& future : futures) {
		future.get();
	}

	// �ð� ���� ����
	auto endTime = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsedTime = endTime - startTime;
	double elapsedSeconds = elapsedTime.count();

	// �浹 ��� ó��
	ProcessCollisionResults(fTimeDelta);

	return S_OK;
}


void CCollider_Manager::ProcessCollisionResults(_float fTimeDelta)
{
	map<pair<CCollider*, CCollider*>, bool> currentCollisions;

	// �� �����忡�� ������ �浹 ����� �ִٸ� m_CollisionResults�� ����
	for (const auto& pair : m_CollisionResults) {
		CCollider* colliderA = pair.first;
		CCollider* colliderB = pair.second;

		// ������ bool ������ ����
		_bool is1PBodyVs2PSkill = (colliderA->m_ColliderGroup == CG_1P_BODY && colliderB->m_ColliderGroup == CG_2P_SKILL);
		_bool is1PBodyVs2PBody = (colliderA->m_ColliderGroup == CG_1P_BODY && colliderB->m_ColliderGroup == CG_2P_BODY);
		_bool is1PSkillVs2PSkill = (colliderA->m_ColliderGroup == CG_1P_SKILL && colliderB->m_ColliderGroup == CG_2P_SKILL);
		_bool is1PSkillVs2PBody = (colliderA->m_ColliderGroup == CG_1P_SKILL && colliderB->m_ColliderGroup == CG_2P_BODY);

		// �浹 �׷쿡 ���� ó��
		if (is1PBodyVs2PSkill)
		{
			Process_1PBody_2PSkill(colliderA, colliderB, fTimeDelta, currentCollisions);
		}
		else if (is1PBodyVs2PBody)
		{
			Process_1PBody_2PBody(colliderA, colliderB, fTimeDelta, currentCollisions);
		}
		else if (is1PSkillVs2PSkill)
		{
			Process_1PSkill_2PSkill(colliderA, colliderB, fTimeDelta, currentCollisions);
		}
		else if (is1PSkillVs2PBody)
		{
			Process_1PSkill_2PBody(colliderA, colliderB, fTimeDelta, currentCollisions);
		}
	}

	// ���� �����ӿ��� �浹�� �͵� �� �̹� �����ӿ��� �浹���� ���� ��� ó��
	for (auto& pair : m_CollisionHistory) {
		if (currentCollisions.find(pair.first) == currentCollisions.end()) {
			pair.first.first->OnCollisionExit(pair.first.second);
			pair.first.second->OnCollisionExit(pair.first.first);
		}
	}

	// �浹 �����丮 ������Ʈ
	m_CollisionHistory = currentCollisions;
}


// �� �浹 ó�� �Լ�

void CCollider_Manager::Process_1PBody_2PSkill(CCollider* colliderA, CCollider* colliderB, _float fTimeDelta, map<pair<CCollider*, CCollider*>, _bool>& currentCollisions)
{
	// 1P �ٵ�� 2P ��ų �浹 ó��
	// ��ü���� ���� �߰� ����
}

void CCollider_Manager::Process_1PBody_2PBody(CCollider* colliderA, CCollider* colliderB, _float fTimeDelta, map<pair<CCollider*, CCollider*>, _bool>& currentCollisions)
{
	// 1P �ٵ�� 2P �ٵ� �浹 ó��
	// ��ü���� ���� �߰� ����
}

void CCollider_Manager::Process_1PSkill_2PSkill(CCollider* colliderA, CCollider* colliderB, _float fTimeDelta, map<pair<CCollider*, CCollider*>, _bool>& currentCollisions)
{
	// 1P ��ų�� 2P ��ų �浹 ó��
	// ��ü���� ���� �߰� ����
}

void CCollider_Manager::Process_1PSkill_2PBody(CCollider* colliderA, CCollider* colliderB, _float fTimeDelta, map<pair<CCollider*, CCollider*>, _bool>& currentCollisions)
{
	// ���ο� �浹���� Ȯ��
	if (m_CollisionHistory.find(make_pair(colliderA, colliderB)) == m_CollisionHistory.end() || !m_CollisionHistory[make_pair(colliderA, colliderB)]) {
		// ���ο� �浹 ����
		colliderA->OnCollisionEnter(colliderB, fTimeDelta);
		colliderB->OnCollisionEnter(colliderA, fTimeDelta);
	}
	else {
		// �浹 ����
		colliderA->OnCollisionStay(colliderB, fTimeDelta);
		colliderB->OnCollisionStay(colliderA, fTimeDelta);
	}

	// ���� �浹 ���� ������Ʈ
	currentCollisions[make_pair(colliderA, colliderB)] = true;
}


_bool CCollider_Manager::IsColliding(CCollider* _pSourCollider, CCollider* _pDestCollider)
{
	_bool isCol = _pSourCollider->isCollision(_pDestCollider);
	if (isCol)
	{
		_pSourCollider->m_isColl = true;
		_pDestCollider->m_isColl = true;
	}
	return isCol;
}

HRESULT CCollider_Manager::Release_Collider(const CCollider* targetCollider)
{
	// ����, m_CollisionHistory���� �ش� �ݶ��̴��� ���õ� ��� �浹 ������ �����ϰų� OnCollisionExit �̺�Ʈ�� ȣ���մϴ�.
	for (auto it = m_CollisionHistory.begin(); it != m_CollisionHistory.end(); )
	{
		if (it->first.first == targetCollider || it->first.second == targetCollider)
		{
			// �浹 ���� �̺�Ʈ ȣ��
			it->first.first->OnCollisionExit(it->first.second);
			it->first.second->OnCollisionExit(it->first.first);

			// �浹 ���� ����
			it = m_CollisionHistory.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (int i = CG_1P_BODY; i < CG_END; ++i) {

		auto it = find(m_Colliders[i].begin(), m_Colliders[i].end(), targetCollider);
		if (it != m_Colliders[i].end()) {
			CCollider* collider = *it;
			Safe_Release(collider);
			m_Colliders[i].erase(it);
			return S_OK; // ���������� ���� �� ����
		}
	}

	return E_FAIL; // �ش� collider�� ã�� ����
}

HRESULT CCollider_Manager::Clear_ColliderGroup(COLLIDERGROUP eRenderGroup)
{
	if (eRenderGroup >= CG_END)
		return E_FAIL;

	for (auto& iter : m_Colliders[eRenderGroup])
		Safe_Release(iter);

	return S_OK;
}




CCollider_Manager* CCollider_Manager::Create()
{
	return new CCollider_Manager();
}

void CCollider_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < CG_END; i++)
	{
		for (auto& pCollider : m_Colliders[i])
			Safe_Release(pCollider);
		m_Colliders[i].clear();
	}

	Safe_Release(m_pGameInstance);
}
