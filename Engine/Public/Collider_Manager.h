#pragma once

#include "Base.h"
#include "Collider.h"

/* 1. ȭ�鿡 �׷����� ��ü���� �׸��� ������� �����Ѵ�. */
/* 2. �����ϰ� �ִ� ��ü���� �����Լ��� ȣ���Ѵ�.(������) */

BEGIN(Engine)
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
	_bool IsRayColliding(const _float3& rayOrigin, const _float3& rayDir, COLLIDERGROUP eColliderGroup, class CGameObject** pHitObject);
	_bool isPointInAABB(const _float3& point, COLLIDERGROUP eColliderGroup, class CGameObject** pHitObject);

public:
	//�ݶ��̴� �׷쿡 �ִ� �ݶ��̴� �� ����
	HRESULT Clear_ColliderGroup(COLLIDERGROUP eRenderGroup);

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