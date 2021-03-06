#include "GLWindow.h"
//#include <stdio.h>

LRESULT CALLBACK GLWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

GLWindow::Window::Window()
{
	exists = false;
}

void GLWindow::Window::create(unsigned int width, unsigned int height, const char * title)
{
	if (exists) { return; }
	
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
	
	if (!RegisterClass(&wClass)) { return; }
	
	// Create and show window
	hWnd = CreateWindow(wClass.lpszClassName, title, WS_OVERLAPPEDWINDOW, 100, 100, width, height, (HWND)NULL, NULL, hInstance, NULL);
	hdc = GetDC(hWnd);
	
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR), //  size of this pfd
		1, // version number
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // support window, OpenGL, double buffered
		PFD_TYPE_RGBA, // RGBA type
		24, // 24-bit color depth
		0, 0, 0, 0, 0, 0, // color bits ignored
		0, // no alpha buffer
		0, // shift bit ignored
		0, // no accumulation buffer
		0, 0, 0, 0, // accum bits ignored
		32, // 32-bit z-buffer
		0, // no stencil buffer
		0, // no auxiliary buffer
		PFD_MAIN_PLANE, // main layer
		0, // reserved
		0, 0, 0 // layer masks ignored
	};
	
	int iPixelFormat; 
	
	iPixelFormat = ChoosePixelFormat(hdc, &pfd);
	
	SetPixelFormat(hdc, iPixelFormat, &pfd);
	
	glContext = wglCreateContext(hdc);
	
	lastEvent.isActive = true;
	exists = true;
}

void GLWindow::Window::destroy()
{
	if (exists)
	{
		if (wglGetCurrentContext() == glContext) { wglMakeCurrent(NULL, NULL); }
		wglDeleteContext(glContext);
		DestroyWindow(hWnd);
		lastEvent.isActive = false;
		exists = false;
	}
}

void GLWindow::Window::show()
{
	if (exists)
	{
		ShowWindow(hWnd, SW_SHOWDEFAULT);
	}
}

void GLWindow::Window::hide()
{
	if (exists)
	{
		ShowWindow(hWnd, SW_HIDE);
	}
}

void GLWindow::Window::swapGLBuffers()
{
	if (exists)
	{
		SwapBuffers(hdc);
	}
}

void GLWindow::Window::makeGLContextCurrent()
{
	if (exists)
	{
		wglMakeCurrent(hdc, glContext);
	}
}

bool GLWindow::Window::pollEvent()
{
	lastEvent.type = GLWindow::EventEmptyType;
	if (!exists) { return false; }
	
	if (IsWindowVisible(hWnd) > 0) {
	} else { lastEvent.isActive = false; }
	
	// Get messages intended for this window
	if (PeekMessage(&lastEvent.msg, hWnd, 0, 0, PM_REMOVE) > 0)
	{
		// Find when window close request is made
		if (IsWindowVisible(hWnd) > 0)
		{
			switch (lastEvent.msg.message)
			{
				case WM_MOUSEMOVE:
					lastEvent.type = GLWindow::MouseMoveEventType;
				break;
				case WM_LBUTTONUP:
				case WM_MBUTTONUP:
				case WM_RBUTTONUP:
				case WM_LBUTTONDOWN:
				case WM_MBUTTONDOWN:
				case WM_RBUTTONDOWN:
					lastEvent.type = GLWindow::MouseButtonEventType;
				break;
			}
		} else { lastEvent.isActive = false; }
		
		// Translate keys with modifiers
		TranslateMessage(&lastEvent.msg);
		
		// Send message to message callback proceedure (GLWindowProc)
		DispatchMessage(&lastEvent.msg);
	}
	
	//if (event.msg.message != WM_PAINT) { printf("%i %i\n", event.msg.hwnd, event.msg.message); }
	
	return lastEvent.isActive;
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
	
	if (type == GLWindow::MouseMoveEventType)
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
	
	if (type == GLWindow::MouseButtonEventType)
	{
		event.isValid = true;
		event.x = GET_X_LPARAM(msg.lParam);
		event.y = GET_Y_LPARAM(msg.lParam);
		
		switch (msg.message)
		{
			case WM_LBUTTONUP: event.button = GLWindow::MouseButtonLeft; event.isPressed = false; break;
			case WM_MBUTTONUP: event.button = GLWindow::MouseButtonMiddle; event.isPressed = false; break;
			case WM_RBUTTONUP: event.button = GLWindow::MouseButtonRight; event.isPressed = false; break;
			case WM_LBUTTONDOWN: event.button = GLWindow::MouseButtonLeft; event.isPressed = true; break;
			case WM_MBUTTONDOWN: event.button = GLWindow::MouseButtonMiddle; event.isPressed = true; break;
			case WM_RBUTTONDOWN: event.button = GLWindow::MouseButtonRight; event.isPressed = true; break;
		}
	} else { event.isValid = false; }
	
	return event;
}

LRESULT CALLBACK GLWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// When user requests to close the window
	if (msg == WM_CLOSE)
	{
		// Don't need to destroy the window as this happens when the GLWindow instance is destroyed
		ShowWindow(hWnd, SW_HIDE);
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