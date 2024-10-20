#pragma once

#include "Base.h"

BEGIN(Engine)

class CInput_Device : public CBase
{
private:
	CInput_Device(void);
	virtual ~CInput_Device(void) = default;

public:
	_byte Get_DIKeyState(_ubyte byKeyID) { return m_byKeyState[byKeyID]; }
	_byte Get_DIMouseState(MOUSEKEYSTATE eMouse) { return m_tMouseState.rgbButtons[eMouse]; }
	_long Get_DIMouseMove(MOUSEMOVESTATE eMouseState) { return *(((_long*)&m_tMouseState) + eMouseState); }
	_byte Get_DIMouseWheelState() { return m_tMouseState.lZ; }

<<<<<<< HEAD
	_bool Key_Pressing(_uint _iKey);
	_bool Key_Down(_uint _iKey);
	_bool Key_Up(_uint _iKey);
=======
	_bool MouseDown(MOUSEKEYSTATE eMouse) const {
		static _byte byLastState[3] = {};
		bool bDown = false;
		if ((m_tMouseState.rgbButtons[eMouse] & 0x80) && !(byLastState[eMouse] & 0x80))
			bDown = true;
		byLastState[eMouse] = m_tMouseState.rgbButtons[eMouse];
		return bDown;
	}

	_bool MousePress(MOUSEKEYSTATE eMouse) const {
		static _byte byLastState[3] = {};
		bool bPress = false;
		if ((m_tMouseState.rgbButtons[eMouse] & 0x80) && !(byLastState[eMouse] & 0x80))
			bPress = true;
		byLastState[eMouse] = m_tMouseState.rgbButtons[eMouse];
		return bPress;
	}

	_bool MouseUp(MOUSEKEYSTATE eMouse) const {
		static _byte byLastState[3] = {};
		bool bUp = false;
		if (!(m_tMouseState.rgbButtons[eMouse] & 0x80) && (byLastState[eMouse] & 0x80))
			bUp = true;
		byLastState[eMouse] = m_tMouseState.rgbButtons[eMouse];
		return bUp;
	}
>>>>>>> origin/ê¹€?œì™„

	_bool Mouse_Pressing(_uint _iButton);
	_bool Mouse_Down(_uint _iButton);
	_bool Mouse_Up(_uint _iButton);

public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void   Update(void);

private:
	LPDIRECTINPUT8         m_pInputSDK = { nullptr };
	LPDIRECTINPUTDEVICE8   m_pKeyBoard = { nullptr };
	LPDIRECTINPUTDEVICE8   m_pMouse = { nullptr };

	_byte m_byKeyState[256] = {}; // Å°º¸µå¿¡ ÀÖ´Â ¸ğµç Å°°ªÀ» ÀúÀåÇÏ±â À§ÇÑ 
	BYTE m_byPrevKeyState[256];   // Previous key stateº¯¼ö
	DIMOUSESTATE m_tMouseState = {};
	_bool m_bKeyState[256] = { false }; // ÀÌÀü Å° »óÅÂ¸¦ ÀúÀåÇÏ±â À§ÇÑ º¯¼ö
	_bool m_bMouseState[8] = { false }; // ÀÌÀü ¸¶¿ì½º »óÅÂ¸¦ ÀúÀåÇÏ±â À§ÇÑ º¯¼ö
	_bool m_bPrevMouseState[8] = { false }; // ÀÌÀü ÇÁ·¹ÀÓÀÇ ¸¶¿ì½º »óÅÂ¸¦ ÀúÀåÇÏ±â À§ÇÑ º¯¼ö

public:
	static CInput_Device* Create(HINSTANCE hInst, HWND hWnd);
	virtual void   Free(void);

};
END

