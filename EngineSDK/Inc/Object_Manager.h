#pragma once

#include "Base.h"

/* »ý¼ºÇÑ °´Ã¼µéÀ» º¸°üÇÑ´Ù. */
/* Prototype ¹æ½ÄÀ¸·Î °´Ã¼¸¦ Ãß°¡ÇÑ´Ù. */


BEGIN(Engine)

class CObject_Manager final : public CBase
{
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	class CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex);
<<<<<<< HEAD
	class list<class CGameObject*> Get_Layer(_uint iLevelIndex, const wstring& strLayerTag);
=======
	//void Set_GameObjectData(_uint iLevelIndex, const _wstring& strLayerTag, _uint iData);
	//_uint Get_GameObjectData(_uint iLevelIndex, const _wstring& strLayerTag);
>>>>>>> origin/ê¹€?œì™„

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(const _wstring& strPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject_ToLayer(_uint iLevelIndex, const _wstring& strPrototypeTag, const _wstring& strLayerTag, void* pArg = nullptr);
	class CGameObject* Clone_GameObject(const _wstring& strPrototypeTag, void* pArg = nullptr);

	//ÆÄÀÏ ÀúÀå, ·Îµå °ü·Ã ÇÔ¼ö
	HRESULT Get_Prototype_Names(vector<string>* pVector);
	HRESULT Add_Object_Layers_Vector(_uint, vector<pair < string, list<CGameObject*>>>* pVector);
	HRESULT Add_Object_Layers_Vector(_uint, vector<pair < _wstring, list<CGameObject*>>>* pVector);










	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

	HRESULT Clear_Resources(_uint iLevelIndex);


	//
	CGameObject* Get_Object(_uint iLevelIndex, const wstring& strLayerTag, _uint iindex);


private:
	map<const wstring, class CGameObject*>		m_Prototypes;

private:
	_uint										m_iNumLevels = { 0 };
	map<const wstring, class CLayer*>*			m_pLayers = { nullptr };

public:
	class CGameObject* Find_Prototype(const wstring& strPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const wstring& strLayerTag);

public:
	static CObject_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END