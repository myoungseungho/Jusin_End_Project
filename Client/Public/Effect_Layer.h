#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CEffect_Layer : public CBase
{
private:
	CEffect_Layer();
	virtual ~CEffect_Layer() = default;

public:
	HRESULT Add_Effect(class CEffect* pEffect);

public:
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	void	Render(_float fTimeDelta);

public:
	vector<class CEffect*> Get_Effects()
	{
		return m_MixtureEffects;
	}
	_float			m_fDuration = { 0.f };
private:
	// ≥π∞≥ ¿Ã∆Â∆Æ ¡∂«’
	vector<class CEffect*>			m_MixtureEffects;

public:
	static CEffect_Layer* Create();
	virtual void Free() override;
};

END