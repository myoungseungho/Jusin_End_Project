#pragma once

#include "GameObject.h"
#include "Client_Defines.h"

#include "AnimationEvent_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CAttacKObject final : public CGameObject
{
public:
	//enum HitMotion { HIT_LIGHT, HIT_MEDIUM, HIT_HEAVY, HIT_CROUCH_MEDIUM, HIT_KNOCK_AWAY_LEFT, HIT_KNOCK_AWAY_UP };

	typedef struct ATTACK_DESC
	{
		//_ushort ihitCharacter_Motion = { 10 };
		HitMotion ihitCharacter_Motion = { HIT_LIGHT };

		AttackGrade  eAttackGrade = { GRADE_NORMAL };
		AttackType   eAttackType = { ATTACKTYPE_HIGH};


		//����� ���, ������ ����� ��� 
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
	
	//1020�߰�
	_bool					m_bGroundSmash = { false };
	//_bool					m_bGain_AttackStep = { true };
	_ushort					m_iGain_AttackStep = { 1 };

	class CCharacter*				m_pOwner = { nullptr };

	CTransform*				m_pOwnerTransform = { nullptr };


	_bool		m_bOwnerNextAnimation = { false };
	_uint		m_iOnwerNextAnimationIndex = {};

private:
	HRESULT Ready_Components(ATTACK_DESC* pDesc);

public:
	static CAttacKObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END