#pragma once

#include "Transform.h"


/* 클라이언트 개발자가 제작한 객체들의 부모 클래스가 된다. */

BEGIN(Renderer)
class CRenderInstance;
END

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	struct GAMEOBJECT_DESC : public CTransform::TRANSFORM_DESC
	{
		_uint		iGameObjectData = {};
	};

protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	class CComponent* Get_Component(const _wstring& strComponentTag) {
		auto	iter = m_Components.find(strComponentTag);
		if (iter == m_Components.end())
			return nullptr;
		return iter->second;
	}

public:
	static const _wstring		m_strTransformTag;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

public:
	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, class CComponent** ppOut, void* pArg = nullptr);

protected:	
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };
	class Renderer::CRenderInstance*		m_pRenderInstance = { nullptr };
	_uint						m_iGameObjectData = { 0 };

	class CTransform*			m_pTransformCom = { nullptr };


protected:
	map<const _wstring, CComponent*>		m_Components;



public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END