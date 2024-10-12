#pragma once

#include "GameObject.h"
#include "Client_Defines.h"
#include "Collider_Manager.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CMelee_Effect final : public CGameObject
{
public:
	enum class PLAYERID
	{
		PLAYER_1P, PLAYER_2P
	};

private:
	CMelee_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMelee_Effect(const CMelee_Effect& Prototype);
	virtual ~CMelee_Effect() = default;

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

	void Make_Collider(CCollider_Manager::COLLIDERGROUP eColliderGroup, _float2 SourcePos, _float2 DestPos);

private:
	vector<CCollider*> m_vecColliderCom;

	_float					m_fRandom = {};
	_float					m_fX = 0.f;
	_float					m_fY = 0.f;
	_float2					m_UnitSize = { 1.f,1.f };
	PLAYERID				m_playerID = {};

public:
	static CMelee_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END