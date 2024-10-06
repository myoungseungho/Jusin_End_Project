#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CEffect_Manager : public CBase
{
	DECLARE_SINGLETON(CEffect_Manager)
private:
	CEffect_Manager();
	virtual ~CEffect_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

private:
	// 낱개 이펙트
	map<const wstring, class CEffect*>		m_EachEffects;

	// 조합된 이펙트들을 보관
	map<const wstring, vector<class CEffect*>*>		m_Effects;

public:
	static CEffect_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END