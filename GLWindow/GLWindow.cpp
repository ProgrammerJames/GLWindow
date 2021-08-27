#include "GLWindow.h"
#include <stdio.h>

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
	wClass.lpszClassName = "TestWindowClassName";
	
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
	GLWindow::Event event{isActive: true};
	MSG msg;
	
	// Get messages intended for this window
	if (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE) > 0)
	{
		// Find when window close request is made
		if (msg.message == WM_CLOSE)
		{
			event.isActive = false;
			DestroyWindow(hWnd);
		}
		
		// Translate keys with modifiers
		TranslateMessage(&msg);
		
		// Send message to message window handle
		DispatchMessage(&msg);
	}
	
	//if (msg.message != WM_PAINT) { printf("%i %i\n", msg.hwnd, msg.message); }
	
	return event;
}

LRESULT CALLBACK GLWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// When user requests to close the window
	if (msg == WM_CLOSE)
	{
		// Send WM_CLOSE to be handled by PeekMessage above
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