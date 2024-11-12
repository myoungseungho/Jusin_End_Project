#pragma once

#include "GameObject.h"
#include "UI_Define.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class CModel;
class CShader;

END

BEGIN(Client)

class CUI_CharaSelectModel final :public CGameObject
{
public:

	typedef struct : CGameObject::GAMEOBJECT_DESC
	{
		_ushort iTeam = 1;
		CUI_Define::PLAYER_SLOT ePlayerSlot = {};
	}Character_DESC;

private:
	CUI_CharaSelectModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_CharaSelectModel(const CUI_CharaSelectModel& Prototype);
	virtual ~CUI_CharaSelectModel() = default;

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
	class CModel* m_pModelCom = { nullptr };
	class CShader* m_pShaderCom = { nullptr };

	string m_strName = {};
	_int iNum = { 1 };
public:
	static CUI_CharaSelectModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END