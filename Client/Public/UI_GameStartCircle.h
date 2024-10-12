#pragma once

#include "UI_BaseAttBuf.h"
#include <queue>

BEGIN(Client)

class CUI_GameStartCircle final :public CUIObject
{
public:
	typedef struct
	{
		_uint iPos = {};
		_float m_fSpeed = {};
	}ANIM_INFO;
private:
	CUI_GameStartCircle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_GameStartCircle(const CUI_GameStartCircle& Prototype);
	virtual ~CUI_GameStartCircle() = default;

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
	void Action_Rotaion(_float fTimeDelta);
	void Action_ScaleAnim(_float fTimeDelta);

	void Init_Position();

private:
	_bool m_bFinishCheck = { FALSE };
	_bool m_isNegative = { FALSE };

	_uint m_iTextureIndex = { 0 };
	_uint m_iFinishPos = {};

	_float m_fScaleAnimTimer = { 0.f };

	queue<ANIM_INFO> m_QueueAnimPos = { };


public:
	static CUI_GameStartCircle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END