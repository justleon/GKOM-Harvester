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

glm::vec3 teethPositions[] = {
	glm::vec3(-0.8f, -0.11f, 0.5f),
	glm::vec3(-0.8f, -0.11f, 0.4f),
	glm::vec3(-0.8f, -0.11f, 0.3f),
	glm::vec3(-0.8f, -0.11f, 0.2f),
	glm::vec3(-0.8f, -0.11f, 0.1f),
	glm::vec3(-0.8f, -0.11f, 0.0f),
	glm::vec3(-0.8f, -0.11f, -0.1f),
	glm::vec3(-0.8f, -0.11f, -0.2f),
	glm::vec3(-0.8f, -0.11f, -0.3f),
	glm::vec3(-0.8f, -0.11f, -0.4f),
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

		//liczba okreœla iloœæ boków w m³ócarce oraz d³ugoœæ boku
		int numberOfSidesInMechanism = 7;

		float angle = 360.0 / numberOfSidesInMechanism;
		float angleInDegrees = 360.0 / numberOfSidesInMechanism;
		//konwersja na radiany
		angle *= (3.1415f/180.0f);
		float sideLengthInMechanism = sqrtf(2*(1-cosf(angle)))*0.07f;
		cout << "Side length: " << sideLengthInMechanism << endl;

		float heightInMechanism = 0.07f * cosf(angle/2.0f);
		cout << "Height: " << heightInMechanism << endl;

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
			for (unsigned int i = 0; i < 10; i++)
			{
				Transformation transformationMatrix({ -0.8f, -0.11f, -0.5f + i*0.1f },
					//miejsce na animcaje lewo-prawo zêbów
				90.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 0.03f, 0.15f, 0.1f });


				Pyramid teeth(0.5f, transformationMatrix);
				teeth.draw(shaderProgram);	
			}

			//pod³o¿e
			Transformation trans1({ 0.0f, -0.5f, 0.0f },
				0.0f,
				{ 1.0f, 0.5f, 0.0f },
				{ 10.0f, 0.05f, 10.0f });
			Cube platform(2.0f, trans1);
			platform.draw(shaderProgram);

			//ty³ nagarniacza
			Transformation trans2({ -0.5f, 0.0f, 0.0f },
				0.0f,
				{ 1.0f, 0.0f, 0.0f },
				{ 0.1f, 0.5f, 2.5f });
			Cube hoeHolder(0.5f, trans2);
			hoeHolder.draw(shaderProgram);

			//wysiêgnik
			Transformation trans3({ -0.20f, 0.12f, 0.0f },
				35.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 0.9f, 0.1f, 0.25f });
			Cube hoeHolder2(0.8f, trans3);
			hoeHolder2.draw(shaderProgram);

			//cylinder trzymaj¹cy wysiêgnik
			Transformation trans4({ 0.12f, 0.35f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 0.3f, 0.3f, 0.5f });
			Cylinder hoeHolder3(0.6f, trans4);
			hoeHolder3.draw(shaderProgram);

			//cylinder obracaj¹cy maszyn¹ do m³ócenia
			Transformation trans5({ -0.8f, 0.0f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 1.0f, 40.0f });
			Cylinder cylinder(0.03f, trans5);
			cylinder.draw(shaderProgram);

			//test sfery
			Transformation trans6({ 1.0f, 1.0f, 1.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 1.0f, 1.0f });
			Sphere spheretest(0.5f, trans6);
			spheretest.draw(shaderProgram);

			//test ko³a
			Transformation trans7({ -0.5f, 1.0f, 1.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 1.0f, 1.0f });
			Wheel wheeltest(0.5f, trans7);
			wheeltest.draw(shaderProgram);

			//uchwyt na zêby
			Transformation trans8({ -0.65f, -0.11f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 1.0f },
				{ 0.35f, 0.025f, 1.7f });
			Cube hoeteethholderbottom(0.7f, trans8);
			hoeteethholderbottom.draw(shaderProgram);

			//lewa œciana nagarnaicza - cube
			Transformation trans9({ -0.65f, 0.0f, 0.6f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.8f, 1.2f, 0.3f });
			Cube leftWallNagarniacz(0.2f, trans9);
			leftWallNagarniacz.draw(shaderProgram);

			//lewa œciana nagarnaicza - cylinder
			Transformation trans10({ -0.8f, 0.0f, 0.6f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 0.95f, 0.23f });
			Cylinder leftCornerNagarniacz(0.25f, trans10);
			leftCornerNagarniacz.draw(shaderProgram);

			//prawa œciana nagarnaicza - cube
			Transformation trans11({ -0.65f, 0.0f, -0.6f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.8f, 1.2f, 0.3f });
			Cube rightWallNagarniacz(0.2f, trans11);
			rightWallNagarniacz.draw(shaderProgram);

			//prawa œciana nagarnaicza - cylinder
			Transformation trans12({ -0.8f, 0.0f, -0.6f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 0.95f, 0.23f });
			Cylinder rightCornerNagarniacz(0.25f, trans12);
			rightCornerNagarniacz.draw(shaderProgram);




			//big box nadwozie placeholder
			Transformation trans13({ 1.14f, 0.86f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 4.0f, 2.0f, 2.0f });
			Cube placeholder1(0.5f, trans13);
			placeholder1.draw(shaderProgram);

			//pod³oga w kabinie
			Transformation trans14({ -0.1f, 0.8f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 0.15f, 1.0f });
			Cube placeholder2(0.5f, trans14);
			placeholder2.draw(shaderProgram);

			//chyba lewy przedni prêt kabiny 
			Transformation trans15({ -0.44f, 1.4f, 0.26f },
				18.0f,
				{ 0.15f, 0.0f, 0.5f },
				{ 0.12f, 2.5f, 0.12f });
			Cube placeholder3(0.5f, trans15);
			placeholder3.draw(shaderProgram);

			//chyba prawy przedni prêt kabiny 
			Transformation trans16({ -0.44f, 1.4f, -0.26f },
				18.0f,
				{ -0.15f, 0.0f, 0.5f },
				{ 0.12f, 2.5f, 0.12f });
			Cube placeholder4(0.5f, trans16);
			placeholder4.draw(shaderProgram);

			//daszek kabiny
			Transformation trans17({ -0.19f, 2.0f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 2.1f, 0.10f, 1.9f });
			Cube placeholder5(0.5f, trans17);
			placeholder5.draw(shaderProgram);

			//chyba prawy tylni prêt kabiny 
			Transformation trans18({ 0.2f, 1.62f, -0.26f },
				9.0f,
				{ -0.5f, 0.0f, 0.0f },
				{ 0.12f, 1.5f, 0.12f });
			Cube placeholder6(0.5f, trans18);
			placeholder6.draw(shaderProgram);

			//chyba lewy tylni prêt kabiny 
			Transformation trans19({ 0.2f, 1.62f, 0.26f },
				9.0f,
				{ 0.5f, 0.0f, 0.0f },
				{ 0.12f, 1.5f, 0.12f });
			Cube placeholder7(0.5f, trans19);
			placeholder7.draw(shaderProgram);

			//rura wydechowa - rura
			Transformation trans20({ 1.6f, 1.36f, 0.26f },
				90.0f,
				{ 0.5f, 0.0f, 0.0f },
				{ 0.12f, 0.12f, 0.6f });
			Cylinder placeholder8(0.5f, trans20);
			placeholder8.draw(shaderProgram);

			//rura wydechowa - puszka na czubku rury
			Transformation trans21({ 1.6f, 1.56f, 0.26f },
				90.0f,
				{ 0.5f, 0.0f, 0.0f },
				{ 0.32f, 0.32f, 0.2f });
			Cylinder placeholder9(0.5f, trans21);
			placeholder9.draw(shaderProgram);

			//rura wydechowa zbo¿owa the base
			Transformation trans22({ 2.4f, 1.2f, 0.36f },
				90.0f,
				{ 0.0f, 0.5f, 0.0f },
				{ 0.20f, 0.22f, 0.6f });
			Cylinder placeholder10(1.0f, trans22);
			placeholder10.draw(shaderProgram);

			//rura wydechowa zbo¿owa the exit
			Transformation trans23({ 2.59f, 1.1f, 0.32f },
				90.0f,
				{ -0.5f, 0.5f, 0.5f },
				{ 0.20f, 0.16f, 0.20f });
			Cylinder placeholder11(1.0f, trans23);
			placeholder11.draw(shaderProgram);

			//big box nadwozie placeholder kube³ na zbo¿e
			Transformation trans24({ 2.64f, 0.66f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 2.0f, 1.0f, 1.5f });
			Cube placeholder12(0.5f, trans24);
			placeholder12.draw(shaderProgram);

			//smaller box podwozie placeholder przód
			Transformation trans25({ 0.8f, 0.35f, 0.0f },
				10.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 2.0f, 1.0f, 1.0f });
			Cube placeholder13(0.5f, trans25);
			placeholder13.draw(shaderProgram);

			//oœ przednia do kó³ trajktora
			Transformation trans26({ 0.4f, 0.1f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 1.0f, 40.0f });
			Cylinder placeholder14(0.03f, trans26);
			placeholder14.draw(shaderProgram);

			//lewego przedniego ko³a
			Transformation trans27({ 0.4f, 0.1f, 0.6f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 1.0f, 1.0f });
			Wheel placeholder15(0.5f, trans27);
			placeholder15.draw(shaderProgram);

			//prawego przedniego ko³a
			Transformation trans28({ 0.4f, 0.1f, -0.6f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 1.0f, 1.0f });
			Wheel placeholder16(0.5f, trans28);
			placeholder16.draw(shaderProgram);

			//smaller box podwozie placeholder ty³
			Transformation trans29({ 1.8f, 0.35f, 0.0f },
				8.0f,
				{ 0.0f, 0.0f, -3.0f },
				{ 2.4f, 1.0f, 1.0f });
			Cube placeholder17(0.5f, trans29);
			placeholder17.draw(shaderProgram);

			//oœ tylnia do kó³ trokatora
			Transformation trans30({ 2.2f, 0.03f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 1.0f, 30.0f });
			Cylinder placeholder18(0.03f, trans30);
			placeholder18.draw(shaderProgram);

			//test lewego tylnego ko³a
			Transformation trans31({ 2.2f, 0.03f, 0.4f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 0.7f, 0.7f, 0.7f });
			Wheel placeholder19(0.5f, trans31);
			placeholder19.draw(shaderProgram);

			//test prawego tylnego ko³a
			Transformation trans32({ 2.2f, 0.03f, -0.4f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 0.7f, 0.7f, 0.7f });
			Wheel placeholder20(0.5f, trans32);
			placeholder20.draw(shaderProgram);


			//transformacja m³ócarki
			Transformation transformationMlocarka({ -0.8f, heightInMechanism, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ sideLengthInMechanism, 0.01f, 0.01f });

			float posX = -0.8f, posY = heightInMechanism;

			for (int i = 0; i < numberOfSidesInMechanism; i++) 
			{
				
				transformationMlocarka.pos[0] = ((posX + 0.8f)*cosf(angle*i)) - (posY*sinf(angle*i)) - 0.8f;
				transformationMlocarka.pos[1] = ((posX + 0.8f) * sinf(angle*i)) + (posY*cosf(angle*i));
				cout << "New coords x:  " << posX << " y: " << posY << endl;
				transformationMlocarka.angle = angleInDegrees*i;
				//m³ócarka generowana proceduralnie
				Cube mlocarkaElement1(1.0f, transformationMlocarka);
				mlocarkaElement1.draw(shaderProgram);
			}
			


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
