#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CQTE_Hit_Situation final : public CGameObject
{
public:
	struct QTE_HIT_SITUATION_DESC
	{
		_float lifeTime = {};
		_int create_Num = {};
	};

private:
	CQTE_Hit_Situation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CQTE_Hit_Situation(const CQTE_Hit_Situation& Prototype);
	virtual ~CQTE_Hit_Situation() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Camera_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	_float m_fLifeTime = {};
	_int m_iCreate_Num = {};
	vector<class CQTE_Hit_UI_Icon*> m_vecHitUIIcon;

public:
	static CQTE_Hit_Situation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END