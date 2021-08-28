#ifndef GL_WINDOW
#define GL_WINDOW

#include <windows.h>
#include <windowsx.h>
#include <gl/gl.h>

// #if defined(WIN32) || defined(__WIN32) || defined(__WIN32__)

class GLWindow
{
	private:
		HDC hdc;
		HWND hWnd;
		WNDCLASS wClass;
		HGLRC glContext;
	
	public:
		enum EventType { EventEmptyType, MouseMoveEventType, MouseButtonEventType };
		enum MouseButton { MouseButtonLeft, MouseButtonMiddle, MouseButtonRight };
		
		class Event;
		
		struct KeyPressEvent;
		struct MouseMoveEvent;
		struct MouseButtonEvent;
		
		GLWindow();
		~GLWindow();
		
		void Show();
		void Hide();
		
		void SwapGLBuffers();
		
		void MakeGLContextCurrent();
		
		GLWindow::Event PollEvent();
};

class GLWindow::Event
{
	protected:
		MSG msg;
		GLWindow::EventType type;
		
		friend class GLWindow;
		
	public:
		Event();
		
		bool isActive;
		GLWindow::EventType Type();
		
		GLWindow::MouseMoveEvent MouseMoveEvent();
		GLWindow::MouseButtonEvent MouseButtonEvent();
};

struct GLWindow::MouseMoveEvent
{
	bool isValid;
	int x, y;
};

struct GLWindow::MouseButtonEvent
{
	bool isValid, isPressed;
	int x, y;
	GLWindow::MouseButton button;
};

struct GLWindow::KeyPressEvent
{
	//
};

#endif