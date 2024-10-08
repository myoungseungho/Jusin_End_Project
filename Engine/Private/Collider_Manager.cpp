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
	// ���� �������� �浹 ���� ����
	map<pair<CCollider*, CCollider*>, bool> currentCollisions;

	for (const auto& pair : m_CollisionResults) {
		CCollider* colliderA = pair.first;
		CCollider* colliderB = pair.second;

		auto key = make_pair(colliderA, colliderB);
		currentCollisions[key] = true;

		if (m_CollisionHistory.find(key) == m_CollisionHistory.end() || !m_CollisionHistory[key]) {
			// ���ο� �浹 ����
			colliderA->OnCollisionEnter(colliderB, fTimeDelta);
			colliderB->OnCollisionEnter(colliderA, fTimeDelta);
		}
		else {
			// �浹 ����
			colliderA->OnCollisionStay(colliderB, fTimeDelta);
			colliderB->OnCollisionStay(colliderA, fTimeDelta);
		}
	}

	// ���� �����ӿ����� �浹������ ���� �����ӿ����� �浹���� �ʴ� ��� ó��
	for (auto& pair : m_CollisionHistory) {
		if (currentCollisions.find(pair.first) == currentCollisions.end()) {
			pair.first.first->OnCollisionExit(pair.first.second);
			pair.first.second->OnCollisionExit(pair.first.first);
		}
	}

	// �浹 �����丮 ������Ʈ
	m_CollisionHistory = currentCollisions;
}

_bool CCollider_Manager::IsColliding(CCollider* _pSourCollider, CCollider* _pDestCollider)
{
	//SourCollider�� DestCollider�� �浹�ϸ� DestCollider�� �浹�ϰ�
	_bool isCol = _pSourCollider->isCollision(_pDestCollider);
	_pDestCollider->m_isColl = isCol;

	return _pSourCollider->isCollision(_pDestCollider);
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

_bool CCollider_Manager::IsRayColliding(const _float3& rayOrigin, const _float3& rayDir, COLLIDERGROUP eColliderGroup, CGameObject** pHitObject)
{
	if (eColliderGroup >= CG_END)
		return false;

	for (auto& collider : m_Colliders[eColliderGroup])
	{
		if (collider->isRayCollision(rayOrigin, rayDir))
		{
			*pHitObject = collider->GetMineGameObject(); // Assuming the collider has a method to get its owner (the game object)
			return true;
		}
	}
	return false;
}

_bool CCollider_Manager::isPointInAABB(const _float3& point, COLLIDERGROUP eColliderGroup, class CGameObject** pHitObject)
{
	if (eColliderGroup >= CG_END)
		return false;

	for (auto& collider : m_Colliders[eColliderGroup])
	{
		if (collider->isPointInAABB(point))
		{
			*pHitObject = collider->GetMineGameObject(); // Assuming the collider has a method to get its owner (the game object)
			return true;
		}
	}
	return false;
}


HRESULT CCollider_Manager::Clear_ColliderGroup(COLLIDERGROUP eRenderGroup)
{
	if ( eRenderGroup >= CG_END)
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
