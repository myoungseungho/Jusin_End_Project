// Client.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "imgui_impl_win32.h"
#include "Client.h"
#include "MainApp.h"
#include "GameInstance.h"
#include "RenderInstance.h"
#include "Imgui_Manager.h"
#include "IMGUI_Shader_Tab.h"
#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE g_hInst;                                // 현재 인스턴스입니다.
HWND g_hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.
	CMainApp*				pMainApp = { nullptr };

    SetProcessDPIAware();

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

	MSG msg;

	pMainApp = CMainApp::Create();
	if (nullptr == pMainApp)
		return FALSE;

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return FALSE;

    CRenderInstance*     pRenderInstance = CRenderInstance::Get_Instance();
    if (nullptr == pRenderInstance)
        return FALSE;

	if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_Default"))))
		return FALSE;

	if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_60"))))
		return FALSE;

    //Update와 Render에 대한 누적시간
	_float		fTimeAcc = { 0.f };
    //FixedUpdate에 대한 누적시간
    _float fixedTimeStep = 1.0f / 50.0f; // FixedUpdate를 위한 고정 시간 간격 (0.02초)
    _float fixedTimeAcc = 0.0f;          // FixedUpdate 호출을 위한 누적 시간
    //프레임 체크에 대한 누적시간
    _float fpsTimeAcc = { 0.f };

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
				break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

        pGameInstance->Compute_TimeDelta(TEXT("Timer_Default")); // Compute_TimeDelta 호출 추가
        _float defaultUnscaledDeltaTime = pGameInstance->Get_UnscaledDeltaTime(TEXT("Timer_Default"));
        fTimeAcc += defaultUnscaledDeltaTime;
        fpsTimeAcc += defaultUnscaledDeltaTime;
        fixedTimeAcc += defaultUnscaledDeltaTime;

        while (fixedTimeAcc >= fixedTimeStep)
        {
            pMainApp->Fixed_Update(fixedTimeStep);
            fixedTimeAcc -= fixedTimeStep;
        }

		if (fTimeAcc >= 1.f / 60.0f)
		{ 
            // 스케일된 델타 타임 계산 (게임 업데이트에 사용)
            pGameInstance->Compute_TimeDelta(TEXT("Timer_60")); // Compute_TimeDelta 호출 추가
            float deltaTime = pGameInstance->Get_ScaledDeltaTime(TEXT("Timer_60"));

            /* 내 게임의 업데이트를 수행한다. (CMainApp)*/
            pMainApp->Update(deltaTime);

            /* 내 게임의 렌더를 수행한다.*/
            pMainApp->Render(deltaTime);

			fTimeAcc = 0.f;
		}
	}

	Safe_Release(pGameInstance);
	Safe_Release(pMainApp);

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   RECT rcWindowed = { 0, 0, g_iWinSizeX, g_iWinSizeY };


   AdjustWindowRect(&rcWindowed, WS_POPUP, FALSE);
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_POPUP,
       CW_USEDEFAULT, 0, rcWindowed.right - rcWindowed.left, rcWindowed.bottom - rcWindowed.top,
       nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   g_hWnd = hWnd;

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_DROPFILES:
    {
        HDROP hDrop = (HDROP)wParam;
        wchar_t szFile[MAX_PATH];
        if (DragQueryFile(hDrop, 0, szFile, MAX_PATH))
        {
            char ch[260];
            char DefChar = ' ';
            WideCharToMultiByte(CP_ACP, 0, szFile, -1, ch, 260, &DefChar, NULL);

            string filePath(ch);

            CImgui_Manager* pImGui_Manager = CImgui_Manager::Get_Instance();
            pImGui_Manager->Access_Shader_Tab()->Create_NodeTexture(filePath);
        }
        DragFinish(hDrop);
    }
    break;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) // ESC 키를 누르면 창 닫기
        {
            PostQuitMessage(0);
        }
        break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
