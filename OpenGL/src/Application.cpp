#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sstream>

const char* APP_TITLE = "Introduction to Modern OpenGL - Hello Triangle";
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

	// Vertex array object
	GLuint vao;
	// Create memory in the graphics card
	glGenBuffers(1, &vbo);
	// Makes buffer the current one. Only one at a time.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Fill buffer with data from vertices. Static Draw: The data store contents will be modified once and used many times.
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//stride: amount of bytes between each vertex. pointer: amount of bytes between attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(0);

	const GLchar* vertexShaderSrc = "#version 330 core\n" "layout (location = 0) in vec3 pos;";

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(gWindow))
	{
		showFPS(gWindow);
		/* Render here */
		glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);


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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Force to create modern core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (gFullScreen)
	{
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* pVmode = glfwGetVideoMode(pMonitor);
		if (pVmode)
		{
			/* Create a fullscreen mode window and its OpenGL context */
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
		std::cerr << "Failed to created GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(gWindow);

	// Handles keyboard inputs 
	auto a = glfwSetKeyCallback(gWindow, glfw_onKey);

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