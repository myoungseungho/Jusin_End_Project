#pragma once

#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CShader_Texture final : public CGameObject
{
public:
	typedef struct : CGameObject::GAMEOBJECT_DESC
	{
		_wstring prototypeKey;
		const _tchar* filePath;
	}SHADER_TEXTURE_DESC;
	typedef struct
	{
		_bool isOn = { false };
		_float2* vDirection = { nullptr };
		_float* fSpeed = { nullptr };
	}Shade_MoveTex;
private:
	CShader_Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShader_Texture(const CShader_Texture& Prototype);
	virtual ~CShader_Texture() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

	void Push_InputTextures(ID3D11ShaderResourceView* pSRV);
	void Push_Shade_MoveTex(_float2* pDirection, _float* pSpeed);

	Shade_MoveTex m_MoveTex;
	CTexture* m_pTextureCom = { nullptr };
private:
	_bool m_isAlpha = { false };
	vector<ID3D11ShaderResourceView*> m_InputTextures;
private:
	_bool m_isTex = { false };
private:
	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	_wstring m_Key;
	_float m_fTime = { 0.f };
private:
	_float					m_fX{}, m_fY{}, m_fSizeX{}, m_fSizeY{};
	_float4x4				m_ViewMatrix{}, m_ProjMatrix{};

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	static CShader_Texture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END