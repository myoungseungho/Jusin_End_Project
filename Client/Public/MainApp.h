#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "RenderInstance.h"
#include "Imgui_Manager.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Renderer)
class CRenderInstance;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void Update(_float fTimeDelta);
	HRESULT Render();

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	CGameInstance*				m_pGameInstance = { nullptr };
	CRenderInstance*			m_pRenderInstance = { nullptr };
private:
	HRESULT Open_Level(LEVELID eStartLevelID);
	HRESULT Ready_Layer_IMGUI_Manager();

public:
	static CMainApp* Create();
	virtual void Free() override;
};

END