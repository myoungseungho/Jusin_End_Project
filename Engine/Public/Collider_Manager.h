#pragma once

#include "Base.h"

/* 1. ȭ�鿡 �׷����� ��ü���� �׸��� ������� �����Ѵ�. */
/* 2. �����ϰ� �ִ� ��ü���� �����Լ��� ȣ���Ѵ�.(������) */

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
	//m_CollisionHistory: ���� �����ӿ����� �浹 ���¸� �����ϴ� ���Դϴ�.
	// Ű�� �� �ݶ��̴��� �����͸� ���� ��(pair)�̰�, ���� �� ���� �浹 ������ ���θ� ��Ÿ���� �ο� ���Դϴ�.
	map<pair<CCollider*, CCollider*>, _bool> m_CollisionHistory;
	class CGameInstance* m_pGameInstance = { nullptr };

	//��Ƽ ������
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