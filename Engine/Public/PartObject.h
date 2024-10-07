#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CPartObject abstract : virtual public CGameObject
{
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC
	{
		const _float4x4* pParentMatrix;
		CGameObject* pContainerObject;
	}PARTOBJECT_DESC;

protected:
	CPartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPartObject(const CPartObject& Prototype);
	virtual~CPartObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

protected:
	/* �� ��Ʈ������Ʈ�� �����ϰ� �ִ� �θ� ��ü�� ���� ���.  */
	const _float4x4*		m_pParentMatrix = {};

	/* ���� TransformŬ������ ��������� �θ������ ���º�ȯ��ķ� ������ �ٲ��.  */
	/* ���� �׸������� m_WorldMatrix = Transform`s WorldMatrix * *m_pParentMatrix */
	_float4x4				m_WorldMatrix = {};

public:
	HRESULT Bind_WorldMatrix(class CShader* pShader, const _char* pConstantName);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END