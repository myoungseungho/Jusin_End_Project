#pragma once

#include "Base.h"

/* 1. 화면에 그려야할 객체들을 그리는 순서대로 보관한다. */
/* 2. 보관하고 있는 객체들의 렌더함수를 호출한다.(렌더콜) */

BEGIN(Engine)

class CCollider;

class CCollider_Manager final : public CBase
{
public:
	enum COLLIDERGROUP { CG_1P_BODY, CG_1P_SKILL, CG_2P_BODY, CG_2P_SKILL, CG_END };

private:
	CCollider_Manager();
	virtual ~CCollider_Manager() = default;

public:
	void Update(_float fTimeDelta);

public:
	HRESULT Add_ColliderObject(COLLIDERGROUP eRenderGroup, CCollider* pColliderCom);
	HRESULT Check_Collision(_float);

	_bool IsColliding(CCollider* a, CCollider* b);
	HRESULT Release_Collider(const CCollider*);
	HRESULT Clear_ColliderGroup(COLLIDERGROUP eRenderGroup);
private:
	void Process_1PBody_2PSkill(CCollider* colliderA, CCollider* colliderB, _float fTimeDelta, map<pair<CCollider*, CCollider*>, _bool>& currentCollisions);
	void Process_1PBody_2PBody(CCollider* colliderA, CCollider* colliderB, _float fTimeDelta, map<pair<CCollider*, CCollider*>, _bool>& currentCollisions);
	void Process_1PSkill_2PSkill(CCollider* colliderA, CCollider* colliderB, _float fTimeDelta, map<pair<CCollider*, CCollider*>, _bool>& currentCollisions);
	void Process_1PSkill_2PBody(CCollider* colliderA, CCollider* colliderB, _float fTimeDelta, map<pair<CCollider*, CCollider*>, _bool>& currentCollisions);


private:
	list<CCollider*>			m_Colliders[CG_END];
	//m_CollisionHistory: 이전 프레임에서의 충돌 상태를 저장하는 맵입니다.
	// 키는 두 콜라이더의 포인터를 담은 쌍(pair)이고, 값은 이 쌍이 충돌 중인지 여부를 나타내는 부울 값입니다.
	map<pair<CCollider*, CCollider*>, _bool> m_CollisionHistory;
	class CGameInstance* m_pGameInstance = { nullptr };

	//멀티 스레드
private:
	void ProcessCollisionResults(_float fTimeDelta);
private:
	vector<pair<CCollider*, CCollider*>> m_CollisionResults;
	mutex m_ResultMutex;

public:
	static CCollider_Manager* Create();
	virtual void Free() override;
};
END