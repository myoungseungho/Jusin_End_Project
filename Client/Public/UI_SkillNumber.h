#pragma once

#include "UI_BaseAttBuf.h"

BEGIN(Client)

class CUI_SkillNumber final :public CUI_BaseAttBuf
{
private:
	CUI_SkillNumber(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_SkillNumber(const CUI_SkillNumber& Prototype);
	virtual ~CUI_SkillNumber() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components();

private:
	_uint m_iSkillCount = {0};

public:
	static CUI_SkillNumber* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END