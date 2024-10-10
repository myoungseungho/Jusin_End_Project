#include "..\Public\GameInstance.h"

#include "Component_Manager.h"
#include "Graphic_Device.h"
#include "Object_Manager.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "Input_Device.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()	
{

}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, HWND hWnd, _bool isWindowed, _uint iNumLevels, _uint iWinSizeX, _uint iWinSizeY, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
	m_pGraphic_Device = CGraphic_Device::Create(hWnd, isWindowed, iWinSizeX, iWinSizeY, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pInput_Device = CInput_Device::Create(hInst, hWnd);
	if (nullptr == m_pInput_Device)
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::Create(iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pComponent_Manager = CComponent_Manager::Create(iNumLevels);
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;


	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	/* 엔진에서 관리하는 객체들 중, 반복적인 갱신이 필요한 객체들이 있다면. */
	/* 여기에서 갱신을 수행해준다. */	


	m_pInput_Device->Update();

	m_pObject_Manager->Priority_Update(fTimeDelta);

	m_pPipeLine->Update();

	m_pObject_Manager->Update(fTimeDelta);

	m_pObject_Manager->Late_Update(fTimeDelta);

	m_pLevel_Manager->Update(fTimeDelta);



}

HRESULT CGameInstance::Render_Engine()
{
	/* 엔진에서 관리하는 객체들 중, 반복적인 렌더가 필요한 객체들이 있다면. */
	/* 여기에서 렌더를 수행해준다. */

	if (FAILED(m_pLevel_Manager->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameInstance::Clear_LevelResources(_uint iLevelIndex)
{
	if (FAILED(m_pObject_Manager->Clear_Resources(iLevelIndex)))
		return E_FAIL;

	if(FAILED(m_pComponent_Manager->Clear_Resources(iLevelIndex)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);	
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	return m_pGraphic_Device->Present();
}


_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	return m_pInput_Device->Get_DIKeyState(byKeyID);
}

_byte CGameInstance::Get_DIMouseState(MOUSEKEYSTATE eMouseKeyState)
{
	return m_pInput_Device->Get_DIMouseState(eMouseKeyState);
}

_long CGameInstance::Get_DIMouseMove(MOUSEMOVESTATE eMouseMoveState)
{
	return m_pInput_Device->Get_DIMouseMove(eMouseMoveState);
}


HRESULT CGameInstance::Change_Level(CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Change_Level(pNewLevel);	
}

HRESULT CGameInstance::Add_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

_float CGameInstance::Compute_TimeDelta(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Compute_TimeDelta(strTimerTag);
}

void CGameInstance::StartSlowMotion(_float fTimeScale)
{
	m_pTimer_Manager->Set_TimeScale(fTimeScale);
}

void CGameInstance::StopSlowMotion()
{
	m_pTimer_Manager->Set_TimeScale(1.0f); // 시간 스케일을 정상으로 복원
}

_float CGameInstance::Get_UnscaledDeltaTime(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Get_UnscaledDeltaTime(strTimerTag);
}

_float CGameInstance::Get_ScaledDeltaTime(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Get_ScaledDeltaTime(strTimerTag);
}
HRESULT CGameInstance::Add_Prototype(const wstring & strPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(strPrototypeTag, pPrototype);	
}

HRESULT CGameInstance::Add_GameObject_ToLayer(_uint iLevelIndex, const wstring & strPrototypeTag, const wstring & strLayerTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject_ToLayer(iLevelIndex, strPrototypeTag, strLayerTag, pArg);
}

CComponent * CGameInstance::Get_Component(_uint iLevelIndex, const _wstring & strLayerTag, const _wstring & strComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return	m_pObject_Manager->Get_Component(iLevelIndex, strLayerTag, strComponentTag, iIndex);
}

CGameObject* CGameInstance::Clone_GameObject(const wstring& strPrototypeTag, void* pArg)
{
	return m_pObject_Manager->Clone_GameObject(strPrototypeTag, pArg);
}

CGameObject* CGameInstance::Get_Object(_uint iLevelIndex, const wstring& strLayerTag, _uint iindex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Object(iLevelIndex, strLayerTag, iindex);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _wstring & strPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);	
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const _wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, strPrototypeTag, pArg);
}


_matrix CGameInstance::Get_Transform_Matrix(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Matrix(eState);
}

_float4x4 CGameInstance::Get_Transform_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Float4x4(eState);
}

_matrix CGameInstance::Get_Transform_Inverse_Matrix(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Inverse_Matrix(eState);
}

_float4x4 CGameInstance::Get_Transform_Inverse_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Inverse_Float4x4(eState);
}

_vector CGameInstance::Get_CamPosition_Vector() const
{
	return m_pPipeLine->Get_CamPosition_Vector();
}

_float4 CGameInstance::Get_CamPosition_Float4() const
{
	return m_pPipeLine->Get_CamPosition_Float4();
}

void CGameInstance::Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	m_pPipeLine->Set_Transform(eState, TransformMatrix);
}



void CGameInstance::Release_Engine()
{
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pInput_Device);

	CGameInstance::Get_Instance()->Destroy_Instance();
}

void CGameInstance::Free()
{
	__super::Free();

	
}
