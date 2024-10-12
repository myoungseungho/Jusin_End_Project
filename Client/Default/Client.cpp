// Client.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
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

// ���� ����:
HINSTANCE g_hInst;                                // ���� �ν��Ͻ��Դϴ�.
HWND g_hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
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

	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	CMainApp*				pMainApp = { nullptr };

    SetProcessDPIAware();

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
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

    //Update�� Render�� ���� �����ð�
	_float		fTimeAcc = { 0.f };
    //FixedUpdate�� ���� �����ð�
    _float fixedTimeStep = 1.0f / 50.0f; // FixedUpdate�� ���� ���� �ð� ���� (0.02��)
    _float fixedTimeAcc = 0.0f;          // FixedUpdate ȣ���� ���� ���� �ð�
    //������ üũ�� ���� �����ð�
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

        pGameInstance->Compute_TimeDelta(TEXT("Timer_Default")); // Compute_TimeDelta ȣ�� �߰�
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
            // �����ϵ� ��Ÿ Ÿ�� ��� (���� ������Ʈ�� ���)
            pGameInstance->Compute_TimeDelta(TEXT("Timer_60")); // Compute_TimeDelta ȣ�� �߰�
            float deltaTime = pGameInstance->Get_ScaledDeltaTime(TEXT("Timer_60"));

            /* �� ������ ������Ʈ�� �����Ѵ�. (CMainApp)*/
            pMainApp->Update(deltaTime);

            /* �� ������ ������ �����Ѵ�.*/
            pMainApp->Render(deltaTime);

			fTimeAcc = 0.f;
		}
	}

	Safe_Release(pGameInstance);
	Safe_Release(pMainApp);

    return (int) msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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
        if (wParam == VK_ESCAPE) // ESC Ű�� ������ â �ݱ�
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

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
