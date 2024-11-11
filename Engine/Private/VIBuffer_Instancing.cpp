#include "..\Public\VIBuffer_Instancing.h"

CVIBuffer_Instancing::CVIBuffer_Instancing(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer { pDevice, pContext }
{
}

CVIBuffer_Instancing::CVIBuffer_Instancing(const CVIBuffer_Instancing & Prototype)
	: CVIBuffer{ Prototype }
	, m_pVBInstance { Prototype.m_pVBInstance }
	, m_iNumInstance { Prototype.m_iNumInstance }
	, m_iInstanceVertexStride { Prototype.m_iInstanceVertexStride }
	, m_iNumIndexPerInstance { Prototype.m_iNumIndexPerInstance }
	, m_pInstanceVertices { Prototype.m_pInstanceVertices }
	, m_pSpeeds { Prototype.m_pSpeeds }
	, m_vPivotPos { Prototype.m_vPivotPos }
	, m_isLoop { Prototype.m_isLoop }
{
	Safe_AddRef(m_pVBInstance);
}

HRESULT CVIBuffer_Instancing::Initialize_Prototype(const VIBUFFER_INSTANCE_DESC* pInitialDesc)
{
	m_iNumInstance = pInitialDesc->iNumInstance;

	m_pSpeeds = new _float[m_iNumInstance];

	m_vPivotPos = pInitialDesc->vPivot;

	m_isLoop = pInitialDesc->isLoop;

	/* 인게임내에서 스피드가 바뀌지 않도록 파티클 원형객체를 생성할때 각 입자별 스피드를 계산하여 저장해둔다. */
	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		m_pSpeeds[i] = Get_Random(pInitialDesc->vSpeed.x, pInitialDesc->vSpeed.y);
	}
	
	return S_OK;
}

HRESULT CVIBuffer_Instancing::Initialize(void * pArg)
{
	
	return S_OK;
}

HRESULT CVIBuffer_Instancing::Bind_Buffers()
{

	ID3D11Buffer*		pVertexBufffers[] = {
		m_pVB,
		m_pVBInstance
	};

	_uint				iVertexStrides[] = {
		m_iVertexStride,
		m_iInstanceVertexStride
	};

	_uint				iOffsets[] = {
		0,
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBufffers, iVertexStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitive_Topology);

	return S_OK;
}

HRESULT CVIBuffer_Instancing::Render()
{
	m_pContext->DrawIndexedInstanced(m_iNumIndexPerInstance, m_iNumInstance, 0, 0, 0);

	return S_OK;
}


HRESULT CVIBuffer_Instancing::Create_InstanceBuffer()
{
	ZeroMemory(&m_BufferDesc, sizeof(m_BufferDesc));

	m_BufferDesc.ByteWidth = m_iNumInstance * m_iInstanceVertexStride;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC; 
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iInstanceVertexStride;

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = m_pInstanceVertices;

	if (FAILED(__super::Create_Buffer(&m_pVBInstance)))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer_Instancing::Spread(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		MappedSubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	VTXINSTANCE*		pMatrices = static_cast<VTXINSTANCE*>(MappedSubResource.pData);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		_vector		vMoveDir = XMVector3Normalize(XMLoadFloat4(&m_pInstanceVertices[i].vTranslation) - XMVectorSetW(XMLoadFloat3(&m_vPivotPos), 1.f));

		XMStoreFloat4(&pMatrices[i].vTranslation,
			XMLoadFloat4(&pMatrices[i].vTranslation) + vMoveDir * m_pSpeeds[i] * fTimeDelta);

		pMatrices[i].vLifeTime.y += fTimeDelta;
		if (m_isLoop == true && pMatrices[i].vLifeTime.y >= pMatrices[i].vLifeTime.x)
		{
			pMatrices[i].vTranslation = m_pInstanceVertices[i].vTranslation;
			pMatrices[i].vLifeTime.y = 0.f;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Instancing::Spread_2D(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		MappedSubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	VTXINSTANCE* pMatrices = static_cast<VTXINSTANCE*>(MappedSubResource.pData);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		// 현재 파티클의 위치를 개별적으로 로드
		float currentX = m_pInstanceVertices[i].vTranslation.x;
		float currentY = m_pInstanceVertices[i].vTranslation.y;

		// 피봇 위치도 개별적으로 로드
		float pivotX = m_vPivotPos.x;
		float pivotY = m_vPivotPos.y;

		// 현재 위치와 피봇 위치를 이용해 이동 방향 계산
		float dirX = currentX - pivotX;
		float dirY = currentY - pivotY;

		// 벡터 정규화
		float length = sqrt(dirX * dirX + dirY * dirY);
		if (length != 0.f)
		{
			dirX /= length;
			dirY /= length;
		}

		// 이동 속도 계산
		float moveX = dirX * m_pSpeeds[i] * fTimeDelta;
		float moveY = dirY * m_pSpeeds[i] * fTimeDelta;

		// 파티클의 현재 위치 업데이트
		pMatrices[i].vTranslation.x += moveX;
		pMatrices[i].vTranslation.y += moveY;

		// 생명 시간 업데이트
		pMatrices[i].vLifeTime.y += fTimeDelta;

		// 루핑 처리
		if (m_isLoop && pMatrices[i].vLifeTime.y >= pMatrices[i].vLifeTime.x)
		{
			// 초기 위치로 리셋
			pMatrices[i].vTranslation.x = m_pInstanceVertices[i].vTranslation.x;
			pMatrices[i].vTranslation.y = m_pInstanceVertices[i].vTranslation.y;
			pMatrices[i].vLifeTime.y = 0.f;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Instancing::Drop(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		MappedSubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	VTXINSTANCE*		pMatrices = static_cast<VTXINSTANCE*>(MappedSubResource.pData);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		// _vector		vMoveDir = XMVectorSet(Get_Random(0.f, 2.f) - 1.f, -1.f, Get_Random(0.f, 2.f) - 1.f, 0.f);
		_vector		vMoveDir = XMVectorSet(0.0f, -1.f, 0.0f, 0.f);

		XMStoreFloat4(&pMatrices[i].vTranslation,
			XMLoadFloat4(&pMatrices[i].vTranslation) + vMoveDir * m_pSpeeds[i] * fTimeDelta);

		pMatrices[i].vLifeTime.y += fTimeDelta;
		if (m_isLoop == true && pMatrices[i].vLifeTime.y >= pMatrices[i].vLifeTime.x)
		{
			pMatrices[i].vTranslation = m_pInstanceVertices[i].vTranslation;
			pMatrices[i].vLifeTime.y = 0.f;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);

}

_float CVIBuffer_Instancing::Get_RandomNormalize()
{
	return (_float)rand() / RAND_MAX;	
}

_float CVIBuffer_Instancing::Get_Random(_float fMin, _float fMax)
{
	return fMin + (fMax - fMin) * Get_RandomNormalize();	
}

void CVIBuffer_Instancing::Free()
{
	__super::Free();

	Safe_Release(m_pVBInstance);

	if (false == m_isCloned)
	{
		Safe_Delete_Array(m_pInstanceVertices);
		Safe_Delete_Array(m_pSpeeds);
		
	}
	

}
