#pragma once

#include "Client_Defines.h"
#include "Effect.h"


BEGIN(Client)

class CEffect_MoveTex : public CEffect
{
private:
	CEffect_MoveTex(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_MoveTex(const CGameObject& Prototype);
	virtual ~CEffect_MoveTex() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_uint		m_iNumWidthColorTexture = { 0 };
	_uint		m_iNumHeightColorTexture = { 0 };
	_float		m_fColorTexcoordSpeed = { 0.f };
	_uint		m_iNumWidthMaskTexture = { 0 };
	_uint		m_iNumHeightMaskTexture = { 0 };
	_float		m_fMaskTexcoordSpeed = { 0.f };


public:
	static CEffect_MoveTex* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END