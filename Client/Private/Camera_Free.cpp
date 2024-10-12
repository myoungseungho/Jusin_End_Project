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

    // ���콺�� ImGui â ���� �ִ��� Ȯ��
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

    // ����Ʈ + ��� ���콺 ��ư�� ���� ���¿��� ī�޶� �̵�
    if ((GetKeyState(VK_SHIFT) & 0x8000) && (m_pGameInstance->Get_DIMouseState(DIMK_WHEEL) & 0x80))
    {
        MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMM_X);
        MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMM_Y);

        // �¿� ���콺 �����ӿ� ���� Go_Left�� Go_Right�� ī�޶� �̵�
        if (MouseMoveX < 0)
            m_pTransformCom->Go_Right(-MouseMoveX * m_fMouseSensor * fTimeDelta);
        else if (MouseMoveX > 0)
            m_pTransformCom->Go_Left(MouseMoveX * m_fMouseSensor * fTimeDelta);

        // ���� ���콺 �����ӿ� ���� Go_Up�� Go_Down�� ī�޶� �̵�
        if (MouseMoveY > 0)
            m_pTransformCom->Go_Up(MouseMoveY * m_fMouseSensor * fTimeDelta);
        else if (MouseMoveY < 0)
            m_pTransformCom->Go_Down(-MouseMoveY * m_fMouseSensor * fTimeDelta);
    }
    else if (m_pGameInstance->Get_DIMouseState(DIMK_WHEEL) & 0x80) // �ٸ� ������ �� ȸ��
    {
        if ((MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMM_X)))
        {
            // Y�� ȸ��
            m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fMouseSensor * MouseMoveX * fTimeDelta);
        }

        if ((MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMM_Y)))
        {
            // ���� ȸ�� (������ ���͸� ������)
            m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), m_fMouseSensor * MouseMoveY * fTimeDelta);
        }
    }

    // �� ��ũ�ѷ� ���� �ܾƿ�
    _long MouseWheel = m_pGameInstance->Get_DIMouseMove(DIMM_WHEEL);
    if (MouseWheel > 0)
    {
        // ���� ���� ���� ����
        m_pTransformCom->Go_Straight(m_fMouseSensor * MouseWheel * fTimeDelta);
    }
    else if (MouseWheel < 0)
    {
        // ���� �Ʒ��� ���� �ܾƿ�
        m_pTransformCom->Go_Backward(-m_fMouseSensor * MouseWheel * fTimeDelta);
    }

    // �⺻ ������Ʈ
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
