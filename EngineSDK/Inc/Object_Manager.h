#pragma once

#include "Base.h"

/* 생성한 객체들을 보관한다. */
/* Prototype 방식으로 객체를 추가한다. */


BEGIN(Engine)

class CObject_Manager final : public CBase
{
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	class CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex);


public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject_ToLayer(_uint iLevelIndex, const wstring& strPrototypeTag, const wstring& strLayerTag, void* pArg = nullptr);

	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

	HRESULT Clear_Resources(_uint iLevelIndex);

private:
	map<const wstring, class CGameObject*>		m_Prototypes;

private:
	_uint										m_iNumLevels = { 0 };
	map<const wstring, class CLayer*>*			m_pLayers = { nullptr };

private:
	class CGameObject* Find_Prototype(const wstring& strPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const wstring& strLayerTag);

public:
	static CObject_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END