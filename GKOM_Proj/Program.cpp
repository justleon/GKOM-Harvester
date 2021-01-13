#define GLEW_STATIC
#include <GL/glew.h>
#include "headers/VertexBuffer.h"
#include "headers/VertexBufferLayout.h"
#include "headers/IndexBuffer.h"
#include "headers/VertexArray.h"
#include "headers/ShaderProgram.h"
#include "headers/Camera.h"
#include "headers/Cube.h"
#include "headers/Pyramid.h"
#include "headers/Cylinder.h"
#include "headers/Sphere.h"
#include "headers/Wheel.h"
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

const GLuint WIDTH = 1280, HEIGHT = 800;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
GLuint LoadMipmapTexture(GLuint texId, const char* fname);

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

ostream& operator<<(ostream& os, const glm::mat4& mx)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
			cout << mx[row][col] << ' ';
		cout << endl;
	}
	return os;
}

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

int main()
{
	if (glfwInit() != GL_TRUE)
	{
		cout << "GLFW initialization failed" << endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	try
	{
		GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GKOM - Kombajn", nullptr, nullptr);
		if (window == nullptr)
			throw exception("GLFW window not created");
		glfwMakeContextCurrent(window);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			throw exception("GLEW Initialization failed");

		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		// tell GLFW to capture our mouse
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// Let's check what are maximum parameters counts
		GLint nrAttributes;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
		cout << "Max vertex attributes allowed: " << nrAttributes << std::endl;
		glGetIntegerv(GL_MAX_TEXTURE_COORDS, &nrAttributes);
		cout << "Max texture coords allowed: " << nrAttributes << std::endl;

		// Build, compile and link shader program
		ShaderProgram shaderProgram("shaders/vertexShader.vert", "shaders/fragmentShader.frag");

		// Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// prepare textures
		GLuint texture0 = LoadMipmapTexture(GL_TEXTURE0, "textures/container.jpg");
		GLuint texture1 = LoadMipmapTexture(GL_TEXTURE1, "textures/weiti.png");

		glEnable(GL_DEPTH_TEST);

		// main event loop
		while (!glfwWindowShouldClose(window))
		{
			processInput(window);

			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// Clear the colorbuffer
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Bind Textures using texture units
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture0);
			shaderProgram.setUniformInt("Texture0", 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture1);
			shaderProgram.setUniformInt("Texture1", 1);

			// Draw our first box
			shaderProgram.Use();

			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();

			shaderProgram.setUniformMat4("view", view);
			shaderProgram.setUniformMat4("projection", projection);
			/*
			for (unsigned int i = 0; i < 10; i++)
			{
				Transformation transformationMatrix(cubePositions[i],
					20.0f * (i + 1) * currentFrame,
					{ 1.0f, 0.3f, 0.5f },
					{ 1.0f, 1.0f, 1.0f });
				Pyramid testCube(1.0f, transformationMatrix);
				testCube.draw(shaderProgram);	
			}
			*/
			
			Transformation trans1({ 0.0f, -0.5f, 0.0f },
				0.0f,
				{ 1.0f, 0.5f, 0.0f },
				{ 10.0f, 0.05f, 10.0f });
			Cube platform(2.0f, trans1);
			platform.draw(shaderProgram);

			Transformation trans2({ -0.5f, 0.0f, 0.0f },
				0.0f,
				{ 1.0f, 0.0f, 0.0f },
				{ 0.1f, 0.5f, 2.5f });
			Cube hoeHolder(0.5f, trans2);
			hoeHolder.draw(shaderProgram);

			Transformation trans3({ -0.16f, 0.06f, 0.0f },
				15.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 0.9f, 0.1f, 0.25f });
			Cube hoeHolder2(0.8f, trans3);
			hoeHolder2.draw(shaderProgram);

			Transformation trans4({ 0.2f, 0.15f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 0.3f, 0.3f, 0.5f });
			Cylinder hoeHolder3(0.6f, trans4);
			hoeHolder3.draw(shaderProgram);

			Transformation trans5({ -0.7f, 0.0f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 1.0f, 17.0f });
			Cylinder cylinder(0.07f, trans5);
			cylinder.draw(shaderProgram);


			Transformation trans6({ 1.0f, 1.0f, 1.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 1.0f, 1.0f });
			Sphere spheretest(0.5f, trans6);
			spheretest.draw(shaderProgram);

			Transformation trans7({ -0.5f, 1.0f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 1.0f, 1.0f });
			Wheel wheeltest(0.5f, trans7);
			wheeltest.draw(shaderProgram);



			// Swap the screen buffers
			glfwSwapBuffers(window);
			// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
			glfwPollEvents();
		}
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
	}
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

GLuint LoadMipmapTexture(GLuint texId, const char* fname)
{
	int width, height;
	unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGB);
	if (image == nullptr)
		throw exception("Failed to load texture file");

	GLuint texture;
	glGenTextures(1, &texture);

	glActiveTexture(texId);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
