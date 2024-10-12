#pragma once

#include "UIObject.h"
#include <queue>

BEGIN(Client)

class CUI_GameStart abstract : public CUIObject
{
public:
	typedef struct
	{
		_uint iPos = {};
		_float m_fSpeed = {};

		_bool bStop = { FALSE };
		_float fStopDuration = { 0.f };

	}ANIM_INFO;

protected:
	CUI_GameStart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_GameStart(const CUI_GameStart& Prototype);
	virtual ~CUI_GameStart() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

protected:
	void Set_AnimPosition(_uint iPos, _float fAnimSpeed, _bool bStop = FALSE, _float fStopDuration = 0.f);
	void Action_ScaleAnim(_float fTimeDelta);

protected:
	_bool m_bAnimStop = { FALSE };

	_float m_fScaleAnimTimer = { 0.f };
	_float m_fStopTimer = { 0.f };

	queue<ANIM_INFO> m_QueueAnimPos = { };

protected:
	virtual HRESULT Bind_ShaderResources();
	virtual HRESULT Ready_Components();

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END

