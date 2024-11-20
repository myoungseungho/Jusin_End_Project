#pragma once

#include "GameObject.h"
#include "Client_Defines.h"

#include "AnimationEvent_Defines.h"


BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CParryingRangedObject  : public CGameObject
{
public:
	//enum HitMotion { HIT_LIGHT, HIT_MEDIUM, HIT_HEAVY, HIT_CROUCH_MEDIUM, HIT_KNOCK_AWAY_LEFT, HIT_KNOCK_AWAY_UP };

	typedef struct PARRYING_RANGED_DESC
	{
		//시작좌표
		_vector vPos;


		//목적지좌표 또는 날아가는 방향


		//이건 기물을 부수는 오브젝트인가
		_bool bDestroyObject = { false };


	};
protected:
	CParryingRangedObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParryingRangedObject(const CParryingRangedObject& Prototype);
	virtual ~CParryingRangedObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Camera_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;



public:
	void Set_RemoteDestory();

public:



protected:
	_bool Check_UpdateStop(_float fTimeDelta);

private:

	_bool m_bDestroyObject = { false };

	_float m_fAccLifeTime = {};

	_float* m_pHeight = {m_pTransformCom->Get_State(CTransform::STATE_POSITION)}
	_float* m_pHeight = { nullptr };// {m_pTransformCom->Get_State(CTransform::STATE_POSITION)}

private:

protected:

	

private:
	HRESULT Ready_Components(PARRYING_RANGED_DESC* pDesc);

public:
	static CParryingRangedObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END