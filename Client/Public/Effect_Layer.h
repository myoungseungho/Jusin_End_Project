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
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	void	Render(_float fTimeDelta);

public:
	HRESULT Add_Effect(class CEffect* pEffect);
	vector<class CEffect*> Get_Effects();
	class CEffect* Find_Effect(const std::wstring& effectName);

public:
	_float			m_fDuration = { 0.f };
	_float			m_fTickPerSecond = {};

private:
	vector<class CEffect*>			m_MixtureEffects;
	_uint						m_iNumEffects = {0};

public:
	static CEffect_Layer* Create();
	virtual void Free() override;
};

END