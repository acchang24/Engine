#include "stdafx.h"
#include "Window.h"
#include "Exception.h"

#define WINWIDTH 800
#define WINHEIGHT 600

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
	try
	{
		Window wnd(WINWIDTH, WINHEIGHT, L"Engine");

		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			static int i = 0;
			while (!wnd.mMouse->IsEmpty())
			{
				const auto e = wnd.mMouse->Read();
				switch (e.GetType())
				{
				case Mouse::Event::Type::LRelease:
				case Mouse::Event::Type::Leave:
					wnd.SetTitle(L"GONE");
					break;
				case Mouse::Event::Type::Move:
					{
						std::ostringstream oss;
						oss << "Mouse Position: (" << e.GetX() << ", " << e.GetY() << ")";
						std::string s = oss.str();
						std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
						std::wstring ws = converter.from_bytes(s);
						wnd.SetTitle(ws);
					}
					break;
				case Mouse::Event::Type::WheelUp:
					i++;
					{
						std::stringstream oss;
						oss << "Up: " << i;
						std::string s = oss.str();
						std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
						std::wstring ws = converter.from_bytes(s);
						wnd.SetTitle(ws);
					}
					break;
				case Mouse::Event::Type::WheelDown:
					i--;
					{
						std::stringstream oss;
						oss << "Down: " << i;
						std::string s = oss.str();
						std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
						std::wstring ws = converter.from_bytes(s);
						wnd.SetTitle(ws);
					}
					break;
				}
			}

			if (wnd.mKeyboard->KeyIsPressed(VK_SPACE))
			{
				MessageBox(nullptr, L"Something HapponNN!", L"Space Key Was Pressed", MB_OK | MB_ICONEXCLAMATION);
			}
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