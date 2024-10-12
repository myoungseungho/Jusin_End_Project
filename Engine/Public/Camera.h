#pragma once

#include "GameObject.h"
#include "Transform.h"

/* Ŭ���̾�Ʈ �����ڰ� ������ ī�޶���� ��ӹ޾ƾ��� �θ�Ŭ����. */
/* ��� ī�޶� �ʿ��� �����Ϳ� ����� �����Ѵ�. */

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct : public CTransform::TRANSFORM_DESC
	{
		_float3		vEye, vAt;
		_float		fFovy, fNear, fFar;
	}CAMERA_DESC;
protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& Prototype);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;
	
	void CameraData_Update(class CVirtual_Camera* virtual_Camera);
private:
	_float3					m_vEye{}, m_vAt{};
	_float					m_fFovy{}, m_fNear{}, m_fFar{};

	_float					m_fViewportWidth{}, m_fViewportHeight{};
	

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};	

END