#include "GLWindow/GLWindow.h"
#include <stdio.h>

// Example of multiple windows being created and input being tested with the first window
//
//
int main()
{
	GLWindow window = GLWindow();
	GLWindow window2 = GLWindow();
	GLWindow::Event event;
	GLWindow::Event event2;
	
	// Get messages/events intended for each window
	while (event.isActive || event2.isActive)
	{
		// Do Stuff
		if (event.isActive) { event = window.PollEvent(); }
		if (event2.isActive) { event2 = window2.PollEvent(); }
		
		if (event.Type() == GLWindow::EventType::GLMouseMoveEvent)
		{
			GLWindow::MouseMoveEvent moveEvent = event.MouseMoveEvent();
			printf("%i %i\n", moveEvent.x, moveEvent.y);
		}
		
		if (event.Type() == GLWindow::EventType::GLMouseButtonEvent)
		{
			GLWindow::MouseButtonEvent buttonEvent = event.MouseButtonEvent();
			printf("%i %i\n", buttonEvent.button, buttonEvent.isPressed);
		}
	}
	
	return 0;
}