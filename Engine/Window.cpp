#include "stdafx.h"
#include "Window.h"
#include "WindowException.h"
#include "Resource.h"

// WindowClass
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() 
	: hInst(GetModuleHandle(nullptr)) // GetModuleHandle to save instance
{
	// Register a window class
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 64, 64, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));

	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	// Unregister the window class
	UnregisterClass(wndClassName, GetInstance());
}

const wchar_t* Window::WindowClass::GetName()
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance()
{
	return wndClass.hInst;
}

//------------------------------------------------------------------
// Window
Window::Window(int width, int height, const wchar_t* name) 
	: mWidth(width)
	, mHeight(height)
{
	mKeyboard = new Keyboard();
	mMouse = new Mouse();

	// Calculates the window size based on client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	// AdjustWindowRect
	if (FALSE == (AdjustWindowRect(&wr, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, FALSE)))
	{
		throw WND_LAST_EXCEPT();
	}

	// Create window and get hWnd
	hWnd = CreateWindowEx(
		0,
		WindowClass::GetName(),
		name,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, (wr.right - wr.left), (wr.bottom - wr.top),
		nullptr,
		nullptr,
		WindowClass::GetInstance(),
		this);
	// Check for errror
	if (hWnd == nullptr)
	{
		throw WND_LAST_EXCEPT();
	}

	// Show the window
	ShowWindow(hWnd, SW_SHOWDEFAULT);

	// Create Graphics object
	mGraphics = std::make_unique<Graphics>(hWnd);
}

Window::~Window()
{
	delete mKeyboard;
	delete mMouse;
	// Destroys window
	DestroyWindow(hWnd);
}

void Window::SetTitle(const std::wstring& title)
{
	if (SetWindowText(hWnd, title.c_str()) == FALSE)
	{
		throw WND_LAST_EXCEPT();
	}
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// Set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// Set message proc to normal handler
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// Forward message to window class handler
		return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
	}
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Pointer to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// Forward the message to window class handler
	return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// Post quit message when user exits program
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
		// Clear keystate when the window loses focus to prevent zombie inputs
		case WM_KILLFOCUS:
			mKeyboard->ClearState();
			break;

		// -------KEYBOARD MESSAGES------- // 
		case WM_KEYDOWN:
		// Use SYSKEY to handle system keys as well
		case WM_SYSKEYDOWN:
			if (!(lParam & 0x40000000) || mKeyboard->AutorepeatIsEnabled())
			{
				mKeyboard->OnKeyPress(static_cast<unsigned int>(wParam));
			}
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			mKeyboard->OnKeyRelease(static_cast<unsigned int>(wParam));
			break;
		case WM_CHAR:
			mKeyboard->OnChar(static_cast<char>(wParam));
			break;

		// -------MOUSE MESSAGES------- //
		case WM_MOUSEMOVE:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			// If cursor is in client region, log the move event
			if (pt.x >= 0 && pt.x <= mWidth && pt.y >= 0 && pt.y <= mHeight)
			{
				mMouse->OnMouseMove(pt.x, pt.y);
				// If current mouse state is not in window, capture mouse and set on enter
				SetCapture(hWnd);
				mMouse->OnMouseEnter();
			}
			// If cursor is not in client, log the move and maintain capture if a click is held (dragging outside of window)
			else
			{
				if (wParam & (MK_LBUTTON | MK_RBUTTON))
				{
					mMouse->OnMouseMove(pt.x, pt.y);
				}
				// Button up or no clock, release capture and log event for leaving
				else
				{
					ReleaseCapture();
					mMouse->OnMouseLeave();
				}
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mMouse->OnLeftClick(pt.x, pt.y);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mMouse->OnRightClick(pt.x, pt.y);
			break;
		}
		case WM_LBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mMouse->OnLeftRelease(pt.x, pt.y);
			break;
		}
		case WM_RBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mMouse->OnRightRelease(pt.x, pt.y);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			// Get how much change per scroll
			const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			// Pass in mouse pos and scroll change to OnWheelDelta
			mMouse->OnWheelDelta(pt.x, pt.y, delta);
			break;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}