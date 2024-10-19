#pragma once

/* 인스턴싱을 통해 그려야하는 모델들이 상속받아야하는 클래스 .*/
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Instancing abstract : public CVIBuffer
{
public:
	typedef struct
	{
		_uint		iNumInstance = { 0 };
		_float3		vRange = {};
		_float3		vCenter = {};
		_float3		vPivot = {};
		_float2		vSpeed = {};
		_float2		vScale = {};

		_float2		vLifeTime = {};
		_bool		isLoop = {};

	}VIBUFFER_INSTANCE_DESC;
protected:
	CVIBuffer_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Instancing(const CVIBuffer_Instancing& Prototype);
	virtual ~CVIBuffer_Instancing() = default;

public:
	virtual HRESULT Initialize_Prototype(const VIBUFFER_INSTANCE_DESC* pInitialDesc);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Bind_Buffers() override;
	virtual HRESULT Render() override;

public:
	void Spread(_float fTimeDelta);
	void Drop(_float fTimeDelta, _bool* isColl);

protected:
	/* 모델(Instance하나)의 상태제어를 위한 행렬정보를 가지는 정점 버퍼.  */
	ID3D11Buffer* m_pVBInstance = { nullptr };

	/* 위의 정점버퍼에 추가된 정점 하나의 크기. */
	_uint					m_iInstanceVertexStride = { 0 };

	/* 인스턴싱을 통해 몇개의 모델(인스턴스)를 그려낼것이다. */
	_uint					m_iNumInstance = { 0 };
	_float3					m_vPivotPos = {};
	_float* m_pSpeeds = { nullptr };
	_bool					m_isLoop = {  };

	_uint					m_iNumIndexPerInstance = { 0 };

	VTXINSTANCE* m_pInstanceVertices = { nullptr };

protected:
	HRESULT Create_InstanceBuffer();


protected:
	_float Get_RandomNormalize();
	_float Get_Random(_float fMin, _float fMax);


public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END