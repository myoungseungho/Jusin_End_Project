#pragma once

#include "Component_Manager.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	/* 엔진을 초기화한다. */
	HRESULT Initialize_Engine(HWND hWnd, _bool isWindowed, _uint iNumLevels, _uint iWinSizeX, _uint iWinSizeY, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);
	void Update_Engine(_float fTimeDelta);
	HRESULT Render_Engine();
	HRESULT Clear_LevelResources(_uint iLevelIndex);

public: /* For.Graphic_Device */		
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);	
	HRESULT Clear_DepthStencil_View();	
	HRESULT Present();


public: /* For.Level_Manager */
	HRESULT Change_Level(class CLevel* pNewLevel);

public: /* For.Timer_Manager */
	HRESULT Add_Timer(const _wstring& strTimerTag);
	_float Compute_TimeDelta(const _wstring& strTimerTag);
	_float Get_ScaledDeltaTime(const _wstring& strTimerTag);

public: /* For.Object_Manager */
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject_ToLayer(_uint iLevelIndex, const wstring& strPrototypeTag, const wstring& strLayerTag, void* pArg = nullptr);
	class CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex = 0);
	class CGameObject* Clone_GameObject(const wstring& strPrototypeTag, void* pArg = nullptr);

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);


private:
	class CGraphic_Device*				m_pGraphic_Device = { nullptr };
	class CLevel_Manager*				m_pLevel_Manager = { nullptr };
	class CTimer_Manager*				m_pTimer_Manager = { nullptr };
	class CObject_Manager*				m_pObject_Manager = { nullptr };
	class CComponent_Manager*			m_pComponent_Manager = { nullptr };

public:
	void Release_Engine();
	
	virtual void Free() override;

};

END

