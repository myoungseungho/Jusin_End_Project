#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	typedef struct
	{
		_float			fSpeedPerSec;
		_float			fRotationPerSec;
	} TRANSFORM_DESC;

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTransform() = default;

public:
	HRESULT Initialize();
	void SetUp_TransformDesc(const TRANSFORM_DESC* pTransformDesc);

private:
	_float4x4				m_WorldMatrix = {};
	_float					m_fSpeedPerSec = {};
	_float					m_fRotationPerSec = {};

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END