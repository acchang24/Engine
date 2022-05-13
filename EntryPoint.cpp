#include "stdafx.h"
//#include <Windows.h>
//#include <sstream>
//
//// Define a custom windows procedure
//LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	switch (msg)
//	{
//	case WM_CLOSE:
//		PostQuitMessage(69);
//		break;
//	case WM_KEYDOWN:
//		if (wParam == 'F')
//		{
//			SetWindowText(hWnd, L"Respect");
//		}
//		break;
//	case WM_KEYUP:
//		if (wParam == 'F')
//		{
//			SetWindowText(hWnd, L"Engine");
//		}
//		break;
//	case WM_CHAR:
//		{
//			static std::wstring title;
//			title.push_back((char)wParam);
//			SetWindowText(hWnd, title.c_str());
//		}
//		break;
//	case WM_LBUTTONDOWN:
//		{
//			POINTS pt = MAKEPOINTS(lParam);
//			std::ostringstream oss;
//			oss << "(" << pt.x << "," << pt.y << ")";
//			std::string s = oss.str();
//			std::wstring t(s.begin(), s.end());
//			SetWindowText(hWnd, t.c_str());
//		}
//		break;
//	}
//
//	return DefWindowProc(hWnd, msg, wParam, lParam);
//}
//
//
//int CALLBACK WinMain(
//	HINSTANCE hInstance, 
//	HINSTANCE hPrevInstance, 
//	LPSTR lpCmdLine, 
//	int nCmdShow)
//{
//
//	// Name of window class
//	const auto pClassName = L"Engine";
//
//	
//
//	// Register a window class
//	WNDCLASSEX wc;
//	ZeroMemory(&wc, sizeof(WNDCLASSEX));
//	wc.cbSize = sizeof(WNDCLASSEX);
//	wc.style = CS_OWNDC;
//	wc.lpfnWndProc = WndProc;
//	wc.cbClsExtra = 0;
//	wc.cbWndExtra = 0;
//	wc.hInstance = hInstance;
//	wc.hIcon = nullptr;
//	wc.hCursor = nullptr;
//	wc.hbrBackground = nullptr;
//	wc.lpszMenuName = nullptr;
//	wc.lpszClassName = pClassName;
//	wc.hIconSm = nullptr;
//
//	RegisterClassEx(&wc);
//
//
//	// Create an instance of the window
//	HWND hWnd = CreateWindowEx(
//		0,
//		pClassName,
//		L"The Window",
//		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
//		200, 200, 640, 480,
//		nullptr,
//		nullptr,
//		hInstance,
//		nullptr
//	);
//
//	// Show the window
//	ShowWindow(hWnd, SW_SHOW);
//
//	// Message pump
//	MSG msg;
//
//	BOOL gResult;
//
//	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
//	{
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//	}
//	if (gResult == -1)
//	{
//		return -1;
//	}
//	else
//	{
//		return (int)msg.wParam;
//	}
//
//	return 0;
//}