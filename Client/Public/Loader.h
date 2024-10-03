#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* 3. 다음 장면(레벨)에 필요한 자원도 로드하낟. */

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLoader() = default;

public:
	HRESULT Initialize(LEVELID eNextLevelID);
	HRESULT Loading();

public:
	_bool isFinished() const {
		return m_isFinished;
	}

	void Draw_Text() {
		SetWindowText(g_hWnd, m_szLoadingText);
	}

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	LEVELID						m_eLevelID = {};
	_bool						m_isFinished = { false };
	_tchar						m_szLoadingText[MAX_PATH] = {};

private:
	HANDLE						m_hThread = {};
	CRITICAL_SECTION			m_Critical_Section = {};
	CGameInstance*				m_pGameInstance = { nullptr };


private:
	HRESULT Loading_For_LogoLevel();	
	HRESULT Loading_For_GamePlayLevel();

public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID);
	virtual void Free() override;
};

END