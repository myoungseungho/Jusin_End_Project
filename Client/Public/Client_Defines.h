#pragma once

#include <process.h>

namespace Client
{
	const unsigned int	g_iWinSizeX = { 1920 };
	const unsigned int	g_iWinSizeY = { 1080 };	


	enum LEVELID { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO,LEVEL_GAMEPLAY, LEVEL_END };


	enum EFFECT_TYPE {EFFECT_SINGLE, EFFECT_MOVETEX, EFFECT_MULTI, EFFECT_END};
}

extern HINSTANCE	g_hInst;
extern HWND			g_hWnd;

using namespace Client;
