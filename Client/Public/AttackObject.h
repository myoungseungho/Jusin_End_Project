#pragma once

#include "GameObject.h"
#include "Client_Defines.h"
#include "Collider_Test.h"



BEGIN(Engine)
class CShader;
class CModel;
class CCollider_Test;
END

BEGIN(Client)

class CAttacKObject final : public CGameObject
{
public:
	enum HitMotion {HIT_LIGHT, HIT_HEAVY, HIT_KNOCK_AWAY_LEFT,  HIT_KNOCK_AWAY_UP};
	typedef struct ATTACK_DESC
	{
		CCollider_Test::COLLIDER_DESC ColliderDesc{};
		//_ushort ihitCharacter_Motion = { 10 };
		HitMotion ihitCharacter_Motion = { HIT_LIGHT };
		_float2 fhitCharacter_Impus = {0,0};
		_float  fhitCharacter_StunTime = { 1.f };
		_float  fLifeTime = { 0.f };
		_ushort iTeam = { 1 };

		_uint	iDamage = { 1 };
		_float	fAnimationLockTime = { 0.1f };

		//_bool   bOwnerGravityTimeReset = { false };
		class CCharacter* pOwner = { nullptr };
	};
private:
	CAttacKObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAttacKObject(const CAttacKObject& Prototype);
	virtual ~CAttacKObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	
	CCollider_Test*			m_pColliderCom = { nullptr };

	HitMotion				m_ihitCharacter_Motion = { HIT_LIGHT };
	_float2					m_fhitCharacter_Impus{};
	_float					m_fhitCharacter_StunTime = { 1.f };
	_float					m_fLifeTime = { 0.f };
	_ushort					m_iTeam = { 1 };
	_ushort					m_iDamage = { 1 };

	_float					m_fAccLifeTime = { 0.f };

	_float3					m_fOffset{};

	_float					m_fAnimationLockTime = { 0.1f };

	//_bool					m_bOwnerGravityTimeReset = false;

	class CCharacter*				m_pOwner = { nullptr };

	CTransform*				m_pOwnerTransform = { nullptr };


private:
	HRESULT Ready_Components(ATTACK_DESC* pDesc);

public:
	static CAttacKObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END