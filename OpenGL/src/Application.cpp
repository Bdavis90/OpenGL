#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sstream>

const char* APP_TITLE = "Introduction to Modern OpenGL - Hello Window 1";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
bool gFullScreen = false;
GLFWwindow* gWindow = NULL;

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void showFPS(GLFWwindow* window);
bool initOpenGL();
int main(void)
{
	if (!initOpenGL())
	{
		std::cerr << "GLFW initalization failed" << std::endl;
		return -1;
	}

	GLfloat vertices[] = {
		0.0f,  0.5f, 0.0f, // Top
		0.5f, -0.5f, 0.0f, // Right
		-0.5f, -0.5f, 0.0f // Left
	};

	// Vertex buffer object
	GLuint vbo;
	// Create memory in the graphics card
	glGenBuffers(1, &vbo);
	// Makes buffer the current one. Only one at a time.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Fill buffer with data from vertices
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	std::cout << glGetString(GL_VERSION) << std::endl;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(gWindow))
	{
		showFPS(gWindow);
		/* Render here */
		glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glBegin(GL_TRIANGLES);
		//glVertex2f(-0.5f, -0.5f);
		//glVertex2f(0.0f, 0.5f);
		//glVertex2f(0.5f, -0.5f);
		//glEnd();

		/* Swap front and back buffers. Front is being displayed, back is currently being drawn */
		glfwSwapBuffers(gWindow);

		/* Poll for and process kb/m events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

bool initOpenGL()
{

	/* Initialize the library */
	if (!glfwInit())
		return false;

	// Set min OpenGL Version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Force to create modern core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (gFullScreen)
	{
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* pVmode = glfwGetVideoMode(pMonitor);
		if (pVmode)
		{
			gWindow = glfwCreateWindow(pVmode->width, pVmode->height, APP_TITLE, pMonitor, NULL);
		}
	}
	else
	{
		/* Create a windowed mode window and its OpenGL context */
		gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);

	}

	if (!gWindow)
	{
		glfwTerminate();
		return false;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(gWindow);

	glfwSetKeyCallback(gWindow, glfw_onKey);

	glewExperimental = GL_TRUE;
	// Initialize glew after window context
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!" << std::endl;
		return false;
	}

	return true;
}

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void showFPS(GLFWwindow* window)
{
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime(); // returns number of seconds since GLFW started, as a double

	elapsedSeconds = currentSeconds - previousSeconds;

	// limit text update 4 times per second

	if (elapsedSeconds > .25)
	{
		previousSeconds = currentSeconds;
		double fps = frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		std::ostringstream outs;
		outs.precision(3);
		outs << std::fixed << APP_TITLE << "   " << "FPS: " << fps
			<< "Frame Time: " << msPerFrame << " (ms)";

		glfwSetWindowTitle(window, outs.str().c_str());

		frameCount = 0;
	}

	frameCount++;
}