#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CQTE_Hit_UI_Effect final : public CGameObject
{
public:
	struct Hit_EFFECT_DESC
	{
		_float	fSizeX{}, fSizeY{}, fX{}, fY{}, fTimer{};
		_int iTextureNum{};
	};

private:
	CQTE_Hit_UI_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CQTE_Hit_UI_Effect(const CQTE_Hit_UI_Effect& Prototype);
	virtual ~CQTE_Hit_UI_Effect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Camera_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

public:
	_float					m_fSizeX{}, m_fSizeY{}, m_fX{}, m_fY{}, m_fTimer{};
	_float4x4				m_ViewMatrix{}, m_ProjMatrix{};
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float m_fElapsedTime = 0.f; // 경과 시간을 저장하는 변수
	_float m_fLifeTime = 0.f;

public:
	static CQTE_Hit_UI_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END