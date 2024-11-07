#pragma once

#include "UIObject.h"

BEGIN(Client)

class CUI_SelectArrow final :public CUIObject
{
private:
	CUI_SelectArrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_SelectArrow(const CUI_SelectArrow& Prototype);
	virtual ~CUI_SelectArrow() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Camera_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components();

private:
	void PositionUpdate(_float fTimeDelta);
	void Move(_float fTimeDelta);

private:
	CUI_Define::PLAYER_ID m_ePlayerID = { CUI_Define::PAWN_END };
	_bool m_bMoveStop = { FALSE };


public:
	static CUI_SelectArrow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END