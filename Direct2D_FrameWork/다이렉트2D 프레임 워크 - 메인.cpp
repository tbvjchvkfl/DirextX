#include <windows.h>
#include "D2DFramework.h"




//const wchar_t gClassName[] = L"MyWindowClass";


//LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpcmdLine, _In_ int nShowcmd)
{
	// D2D Factory
	// ��� DirectX �Լ��� HRESULT�� ��ȯ�Ѵٰ� ���� ����. (�� ������ ��κ� HRESULT�� ��ȯ�ϴ� �Լ��� ����)
	try
	{
		D2DFramework myFramework;

		myFramework.Initialize(hInstance, L"Framework", 1024, 768);

		int ret = myFramework.GameLoop();

		myFramework.Release();

		return ret;

		//HWND hwnd;
		//WNDCLASSEX wc;

		//// 1�ܰ� - ������ Ŭ���� ���
		//ZeroMemory(&wc, sizeof(WNDCLASSEX));

		//wc.style = CS_HREDRAW | CS_VREDRAW;
		//wc.lpszClassName = gClassName;
		//wc.hInstance = hInstance;
		//wc.lpfnWndProc = WindowProc;
		//wc.cbSize = sizeof(WNDCLASSEX);
		//wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		//wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

		//if (!RegisterClassEx(&wc))
		//{
		//	MessageBox(nullptr, L"Failed to register!", L"Error", MB_ICONEXCLAMATION | MB_OK);
		//	return 0;
		//}

		//// 2�ܰ� ������ ����
		//RECT wr = { 0,0,1024,768 };
		//AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
		//hwnd = CreateWindowEx(0, gClassName, L"Solitatire Game", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, hInstance, NULL);

		//if (hwnd == nullptr)
		//{
		//	MessageBox(nullptr, L"Failed to create!", L"Error", MB_ICONEXCLAMATION | MB_OK);
		//	return 0;
		//}

		//myFramework.InitD2D(hwnd);

		/*ShowWindow(hwnd, nShowcmd);
		UpdateWindow(hwnd);*/


		//// ���ӿ��� ���� ���Ǵ� Message����
		//MSG msg;
		//while (true)
		//{
		//	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		//	{
		//		TranslateMessage(&msg);
		//		DispatchMessage(&msg);

		//		if (msg.message == WM_QUIT)
		//		{
		//			break;
		//		}
		//	}
		//	else
		//	{
		//		myFramework.Render();
		//	}
		//}

		//myFramework.Release();
		//return static_cast<int>(msg.wParam);
	}
	catch (const com_exception& e)
	{
		OutputDebugStringA(e.what());
		MessageBoxA(nullptr, e.what(), "Error", MB_OK);
	}
}


//LRESULT WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
//{
//	switch (message)
//	{
//	case WM_CLOSE:
//	{
//		DestroyWindow(hwnd);
//	}
//	break;
//	case WM_DESTROY:
//	{
//		PostQuitMessage(0);
//	}
//	break;
//	default:
//	{
//		return DefWindowProc(hwnd, message, wparam, lparam);
//	}
//	}
//	return 0;
//}