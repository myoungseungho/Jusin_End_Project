#pragma once

#include "Base.h"

/* 객체들을 리스트로 보관한다. */

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	class CComponent* Get_Component(const _wstring& strComponentTag, _uint iIndex);
	list<class CGameObject*> Get_Object_List() { return m_GameObjects; }

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);

	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

private:
	list<class CGameObject*>			m_GameObjects;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END