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

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	//파일 저장관련
	HRESULT Add_List(list<CGameObject*>*);

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