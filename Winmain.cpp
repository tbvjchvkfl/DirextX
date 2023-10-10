#include <windows.h>
#include <d2d1.h>

#pragma comment (lib, "d2d1.lib")

const wchar_t gClassName[] = L"SolitaireWindowClass";

// 1. Direct2D ���丮 ����
// 2. ���� Ÿ�� ����
// 3. �׸��� (���� Ÿ��)
// 4. ���ҽ� ����


ID2D1Factory* gpD2DFactory{};
ID2D1HwndRenderTarget* gpRenderTarget{};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

// Test
// std::list<solitaire::Card> myDeck;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpcmdLine, _In_ int nShowcmd)
{
	// D2D Factory
	// ��� DirectX �Լ��� HRESULT�� ��ȯ�Ѵٰ� ���� ����. (�� ������ ��κ� HRESULT�� ��ȯ�ϴ� �Լ��� ����)

	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &gpD2DFactory);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Failed to Create Factory", L"Error", MB_OK);
		return 0;
	}

	HWND hwnd;
	WNDCLASSEX wc;

	// 1�ܰ� - ������ Ŭ���� ���
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed to register!", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// 2�ܰ� ������ ����
	RECT wr = { 0,0,1024,768 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
	hwnd = CreateWindowEx(0, gClassName, L"Solitatire Game", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, hInstance, NULL);

	if (hwnd == nullptr)
	{
		MessageBox(nullptr, L"Failed to create!", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	GetClientRect(hwnd, &wr);

	gpD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)), &gpRenderTarget);

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Failed to Create HWNDRenderTarget!", L"Error", MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nShowcmd);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// ����
	if (gpRenderTarget != nullptr)
	{
		gpRenderTarget->Release();
		gpRenderTarget = nullptr;
	}
	if (gpD2DFactory != nullptr)
	{
		gpD2DFactory->Release();
		gpD2DFactory = nullptr;
	}
	
	
	return static_cast<int>(msg.wParam);
}

void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	
	// RenderTarget�� �׸���
	gpRenderTarget->BeginDraw();
	gpRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));
	gpRenderTarget->EndDraw();

	EndPaint(hwnd, &ps);
}

LRESULT WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_LBUTTONUP:
	{
		//gLogic.OnClick(LOWORD(lparam), HIWORD(lparam));
	}
	break;
	case WM_PAINT:
	{
		OnPaint(hwnd);
	}
	break;
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	default:
	{
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
	}
	return 0;
}