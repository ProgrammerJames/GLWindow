#ifndef GL_WINDOW
#define GL_WINDOW

#include <windows.h>
#include <windowsx.h>
#include <gl/gl.h>

// #if defined(WIN32) || defined(__WIN32) || defined(__WIN32__)

namespace GLWindow
{
	enum EventType { EventEmptyType, MouseMoveEventType, MouseButtonEventType };
	enum MouseButton { MouseButtonLeft, MouseButtonMiddle, MouseButtonRight };
	
	class Window;
	
	class Event;
	
	struct KeyPressEvent;
	struct MouseMoveEvent;
	struct MouseButtonEvent;
};

class GLWindow::Event
{
	protected:
		MSG msg;
		GLWindow::EventType type;
		
		friend class GLWindow::Window;
		
	public:
		Event();
		
		bool isActive;
		GLWindow::EventType Type();
		
		GLWindow::MouseMoveEvent MouseMoveEvent();
		GLWindow::MouseButtonEvent MouseButtonEvent();
};

class GLWindow::Window
{
	private:
		bool exists;
		HDC hdc;
		HWND hWnd;
		WNDCLASS wClass;
		HGLRC glContext;
	
	public:
		GLWindow::Event lastEvent;
	
		Window();
		void create(unsigned int w, unsigned int h, const char * t);
		void destroy();
		
		bool pollEvent();
		
		void show();
		void hide();
		void swapGLBuffers();
		void makeGLContextCurrent();
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