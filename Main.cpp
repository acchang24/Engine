#include "stdafx.h"
#include "Window.h"
#include "Exception.h"

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
	try
	{
		Window wnd(800, 600, L"Engine");
		//Window wnd2(400, 300, L"This is another window");

		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (gResult == -1)
		{
			return -1;
		}

		return (int)msg.wParam;
	}
	catch (const Exception& e)
	{
		MessageBox(nullptr, e.What().c_str(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, L"No details available", L"Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}