#pragma once

#include "GameObject.h"
#include "Client_Defines.h"
#include "Collider_Manager.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CEffect final : public CGameObject
{
private:
	CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect(const CEffect& Prototype);
	virtual ~CEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

public:
	virtual void OnCollisionEnter(class CCollider* other, _float fTimeDelta) override;
	virtual void OnCollisionStay(class CCollider* other, _float fTimeDelta) override;
	virtual void OnCollisionExit(class CCollider* other) override;

	void Make_Effect_Collider_EnergyAttack(CCollider_Manager::COLLIDERGROUP eColliderGroup, _float2 SourcePos, _float2 DestPos);


private:
	vector<CCollider*>				m_vecColliderCom;

	_float					m_fRandom = {};
	_float					m_fX = 0.f;
	_float					m_fY = 0.f;
	_float2					m_UnitSize = { 3.f,3.f };

public:
	static CEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END