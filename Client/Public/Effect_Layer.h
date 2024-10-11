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

private:
	// ≥π∞≥ ¿Ã∆Â∆Æ ¡∂«’
	vector<class CEffect*>			m_MixtureEffects;
	_float			m_fDuration;
public:
	static CEffect_Layer* Create();
	virtual void Free() override;
};

END