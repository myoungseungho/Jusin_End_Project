#pragma once

#include "Transform.h"


/* Ŭ���̾�Ʈ �����ڰ� ������ ��ü���� �θ� Ŭ������ �ȴ�. */

BEGIN(Renderer)
class CRenderInstance;
END

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	struct GAMEOBJECT_DESC : public CTransform::TRANSFORM_DESC
	{
		_bool isParsing = { false };
		wstring prototypeTag;
		wstring layerName;
		_uint levelIndex;
		_float3 position;
		_float3 rightRotation;
		_float3 upRotation;
		_float3 lookRotation;
		_float3 scale;
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
	/* �׻� ���� �ʾҴ� m_iGameObjectData �� ����Ÿ�� �ĺ���ȣ�μ� �� �� �ְ� �ټ��� ����*/
	void Set_GameObjectData(_uint iData) {
		m_iGameObjectData = iData;
	}
	_uint Get_GameObjectData() { return m_iGameObjectData; }
public:
	static const _wstring		m_strTransformTag;

public:
	void a();
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

	// ��ü ���� (������)
	_bool Get_Dead() { return m_bDead; }
	void Set_Dead() { m_bDead = TRUE; }
	//

public:
	virtual void OnCollisionEnter(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionStay(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionExit(class CCollider* other);

public:
	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, class CComponent** ppOut, void* pArg = nullptr);

	//Ȱ��ȭ ��Ȱ��ȭ ����
	void SetActive(_bool active) { m_bIsActive = active; }
	_bool IsActive() const { return m_bIsActive; }

	//IMGUI ����
	void SetImguiSelect(_bool _bIsImguiClick) { m_bIsImguiClick = _bIsImguiClick; };

protected:	
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };
	class Renderer::CRenderInstance*		m_pRenderInstance = { nullptr };

	class CTransform*			m_pTransformCom = { nullptr };

	//Ȱ��ȭ ��Ȱ��ȭ ����
	_bool						m_bIsActive = { true };

	// ��ü ���� (������)

	_bool m_bDead = { FALSE };

	//
public:
	void SetActive(_bool active) { m_bIsActive = active; }
	_bool IsActive() const { return m_bIsActive; }

	//IMGUI ����
	_bool						m_bIsImguiClick = { false };
	//�Ľ� ����
	_bool						m_bIsPasingObject = { false };
	_uint						m_iGameObjectData = { 0 };


protected:
	map<const _wstring, CComponent*>		m_Components;



public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END