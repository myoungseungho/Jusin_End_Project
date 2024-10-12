#include "stdafx.h"
#include "..\Public\Camera_Free.h"
#include "Imgui_Manager.h"
#include "GameInstance.h"

CCamera_Free::CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{

}

CCamera_Free::CCamera_Free(const CCamera_Free& Prototype)
	: CCamera{ Prototype }
{

}

HRESULT CCamera_Free::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Free::Initialize(void* pArg)
{
	m_fMouseSensor = static_cast<CCamera_Free::CAMERA_FREE_DESC*>(pArg)->fSensor;	
	m_fMouseSensor = 1.f;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	CTransform::TRANSFORM_DESC tDesc{};
	tDesc.fSpeedPerSec = 0.1f;
	tDesc.fRotationPerSec = 0.1f;
	//tDesc.tDesc.fSpeedPerSec = 5.f;RotationPerSec = XMConvertToRadians(30.f);
	m_pTransformCom->SetUp_TransformDesc(&tDesc);

	return S_OK;
}

void CCamera_Free::Priority_Update(_float fTimeDelta)
{
    POINT ptMouse{};
    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    CImgui_Manager::IMGUI_SCREEN tDesc = CImgui_Manager::Get_Instance()->Get_Screen_Desc();

    // 마우스가 ImGui 창 위에 있는지 확인
    _bool isOverShaderImGui = (ptMouse.x >= tDesc.ShaderImGuiPos.x &&
        ptMouse.x <= tDesc.ShaderImGuiPos.x + tDesc.ShaderImGuiSize.x &&
        ptMouse.y >= tDesc.ShaderImGuiPos.y &&
        ptMouse.y <= tDesc.ShaderImGuiPos.y + tDesc.ShaderImGuiSize.y);

    if (isOverShaderImGui == true)
    {
        __super::Priority_Update(fTimeDelta);
        return;
    }

    _long MouseMoveX = {};
    _long MouseMoveY = {};

    // 시프트 + 가운데 마우스 버튼을 누른 상태에서 카메라 이동
    if ((GetKeyState(VK_SHIFT) & 0x8000) && (m_pGameInstance->Get_DIMouseState(DIMK_WHEEL) & 0x80))
    {
        MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMM_X);
        MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMM_Y);

        // 좌우 마우스 움직임에 따라 Go_Left와 Go_Right로 카메라 이동
        if (MouseMoveX < 0)
            m_pTransformCom->Go_Right(-MouseMoveX * m_fMouseSensor * fTimeDelta);
        else if (MouseMoveX > 0)
            m_pTransformCom->Go_Left(MouseMoveX * m_fMouseSensor * fTimeDelta);

        // 상하 마우스 움직임에 따라 Go_Up과 Go_Down로 카메라 이동
        if (MouseMoveY > 0)
            m_pTransformCom->Go_Up(MouseMoveY * m_fMouseSensor * fTimeDelta);
        else if (MouseMoveY < 0)
            m_pTransformCom->Go_Down(-MouseMoveY * m_fMouseSensor * fTimeDelta);
    }
    else if (m_pGameInstance->Get_DIMouseState(DIMK_WHEEL) & 0x80) // 휠만 눌렀을 때 회전
    {
        if ((MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMM_X)))
        {
            // Y축 회전
            m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fMouseSensor * MouseMoveX * fTimeDelta);
        }

        if ((MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMM_Y)))
        {
            // 상하 회전 (오른쪽 벡터를 축으로)
            m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), m_fMouseSensor * MouseMoveY * fTimeDelta);
        }
    }

    // 휠 스크롤로 줌인 줌아웃
    _long MouseWheel = m_pGameInstance->Get_DIMouseMove(DIMM_WHEEL);
    if (MouseWheel > 0)
    {
        // 휠을 위로 돌려 줌인
        m_pTransformCom->Go_Straight(m_fMouseSensor * MouseWheel * fTimeDelta);
    }
    else if (MouseWheel < 0)
    {
        // 휠을 아래로 돌려 줌아웃
        m_pTransformCom->Go_Backward(-m_fMouseSensor * MouseWheel * fTimeDelta);
    }

    // 기본 업데이트
    __super::Priority_Update(fTimeDelta);
}

void CCamera_Free::Update(_float fTimeDelta)
{
}

void CCamera_Free::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera_Free::Render(_float fTimeDelta)
{
	return S_OK;
}

CCamera_Free* CCamera_Free::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Free* pInstance = new CCamera_Free(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCamera_Free"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Free::Clone(void* pArg)
{
	CCamera_Free* pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCamera_Free"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();

}
