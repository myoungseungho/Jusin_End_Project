#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CQTE_UI_Icon final : public CGameObject
{
public:
	struct QTE_UI_ICON_DESC
	{
		_float	fSizeX{}, fSizeY{}, fX{}, fY{}, fAlpha{}, iTextureNumber{};
	};

	enum IconState
	{
		NOT_SELECTED,        // 기본 상태
		SELECTED,            // 현재 선택된 상태
		ALREADY_PRESSED    // 올바르게 눌린 상태
	};

private:
	CQTE_UI_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CQTE_UI_Icon(const CQTE_UI_Icon& Prototype);
	virtual ~CQTE_UI_Icon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Camera_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

	void Set_State(IconState state) { m_State = state; }; // 상태 설정

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float					m_fSizeX{}, m_fSizeY{}, m_fX{}, m_fY{}, m_fAlpha{};
	_float4x4				m_ViewMatrix{}, m_ProjMatrix{};
	_int m_iTextureNumber = {};

	_bool m_bIsSelect = { false };
	IconState m_State = NOT_SELECTED; // 아이콘 상태

public:
	static CQTE_UI_Icon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END