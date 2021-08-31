#include "GLWindow/GLWindow.h"
#include <stdio.h>

// Example of multiple windows being created and input being tested with the first window
// Compile Command: g++ window-test.cpp GLWindow/GLWindow.cpp -lopengl32 -lgdi32 -o bin/test.exe
//
int main()
{
	GLWindow::Window window = GLWindow::Window();
	
	window.create(800, 600, "Window Test");
	window.show();
	
	window.makeGLContextCurrent();
	
	glClearColor(0.f, 0.1f, 0.25f, 1.f);
	
	// Get messages/events intended for each window
	while (window.lastEvent.isActive)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Do Stuff
		window.pollEvent();
		
		if (window.lastEvent.Type() == GLWindow::MouseMoveEventType)
		{
			GLWindow::MouseMoveEvent moveEvent = window.lastEvent.MouseMoveEvent();
			//printf("%i %i\n", moveEvent.x, moveEvent.y);
		}
		
		if (window.lastEvent.Type() == GLWindow::MouseButtonEventType)
		{
			GLWindow::MouseButtonEvent buttonEvent = window.lastEvent.MouseButtonEvent();
			//printf("%i %i\n", buttonEvent.button, buttonEvent.isPressed);
		}
		
		window.swapGLBuffers();
		//printf("%i %i\n", event.isActive, event2.isActive);
	}
	
	window.destroy();
	
	return 0;
}