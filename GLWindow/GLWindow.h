#ifndef GL_WINDOW
#define GL_WINDOW

#include <windows.h>

// #if defined(WIN32) || defined(__WIN32) || defined(__WIN32__)

class GLWindow
{
	private:
		HWND hWnd;
		WNDCLASS wClass;
	
	public:
		struct Event;
		
		GLWindow();
		~GLWindow();
		
		GLWindow::Event PollEvent();
};

struct GLWindow::Event
{
	bool isActive;
};

#endif