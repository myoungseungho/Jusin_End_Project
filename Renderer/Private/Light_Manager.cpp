#include "..\Public\Light_Manager.h"
#include "Light.h"

CLight_Manager::CLight_Manager(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

const LIGHT_DESC * CLight_Manager::Get_LightDesc(_uint iLightIndex) const
{
	auto	iter = m_Lights.begin();

	for (size_t i = 0; i < iLightIndex; i++)
		++iter;
	

	return (*iter)->Get_LightDesc();
}

HRESULT CLight_Manager::Initialize()
{

	return S_OK;
}

HRESULT CLight_Manager::Add_Light(const LIGHT_DESC & LightDesc)
{
	CLight*			pLight = CLight::Create(LightDesc);
	if (nullptr == pLight)
		return E_FAIL;

	m_Lights.emplace_back(pLight);

	return S_OK;
}

HRESULT CLight_Manager::Render_Lights(CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	for (auto& pLight : m_Lights)
		pLight->Render(pShader, pVIBuffer);

	return S_OK;
}

CLight_Manager * CLight_Manager::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLight_Manager*		pInstance = new CLight_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLight_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLight_Manager::Free()
{
	__super::Free();

	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	m_Lights.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}