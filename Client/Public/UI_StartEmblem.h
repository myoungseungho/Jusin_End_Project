#pragma once

#include "UI_Start.h"
#include <queue>

BEGIN(Client)

class CUI_StartEmblem final :public CUI_Start
{
public:
	typedef struct
	{
		_uint iPos = {};
		_float m_fSpeed = {};

		_bool bStop = { FALSE };
		_float fStopDuration = { 0.f };

	}ANIM_INFO;
private:
	CUI_StartEmblem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_StartEmblem(const CUI_StartEmblem& Prototype);
	virtual ~CUI_StartEmblem() = default;

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

private:
	_uint m_iTextureIndex = { 0 };


public:
	static CUI_StartEmblem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END