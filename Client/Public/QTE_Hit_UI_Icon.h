#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CQTE_Hit_UI_Icon final : public CGameObject
{
private:
	CQTE_Hit_UI_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CQTE_Hit_UI_Icon(const CQTE_Hit_UI_Icon& Prototype);
	virtual ~CQTE_Hit_UI_Icon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Camera_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:


	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float					m_fSizeX{}, m_fSizeY{}, m_fX{}, m_fY{}, m_fAlpha{};
	_float4x4				m_ViewMatrix{}, m_ProjMatrix{};
	_int m_iTextureNumber = {};

	class CGameObject* m_pSameGrab = { nullptr };

public:
	static CQTE_Hit_UI_Icon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END