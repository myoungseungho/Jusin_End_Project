#include "..\Public\VIBuffer_Instancing.h"
#include <random>

CVIBuffer_Instancing::CVIBuffer_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

CVIBuffer_Instancing::CVIBuffer_Instancing(const CVIBuffer_Instancing& Prototype)
	: CVIBuffer{ Prototype }
	, m_pVBInstance{ Prototype.m_pVBInstance }
	, m_iNumInstance{ Prototype.m_iNumInstance }
	, m_iInstanceVertexStride{ Prototype.m_iInstanceVertexStride }
	, m_iNumIndexPerInstance{ Prototype.m_iNumIndexPerInstance }
	, m_pInstanceVertices{ Prototype.m_pInstanceVertices }
	, m_pSpeeds{ Prototype.m_pSpeeds }
	, m_vPivotPos{ Prototype.m_vPivotPos }
	, m_isLoop{ Prototype.m_isLoop }
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

HRESULT CVIBuffer_Instancing::Initialize(void* pArg)
{

	return S_OK;
}

HRESULT CVIBuffer_Instancing::Bind_Buffers()
{

	ID3D11Buffer* pVertexBufffers[] = {
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

	VTXINSTANCE* pMatrices = static_cast<VTXINSTANCE*>(MappedSubResource.pData);

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

_bool CVIBuffer_Instancing::Spread_2D(_float fTimeDelta)
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
		// 루프가 안돌지만 라이프타임 시간을 넘어섰을 때
		else if (!m_isLoop && pMatrices[i].vLifeTime.y >= pMatrices[i].vLifeTime.x)
		{
			m_pContext->Unmap(m_pVBInstance, 0);
			return true;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
	return false;
}

_bool CVIBuffer_Instancing::Spread_FireCracker_2D(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE MappedSubResource{};

	// 버퍼 매핑
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	// 파티클 데이터 접근
	VTXINSTANCE* pMatrices = static_cast<VTXINSTANCE*>(MappedSubResource.pData);

	// 중력 가속도 (필요에 따라 조정 가능)
	const float gravity = -5000.f; // 예: 픽셀/초²

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		// 현재 파티클의 위치 로드
		float currentX = pMatrices[i].vTranslation.x;
		float currentY = pMatrices[i].vTranslation.y;

		// 피봇 위치 로드
		float pivotX = m_vPivotPos.x;
		float pivotY = m_vPivotPos.y;

		// 이동 방향 계산
		float dirX = currentX - pivotX;
		float dirY = currentY - pivotY;

		// 벡터 정규화
		float length = sqrt(dirX * dirX + dirY * dirY);
		if (length != 0.f)
		{
			dirX /= length;
			dirY /= length;
		}

		// 이동 속도 계산 변수
		float moveX = 0.f;
		float moveY = 0.f;

		// 생명 시간 비율 계산
		float ratio = pMatrices[i].vLifeTime.y / pMatrices[i].vLifeTime.x;
		ratio = max(0.0f, min(ratio, 1.0f)); // 0.0f ~ 1.0f로 클램프

		if (ratio < 0.8f)
		{
			// ratio가 0.8 미만일 때는 Spread_2D와 동일하게 이동
			moveX = dirX * m_pSpeeds[i] * fTimeDelta;
			moveY = dirY * m_pSpeeds[i] * fTimeDelta;
		}
		else
		{
			// ratio가 0.8 이상일 때는 중력의 영향을 받아 아래로 떨어짐
			// 중력 효과를 적용하여 moveY에 중력 가속도 추가
			float gravityEffect = gravity * fTimeDelta; // 중력 가속도 적용
			moveY = gravityEffect; // 아래로 떨어짐
		}

		// 파티클 위치 업데이트
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
		else if (!m_isLoop && pMatrices[i].vLifeTime.y >= pMatrices[i].vLifeTime.x)
		{
			// 파티클 비활성화 또는 삭제 로직 추가 가능
			m_pContext->Unmap(m_pVBInstance, 0);
			return true;
		}
	}

	// 버퍼 언매핑
	m_pContext->Unmap(m_pVBInstance, 0);
	return false;
}

_bool CVIBuffer_Instancing::Half_Spread_X_2D(_float fTimeDelta)
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

		// 아래로 향하는 경우 dirY를 양수로 변경하여 위로 향하도록 수정
		if (dirX < 0.f)
		{
			dirX = -dirX;
			dirY = -dirY;
		}


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
		// 루프가 안돌지만 라이프타임 시간을 넘어섰을 때
		else if (!m_isLoop && pMatrices[i].vLifeTime.y >= pMatrices[i].vLifeTime.x)
		{
			m_pContext->Unmap(m_pVBInstance, 0);
			return true;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
	return false;
}

_bool CVIBuffer_Instancing::Half_Spread_Y_2D(_float fTimeDelta)
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

		// 아래로 향하는 경우 dirY를 양수로 변경하여 위로 향하도록 수정
		if (dirY < 0.f)
		{
			dirX = -dirX;
			dirY = -dirY;
		}


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
		// 루프가 안돌지만 라이프타임 시간을 넘어섰을 때
		else if (!m_isLoop && pMatrices[i].vLifeTime.y >= pMatrices[i].vLifeTime.x)
		{
			m_pContext->Unmap(m_pVBInstance, 0);
			return true;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
	return false;
}

_bool CVIBuffer_Instancing::Spiral_Spread_2D(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE MappedSubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	VTXINSTANCE* pMatrices = static_cast<VTXINSTANCE*>(MappedSubResource.pData);

	const float spiralSpeed = XM_PI; // 회전 속도 (라디안 단위)

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		float currentX = m_pInstanceVertices[i].vTranslation.x;
		float currentY = m_pInstanceVertices[i].vTranslation.y;

		float pivotX = m_vPivotPos.x;
		float pivotY = m_vPivotPos.y;

		float dirX = currentX - pivotX;
		float dirY = currentY - pivotY;

		// 각도 계산
		float angle = atan2(dirY, dirX);
		angle += spiralSpeed * fTimeDelta; // 회전

		// 새로운 방향 벡터 계산
		dirX = cosf(angle);
		dirY = sinf(angle);

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
			pMatrices[i].vTranslation.x = m_pInstanceVertices[i].vTranslation.x;
			pMatrices[i].vTranslation.y = m_pInstanceVertices[i].vTranslation.y;
			pMatrices[i].vLifeTime.y = 0.f;
		}
		else if (!m_isLoop && pMatrices[i].vLifeTime.y >= pMatrices[i].vLifeTime.x)
		{
			m_pContext->Unmap(m_pVBInstance, 0);
			return true;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
	return false;
}

_bool CVIBuffer_Instancing::Circular_Spread_2D(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE MappedSubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	VTXINSTANCE* pMatrices = static_cast<VTXINSTANCE*>(MappedSubResource.pData);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		float currentX = m_pInstanceVertices[i].vTranslation.x;
		float currentY = m_pInstanceVertices[i].vTranslation.y;

		float pivotX = m_vPivotPos.x;
		float pivotY = m_vPivotPos.y;

		float dirX = currentX - pivotX;
		float dirY = currentY - pivotY;

		// 각도 랜덤화 (0도 ~ 360도)
		float angle = Get_Random(0.f, 360.f) * (XM_PI / 180.f);
		dirX = cosf(angle);
		dirY = sinf(angle);

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
			pMatrices[i].vTranslation.x = m_pInstanceVertices[i].vTranslation.x;
			pMatrices[i].vTranslation.y = m_pInstanceVertices[i].vTranslation.y;
			pMatrices[i].vLifeTime.y = 0.f;
		}
		else if (!m_isLoop && pMatrices[i].vLifeTime.y >= pMatrices[i].vLifeTime.x)
		{
			m_pContext->Unmap(m_pVBInstance, 0);
			return true;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
	return false;
}

_bool CVIBuffer_Instancing::Elliptical_Spread_2D(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE MappedSubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	VTXINSTANCE* pMatrices = static_cast<VTXINSTANCE*>(MappedSubResource.pData);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		float currentX = m_pInstanceVertices[i].vTranslation.x;
		float currentY = m_pInstanceVertices[i].vTranslation.y;

		float pivotX = m_vPivotPos.x;
		float pivotY = m_vPivotPos.y;

		float dirX = currentX - pivotX;
		float dirY = currentY - pivotY;

		// 타원형 스프레드를 위해 Y축 속도 감소
		float ellipseFactor = 0.5f; // Y축 속도 비율
		dirY *= ellipseFactor;

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
			pMatrices[i].vTranslation.x = m_pInstanceVertices[i].vTranslation.x;
			pMatrices[i].vTranslation.y = m_pInstanceVertices[i].vTranslation.y;
			pMatrices[i].vLifeTime.y = 0.f;
		}
		else if (!m_isLoop && pMatrices[i].vLifeTime.y >= pMatrices[i].vLifeTime.x)
		{
			m_pContext->Unmap(m_pVBInstance, 0);
			return true;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
	return false;
}

_bool CVIBuffer_Instancing::Random_Wiggle_Spread_2D(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE MappedSubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	VTXINSTANCE* pMatrices = static_cast<VTXINSTANCE*>(MappedSubResource.pData);

	const float scaleFactor = 50.f; // 진동 강도 스케일 팩터

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		float currentX = m_pInstanceVertices[i].vTranslation.x;
		float currentY = m_pInstanceVertices[i].vTranslation.y;

		float pivotX = m_vPivotPos.x;
		float pivotY = m_vPivotPos.y;

		float dirX = currentX - pivotX;
		float dirY = currentY - pivotY;

		// 벡터 정규화
		float length = sqrt(dirX * dirX + dirY * dirY);
		if (length != 0.f)
		{
			dirX /= length;
			dirY /= length;
		}

		// 진동 강도를 속도와 연동
		float wiggleIntensity = m_pSpeeds[i] * scaleFactor;

		// 랜덤 진동 추가
		float wiggleX = ((rand() % 100) / 100.f - 0.5f) * wiggleIntensity;
		float wiggleY = ((rand() % 100) / 100.f - 0.5f) * wiggleIntensity;

		// 이동 속도 계산
		float moveX = dirX * m_pSpeeds[i] * fTimeDelta + wiggleX * fTimeDelta;
		float moveY = dirY * m_pSpeeds[i] * fTimeDelta + wiggleY * fTimeDelta;

		// 파티클의 현재 위치 업데이트
		pMatrices[i].vTranslation.x += moveX;
		pMatrices[i].vTranslation.y += moveY;

		// 생명 시간 업데이트
		pMatrices[i].vLifeTime.y += fTimeDelta;

		// 루핑 처리
		if (m_isLoop && pMatrices[i].vLifeTime.y >= pMatrices[i].vLifeTime.x)
		{
			pMatrices[i].vTranslation.x = m_pInstanceVertices[i].vTranslation.x;
			pMatrices[i].vTranslation.y = m_pInstanceVertices[i].vTranslation.y;
			pMatrices[i].vLifeTime.y = 0.f;
		}
		else if (!m_isLoop && pMatrices[i].vLifeTime.y >= pMatrices[i].vLifeTime.x)
		{
			m_pContext->Unmap(m_pVBInstance, 0);
			return true;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
	return false;
}

_bool CVIBuffer_Instancing::Random_Wiggle_Half_Spread_2D(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE MappedSubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	VTXINSTANCE* pMatrices = static_cast<VTXINSTANCE*>(MappedSubResource.pData);

	const float scaleFactor = 50.f; // 진동 강도 스케일 팩터

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		float currentX = m_pInstanceVertices[i].vTranslation.x;
		float currentY = m_pInstanceVertices[i].vTranslation.y;

		float pivotX = m_vPivotPos.x;
		float pivotY = m_vPivotPos.y;

		float dirX = currentX - pivotX;
		float dirY = currentY - pivotY;

		// 벡터 정규화
		float length = sqrt(dirX * dirX + dirY * dirY);
		if (length != 0.f)
		{
			dirX /= length;
			dirY /= length;
		}

		// 진동 강도를 속도와 연동
		float wiggleIntensity = m_pSpeeds[i] * scaleFactor;

		// 랜덤 진동 추가
		float wiggleX = ((rand() % 100) / 100.f - 0.5f) * wiggleIntensity;
		float wiggleY = ((rand() % 100) / 100.f - 0.5f) * wiggleIntensity;

		// 이동 속도 계산
		float moveX = dirX * m_pSpeeds[i] * fTimeDelta + wiggleX * fTimeDelta;
		float moveY = dirY * m_pSpeeds[i] * fTimeDelta + wiggleY * fTimeDelta;

		if (moveX < 0.f)
			moveX = -moveX;

		// 파티클의 현재 위치 업데이트
		pMatrices[i].vTranslation.x += moveX;
		pMatrices[i].vTranslation.y += moveY;

		// 생명 시간 업데이트
		pMatrices[i].vLifeTime.y += fTimeDelta;

		// 루핑 처리
		if (m_isLoop && pMatrices[i].vLifeTime.y >= pMatrices[i].vLifeTime.x)
		{
			pMatrices[i].vTranslation.x = m_pInstanceVertices[i].vTranslation.x;
			pMatrices[i].vTranslation.y = m_pInstanceVertices[i].vTranslation.y;
			pMatrices[i].vLifeTime.y = 0.f;
		}
		else if (!m_isLoop && pMatrices[i].vLifeTime.y >= pMatrices[i].vLifeTime.x)
		{
			m_pContext->Unmap(m_pVBInstance, 0);
			return true;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
	return false;
}

void CVIBuffer_Instancing::Drop(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		MappedSubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	VTXINSTANCE* pMatrices = static_cast<VTXINSTANCE*>(MappedSubResource.pData);

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
