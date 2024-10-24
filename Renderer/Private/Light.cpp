#include "..\Public\Light.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"

CLight::CLight()
{
}

HRESULT CLight::Initialize(const LIGHT_DESC & LightDesc)
{
	m_LightDesc = LightDesc;

	return S_OK;
}

HRESULT CLight::Render_Map(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	_uint		iPassIndex = { 0 };

	if (LIGHT_DESC::TYPE_DIRECTIONAL == m_LightDesc.eType)
	{
		iPassIndex = 1;

		if (FAILED(pShader->Bind_RawValue("g_vLightDir", &m_LightDesc.vDirection, sizeof(_float4))))
			return E_FAIL;
	}
	else
	{
		iPassIndex = 2;
		if (FAILED(pShader->Bind_RawValue("g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4))))
			return E_FAIL;
		if (FAILED(pShader->Bind_RawValue("g_fLightRange", &m_LightDesc.fRange, sizeof(_float))))
			return E_FAIL;
	}

	if (FAILED(pShader->Bind_RawValue("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4))))
		return E_FAIL;
	
	pShader->Begin(iPassIndex);

	pVIBuffer->Render();

	return S_OK;
}

HRESULT CLight::Render_Player(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	_uint		iPassIndex = { 0 };

	if (LIGHT_DESC::TYPE_DIRECTIONAL == m_LightDesc.eType)
	{
		iPassIndex = 5;

		if (FAILED(pShader->Bind_RawValue("g_vLightDir", &m_LightDesc.vDirection, sizeof(_float4))))
			return E_FAIL;

		if (FAILED(pShader->Bind_RawValue("g_iPlayerDirection", m_LightDesc.pPlayerDirection, sizeof(_int))))
			return E_FAIL;
	}

	if (FAILED(pShader->Bind_RawValue("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_iPlayerDirection", m_LightDesc.pPlayerDirection, sizeof(_int))))
		return E_FAIL;

	pShader->Begin(iPassIndex);

	pVIBuffer->Render();

	return S_OK;
}

HRESULT CLight::Render_Effect(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	_uint		iPassIndex = { 0 };
	iPassIndex = 6;

	//if (FAILED(pShader->Bind_RawValue("g_iPlayerDirection", m_LightDesc.pPlayerDirection, sizeof(_int))))
	//	return E_FAIL;

	if (FAILED(pShader->Bind_RawValue("g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_fLightRange", &m_LightDesc.fRange, sizeof(_float))))
		return E_FAIL;

	if (FAILED(pShader->Bind_RawValue("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(pShader->Bind_RawValue("g_fLightAccTime", &m_LightDesc.fAccTime, sizeof(_float))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_fLightLifeTime", &m_LightDesc.fLifeTime, sizeof(_float))))
		return E_FAIL;

	/*
	* 
	* float g_fLightAccTime;
float g_fLightLifeTime;
	이미 먼저 m_fTimeDelta를 더해준 상태 들어온 시간에대한 임계점을 정할지는 판단해봐야됨
	만약 기준이 있다면 알파를 진행시간 m_fAccTime 및 m_fLifeTime 사이값
	
	*/

	pShader->Begin(iPassIndex);

	pVIBuffer->Render();

	return S_OK;
}

HRESULT CLight::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer, _int iPassIndex)
{
	switch (iPassIndex)
	{
	case 1: /* Map Obj */
	if (FAILED(Render_Map(pShader, pVIBuffer)))
		return E_FAIL;
	break;
	case 5: /* Player */
	if (FAILED(Render_Player(pShader, pVIBuffer)))
		return E_FAIL;
	break;
	case 6: /* Effect */
	if (FAILED(Render_Effect(pShader, pVIBuffer)))
		return E_FAIL;
	break;
	}

	return S_OK;
}

CLight * CLight::Create(const LIGHT_DESC & LightDesc)
{
	CLight*		pInstance = new CLight();

	if (FAILED(pInstance->Initialize(LightDesc)))
	{
		MSG_BOX(TEXT("Failed to Created : CLight"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLight::Free()
{
	__super::Free();

}