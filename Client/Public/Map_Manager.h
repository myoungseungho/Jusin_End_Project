#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Base.h"
BEGIN(Engine)
class CGameInstance;
class CGameObject;
class CModel;
END

BEGIN(Client)

class CMap_Manager : public CBase
{
	DECLARE_SINGLETON(CMap_Manager)
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC
	{
		_wstring m_PrototypeKey;
	}Map_Object_Key;
	enum MAP_TYPE { MAP_SPACE, MAP_DEST_SPACE, MAP_VOLCANO, MAP_DEST_VOLCANO, MAP_END };
private:
	CMap_Manager();
	virtual ~CMap_Manager() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Camera_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	void Render(_float fTimeDelta);
public:
	void Map_Change(MAP_TYPE eMapType);
	_float2 Active_DestructiveFinish(_bool isRight);
	
	void Push_MapObject(MAP_TYPE eMapType, _wstring& strKey,class CGameObject* pGameObject);
public:
	map<const wstring, class CGameObject*>			m_SpaceModels;
	map<const wstring, class CGameObject*>			m_Destructive_SpaceModels;
	map<const wstring, class CGameObject*>			m_VolcanoModels;
	map<const wstring, class CGameObject*>			m_Destructive_VolcanoModels;

	MAP_TYPE m_eCurMap = { MAP_SPACE };
	_bool m_isRight = { false };
	_bool m_isDestructive_Active = { false };
	_float m_AccTime = { 0.f };
private:
	CGameInstance* m_pGameInstance = { nullptr };

	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
public:
	HRESULT Ready_Components();
	virtual void Free() override;
};

END