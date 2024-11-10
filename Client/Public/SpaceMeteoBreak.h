#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Effect_Layer.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CModel;
END

BEGIN(Client)

class CSpaceMeteoBreak final : public CGameObject
{
public:
	enum Space_TextureType { SPACE_DIFFUSE,SPACE_STAR_1, SPACE_STAR_2, SPACE_END};

private:
	CSpaceMeteoBreak(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSpaceMeteoBreak(const CSpaceMeteoBreak& Prototype);
	virtual ~CSpaceMeteoBreak() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Camera_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	_float3 m_vFragmentMoveDir[11];
	_float4 m_vFragmentPosition[11];
	CShader*				m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom_Diffuse = { nullptr };
	CModel*					m_pModelCom = { nullptr };
	CModel* m_pFragmentModelCom[11] = { nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
private:
	const _float m_fFastTimeLimit = { 0.3f };
	_float m_fAccTime = { 0.f };
	_bool m_isFastSwitch = { true };
	_float m_fSpeed = { 0.3f };

	_bool m_isBrakeSwitch = { false };
	_float m_fBrakeSwitchTime = { 0.f };
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CSpaceMeteoBreak* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END