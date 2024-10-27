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
	atomic_bool isFinished();

	void Draw_Text() {
		SetWindowText(g_hWnd, m_szLoadingText);
	}

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	LEVELID						m_eLevelID = {};
	_tchar						m_szLoadingText[MAX_PATH] = {};

private:
	CGameInstance*				m_pGameInstance = { nullptr };

	//스레드 관련

	atomic_bool m_isFinished;
	vector<future<HRESULT>> m_futures;

	// 스레드 안전성을 위한 뮤텍스
	mutex m_TextMutex;

private:
	HRESULT Loading_For_Loading();
	HRESULT Loading_For_Logo();
	HRESULT Loading_For_GamePlayLevel();
	HRESULT Load_UI_Resources_Logo();
	HRESULT Load_UI_Resources_GamePlay();
	HRESULT Load_Texture_Resources_GamePlay();
	HRESULT Load_Model_Resources_GamePlay();
	HRESULT Load_Prototype_Object_GamePlay();
	HRESULT Load_Prototype_Component_GamePlay();

public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID);
	virtual void Free() override;
};

END