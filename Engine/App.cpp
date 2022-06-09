#include "stdafx.h"
#include "App.h"
#include <iomanip>
#include "Graphics.h"

#define WINWIDTH 800
#define WINHEIGHT 600

App::App()
{
	wnd = new Window(WINWIDTH, WINHEIGHT, L"Engine");
	running = true;
}

App::~App()
{

}

void App::Init()
{

}

void App::ShutDown()
{

}

int App::Run()
{
	// Initialize everything
	Init();

	MSG msg = {};

	// Get the current time
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

	while (running)
	{
		// Check to see if any messages are waiting in the queue
		while ((PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			switch (msg.message)
			{
			case WM_QUIT:
				running = false;
				break;
			}

			/*static int i = 0;
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
			if (wnd.mKeyboard->KeyIsPressed('A'))
			{
				MessageBox(nullptr, L"Something HapponNN!", L"A Key Was Pressed", MB_OK | MB_ICONEXCLAMATION);
			}*/
		}

		if (running)
		{
			std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
			double duration = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			float deltaTime = (float)(0.000000001 * duration);
			start = end;


			/*time += deltaTime;
			std::ostringstream oss;
			oss << "Time Elapsed: " << std::setprecision(3) << std::fixed << 1.0f / deltaTime << "s";
			std::string str = oss.str();
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::wstring ws = converter.from_bytes(str);
			wnd.SetTitle(ws);*/

			RenderFrame();
		}
	}

	delete wnd;

	return (int)msg.wParam;
}

void App::RenderFrame()
{
	Graphics* g = wnd->GetGraphics();

	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	
	// set render target
	g->SetBuffer(g->GetBackBuffer(), g->GetDepthStencilView());

	g->ClearBuffer(c, c, 1.0f);

	g->ClearDepthBuffer(g->GetDepthStencilView(), 1.0f);

	g->DrawTestTriangle(0.0f, 0.0f, 1.0f);

	g->DrawTestTriangle(wnd->mMouse->GetPosX() / 400.0f - 1.0f, -wnd->mMouse->GetPosY() / 300.0f + 1.0f, -1.0f);

	g->EndFrame();
}