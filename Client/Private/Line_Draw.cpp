#include "stdafx.h"
#include "Line_Draw.h"
#include "RenderInstance.h"
#include "GameInstance.h"

CLine_Draw::CLine_Draw(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CLine_Draw::CLine_Draw(const CLine_Draw& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CLine_Draw::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLine_Draw::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CLine_Draw::Priority_Update(_float fTimeDelta)
{
}

void CLine_Draw::Update(_float fTimeDelta)
{
}

void CLine_Draw::Late_Update(_float fTimeDelta)
{

}

HRESULT CLine_Draw::Render(_float fTimeDelta)
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CLine_Draw::Set_LinePoints(const _float3& vStart, const _float3& vEnd)
{
	m_vStartPoint = vStart;
	m_vEndPoint = vEnd;

	_float4 CamPosition = m_pGameInstance->Get_CamPosition_Float4();
	_float3 CamPositionFloat3 = _float3(CamPosition.x, CamPosition.y, CamPosition.z);

	m_pVIBufferCom->CalculateQuad(vStart, vEnd, m_fThickness, CamPositionFloat3);
}

HRESULT CLine_Draw::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLine_Draw::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	_float4 color = { 0.f, 1.f, 0.f, 1.f };

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &color, sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CLine_Draw* CLine_Draw::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) {
	CLine_Draw* pInstance = new CLine_Draw(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype())) {
		MessageBox(0, L"CLine_Draw Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CLine_Draw::Clone(void* pArg) {
	CLine_Draw* pClone = new CLine_Draw(*this);
	if (FAILED(pClone->Initialize(pArg))) {
		MessageBox(0, L"CLine_Draw Cloned Failed", L"System Error", MB_OK);
		Safe_Release(pClone);
	}
	return pClone;
}

void CLine_Draw::Free() {

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);

	__super::Free();
}