#pragma once

#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CPawn abstract : public CGameObject
{
protected:
	CPawn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPawn(const CPawn& Prototype);
	virtual ~CPawn() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

	_uint					m_iComboCount = { 0 };
	_float					m_fStunTImer = { 0.f };
	_uint					m_iHp = { 0 };
	_bool					m_bStun = { FALSE };

	_uint					m_iTmpe = { 0 };

	class CUI_Manager* m_pUIManager = { nullptr };


public:
	virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;
};

END