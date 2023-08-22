#include<glad/glad.h>
#include<glfw/glfw3.h>

#include<ShaderH.h>
#include<iostream>
#include<cmath>
#include<CubeRender.h>



const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 480;

void ProcessInputs(GLFWwindow* window);
float ConvertToRGB(float val);

//STARTING POINT OF OUR PROGRAM
int main()
{
	//INITIALIZE GLFW LIB's AND CONFIG.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"MYFIRSTWINDOW",NULL,NULL);
	if (window == NULL)
	{
		std::cout << "FAILED TO CREATE THE WINDOW! " << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//INITIALIZING GLAD FUNCTIONS
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "FAILED TO INITIALIZE GLAD!" << std::endl;
		glfwTerminate();
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	InitCubeVerts(0.0,0.0f,0.7f);
	//RENDER LOOP
	while (!glfwWindowShouldClose(window))
	{
		ProcessInputs(window);

	
		glClearColor(0.2f,0.2f,0.2f,1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		RenderCube("vertex.shader","fragment.shader");

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	DeleteCubeData();

	glfwTerminate();
	return 0;
}

float ConvertToRGB(float val)
{
	return (val / 255.0f);
}

void ProcessInputs(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window,true);
	}
}