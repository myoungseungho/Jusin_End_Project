#include "..\Public\VIBuffer_Trail_Rect.h"

CVIBuffer_Trail_Rect::CVIBuffer_Trail_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CVIBuffer{ pDevice, pContext }
{
}

CVIBuffer_Trail_Rect::CVIBuffer_Trail_Rect(const CVIBuffer_Trail_Rect& Prototype)
    : CVIBuffer{ Prototype }
{
}

HRESULT CVIBuffer_Trail_Rect::Initialize_Prototype()
{
    m_iNumVertexBuffers = 1;
    m_iNumVertices = 8;
    m_iVertexStride = sizeof(VTXPOSTEX);

    m_iNumIndices = 12;
    m_iIndexStride = sizeof(_ushort);
    m_eIndexFormat = DXGI_FORMAT_R16_UINT;
    m_ePrimitive_Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
    m_BufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
    m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC; // 동적 할당
    m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU 접근 가능
    m_BufferDesc.MiscFlags = 0;
    m_BufferDesc.StructureByteStride = m_iVertexStride;
   

    VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VTXPOSTEX) * m_iNumVertices);

    // 초기 정점 위치는 임의로 설정 (나중에 업데이트 예정)
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

HRESULT CVIBuffer_Trail_Rect::Initialize_Prototype_Half()
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
    m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* 정적할당한다. 추후 우리가 직접 정점버퍼의 공간을 변경할 일이 없다. */
    m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    m_BufferDesc.CPUAccessFlags = 0;
    m_BufferDesc.MiscFlags = 0;
    m_BufferDesc.StructureByteStride = m_iVertexStride;

    /* 장치가 할당해주는 공간에 채워넣어야할 값들을 내가 미리 준비한다. */
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
    m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* 정적할당한다. 추후 우리가 직접 정점버퍼의 공간을 변경할 일이 없다. */
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


HRESULT CVIBuffer_Trail_Rect::Initialize(void* pArg)
{
    return S_OK;
}

void CVIBuffer_Trail_Rect::CalculateQuad(const _float3& start)
{
    //// 현재 텍스처 위치를 버퍼에 추가
    VTXPOSTEX trailVertices[8];
    // 첫 번째 원
    trailVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f); // 왼쪽 위
    trailVertices[0].vTexcoord = _float2(0.0f, 0.0f);

    trailVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f); // 오른쪽 위
    trailVertices[1].vTexcoord = _float2(1.0f, 0.0f);

    trailVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f); // 오른쪽 아래
    trailVertices[2].vTexcoord = _float2(1.0f, 1.0f);

    trailVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f); // 왼쪽 아래
    trailVertices[3].vTexcoord = _float2(0.0f, 1.0f);

    // 두 번째 원 (위치 이동)
    trailVertices[4].vPosition = _float3(0.5f, 0.5f, 0.f); // 왼쪽 위
    trailVertices[4].vTexcoord = _float2(0.0f, 0.0f);

    trailVertices[5].vPosition = _float3(1.5f, 0.5f, 0.f); // 오른쪽 위
    trailVertices[5].vTexcoord = _float2(1.0f, 0.0f);

    trailVertices[6].vPosition = _float3(1.5f, -0.5f, 0.f); // 오른쪽 아래
    trailVertices[6].vTexcoord = _float2(1.0f, 1.0f);

    trailVertices[7].vPosition = _float3(0.5f, -0.5f, 0.f); // 왼쪽 아래
    trailVertices[7].vTexcoord = _float2(0.0f, 1.0f);

    // Trail 버퍼 매핑 및 업데이트
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    if (SUCCEEDED(hr)) {
        VTXPOSTEX* pVertexData = reinterpret_cast<VTXPOSTEX*>(mappedResource.pData);
        memcpy(pVertexData, trailVertices, sizeof(trailVertices));
        m_pContext->Unmap(m_pVB, 0);  
    }
}

CVIBuffer_Trail_Rect* CVIBuffer_Trail_Rect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CVIBuffer_Trail_Rect* pInstance = new CVIBuffer_Trail_Rect(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CVIBuffer_Trail_Rect"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CVIBuffer_Trail_Rect* CVIBuffer_Trail_Rect::Create_Half(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CVIBuffer_Trail_Rect* pInstance = new CVIBuffer_Trail_Rect(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype_Half()))
    {
        MSG_BOX(TEXT("Failed to Created : CVIBuffer_Trail_Rect"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CVIBuffer_Trail_Rect::Clone(void* pArg)
{
    CVIBuffer_Trail_Rect* pInstance = new CVIBuffer_Trail_Rect(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_Trail_Rect"));
        Safe_Release(pInstance);
    }

    return pInstance;
}



void CVIBuffer_Trail_Rect::Free()
{
    __super::Free();


}
