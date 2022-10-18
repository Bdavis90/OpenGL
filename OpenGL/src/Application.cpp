#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <fstream>
#include <string>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"


const char* APP_TITLE = "Introduction to Modern OpenGL - Hello Shader";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
bool gFullScreen = false;
GLFWwindow* gWindow = NULL;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void showFPS(GLFWwindow* window);
bool initOpenGL();
bool gWireframe = false;


int main(void)
{
	if (!initOpenGL())
	{
		std::cerr << "GLFW initalization failed" << std::endl;
		return -1;
	}


#pragma region Interleaved Buffer Layout

#if 1

	GLfloat vertices[] = {
		// position			// color
		0.0f,  0.5f, 0.0f,	//1.0f, 0.0f, 0.0f,// Top
		0.5f, -0.5f, 0.0f,  //0.0f, 1.0f, 0.0f,// Right
		-0.5f, -0.5f, 0.0f, //0.0f, 0.0f, 1.0f // Left
	};

	// Vertex buffer object
	VertexBuffer vb(vertices, 9 * sizeof(GLfloat));

	// Vertex array object
	VertexArray va;
	VertexBufferLayout layout;
	layout.Push<GLfloat>(3);
	va.AddBuffer(vb, layout);

#endif // 0

#pragma endregion 

#pragma region Separate Buffer Layout
#if 0
	GLfloat vert_pos[] = {
		0.0f,  0.5f, 0.0f, // Top
		0.5f, -0.5f, 0.0f, // Right
		-0.5f, -0.5f, 0.0f, // Left
	};

	GLfloat vert_color[] = {
		1.0f, 0.0f, 0.0f, // Top
		0.0f, 1.0f, 0.0f, // Right
		0.0f, 0.0f, 1.0f  // Left
	};

	// Vertex buffer object
	VertexBuffer vbo(vert_pos, 9 * sizeof(GLfloat)), vbo2(vert_color, 9 * sizeof(GLfloat));

	// Vertex array object
	GLuint vao;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// POSITION
	//stride: amount of bytes between each vertex. pointer: amount of bytes between attribute
	vbo.Bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// COLOR
	vbo2.Bind();
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
#endif // 0

#pragma endregion

#pragma region Element/Index Buffer Layout
#if 0
	const GLchar* vertexShaderSrc =
		// what shader model to use
		"#version 330 core\n"
		"layout (location = 0) in vec3 pos;"
		"void main()"
		"{"
		"	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"
		"}";

	const GLchar* fragmentShaderSrc =
		"#version 330 core\n"
		"out vec4 frag_color;"
		"uniform vec4 u_Color;"
		"void main()"
		"{"
		"	frag_color = u_Color;"
		"}";

	GLfloat vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	GLuint indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// Vertex buffer object
	VertexBuffer vbo(vertices, 12 * sizeof(GLfloat));
	IndexBuffer ib(indices, 6);

	// Vertex array object
	VertexArray va;
	VertexBufferLayout layout;
	layout.Push<GLfloat>(3);
	va.AddBuffer(vbo, layout);

	//GLCall(glGenVertexArrays(1, &vao));
	//GLCall(glBindVertexArray(vao));

	////stride: amount of bytes between each vertex. pointer: amount of bytes between attribute
	//GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0));
	//GLCall(glEnableVertexAttribArray(0));

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, &vertexShaderSrc, 0);
	glShaderSource(fs, 1, &fragmentShaderSrc, 0);

	glCompileShader(vs);
	glCompileShader(fs);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);

#endif // 0

#pragma endregion

#pragma region Side by side Triangle
#if 0
	GLfloat vertices[] = {
		// first triangle
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
		// second triangle
		 0.0f, -0.5f, 0.0f,  // left
		 0.9f, -0.5f, 0.0f,  // right
		 0.45f, 0.5f, 0.0f   // top 
	};

	VertexBuffer vbo(vertices, 3 * 3 * 2 * sizeof(GLfloat));
	GLuint vao;

	glGenVertexArrays(1, &vao);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(vao);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered vbo as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the vao afterwards so other vao calls won't accidentally modify this vao, but this rarely happens. Modifying other
	// vaos requires a call to glBindVertexArray anyways so we generally don't unbind vaos (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

#endif // 0

#pragma endregion



	Shader shader("res/shaders/Basic.shader");
	shader.Bind();
	shader.SetUniform4f("u_Color", .08f, .3f, .8f, 1.f);
	shader.Unbind();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(gWindow))
	{
		showFPS(gWindow);
		/* Render here */
		glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.Bind();
		va.Bind();
		//ib.Bind();
		//GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
		va.Unbind();
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
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(gWindow);
	// Turn off vsync
	glfwSwapInterval(0);

	// Handles keyboard inputs 
	glfwSetKeyCallback(gWindow, glfw_onKey);
	glfwSetFramebufferSizeCallback(gWindow, framebuffer_size_callback);

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

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		gWireframe = !gWireframe;
		if (gWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
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
			<< " Frame Time: " << msPerFrame << " (ms)";

		glfwSetWindowTitle(window, outs.str().c_str());

		frameCount = 0;
	}

	frameCount++;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
