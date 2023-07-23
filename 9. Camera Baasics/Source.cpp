#include<glad/glad.h>
#include<GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<ShaderH.h>
#include<Camera.h>

#include<iostream>
#include<cmath>

const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT= 640;

void ProcessInputs(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


int main()
{
	//USAGE of Opengl v3.3 with core profile
	//GLFW Initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"MCACGPractical1",NULL,NULL);
	if (window == NULL)
	{
		std::cout << "FAILED TO CREATE A WINDOW!";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window,mouse_callback);
	glfwSetScrollCallback(window,scroll_callback);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "FAILED TO INITIALIZE GLAD!" << std::endl;
		return -1;
	}


	/// <summary> // NDC Coordinates - [-1,1]
	/// 9 float values|| 4 bytes || 9 x 4 bytes = 36 bytes
	/// </summary>
	/// <returns></returns>
	 float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

	 glm::vec3 cubePositions[] = {
		 glm::vec3(0.0f,  0.0f,  0.0f),
		 glm::vec3(2.0f,  5.0f, -15.0f),
		 glm::vec3(-1.5f, -2.2f, -2.5f),
		 glm::vec3(-3.8f, -2.0f, -12.3f),
		 glm::vec3(2.4f, -0.4f, -3.5f),
		 glm::vec3(-1.7f,  3.0f, -7.5f),
		 glm::vec3(1.3f, -2.0f, -2.5f),
		 glm::vec3(1.5f,  2.0f, -2.5f),
		 glm::vec3(1.5f,  0.2f, -1.5f),
		 glm::vec3(-1.3f,  1.0f, -1.5f)
	 };
	
	//LOADING AND APPLYING A TEXTURE TO A PRIMITIVE
	// 1. DECLARING UV/TEXTURE COORDINATES
	// 2. GENERATE THE TEXTURE BUFFER
	// 3. SET TEXTURE WRAPPING OPTIONS
	// 4. SET TEXTURE FILTERING OPTIONS
	// 5. LOADING THE TEXTURE INTO ENVIRONMENT USING STB_IMAGE.H FUNCTIONS
	// 6. GENERATE MIPMAPS 
	//Enable the depth testing
	glEnable(GL_DEPTH_TEST);
	Shader myShader("vertex.shader","fragment.shader");

	unsigned int VBO, VAO; // EBO - ELEMENT BUFFER OBJECT
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1,&VBO);
	

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	
	
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//LOADING TEXTURE
	unsigned int wallTexture;
	glGenTextures(1,&wallTexture);
	glBindTexture(GL_TEXTURE_2D,wallTexture);
	//Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Set the texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	//Loading the image byte data
	int img_Width, img_Height, clrChannels;

	unsigned char* imgData = stbi_load("wallTex.jpg",&img_Width,&img_Height,&clrChannels,0);
	if (imgData)
	{
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img_Width,img_Height,0,GL_RGB,GL_UNSIGNED_BYTE,imgData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "FAILED TO LOAD THE TEXTURE!"<<std::endl;
	}
	stbi_image_free(imgData);
	
	
	//RENDER LOOP
	while (!glfwWindowShouldClose(window))
	{
		float currFrameTime = (float)glfwGetTime();
		deltaTime = currFrameTime - lastFrame;
		lastFrame = currFrameTime;

		ProcessInputs(window);

		glClearColor(0.2f,0.1f,0.3f,1.0f);
		glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

		float timeT = glfwGetTime();
		myShader.use();

		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		
		myShader.setMat4("view", view);
		myShader.setMat4("projection", projection);

		glBindTexture(GL_TEXTURE_2D,wallTexture);
		glBindVertexArray(VAO);
		float rotSpeed = 3.5f;
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = glfwGetTime() * (i+1) * rotSpeed;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			myShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1,&VBO);
	
	//glDeleteProgram(Shader);

	glfwTerminate();
	return 0;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)\
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void ProcessInputs(GLFWwindow* window)
{
	float movSpeed = 0.03f;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT,deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
}