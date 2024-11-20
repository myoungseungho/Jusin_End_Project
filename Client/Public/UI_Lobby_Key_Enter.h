#pragma once

#include "UIObject.h"

BEGIN(Engine)

class CTexture;
class 

END

BEGIN(Client)

class CUI_Lobby_Key_Enter final :public CGameObject
{
private:
	CUI_Lobby_Key_Enter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Lobby_Key_Enter(const CUI_Lobby_Key_Enter& Prototype);
	virtual ~CUI_Lobby_Key_Enter() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Camera_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Bind_ShaderResources();

public:
	static CUI_Lobby_Key_Enter* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END