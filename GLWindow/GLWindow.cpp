#include "GLWindow.h"
//#include <stdio.h>

LRESULT CALLBACK GLWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

GLWindow::GLWindow()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	
	// Register Window Class
	// CS_OWNDC - Make sure class has it's own context we can bind to opengl with
	// CS_HREDRAW | CS_VREDRAW - redraw screen every time window is resized
	wClass = WNDCLASS{};
	wClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;
	wClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hInstance = hInstance;
	wClass.lpfnWndProc = GLWindowProc;
	wClass.lpszMenuName = NULL;
	wClass.lpszClassName = "GLWindowClass";
	
	if (!RegisterClass(&wClass)) { /*return;*/ }
	
	// Create and show window
	hWnd = CreateWindow(wClass.lpszClassName, "Window Title", WS_OVERLAPPED, 100, 100, 800, 600, (HWND)NULL, NULL, hInstance, NULL);
	
	ShowWindow(hWnd, SW_NORMAL);
}

GLWindow::~GLWindow()
{
	DestroyWindow(hWnd);
}

GLWindow::Event GLWindow::PollEvent()
{
	GLWindow::Event event;
	event.type = GLWindow::EventType::GLEventEmpty;
	
	// Get messages intended for this window
	if (PeekMessage(&event.msg, hWnd, 0, 0, PM_REMOVE) > 0)
	{
		// Find when window close request is made
		if (event.msg.message == WM_CLOSE)
		{
			event.isActive = false;
			DestroyWindow(hWnd);
		} else {
			switch (event.msg.message)
			{
				case WM_MOUSEMOVE:
					event.type = GLWindow::EventType::GLMouseMoveEvent;
				break;
				case WM_LBUTTONUP:
				case WM_MBUTTONUP:
				case WM_RBUTTONUP:
				case WM_LBUTTONDOWN:
				case WM_MBUTTONDOWN:
				case WM_RBUTTONDOWN:
					event.type = GLWindow::EventType::GLMouseButtonEvent;
				break;
			}
		}
		
		// Translate keys with modifiers
		TranslateMessage(&event.msg);
		
		// Send message to message callback proceedure (GLWindowProc)
		DispatchMessage(&event.msg);
	}
	
	//if (msg.message != WM_PAINT) { printf("%i %i\n", msg.hwnd, msg.message); }
	
	return event;
}

GLWindow::Event::Event()
{
	isActive = true;
}

GLWindow::EventType GLWindow::Event::Type()
{
	return type;
}

GLWindow::MouseMoveEvent GLWindow::Event::MouseMoveEvent()
{
	GLWindow::MouseMoveEvent event;
	
	if (type == GLWindow::EventType::GLMouseMoveEvent)
	{
		event.isValid = true;
		event.x = GET_X_LPARAM(msg.lParam);
		event.y = GET_Y_LPARAM(msg.lParam);
	} else { event.isValid = false; }
	
	return event;
}

GLWindow::MouseButtonEvent GLWindow::Event::MouseButtonEvent()
{
	GLWindow::MouseButtonEvent event;
	
	if (type == GLWindow::EventType::GLMouseButtonEvent)
	{
		event.isValid = true;
		event.x = GET_X_LPARAM(msg.lParam);
		event.y = GET_Y_LPARAM(msg.lParam);
		
		switch (msg.message)
		{
			case WM_LBUTTONUP: event.button = GLWindow::MouseButton::MBLeft; event.isPressed = false; break;
			case WM_MBUTTONUP: event.button = GLWindow::MouseButton::MBMiddle; event.isPressed = false; break;
			case WM_RBUTTONUP: event.button = GLWindow::MouseButton::MBRight; event.isPressed = false; break;
			case WM_LBUTTONDOWN: event.button = GLWindow::MouseButton::MBLeft; event.isPressed = true; break;
			case WM_MBUTTONDOWN: event.button = GLWindow::MouseButton::MBMiddle; event.isPressed = true; break;
			case WM_RBUTTONDOWN: event.button = GLWindow::MouseButton::MBRight; event.isPressed = true; break;
		}
	} else { event.isValid = false; }
	
	return event;
}

LRESULT CALLBACK GLWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// When user requests to close the window
	if (msg == WM_CLOSE)
	{
		// Send WM_CLOSE to be handled by PeekMessage above
		// For whatever reason this message is not initally passed to the above, need to do this manually
		// Don't need to destroy the window as this happens in the above
		PostMessage(hWnd, WM_CLOSE, wParam, lParam);
		return 0;
	}
	
	// When the window get's destroyed
	if (msg == WM_DESTROY)
	{
		// We don't actually need to exit the process at this point
		// Don't call PostQuitMessage(0) unless we need to end the process
		return 0;
	}
	
	if (msg == WM_PAINT)
	{
		return 0;
	}
	
	return DefWindowProc(hWnd, msg, wParam, lParam);
}