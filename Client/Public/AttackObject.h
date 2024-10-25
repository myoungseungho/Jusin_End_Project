#pragma once

#include "GameObject.h"
#include "Client_Defines.h"

#include "AnimationEvent_Defines.h"


BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CAttackObject  : public CGameObject
{
public:
	//enum HitMotion { HIT_LIGHT, HIT_MEDIUM, HIT_HEAVY, HIT_CROUCH_MEDIUM, HIT_KNOCK_AWAY_LEFT, HIT_KNOCK_AWAY_UP };

	typedef struct ATTACK_DESC
	{
		//CCollider::COLLIDER_DESC  ColliderDesc{};
		CBounding_AABB::BOUNDING_AABB_DESC ColliderDesc{};

		HitMotion ihitCharacter_Motion = { HIT_LIGHT };

		AttackGrade  eAttackGrade = { GRADE_NORMAL };
		AttackType   eAttackType = { ATTACKTYPE_HIGH};


		//잡기의 경우, 성공시 진행될 모션 
		_bool		bOwnerNextAnimation = { false };
		_uint		iOnwerNextAnimationIndex = {};


		_float2 fhitCharacter_Impus = {0,0};
		_float  fhitCharacter_StunTime = { 1.f };
		_float  fLifeTime = { 0.f };
		_ushort iTeam = { 1 };

		_uint	iDamage = { 1 };
		_float	fAnimationLockTime = { 0.1f };

		//_bool   bOwnerGravityTimeReset = { false };
		_bool		bGroundSmash = { false};
		//_bool		bGainAttackStep = { true };
		_ushort		iGainAttackStep = { 1 };
		class CCharacter* pOwner = { nullptr };
	};
protected:
	CAttackObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAttackObject(const CAttackObject& Prototype);
	virtual ~CAttackObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;


	class CCharacter* Get_pOwner() { return m_pOwner; };

	

public:
	//void Set_RemoteDestory();

public:
	virtual void OnCollisionEnter(class CCollider* other, _float fTimeDelta) override;
	virtual void OnCollisionStay(class CCollider* other, _float fTimeDelta) override;
	virtual void OnCollisionExit(class CCollider* other) override;


private:
	void CollisingAttack();
	void CollisingPlayer();



protected:
	//CCollider_Test*			m_pColliderCom = { nullptr };

	HitMotion				m_ihitCharacter_Motion = { HIT_LIGHT };
	AttackGrade				m_eAttackGrade = { GRADE_NORMAL };
	AttackType				m_eAttackType = { ATTACKTYPE_HIGH };

	_float2					m_fhitCharacter_Impus{};
	_float					m_fhitCharacter_StunTime = { 1.f };
	_float					m_fLifeTime = { 0.f };
	_ushort					m_iTeam = { 1 };
	_ushort					m_iDamage = { 1 };

	_float					m_fAccLifeTime = { 0.f };

	_float3					m_fOffset{};

	_float					m_fAnimationLockTime = { 0.1f };

	//_bool					m_bOwnerGravityTimeReset = false;
	
	//1020추가
	_bool					m_bGroundSmash = { false };
	//_bool					m_bGain_AttackStep = { true };
	_ushort					m_iGain_AttackStep = { 1 };

	class CCharacter*				m_pOwner = { nullptr };

	CTransform*				m_pOwnerTransform = { nullptr };

	CCollider* m_pColliderCom = { nullptr };
	_bool		m_bOwnerNextAnimation = { false };
	_uint		m_iOnwerNextAnimationIndex = {};


	_bool		m_bEnableDestory = true;

private:
	HRESULT Ready_Components(ATTACK_DESC* pDesc);

public:
	static CAttackObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END