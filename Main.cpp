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

		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (wnd.mKeyboard->KeyIsPressed(VK_MENU))
			{
				MessageBox(nullptr, L"Something HapponNN!", L"Alt Key Was Pressed", MB_OK | MB_ICONEXCLAMATION);
			}
			if (wnd.mKeyboard->KeyIsPressed('A'))
			{
				MessageBox(nullptr, L"Something HapponNN!", L"A Key Was Pressed", MB_OK | MB_ICONEXCLAMATION);
			}
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