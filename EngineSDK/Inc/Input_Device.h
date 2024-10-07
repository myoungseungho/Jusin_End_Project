#pragma once

#include "Base.h"

BEGIN(Engine)

class CInput_Device : public CBase
{
private:
	CInput_Device(void);
	virtual ~CInput_Device(void) = default;
	
public:
	_byte	Get_DIKeyState(_ubyte byKeyID)			{ 
		return m_byKeyState[byKeyID]; }

	

	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse) 	{ 	
		return m_tMouseState.rgbButtons[eMouse]; 	
	}

	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)	
	{	
		return *(((_long*)&m_tMouseState) + eMouseState);	
	}
	
public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void	Update(void);

private:
	LPDIRECTINPUT8			m_pInputSDK = { nullptr };

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = { nullptr };
	LPDIRECTINPUTDEVICE8	m_pMouse = { nullptr };



private:	
	_byte					m_byKeyState[256] = {};		// Ű���忡 �ִ� ��� Ű���� �����ϱ� ���� ����
	DIMOUSESTATE			m_tMouseState = {};

public:
	static CInput_Device* Create(HINSTANCE hInst, HWND hWnd);
	virtual void	Free(void);

};
END

