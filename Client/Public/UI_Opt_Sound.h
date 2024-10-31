#pragma once

#include "UI_Option.h"

BEGIN(Client)

class CUI_Opt_Sound abstract : public CUI_Option
{
public:
	enum SOUND_MENU { BGM, SFX, VOICE, MENU_END };

protected:
	CUI_Opt_Sound(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Opt_Sound(const CUI_Opt_Sound& Prototype);
	virtual ~CUI_Opt_Sound() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Camera_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

protected:
	virtual HRESULT Bind_ShaderResources();
	virtual HRESULT Ready_Components();

private:
	void MenuChange();

protected:
	SOUND_MENU m_eMenuValue = { MENU_END };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END

