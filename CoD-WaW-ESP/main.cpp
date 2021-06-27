#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <GLFW/glfw3.h>

#include "Offsets.h"
#include <MemMan.h>
#include "DataTypes.h"
#include "Helpers.h"

using namespace Zombies;

int main(void)
{
	GLFWwindow* Window;
	//ShowWindow(GetConsoleWindow(), SW_SHOW);

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_FLOATING, true);
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_MAXIMIZED, true);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
	glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, true);
	/* Create a windowed mode window and its OpenGL context */
	Window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
	if (!Window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetWindowAttrib(Window, GLFW_DECORATED, false);

	/* Make the window's context current */
	glfwMakeContextCurrent(Window);

	//Get Process
	uintptr_t Process = MemMan::GetProcess(L"cod5sp.exe");
	if (Process == 0)
	{
		std::cout << "Process is Invalid\n";
		return -1;
	}

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(Window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//Get View Matrix
		ViewMatrix Matrix = MemMan::ReadMem<ViewMatrix>(AViewMatrix);

		//Loop Through Entity List
		for (short int i = 0; i < 40; ++i)
		{
			uintptr_t Entity = MemMan::ReadMem<uintptr_t>(AEntityList + i * Offsets::ODistanceBetween);
			if (Entity == NULL) continue;

			//Get Entity Health to check if > 0
			int Health = MemMan::ReadMem<int>(Entity + Offsets::OHealth);
			if (Health <= 0) continue;

			int TestVal = MemMan::ReadMem<int>(Entity + 0x4);
			if (TestVal != 16) continue;

			//Get Entity Location
			Vec3 Location = MemMan::ReadMem<Vec3>(Entity + Offsets::OLocation);

			Vec2 ScreenCoords;
			//W2S Location
			if (!WorldToScreen(Location, ScreenCoords, Matrix.Matrix)) continue;

			//Draw To Screen
			glBegin(GL_LINES);
			glVertex2f(0.0f, -1.0f);
			glVertex2f(ScreenCoords.X, ScreenCoords.Y);
			glEnd();
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(Window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}