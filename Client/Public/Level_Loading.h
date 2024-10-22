#pragma once

#include "Client_Defines.h"
#include "Level.h"

/* 1. �ε� ��鿡 �ʿ��� ��ü�� �����, ���ҽ��� �ε��Ѵ�. */
/* 2. �ε� ����� �������� �����ش�. */


BEGIN(Client)

class CLevel_Loading final : public CLevel
{
private:
	CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Loading() = default;

public:
	HRESULT Ready_Prototype_Component();
	HRESULT Ready_Layer();

	virtual HRESULT Initialize(LEVELID eNextLevelID);
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	LEVELID				m_eNextLevelID = {};
	class CLoader*		m_pLoader = { nullptr };

	static _bool m_bIsLevelPrepared;

public:
	static CLevel_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID);
	virtual void Free() override;
};

END