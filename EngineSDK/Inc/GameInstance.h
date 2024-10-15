#pragma once

#include "Component_Manager.h"
#include "PipeLine.h"
#include "ThreadPool.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	/* 엔진을 초기화한다. */
	HRESULT Initialize_Engine(HINSTANCE hInst, HWND hWnd, _bool isWindowed, _uint iNumLevels, _uint iWinSizeX, _uint iWinSizeY, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);
	void Update_Engine(_float fTimeDelta);
	HRESULT Render_Engine(_float fTimeDelta);
	HRESULT Clear_LevelResources(_uint iLevelIndex);

public: /* For.Graphic_Device */		
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);	
	HRESULT Clear_DepthStencil_View();	
	HRESULT Present();

public: /* For.Input_Device */
	_byte	Get_DIKeyState(_ubyte byKeyID);
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouseKeyState);
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseMoveState);
	_bool MouseDown(MOUSEKEYSTATE eMouse);
	_bool MousePress(MOUSEKEYSTATE eMouse);
	_bool MouseUp(MOUSEKEYSTATE eMouse);
public: /* For.Level_Manager */
	HRESULT Change_Level(class CLevel* pNewLevel);
	_uint Get_CurrentLevel_Index();
	_uint Get_LoadingLevel_Index();
	HRESULT Set_LoadingLevel_Index(_uint);

public: /* For.Timer_Manager */
	HRESULT Add_Timer(const _wstring& strTimerTag);
	_float Compute_TimeDelta(const _wstring& strTimerTag);
	void StartSlowMotion(_float fTimeScale);
	void StopSlowMotion();
	_float Get_UnscaledDeltaTime(const _wstring& strTimerTag);
	_float Get_ScaledDeltaTime(const _wstring& strTimerTag);

public: /* For.Object_Manager */
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject_ToLayer(_uint iLevelIndex, const wstring& strPrototypeTag, const wstring& strLayerTag, void* pArg = nullptr);
	class CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex = 0);
	class CGameObject* Clone_GameObject(const wstring& strPrototypeTag, void* pArg = nullptr);
	class CGameObject* Find_Prototype(const wstring& strPrototypeTag);

	HRESULT Get_Prototype_Names(vector<string>* pVector);
	HRESULT Add_Object_Layers_Vector(_uint iLevelIndex, vector<pair < string, list<CGameObject*>>>*);
	HRESULT Add_Object_Layers_Vector(_uint iLevelIndex, vector<pair < _wstring, list<CGameObject*>>>*);

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
	vector<const _wstring*>* Find_Prototype_Include_Key(_uint iLevelIndex, const _wstring& strIncludeTag);

public: /* For.PipeLine */
	_matrix Get_Transform_Matrix(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_float4x4 Get_Transform_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_matrix Get_Transform_Inverse_Matrix(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_float4x4 Get_Transform_Inverse_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_vector Get_CamPosition_Vector() const;
	_float4 Get_CamPosition_Float4() const;
	void Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix);

public: /* For.ThreadPool */
	HRESULT Initialize_ThreadPool(size_t ThreadCount);
	_uint Get_ThreadNumber();

	// 템플릿 EnqueueTask 함수: CThreadPool의 EnqueueTask를 호출
	template<typename FunctionType>
	future<typename result_of<FunctionType()>::type> EnqueueTask(FunctionType task)
	{
		if (m_pThreadPool)
			return m_pThreadPool->EnqueueTask(task);
		else
			throw runtime_error("ThreadPool is not initialized.");
	}

public: /* For.Collider_Manager*/
	HRESULT Add_ColliderObject(CCollider_Manager::COLLIDERGROUP eRenderGroup, class CCollider* pRenderObject);

public: /* For.FileManager */
	HRESULT SaveObjects(const wstring& filename, void* pArg);
	void* LoadObjects(const wstring& filename);

private:
	class CGraphic_Device*				m_pGraphic_Device = { nullptr };
	class CInput_Device*				m_pInput_Device = { nullptr };
	class CLevel_Manager*				m_pLevel_Manager = { nullptr };
	class CTimer_Manager*				m_pTimer_Manager = { nullptr };
	class CObject_Manager*				m_pObject_Manager = { nullptr };
	class CComponent_Manager*			m_pComponent_Manager = { nullptr };
	class CCollider_Manager*			m_pCollider_Manager = { nullptr };
	class CPipeLine*					m_pPipeLine = { nullptr };
	class CThreadPool*					m_pThreadPool = { nullptr };
	class CFile_Manager*				m_pFile_Manager = { nullptr };
	
public:
	void Release_Engine();
	
	virtual void Free() override;

};

END