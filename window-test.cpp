#include "GLWindow/GLWindow.h"
#include <stdio.h>

int main()
{
	GLWindow window = GLWindow();
	GLWindow window2 = GLWindow();
	GLWindow::Event event{isActive: true};
	GLWindow::Event event2{isActive: true};
	
	// Get messages intended for the running process
	while (event.isActive || event2.isActive)
	{
		// Do Stuff
		if (event.isActive) { event = window.PollEvent(); }
		if (event2.isActive) { event2 = window2.PollEvent(); }
	}
	
	return 0;
}