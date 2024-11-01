#pragma once

//#include "GameObject.h"
//#include "Client_Defines.h"
//#include "AnimationEvent_Defines.h"
#include "AttackObject.h"



BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CAttackObject_Energy final : public CAttackObject
{
public:
	//enum HitMotion { HIT_LIGHT, HIT_MEDIUM, HIT_HEAVY, HIT_CROUCH_MEDIUM, HIT_KNOCK_AWAY_LEFT, HIT_KNOCK_AWAY_UP };

	enum Energy_LightColor { ENERGY_LIGHT_NONE, ENERGY_LIGHT_YELLOW};
	enum Energy_Direction { ENERGY_DIRECTION_RIGHT, ENERGY_DIRECTION_RIGHT_UP, ENERGY_DIRECTION_RIGHT_DOWN};

	typedef struct ATTACK_RANGED_DESC : public  CAttackObject::ATTACK_DESC
	{
		_float2 fStartOffset = {};
		_float2 fMoveSpeedNoneDirection = {};

		_short iAttackCount = { 5 };
		_short iPlayerDirection = {};		//
		
		Energy_LightColor eExplosionColor = { ENERGY_LIGHT_NONE };
	};
private:
	CAttackObject_Energy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAttackObject_Energy(const CAttackObject_Energy& Prototype);
	virtual ~CAttackObject_Energy() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;


public:
	//void Set_RemoteDestory();

public:
	virtual void OnCollisionEnter(class CCollider* other, _float fTimeDelta) override;
	virtual void OnCollisionStay(class CCollider* other, _float fTimeDelta) override;
	virtual void OnCollisionExit(class CCollider* other) override;

	void Erase();

private:
	void CollisingAttack();
	void CollisingPlayer();


	_bool Check_MapOut();

	void Add_YellowLight();
	void Add_YellowLight(_float3 fPosition);

private:
	_float2	m_fStartOffset = {};
	_float2 m_fMoveSpeedNoneDirection = {};
	_short m_iAttackCount = { 5 };
	_short  m_iPlayerDirection = {};		

	_bool m_bDying = false;

	Energy_LightColor m_eEnegrgyColor = { ENERGY_LIGHT_NONE };

public:
	static CAttackObject_Energy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END