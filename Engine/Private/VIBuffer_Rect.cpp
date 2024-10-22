#include "..\Public\VIBuffer_Rect.h"

CVIBuffer_Rect::CVIBuffer_Rect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer { pDevice, pContext }
{
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect & Prototype)
	: CVIBuffer{ Prototype }
{
}

HRESULT CVIBuffer_Rect::Initialize_Prototype()
{
    m_iNumVertexBuffers = 1;
    m_iNumVertices = 4;
    m_iVertexStride = sizeof(VTXPOSTEX);

    m_iNumIndices = 6;
    m_iIndexStride = sizeof(_ushort);
    m_eIndexFormat = DXGI_FORMAT_R16_UINT;
    m_ePrimitive_Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
    m_BufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
    m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC; // ���� �Ҵ�
    m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU ���� ����
    m_BufferDesc.MiscFlags = 0;
    m_BufferDesc.StructureByteStride = m_iVertexStride;

    VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VTXPOSTEX) * m_iNumVertices);

    // �ʱ� ���� ��ġ�� ���Ƿ� ���� (���߿� ������Ʈ ����)
    pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
    pVertices[0].vTexcoord = _float2(0.0f, 0.f);

    pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
    pVertices[1].vTexcoord = _float2(1.0f, 0.f);

    pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
    pVertices[2].vTexcoord = _float2(1.0f, 1.0f);

    pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
    pVertices[3].vTexcoord = _float2(0.0f, 1.0f);

    ZeroMemory(&m_InitialData, sizeof(m_InitialData));
    m_InitialData.pSysMem = pVertices;

    if (FAILED(__super::Create_Buffer(&m_pVB)))
        return E_FAIL;

    Safe_Delete_Array(pVertices);
#pragma endregion

#pragma region INDEX_BUFFER
    m_BufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
    m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
    m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    m_BufferDesc.CPUAccessFlags = 0;
    m_BufferDesc.MiscFlags = 0;
    m_BufferDesc.StructureByteStride = 0;

    _ushort* pIndices = new _ushort[m_iNumIndices];
    ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

    pIndices[0] = 0;
    pIndices[1] = 1;
    pIndices[2] = 2;

    pIndices[3] = 0;
    pIndices[4] = 2;
    pIndices[5] = 3;

    ZeroMemory(&m_InitialData, sizeof(m_InitialData));
    m_InitialData.pSysMem = pIndices;

    if (FAILED(__super::Create_Buffer(&m_pIB)))
        return E_FAIL;

    Safe_Delete_Array(pIndices);
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Rect::Initialize_Prototype_Half()
{
	m_iNumVertexBuffers = 1;
	m_iNumVertices = 4;
	m_iVertexStride = sizeof(VTXPOSTEX);

	m_iNumIndices = 6;
	m_iIndexStride = 2;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitive_Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* �����Ҵ��Ѵ�. ���� �츮�� ���� ���������� ������ ������ ���� ����. */
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	/* ��ġ�� �Ҵ����ִ� ������ ä���־���� ������ ���� �̸� �غ��Ѵ�. */
	VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOSTEX) * m_iNumVertices);

	pVertices[0].vPosition = _float3(-0.25f, 0.25f, 0.f);
	pVertices[0].vTexcoord = _float2(0.0f, 0.f);

	pVertices[1].vPosition = _float3(0.25f, 0.25f, 0.f);
	pVertices[1].vTexcoord = _float2(1.0f, 0.f);

	pVertices[2].vPosition = _float3(0.25f, -0.25f, 0.f);
	pVertices[2].vTexcoord = _float2(1.0f, 1.0f);

	pVertices[3].vPosition = _float3(-0.25f, -0.25f, 0.f);
	pVertices[3].vTexcoord = _float2(0.0f, 1.0f);

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEX_BUFFER
	m_BufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* �����Ҵ��Ѵ�. ���� �츮�� ���� ���������� ������ ������ ���� ����. */
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = /*m_iIndexStride*/0;

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}


