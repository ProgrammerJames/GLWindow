#ifndef GL_WINDOW
#define GL_WINDOW

#include <windows.h>
#include <windowsx.h>

// #if defined(WIN32) || defined(__WIN32) || defined(__WIN32__)

class GLWindow
{
	private:
		HWND hWnd;
		WNDCLASS wClass;
	
	public:
		enum EventType { GLEventEmpty, GLMouseMoveEvent, GLMouseButtonEvent };
		enum MouseButton { MBLeft, MBMiddle, MBRight };
		
		class Event;
		
		struct KeyPressEvent;
		struct MouseMoveEvent;
		struct MouseButtonEvent;
		
		GLWindow();
		~GLWindow();
		
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

struct GLWindow::KeyPressEvent
{
	//
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

#endif