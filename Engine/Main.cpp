#include "stdafx.h"
#include "Window.h"
#include "Exception.h"
#include "App.h"
#include <iomanip>
#include <codecvt>

#define WINWIDTH 800
#define WINHEIGHT 600

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
	bool running = true;
	float time = 0.0f;
	try
	{
		//Window wnd(WINWIDTH, WINHEIGHT, L"Engine");
		//MSG msg;
		//BOOL gResult;

		//std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

		//while (running)
		//{
		//	// Check to see if any messages are waiting in the queue
		//	while ((gResult = PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) > 0)
		//	{
		//		static int i = 0;
		//		switch (msg.message)
		//		{
		//		case WM_QUIT:
		//			running = false;
		//			break;
		//		}
		//		
		//		while (!wnd.mMouse->IsEmpty())
		//		{
		//			const auto e = wnd.mMouse->Read();
		//			switch (e.GetType())
		//			{
		//			case Mouse::Event::Type::Leave:
		//				wnd.SetTitle(L"GONE");
		//				break;
		//			case Mouse::Event::Type::Move:
		//			{
		//				std::ostringstream oss;
		//				oss << "Mouse Position: (" << e.GetX() << ", " << e.GetY() << ")";
		//				std::string s = oss.str();
		//				std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		//				std::wstring ws = converter.from_bytes(s);
		//				wnd.SetTitle(ws);
		//			}
		//			break;
		//			case Mouse::Event::Type::WheelUp:
		//				i++;
		//				{
		//					std::stringstream oss;
		//					oss << "Up: " << i;
		//					std::string s = oss.str();
		//					std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		//					std::wstring ws = converter.from_bytes(s);
		//					wnd.SetTitle(ws);
		//				}
		//				break;
		//			case Mouse::Event::Type::WheelDown:
		//				i--;
		//				{
		//					std::stringstream oss;
		//					oss << "Down: " << i;
		//					std::string s = oss.str();
		//					std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		//					std::wstring ws = converter.from_bytes(s);
		//					wnd.SetTitle(ws);
		//				}
		//				break;
		//			}
		//		}

		//		TranslateMessage(&msg);
		//		DispatchMessage(&msg);
		//	}

		//	if (running)
		//	{
		//		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		//		double duration = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		//		float deltaTime = (float)(0.000000001 * duration);
		//		start = end;


		//		/*time += deltaTime;
		//		std::ostringstream oss;
		//		oss << "Time Elapsed: " << std::setprecision(3) << std::fixed << time << "s";
		//		std::string str = oss.str();
		//		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		//		std::wstring ws = converter.from_bytes(str);
		//		wnd.SetTitle(ws);*/
		//	}
		//}
		//if (gResult == -1)
		//{
		//	return -1;
		//}
		//return (int)msg.wParam;




		return App{}.Go();

		/*Window wnd(WINWIDTH, WINHEIGHT, L"Engine");

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

		return (int)msg.wParam;*/
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