#include "D2DFramework.h"

#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")

HRESULT D2DFramework::InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT w, UINT h)
{
	HWND hwnd;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = D2DFramework::WndProc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

	if (!RegisterClassEx(&wc))
	{
		return E_FAIL;
	}

	RECT wr = { 0,0,static_cast<LONG>(w),static_cast<LONG>(h)};
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
	hwnd = CreateWindowEx(0, gClassName, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, hInstance, NULL);

	if (hwnd == nullptr)
	{
		return E_FAIL;
	}
	mHwnd = hwnd;

	SetWindowLongPtr(mHwnd, GWLP_USERDATA, reinterpret_cast <LONG_PTR> (this));

	return S_OK;
}

HRESULT D2DFramework::InitD2D(HWND hwnd)
{
	HRESULT hr;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, mspD2DFactory.GetAddressOf());
	ThrowIfFailed(hr);

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(mspWriteFactory), reinterpret_cast<IUnknown**>(mspWriteFactory.GetAddressOf()));
	
	hr = CreateDeviceResources();
    return hr;
}

HRESULT D2DFramework::CreateDeviceResources()
{
	RECT wr;
	GetClientRect(mHwnd, &wr);
	HRESULT hr;
	hr = mspD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(mHwnd, D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)), mspRenderTarget.GetAddressOf());

	ThrowIfFailed(hr);
	return S_OK;
}

HRESULT D2DFramework::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT w, UINT h)
{
	ThrowIfFailed(CoInitialize(nullptr));

	ThrowIfFailed(InitWindow(hInstance, title, w, h));
	ThrowIfFailed(InitD2D(mHwnd), "Failed To InitD2D");


	HRESULT hr = BitmapManager::Instance().Initialize(mspRenderTarget.Get());
	ThrowIfFailed(hr, "Failed To BitmapManager Initialize");

	ShowWindow(mHwnd, SW_SHOW);
	UpdateWindow(mHwnd);

	return S_OK;
}

void D2DFramework::Release()
{
	BitmapManager::Instance().Release();
	mspRenderTarget.Reset();
	mspD2DFactory.Reset();

	CoUninitialize();
}

int D2DFramework::GameLoop()
{
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			Render();
		}
	}
	return static_cast<int>(msg.wParam);
}

void D2DFramework::Render()
{
	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));
	HRESULT hr = mspRenderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		CreateDeviceResources();
	}
}

void D2DFramework::ShowError(LPCWSTR msg, LPCWSTR title)
{
    MessageBox(nullptr, msg, title, MB_OK);
}

LRESULT D2DFramework::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	D2DFramework* pFramwork = reinterpret_cast<D2DFramework*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	
	switch (message)
	{
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
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	}
	return 0;
}

void D2DFramework::WriteText(const WCHAR* text, FLOAT x, FLOAT y, FLOAT width, FLOAT height, const WCHAR* fontFamily, FLOAT fontsize, const D2D1_COLOR_F& textColor)
{
	if (mspRenderTarget)
	{
		Microsoft::WRL::ComPtr<IDWriteTextFormat> cpTexform;
		
		mspWriteFactory->CreateTextFormat(
			fontFamily,
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			fontsize,
			L"",
			&cpTexform
		);

		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> cpTexBrush;
		mspRenderTarget->CreateSolidColorBrush(textColor, &cpTexBrush);

		D2D1_RECT_F TextRect = D2D1::RectF(x, y, x + width, y + height);
		mspRenderTarget->DrawText(text, wcslen(text), cpTexform.Get(), TextRect, cpTexBrush.Get());
	}
}
