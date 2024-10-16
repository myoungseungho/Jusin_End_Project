#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CModel;
END

BEGIN(Client)

class CFallingStar final : public CGameObject
{
public:
	enum Space_TextureType { SPACE_DIFFUSE,SPACE_STAR_1, SPACE_STAR_2, SPACE_END};

private:
	CFallingStar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFallingStar(const CFallingStar& Prototype);
	virtual ~CFallingStar() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	CShader*				m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom_Diffuse = { nullptr };
	CModel*					m_pModelCom = { nullptr };

private:
	_float2 m_fSpriteAnimCount = { 16.f,1.f };
	_float2 m_fSpriteSize = { 0.f,0.f };
	_float2 m_fSpriteCurPos = { 0.f,0.f };
	_float m_fAccTime = { 0.f };
private:
	_bool m_isStarSwitch = { false };
	_float	m_fMaskStar_Value_1 = { 0.f };
	_float	m_fMaskStar_Value_2 = { 0.3f };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CFallingStar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END