HRESULT CVIBuffer_Rect::Initialize(void * pArg)
{
	return S_OK;
}

void CVIBuffer_Rect::CalculateQuad(const _float3& start, const _float3& end, _float thickness, const _float3& cameraPos)
{
    // XMFLOAT3�� _vector�� �ε�
    _vector startVec = XMLoadFloat3(&start);
    _vector endVec = XMLoadFloat3(&end);
    _vector cameraVec = XMLoadFloat3(&cameraPos);

    // ���� ���� ���� ���
    _vector direction = XMVectorSubtract(endVec, startVec);

    // ���� ������ ���� ���
    _vector lengthVec = XMVector3Length(direction);
    float length;
    XMStoreFloat(&length, lengthVec);

    if (length == 0.0f)
        return; // �������� ������ ������ ��� ó������ ����

    // ���� ���� ����ȭ
    _vector directionNorm = XMVectorDivide(direction, lengthVec);

    // ���� ���� ���
    _vector midpoint = XMVectorAdd(startVec, direction);
    midpoint = XMVectorScale(midpoint, 0.5f);

    // �������� ī�޶�� ���ϴ� ���� ���
    _vector toCamera = XMVectorSubtract(cameraVec, midpoint);

    // ī�޶� ���� ���� ����ȭ
    _vector toCameraNorm = XMVector3Normalize(toCamera);

    // ��� ���� ��� (�� ����� ī�޶� ������ ���� ��)
    _vector normal = XMVector3Cross(directionNorm, toCameraNorm);

    // ��� ���� ����ȭ
    normal = XMVector3Normalize(normal);

    // ��� ���Ϳ� �β��� ���� ����
    _vector halfThickness = XMVectorReplicate(thickness / 2.0f);
    normal = XMVectorMultiply(normal, halfThickness);

    // ��� ���͸� XMFLOAT3�� ����
    XMFLOAT3 normalFloat3;
    XMStoreFloat3(&normalFloat3, normal);

    // �簢���� �� ���� ���
    VTXPOSTEX vertices[4];
    vertices[0].vPosition = _float3(
        start.x + normalFloat3.x,
        start.y + normalFloat3.y,
        start.z + normalFloat3.z
    );
    vertices[1].vPosition = _float3(
        end.x + normalFloat3.x,
        end.y + normalFloat3.y,
        end.z + normalFloat3.z
    );
    vertices[2].vPosition = _float3(
        end.x - normalFloat3.x,
        end.y - normalFloat3.y,
        end.z - normalFloat3.z
    );
    vertices[3].vPosition = _float3(
        start.x - normalFloat3.x,
        start.y - normalFloat3.y,
        start.z - normalFloat3.z
    );

    // �ؽ�ó ��ǥ ���� (�ʿ信 ���� ���� ����)
    vertices[0].vTexcoord = _float2(0.0f, 0.0f);
    vertices[1].vTexcoord = _float2(1.0f, 0.0f);
    vertices[2].vTexcoord = _float2(1.0f, 1.0f);
    vertices[3].vTexcoord = _float2(0.0f, 1.0f);

    // ���� ���� ���� �� ������Ʈ
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

    HRESULT hr = m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(hr))
    {
        // ���� ���� �� ó�� (��: �α� ���)
        return;
    }

    // ���� ������ ����
    VTXPOSTEX* pVertexData = reinterpret_cast<VTXPOSTEX*>(mappedResource.pData);
    memcpy(pVertexData, vertices, sizeof(vertices));

    // ���� ����
    m_pContext->Unmap(m_pVB, 0);
}

CVIBuffer_Rect * CVIBuffer_Rect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CVIBuffer_Rect*		pInstance = new CVIBuffer_Rect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CVIBuffer_Rect* CVIBuffer_Rect::Create_Half(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype_Half()))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Rect::Clone(void * pArg)
{
	CVIBuffer_Rect*		pInstance = new CVIBuffer_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CVIBuffer_Rect::Free()
{
	__super::Free();


}
