#include "..\Public\Input_Device.h"

Engine::CInput_Device::CInput_Device(void)
{

}



HRESULT Engine::CInput_Device::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{

    // DInput �İ�ü�� �����ϴ� �Լ�
    if (FAILED(DirectInput8Create(hInst,
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        (void**)&m_pInputSDK,
        NULL)))
        return E_FAIL;

    // Ű���� ��ü ����
    if (FAILED((m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr))))
        return E_FAIL;

    // ������ Ű���� ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
    m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

    // ��ġ�� ���� �������� �������ִ� �Լ�, (Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�)
    m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

    // ��ġ�� ���� access ������ �޾ƿ��� �Լ�
    m_pKeyBoard->Acquire();


    // ���콺 ��ü ����
    if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
        return E_FAIL;

    // ������ ���콺 ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
    m_pMouse->SetDataFormat(&c_dfDIMouse);

    // ��ġ�� ���� �������� �������ִ� �Լ�, Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�
    m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

    // ��ġ�� ���� access ������ �޾ƿ��� �Լ�
    m_pMouse->Acquire();


    return S_OK;
}

_bool CInput_Device::Key_Pressing(_uint _iKey) {
	return (m_byKeyState[_iKey] & 0x80) != 0;
}

_bool CInput_Device::Key_Down(_uint _iKey) {
	return (!(m_byPrevKeyState[_iKey] & 0x80) && (m_byKeyState[_iKey] & 0x80));
}

_bool CInput_Device::Key_Up(_uint _iKey) {
	return ((m_byPrevKeyState[_iKey] & 0x80) && !(m_byKeyState[_iKey] & 0x80));
}


void Engine::CInput_Device::Update(void)
{
	// ���� Ű ���¸� �����մϴ�.
	memcpy(m_byPrevKeyState, m_byKeyState, sizeof(m_byKeyState));
	/* Ű����� ���콺�� ���¸� ���ͼ� �������ش�. */

	//���� Ű ����
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);

	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);

	// ���� ���콺 ���¸� �����մϴ�.
	for (int i = 0; i < 3; ++i) {
		m_bPrevMouseState[i] = m_bMouseState[i];
	}

	// ���� �������� ���콺 ���¸� �����մϴ�.
	for (int i = 0; i < 3; ++i) {
		m_bMouseState[i] = (m_tMouseState.rgbButtons[i] & 0x80) != 0;
	}
}


CInput_Device * CInput_Device::Create(HINSTANCE hInst, HWND hWnd)
{
    CInput_Device* pInstance = new CInput_Device();

    if (FAILED(pInstance->Ready_InputDev(hInst, hWnd)))
    {
        MSG_BOX(TEXT("Failed to Created : CInput_Device"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Engine::CInput_Device::Free(void)
{
    Safe_Release(m_pKeyBoard);
    Safe_Release(m_pMouse);
    Safe_Release(m_pInputSDK);
}

