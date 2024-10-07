#pragma once

#include "UI_BaseAttBuf.h"

BEGIN(Client)

class CUI_SkillGauge final :public CUI_BaseAttBuf
{
private:
	CUI_SkillGauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_SkillGauge(const CUI_SkillGauge& Prototype);
	virtual ~CUI_SkillGauge() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components();

public:
	static CUI_SkillGauge* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END