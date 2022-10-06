#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sstream>

const char* APP_TITLE = "Introduction to Modern OpenGL - Hello Triangle";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
bool gFullScreen = false;
GLFWwindow* gWindow = NULL;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void showFPS(GLFWwindow* window);
bool initOpenGL();
bool gWireframe = false;

const GLchar* vertexShaderSrc =
// what shader model to use
"#version 330 core\n"
"layout (location = 0) in vec3 pos;"
"layout (location = 1) in vec3 color;"
"out vec3 vert_color;"
"void main()"
"{"
"	vert_color = color;"
"	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"
"}";

const GLchar* fragmentShaderSrc =
"#version 330 core\n"
"in vec3 vert_color;"
"out vec4 frag_color;"
"void main()"
"{"
"	frag_color = vec4(vert_color, 1.0f);"
"}";


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
		0.0f,  0.5f, 0.0f,	1.0f, 0.0f, 0.0f,// Top
		0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,// Right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f // Left
	};

	// Vertex buffer object
	GLuint vbo;

	// Vertex array object
	GLuint vao;
	// Create memory in the graphics card
	glGenBuffers(1, &vbo);
	// Makes buffer the current one. Only one at a time.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Fill buffer with data from vert_pos. Static Draw: The data store contents will be modified once and used many times.
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// INTERVEAVED BUFFER EXAMPLE
	// position
	//stride: amount of bytes between each vertex. pointer: amount of bytes between attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, 0);
	glEnableVertexAttribArray(0);

	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);
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
	GLuint vbo, vbo2;

	// Vertex array object
	GLuint vao;
	// Create memory in the graphics card
	glGenBuffers(1, &vbo);

	// POSITION
	// Makes buffer the current one. Only one at a time.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Fill buffer with data from vert_pos. Static Draw: The data store contents will be modified once and used many times.
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vert_pos, GL_STATIC_DRAW);

	// COLOR
	// Create memory in the graphics card
	glGenBuffers(1, &vbo2);
	// Makes buffer the current one. Only one at a time.
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	// Fill buffer with data from vert_pos. Static Draw: The data store contents will be modified once and used many times.
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vert_color, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// POSITION
	//stride: amount of bytes between each vertex. pointer: amount of bytes between attribute
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// COLOR
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
#endif // 0

#pragma endregion

#pragma region Element Buffer Layout
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
		"void main()"
		"{"
		"	frag_color = vec4(.35f, .96f, .3f, 1.0f);"
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
	GLuint vbo, ebo;

	// Vertex array object
	GLuint vao;
	// Create memory in the graphics card
	glGenBuffers(1, &vbo);
	// Makes buffer the current one. Only one at a time.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Fill buffer with data from vert_pos. Static Draw: The data store contents will be modified once and used many times.
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create memory in the graphics card
	glGenBuffers(1, &ebo);
	// Makes buffer the current one. Only one at a time.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	// Fill buffer with data from vert_pos. Static Draw: The data store contents will be modified once and used many times.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);


	//stride: amount of bytes between each vertex. pointer: amount of bytes between attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

#endif // 0

#pragma endregion

#pragma region Index Buffer
#if 0
	float vertices[] = {
		// first triangle
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
		// second triangle
		 0.0f, -0.5f, 0.0f,  // left
		 0.9f, -0.5f, 0.0f,  // right
		 0.45f, 0.5f, 0.0f   // top 
	};

	unsigned int vbo, vao;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
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

	//Create vertex shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	// use our vertex shader source code
	glShaderSource(vs, 1, &vertexShaderSrc, NULL);
	// compile our vertex shader
	glCompileShader(vs);

	GLint result;
	GLchar infoLog[512];
	glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
		std::cerr << "Error! Vertex shader failed to compile. " << infoLog << std::endl;
	}

	//Create fragment shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	// use our fragment shader source code
	glShaderSource(fs, 1, &fragmentShaderSrc, NULL);
	// compile our fragment shader
	glCompileShader(fs);

	glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
		std::cerr << "Error! Fragment shader failed to compile. " << infoLog << std::endl;
	}

	// Create program for our shader
	GLuint shaderProgram = glCreateProgram();
	// Attach our shaders to the program
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		std::cerr << "Error! Shader Program linker failed. " << infoLog << std::endl;
	}

	// Delete already linked shaders
	glDeleteShader(vs);
	glDeleteShader(fs);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(gWindow))
	{
		showFPS(gWindow);
		/* Render here */
		glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindVertexArray(vao);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);


		/* Swap front and back buffers. Front is being displayed, back is currently being drawn */
		glfwSwapBuffers(gWindow);

		/* Poll for and process kb/m events */
		glfwPollEvents();
	}


	// Delete the program before exiting
	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	//glDeleteBuffers(1, &ebo);

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
			<< "Frame Time: " << msPerFrame << " (ms)";

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