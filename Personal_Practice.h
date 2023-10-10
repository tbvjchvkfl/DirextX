#pragma once
#include <Windows.h>
#include <d2d1.h>

#pragma comment (lib, "d2d1.lib")

const wchar_t gClassName[] = L"Window_Direct2D";

// Direct2D ���丮 ����
// ���� Ÿ�� ����
// �׸���
// ���ҽ� ����

ID2D1Factory* D2DFac{};
ID2D1RenderTarget* D2DRender{};

LRESULT CALLBACK WIndowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevinstance, _In_ LPSTR Lpcmdline, _In_ int nShowcmd)
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &D2DFac);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Failed to Create Factory", L"Error", MB_OK);
		return 0;
	}
	
}