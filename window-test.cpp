#include "GLWindow/GLWindow.h"
#include <stdio.h>

// Example of multiple windows being created and input being tested with the first window
// Compile Command: g++ window-test.cpp GLWindow/GLWindow.cpp -lopengl32 -lgdi32 -o bin/test.exe
//
int main()
{
	GLWindow window = GLWindow();
	GLWindow window2 = GLWindow();
	GLWindow::Event event;
	GLWindow::Event event2;
	
	window.Show();
	window2.Show();
	
	// Get messages/events intended for each window
	while (event.isActive || event2.isActive)
	{
		// Do Stuff
		if (event.isActive) { event = window.PollEvent(); }
		if (event2.isActive) { event2 = window2.PollEvent(); }
		
		if (event.Type() == GLWindow::MouseMoveEventType)
		{
			GLWindow::MouseMoveEvent moveEvent = event.MouseMoveEvent();
			//printf("%i %i\n", moveEvent.x, moveEvent.y);
		}
		
		if (event.Type() == GLWindow::MouseButtonEventType)
		{
			GLWindow::MouseButtonEvent buttonEvent = event.MouseButtonEvent();
			//printf("%i %i\n", buttonEvent.button, buttonEvent.isPressed);
		}
		
		//printf("%i %i\n", event.isActive, event2.isActive);
	}
	
	return 0;
}