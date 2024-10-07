#pragma once

#include "UIObject.h"
#include "Monster.h"

BEGIN(Client)

class CUI_Chara_Icon final :public CUIObject
{
private:
	CUI_Chara_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Chara_Icon(const CUI_Chara_Icon& Prototype);
	virtual ~CUI_Chara_Icon() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components();

private:
	_uint m_iCharaID = { 0 };

public:
	static CUI_Chara_Icon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END