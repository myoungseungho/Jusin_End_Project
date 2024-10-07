#include "..\Public\Collider_Manager.h"

#include "GameObject.h"
#include "Component.h"
CCollider_Manager::CCollider_Manager()
{
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
	// 현재 프레임에서의 충돌 상태를 저장하는 맵입니다.
	map<pair<CCollider*, CCollider*>, bool> currentCollisions;

	// 다른 그룹 간의 충돌 체크
	// 4중 For문
	for (size_t i = 0; i < CG_END; i++)
	{
		// `m_Colliders[i]` 리스트에서 죽은 객체 제거
		m_Colliders[i].remove_if([](CCollider* collider) {
			if (collider->GetMineGameObject()->m_bDied)
			{
				Safe_Release(collider);
				return true; 
			}
			return false;
			});

		for (size_t j = i + 1; j < CG_END; j++)
		{
			for (auto& colliderA : m_Colliders[i])
			{
				// `m_Colliders[j]` 리스트에서 죽은 객체 제거
				m_Colliders[j].remove_if([](CCollider* collider) {
					if (collider->GetMineGameObject()->m_bDied)
					{
						Safe_Release(collider); 
						return true;
					}
					return false;
					});

				for (auto& colliderB : m_Colliders[j])
				{
					if (IsColliding(colliderA, colliderB)) {

						auto key = make_pair(colliderA, colliderB);
						currentCollisions[key] = true;

						if (m_CollisionHistory.find(key) == m_CollisionHistory.end() || !m_CollisionHistory[key]) {
							colliderA->OnCollisionEnter(colliderB, fTimeDelta);
							colliderB->OnCollisionEnter(colliderA, fTimeDelta);
						}
						else {
							colliderA->OnCollisionStay(colliderB, fTimeDelta);
							colliderB->OnCollisionStay(colliderA, fTimeDelta);
						}
					}
				}
			}
		}
	}

	//// 같은 그룹 내의 충돌 체크 (CG_PLAYER)
	//size_t group = CG_PLAYER;
	//for (auto itA = m_Colliders[group].begin(); itA != m_Colliders[group].end(); ++itA)
	//{
	//	auto itB = itA;
	//	++itB;
	//	for (; itB != m_Colliders[group].end(); ++itB)
	//	{
	//		CCollider* colliderA = *itA;
	//		CCollider* colliderB = *itB;

	//		if (IsColliding(colliderA, colliderB)) {

	//			auto key = make_pair(colliderA, colliderB);
	//			currentCollisions[key] = true;

	//			if (m_CollisionHistory.find(key) == m_CollisionHistory.end() || !m_CollisionHistory[key]) {
	//				colliderA->OnCollisionEnter(colliderB, fTimeDelta);
	//				colliderB->OnCollisionEnter(colliderA, fTimeDelta);
	//			}
	//			else {
	//				colliderA->OnCollisionStay(colliderB, fTimeDelta);
	//				colliderB->OnCollisionStay(colliderA, fTimeDelta);
	//			}
	//		}
	//	}
	//}

	// 이전 프레임에서는 충돌했지만 현재 프레임에서는 충돌하지 않는 경우 OnCollisionExit 이벤트를 호출합니다.
	for (auto& pair : m_CollisionHistory) {
		if (currentCollisions.find(pair.first) == currentCollisions.end() || !currentCollisions[pair.first]) {
			pair.first.first->OnCollisionExit(pair.first.second);
			pair.first.second->OnCollisionExit(pair.first.first);
		}
	}

	// 다음 프레임을 위한 충돌 기록 업데이트
	m_CollisionHistory = currentCollisions;
	return S_OK;
}

_bool CCollider_Manager::IsColliding(CCollider* _pSourCollider, CCollider* _pDestCollider)
{
	return _pSourCollider->isCollision(_pDestCollider);
}

HRESULT CCollider_Manager::Release_Collider(const CCollider* targetCollider)
{
	// 먼저, m_CollisionHistory에서 해당 콜라이더와 관련된 모든 충돌 정보를 제거하거나 OnCollisionExit 이벤트를 호출합니다.
	for (auto it = m_CollisionHistory.begin(); it != m_CollisionHistory.end(); )
	{
		if (it->first.first == targetCollider || it->first.second == targetCollider)
		{
			// 충돌 종료 이벤트 호출
			it->first.first->OnCollisionExit(it->first.second);
			it->first.second->OnCollisionExit(it->first.first);

			// 충돌 정보 제거
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
			return S_OK; // 성공적으로 해제 및 삭제
		}
	}

	return E_FAIL; // 해당 collider를 찾지 못함
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
}
