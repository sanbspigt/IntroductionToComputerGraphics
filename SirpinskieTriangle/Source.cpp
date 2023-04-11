#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include<cmath>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


const int SCR_WIDTH = 720;
const int SCR_HEIGHT = 720;

void frameBuffer_Size_Callback(GLFWwindow* window, int width, int height);
unsigned int Compileshader();



//Shader Sources...................................
const char* vertexShaderCode =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec3 outCol;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   outCol = vec3(aPos.x, aPos.y, aPos.z);\n"
"}\0";
const char* fragmentShaderCode =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 outCol;\n"
"uniform vec3 tCol;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(tCol.x,tCol.y,tCol.z, 1.0f);\n"
"}\n\0";
//...................................


typedef glm::vec2 vec2;

//Generate Single Triangle vertices
void Triangle(vec2 points[], vec2 a, vec2 b, vec2 c)
{
    static int i = 0;

    points[i++] = a;
    points[i++] = b;
    points[i++] = c;
}


/// <param name="K"></param> // No. of divisions 
void DivideTriangle(vec2 points[],vec2 a,vec2 b,vec2 c,int K)
{
    if (K > 0) {
        vec2 ab = (a + b) / 2.0f;
        vec2 bc = (b + c) / 2.0f;
        vec2 ac = (a + c) / 2.0f;

        DivideTriangle(points, a, ab, ac, K - 1);
        DivideTriangle(points, c, ac, bc, K - 1);
        DivideTriangle(points, b, bc, ab, K - 1);
    }
    else
        Triangle(points,a,b,c);
}



int main()
{
    //NumVertices = pow(3, NumDivisions + 1);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OPENGL WINDOW", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "FAILED TO CREATE A WINDOW!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBuffer_Size_Callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "FAILED TO INITIALIZE GLAD!" << std::endl;
        return -1;
    }
    //-----------------------------------------------------------------------

    const int NumDivisions = 6;
    const int NumVertices = 2187; // = pow(3,NumDivisions + 1)
    

    vec2 points[NumVertices];

    vec2 pointA = vec2(0.0f,1.0f);
    vec2 pointB = vec2(-1.0f,-1.0f);
    vec2 pointC = vec2(1.0f,-1.0f);

    DivideTriangle(points, pointA, pointB, pointC, NumDivisions);

    //Get reference for the linked shader.
    unsigned int shaderProgram = Compileshader();
    //Create VBO, VAO
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);


    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
   


    glBindVertexArray(0);

    //-------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.1f, 0.2f, 1.0f);//RGBA R- RED||G - GREEN || B- BLUE || A - ALPHA(OPACITY)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //processInput(window);
        glUseProgram(shaderProgram);
        int uniformLocTCol = glGetUniformLocation(shaderProgram,"tCol");
        float timeT = glfwGetTime();
        glUniform3f(uniformLocTCol, sin(timeT*(rand()%1000)), sin(timeT * (rand() % 1000)), sin(timeT * (rand() % 1000)));
        //glEnable(GL_DEPTH_TEST);
        // glPointSize(6.0f);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, NumVertices);
        //glDrawElements(GL_LINES, (unsigned int)sphere.getLineIndexSize(), GL_UNSIGNED_INT, sphere.getLineIndices());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

void frameBuffer_Size_Callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

unsigned int Compileshader()
{
    unsigned int shaderProgram;
    unsigned int vertexShader, fragmentShader;
    //Vertex Shader compilation 
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);
    /// <summary>
    /// Check the compilation success and print any errors to the log.
    /// </summary>
    /// <returns></returns>
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR!Vertex Shader compilation failed!\n" << infoLog << std::endl;
    }
    //Fragment shader compilation
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);
    /// <summary>
    /// Check the compilation success and print any errors to the log.
    /// </summary>
    /// <returns></returns>	
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR!Fragment Shader compilation failed!\n" << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR!Shader Linking failed!\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}


void processInput(GLFWwindow* window)
{
    float offset = 0.001f;


}



