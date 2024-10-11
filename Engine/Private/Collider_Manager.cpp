#include "..\Public\Collider_Manager.h"

#include "GameObject.h"
#include "Component.h"
#include "GameInstance.h"
#include <chrono> // 시간 측정을 위해 추가
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
	// 이전 프레임의 충돌 결과 초기화
	m_CollisionResults.clear();

	// 모든 콜라이더의 m_isColl 플래그를 false로 초기화
	for (auto& group : m_Colliders)
	{
		for (auto& collider : group)
			collider->m_isColl = false;
	}


	// 충돌 쌍 생성
	vector<pair<CCollider*, CCollider*>> collisionPairs;

	// 람다 익명함수로 충돌 쌍 생성
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

	// 시간 측정 시작
	auto startTime = std::chrono::high_resolution_clock::now();


	// 스레드풀을 사용하여 작업 분할 및 충돌 검사 수행
	size_t numThreads = m_pGameInstance->Get_ThreadNumber();
	size_t totalPairs = collisionPairs.size();
	size_t pairsPerThread = (totalPairs + numThreads - 1) / numThreads; // 올림 계산

	//future<void>는 반환값이 없는 비동기 작업을 나타낸다
	vector<future<void>> futures;

	for (size_t t = 0; t < numThreads; ++t) {
		size_t startIdx = t * pairsPerThread;
		if (startIdx >= totalPairs) break;
		size_t endIdx = min(startIdx + pairsPerThread, totalPairs);

		// 각 스레드가 처리할 충돌 쌍 부분 벡터 생성
		vector<pair<CCollider*, CCollider*>> taskPairs(
			collisionPairs.begin() + startIdx,
			collisionPairs.begin() + endIdx
		);

		// 스레드풀에 작업 제출
		futures.emplace_back(m_pGameInstance->EnqueueTask(

			//Task의 람다함수
			[this, taskPairs, fTimeDelta]() {
				vector<pair<CCollider*, CCollider*>> localCollisions;

				for (const auto& pair : taskPairs) {
					if (IsColliding(pair.first, pair.second)) {
						localCollisions.emplace_back(pair);
					}
				}

				// 스레드 안전하게 결과를 저장
				{
					lock_guard<mutex> lock(m_ResultMutex);
					m_CollisionResults.insert(m_CollisionResults.end(), localCollisions.begin(), localCollisions.end());
				}
			}));
	}

	// 모든 작업 완료 대기
	for (auto& future : futures) {
		future.get();
	}

	// 시간 측정 종료
	auto endTime = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsedTime = endTime - startTime;
	double elapsedSeconds = elapsedTime.count();

	// 충돌 결과 처리
	ProcessCollisionResults(fTimeDelta);

	return S_OK;
}


void CCollider_Manager::ProcessCollisionResults(_float fTimeDelta)
{
	map<pair<CCollider*, CCollider*>, bool> currentCollisions;

	// 각 스레드에서 취합한 충돌 결과가 있다면 m_CollisionResults에 저장
	for (const auto& pair : m_CollisionResults) {
		CCollider* colliderA = pair.first;
		CCollider* colliderB = pair.second;

		// 조건을 bool 변수로 저장
		_bool is1PBodyVs2PSkill = (colliderA->m_ColliderGroup == CG_1P_BODY && colliderB->m_ColliderGroup == CG_2P_SKILL);
		_bool is1PBodyVs2PBody = (colliderA->m_ColliderGroup == CG_1P_BODY && colliderB->m_ColliderGroup == CG_2P_BODY);
		_bool is1PSkillVs2PSkill = (colliderA->m_ColliderGroup == CG_1P_SKILL && colliderB->m_ColliderGroup == CG_2P_SKILL);
		_bool is1PSkillVs2PBody = (colliderA->m_ColliderGroup == CG_1P_SKILL && colliderB->m_ColliderGroup == CG_2P_BODY);

		// 충돌 그룹에 따른 처리
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

	// 이전 프레임에서 충돌한 것들 중 이번 프레임에서 충돌하지 않은 경우 처리
	for (auto& pair : m_CollisionHistory) {
		if (currentCollisions.find(pair.first) == currentCollisions.end()) {
			pair.first.first->OnCollisionExit(pair.first.second);
			pair.first.second->OnCollisionExit(pair.first.first);
		}
	}

	// 충돌 히스토리 업데이트
	m_CollisionHistory = currentCollisions;
}


// 각 충돌 처리 함수

void CCollider_Manager::Process_1PBody_2PSkill(CCollider* colliderA, CCollider* colliderB, _float fTimeDelta, map<pair<CCollider*, CCollider*>, _bool>& currentCollisions)
{
	// 1P 바디와 2P 스킬 충돌 처리
	// 구체적인 로직 추가 가능
}

void CCollider_Manager::Process_1PBody_2PBody(CCollider* colliderA, CCollider* colliderB, _float fTimeDelta, map<pair<CCollider*, CCollider*>, _bool>& currentCollisions)
{
	// 1P 바디와 2P 바디 충돌 처리
	// 구체적인 로직 추가 가능
}

void CCollider_Manager::Process_1PSkill_2PSkill(CCollider* colliderA, CCollider* colliderB, _float fTimeDelta, map<pair<CCollider*, CCollider*>, _bool>& currentCollisions)
{
	// 1P 스킬과 2P 스킬 충돌 처리
	// 구체적인 로직 추가 가능
}

void CCollider_Manager::Process_1PSkill_2PBody(CCollider* colliderA, CCollider* colliderB, _float fTimeDelta, map<pair<CCollider*, CCollider*>, _bool>& currentCollisions)
{
	// 새로운 충돌인지 확인
	if (m_CollisionHistory.find(make_pair(colliderA, colliderB)) == m_CollisionHistory.end() || !m_CollisionHistory[make_pair(colliderA, colliderB)]) {
		// 새로운 충돌 시작
		colliderA->OnCollisionEnter(colliderB, fTimeDelta);
		colliderB->OnCollisionEnter(colliderA, fTimeDelta);
	}
	else {
		// 충돌 지속
		colliderA->OnCollisionStay(colliderB, fTimeDelta);
		colliderB->OnCollisionStay(colliderA, fTimeDelta);
	}

	// 현재 충돌 상태 업데이트
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
