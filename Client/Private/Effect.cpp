#include "..\Public\Effect.h"

CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
}

CEffect::CEffect(const CGameObject& Prototype)
{
}

HRESULT CEffect::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CEffect::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

void CEffect::Priority_Update(_float fTimeDelta)
{
}

void CEffect::Update(_float fTimeDelta)
{
}

void CEffect::Late_Update(_float fTimeDelta)
{
}

HRESULT CEffect::Render()
{
	return E_NOTIMPL;
}

CEffect* CEffect::Clone(void* pArg)
{
	return nullptr;
}

void CEffect::Free()
{
}
