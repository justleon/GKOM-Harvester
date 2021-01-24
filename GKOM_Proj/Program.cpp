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
#include "headers/Trapezoid.h"
#include "headers/Triangle.h"
#include "headers/Harvester.h"
#include "headers/Lamp.h"
#include "headers/Skybox.h"
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

#define LIGHT_POSITION 20.0f, 15.0f, 50.0f
#define LIGHT_AMBIENT 0.2f, 0.2f, 0.2f
#define LIGHT_DIFFUSE 0.9f, 0.9f, 0.9f
#define LIGHT_SPECULAR 1.0f, 1.0f, 1.0f
#define LIGHT_COLOR 1.0f, 1.0f, 0.8f

glm::vec3 lightPos(LIGHT_POSITION);
glm::vec3 light_ambient(LIGHT_AMBIENT);
glm::vec3 light_diffuse(LIGHT_DIFFUSE);
glm::vec3 light_specular(LIGHT_SPECULAR);
glm::vec3 light_color(LIGHT_COLOR);
glm::vec3 light_spec_half(0.5f, 0.5f, 0.5f);

const GLuint WIDTH = 1280, HEIGHT = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
GLuint LoadMipmapTexture(GLuint texId, const char* fname);

//camera
Camera camera(glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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
		ShaderProgram lampShader("shaders/lampShader.vert", "shaders/lampShader.frag");

		// Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// prepare textures
		GLuint texture0 = LoadMipmapTexture(GL_TEXTURE0, "textures/harv_side.png");

		glEnable(GL_DEPTH_TEST);

		//zmienna określa ilość boków w młócarce
		int numberOfSidesInMechanism = 9;
		//zmienna określa ilość elementów w młócarce
		int numberOfMechanisms = 5;
		//zmienna okreśła długość rury zbożowej
		float lengthOfWheatPipe = 0.8f;

		Harvester harvester(numberOfSidesInMechanism, numberOfMechanisms, lengthOfWheatPipe);

		//zmienna określające młyn
		int numWings = 5;
		float wingSpeed = 10.0f;
		float wingAngle = 180 / numWings;

		auto skybox = Skybox();

		// main event loop
		while (!glfwWindowShouldClose(window))
		{
			processInput(window);

			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// Clear the colorbuffer
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Bind Textures using texture units
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture0);
			shaderProgram.setUniformInt("tex", 0);

			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();

			lampShader.Use();
			lampShader.setUniformMat4("view", view);
			lampShader.setUniformMat4("projection", projection);
			lampShader.setUniformFloat("intensity", 1.0f);
			lampShader.setUniformVec3f("lightColor", light_color);

			// Draw our first box
			shaderProgram.Use();
			shaderProgram.setUniformVec3f("light.position", lightPos);
			shaderProgram.setUniformVec3f("viewPos", camera.Position);

			shaderProgram.setUniformVec3f("light.ambient", light_ambient);
			shaderProgram.setUniformVec3f("light.diffuse", light_diffuse);
			shaderProgram.setUniformVec3f("light.specular", light_specular);

			shaderProgram.setUniformVec3f("material.specular", light_spec_half);
			shaderProgram.setUniformFloat("material.shininess", 64.0f);

			shaderProgram.setUniformMat4("view", view);
			shaderProgram.setUniformMat4("projection", projection);
			Transformation transformationMatrix({ -1.18f, -0.1f, -1.15f },
				90.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 0.03f, 0.25f, 0.1f });

			for (unsigned int i = 0; i < 20; i++)
			{
				transformationMatrix.pos[2] = harvester.firstTeethPosition + i * 0.1f;

				Pyramid teeth(0.5f, transformationMatrix);
				teeth.draw(shaderProgram);
			}

			//animacja ruchu zębów
			if (harvester.teethDirection) {
				harvester.firstTeethPosition += 0.015f;
			}
			else {
				harvester.firstTeethPosition -= 0.015f;
			}

			if (harvester.firstTeethPosition > -0.75f)
			{
				harvester.teethDirection = false;
			}
			if (harvester.firstTeethPosition < -1.15f)
			{
				harvester.teethDirection = true;
			}
			
			Transformation light(lightPos,
				0.0f,
				{ 1.0f, 1.0f, 1.0f },
				{ 1.0f, 1.0f, 1.0f });
			Lamp lightSource(5.0f, light);
			lightSource.draw(lampShader);
			
			//bryła młyna
			Transformation t1({ -5.0f, 6.0f, -8.0f },
				90.0f,
				{ 1.0f, 0.0f, 0.0f },
				{ 1.0f, 1.0f, 2.5f });
			Cylinder bryla(5.0f, t1);
			bryla.draw(shaderProgram);

			//dach mlyna
			Transformation t2({ -5.0f, 14.0f, -8.0f },
				0.0f,
				{ 1.0f, 0.0f, 1.0f },
				{ 1.0f, 1.0f, 1.0f });
			Pyramid dach(6.50f, t2);
			dach.draw(shaderProgram);

			//wal młyna
			Transformation t3({ -5.0f, 8.0f, -5.0f },
				wingSpeed * currentFrame,
				{ 0.0f, 0.0f, 1.0f },
				{ 1.0f, 1.0f, 3.0f });
			Cylinder wal(0.6f, t3);
			wal.draw(shaderProgram);

			for (int i = 0; i < numWings; i++) {
				Transformation t3({ -5.0f, 8.0f, -4.3f + i * 0.001f },
					i * wingAngle + wingSpeed * currentFrame,
					{ 0.0f, 0.0f, 1.0f },
					{ 2.0f, 40.0f, 0.2f });
				Cube wing(0.3f, t3);
				wing.draw(shaderProgram);
			}

			//pod�o�e
			Transformation trans1({ 0.0f, -0.19f, 0.0f },
				0.0f,
				{ 1.0f, 0.5f, 0.0f },
				{ 10.0f, 0.05f, 10.0f });
			Cube platform(2.0f, trans1);
			platform.draw(shaderProgram);

			//ty� nagarniacza
			Transformation trans2({ -0.5f, 0.2f, 0.0f },
				0.0f,
				{ 1.0f, 0.0f, 0.0f },
				{ 0.1f, 1.2f, 5.0f });
			Cube hoeHolder(0.5f, trans2);
			hoeHolder.draw(shaderProgram);

			//wysi�gnik
			Transformation trans3({ -0.18f, 0.15f, 0.0f },
				30.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 0.9f, 0.15f, 0.4f });
			Cube hoeHolder2(0.8f, trans3);
			hoeHolder2.draw(shaderProgram);

			//cylinder trzymaj�cy wysi�gnik
			Transformation trans4({ 0.12f, 0.35f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 0.5f, 0.5f, 0.7f });
			Cylinder hoeHolder3(0.6f, trans4);
			hoeHolder3.draw(shaderProgram);

			//cylinder obracaj�cy maszyn� do m��cenia
			Transformation trans5({ -1.17f, 0.2f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.5f, 1.5f, 50.0f });
			Cylinder cylinder(0.05f, trans5);
			cylinder.draw(shaderProgram);

			//uchwyt na z�by
			Transformation trans8({ -0.82f, -0.1f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 1.0f },
				{ 0.9f, 0.025f, 3.6f });
			Cube hoeteethholderbottom(0.7f, trans8);
			hoeteethholderbottom.draw(shaderProgram);

			//lewa �ciana nagarnaicza - cube
			Transformation trans9({ -0.82f, 0.2f, 1.25f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.4f, 1.2f, 0.1f });
			Cube leftWallNagarniacz(0.5f, trans9);
			leftWallNagarniacz.draw(shaderProgram);

			//lewa �ciana nagarnaicza - cylinder
			Transformation trans10({ -1.17f, 0.2f, 1.25f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.2f, 1.2f, 0.12f });
			Cylinder leftCornerNagarniacz(0.5f, trans10);
			leftCornerNagarniacz.draw(shaderProgram);

			//prawa �ciana nagarnaicza - cube
			Transformation trans11({ -0.82f, 0.2f, -1.25f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.4f, 1.2f, 0.1f });
			Cube rightWallNagarniacz(0.5f, trans11);
			rightWallNagarniacz.draw(shaderProgram);

			//prawa �ciana nagarnaicza - cylinder
			Transformation trans12({ -1.17f, 0.2f, -1.25f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.2f, 1.2f, 0.12f });
			Cylinder rightCornerNagarniacz(0.5f, trans12);
			rightCornerNagarniacz.draw(shaderProgram);





			//big box nadwozie placeholder
			Transformation trans13({ 1.14f, 0.86f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 4.0f, 2.0f, 2.0f });
			Cube placeholder1(0.5f, trans13);
			placeholder1.draw(shaderProgram);

			//pod�oga w kabinie
			Transformation trans14({ -0.1f, 0.8f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 0.15f, 1.0f });
			Cube placeholder2(0.5f, trans14);
			placeholder2.draw(shaderProgram);

			//chyba lewy przedni pr�t kabiny 
			Transformation trans15({ -0.44f, 1.4f, 0.26f },
				18.0f,
				{ 0.15f, 0.0f, 0.5f },
				{ 0.12f, 2.5f, 0.12f });
			Cube placeholder3(0.5f, trans15);
			placeholder3.draw(shaderProgram);

			//chyba prawy przedni pr�t kabiny 
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

			//chyba prawy tylni pr�t kabiny 
			Transformation trans18({ 0.2f, 1.62f, -0.26f },
				9.0f,
				{ -0.5f, 0.0f, 0.0f },
				{ 0.12f, 1.5f, 0.12f });
			Cube placeholder6(0.5f, trans18);
			placeholder6.draw(shaderProgram);

			//chyba lewy tylni pr�t kabiny 
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

			//rura wydechowa zbożowa podstawa 1
			Transformation trans222({ 2.5f, 1.0f, 0.0f },
				90.0f,
				{ 2.0f, 0.0f, 0.0f },
				{ 0.15f, 0.15f, 0.4f });
			Cylinder cylinderPlaceholder1(1.0f, trans222);
			cylinderPlaceholder1.draw(shaderProgram);

			//rura wydechowa zbożowa łącznik
			Transformation trans223({ 2.5f, 1.2f, 0.0f },
				90.0f,
				{ 2.0f, 0.0f, 0.0f },
				{ 0.2f, 0.2f, 0.2f });
			Sphere spherePlaceholder1(1.0f, trans223);
			spherePlaceholder1.draw(shaderProgram);

			//trans22 i trans23 do animacji obrotu rury zbożowej
			//rura wydechowa zbo�owa the base
			Transformation trans22({ 2.5f + (lengthOfWheatPipe/2.0f), 1.2f, 0.0f },
				90.0f,
				{ 0.0f, 2.0f, 0.0f },
				{ 0.15f, 0.15f, lengthOfWheatPipe });
			Cylinder placeholder10(1.0f, trans22);
			placeholder10.draw(shaderProgram);

			//rura wydechowa zbo�owa the exit
			Transformation trans23({ 2.5f + lengthOfWheatPipe, 1.175f, 0.0f },
				90.0f,
				{ 2.0f, 0.0f, 0.0f },
				{ 0.20f, 0.2f, 0.20f });
			Cylinder placeholder11(1.0f, trans23);
			placeholder11.draw(shaderProgram);

			//big box nadwozie placeholder kube� na zbo�e
			Transformation trans24({ 2.64f, 0.66f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 2.0f, 1.0f, 1.5f });
			Cube placeholder12(0.5f, trans24);
			placeholder12.draw(shaderProgram);

			//smaller box podwozie placeholder prz�d
			Transformation trans25({ 0.8f, 0.35f, 0.0f },
				10.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 2.0f, 1.0f, 1.0f });
			Cube placeholder13(0.5f, trans25);
			placeholder13.draw(shaderProgram);

			//o� przednia do k� trajktora
			Transformation trans26({ 0.4f, 0.1f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 1.0f, 40.0f });
			Cylinder placeholder14(0.03f, trans26);
			placeholder14.draw(shaderProgram);

			//lewego przedniego ko�a
			Transformation trans27({ 0.4f, 0.1f, 0.6f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 1.0f, 1.0f });
			Wheel placeholder15(0.5f, trans27);
			placeholder15.draw(shaderProgram);

			//prawego przedniego ko�a
			Transformation trans28({ 0.4f, 0.1f, -0.6f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 1.0f, 1.0f });
			Wheel placeholder16(0.5f, trans28);
			placeholder16.draw(shaderProgram);

			//smaller box podwozie placeholder ty�
			Transformation trans29({ 1.8f, 0.35f, 0.0f },
				8.0f,
				{ 0.0f, 0.0f, -3.0f },
				{ 2.4f, 1.0f, 1.0f });
			Cube placeholder17(0.5f, trans29);
			placeholder17.draw(shaderProgram);

			//o� tylnia do k� trokatora
			Transformation trans30({ 2.2f, 0.03f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 1.0f, 30.0f });
			Cylinder placeholder18(0.03f, trans30);
			placeholder18.draw(shaderProgram);

			//test lewego tylnego ko�a
			Transformation trans31({ 2.2f, 0.03f, 0.4f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 0.7f, 0.7f, 0.7f });
			Wheel placeholder19(0.5f, trans31);
			placeholder19.draw(shaderProgram);

			//test prawego tylnego ko�a
			Transformation trans32({ 2.2f, 0.03f, -0.4f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 0.7f, 0.7f, 0.7f });
			Wheel placeholder20(0.5f, trans32);
			placeholder20.draw(shaderProgram);

			//pr�t pomi�dzy pod�og� kabiny a najwy�szym schodkiem
			Transformation trans33({ -0.18f, 0.7f, 0.22f },
				18.0f,
				{ -0.15f, 0.0f, 0.5f },
				{ 0.06f, 0.4f, 0.06f });
			Cube placeholder21(0.5f, trans33);
			placeholder21.draw(shaderProgram);

			//najwy�szy schodek
			Transformation trans34({ -0.01f, 0.6f, 0.23f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 0.6f, 0.06f, 0.12f });
			Cube placeholder22(0.5f, trans34);
			placeholder22.draw(shaderProgram);

			//rurka drabinkowa bli�sza wykaszarce
			Transformation trans35({ -0.1f, 0.37f, 0.23f },
				90.0f,
				{ 0.5f, 0.0f, 0.0f },
				{ 0.12f, 0.12f, 1.6f });
			Cylinder placeholder23(0.3f, trans35);
			placeholder23.draw(shaderProgram);

			//rurka drabinkowa bli�sza korpusowi
			Transformation trans36({ 0.1f, 0.37f, 0.23f },
				90.0f,
				{ 0.5f, 0.0f, 0.0f },
				{ 0.12f, 0.12f, 1.6f });
			Cylinder placeholder24(0.3f, trans36);
			placeholder24.draw(shaderProgram);

			//schodek drugi od g�ry
			Transformation trans37({ 0.0f, 0.5f, 0.23f },
				90.0f,
				{ 0.0f, 0.5f, 0.0f },
				{ 0.12f, 0.12f, 0.8f });
			Cylinder placeholder25(0.3f, trans37);
			placeholder25.draw(shaderProgram);

			//schodek trzeci od g�ry
			Transformation trans38({ 0.0f, 0.4f, 0.23f },
				90.0f,
				{ 0.0f, 0.5f, 0.0f },
				{ 0.12f, 0.12f, 0.8f });
			Cylinder placeholder26(0.3f, trans38);
			placeholder26.draw(shaderProgram);

			//schodek czwarty od g�ry
			Transformation trans39({ 0.0f, 0.3f, 0.23f },
				90.0f,
				{ 0.0f, 0.5f, 0.0f },
				{ 0.12f, 0.12f, 0.8f });
			Cylinder placeholder27(0.3f, trans39);
			placeholder27.draw(shaderProgram);

			//drzwi kombonojaju lewe
			Transformation trans40({ -0.06f, 1.0f, 0.22f },
				180.0f,
				{ 0.0f, 0.0f, 0.5f },
				{ 1.6f, 1.2f, 0.12f });
			Trapezoid placeholder28(0.3f, trans40);
			placeholder28.draw(shaderProgram);


			//drzwi kombonojaju prawe
			Transformation trans41({ -0.06f, 1.0f, -0.22f },
				180.0f,
				{ 0.0f, 0.0f, 0.5f },
				{ 1.6f, 1.2f, 0.12f });
			Trapezoid placeholder29(0.3f, trans41);
			placeholder29.draw(shaderProgram);

			//drzwi kombonojaju prawe trójkąt
			Transformation trans42({ -0.3f, 1.26f, -0.22f },
				50.0f,
				{ 0.0f, 0.0f, 0.5f },
				{ 0.9f, 2.0f, 0.12f });
			Triangle placeholder30(0.3f, trans42);
			placeholder30.draw(shaderProgram);

			//drzwi kombonojaju lewe trójkąt
			Transformation trans43({ -0.3f, 1.26f, 0.22f },
				50.0f,
				{ 0.0f, 0.0f, 0.5f },
				{ 0.9f, 2.0f, 0.12f });
			Triangle placeholder31(0.3f, trans43);
			placeholder31.draw(shaderProgram);


			harvester.speedOfMechanism += 0.01f;


			//transformacja wykaszarki
			Transformation transformationMlocarka({ trans5.pos[0], harvester.heightInMechanism + trans5.pos[1], 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ harvester.sideLengthInMechanism, 0.01f, 0.01f });

			float posX = trans5.pos[0], posY = harvester.heightInMechanism + trans5.pos[1], posZ = -1.2f;


			//obliczenie pozycji wykaszarki/młócarki
			for (int j = 0; j < numberOfMechanisms; j++)
			{
				transformationMlocarka.pos[2] = posZ + j * harvester.distanceBetweenMechanisms;
				for (int i = 0; i < numberOfSidesInMechanism; i++)
				{
					transformationMlocarka.pos[0] = ((posX + 1.17f) * cosf((harvester.angle * i) + harvester.speedOfMechanism)) - ((posY - 0.2f) * sinf((harvester.angle * i) + harvester.speedOfMechanism)) - 1.17f;
					transformationMlocarka.pos[1] = ((posX + 1.17f) * sinf((harvester.angle * i) + harvester.speedOfMechanism)) + ((posY - 0.2f) * cosf((harvester.angle * i) + harvester.speedOfMechanism)) + 0.2f;
					transformationMlocarka.angle = harvester.angleDiffrence + (harvester.angleInDegrees * (i + ((numberOfSidesInMechanism * 0.5f) - 1.0f)));
					//młócarka generowana proceduralnie
					Cube mlocarkaElement1(1.0f, transformationMlocarka);
					mlocarkaElement1.draw(shaderProgram);
				}
			}

			posX = trans5.pos[0] - (harvester.sideLengthInMechanism / 2.0f);
			posY = harvester.heightInMechanism + trans5.pos[1];
			posZ = 0.0f;

			Transformation transformationMlocarkaPoprzeczka({ trans5.pos[0] - (harvester.sideLengthInMechanism / 2.0f), harvester.heightInMechanism + trans5.pos[1], 0.0f },
				(harvester.angleInDegrees / 2.0f) + harvester.angleDiffrence,
				{ 0.0f, 0.0f, 3.0f },
				{ 0.01f, 0.01f, 2.4f });

			for (int i = 0; i < numberOfSidesInMechanism; i++)
			{
				transformationMlocarkaPoprzeczka.pos[0] = ((posX + 1.17f) * cosf((harvester.angle * i) + harvester.speedOfMechanism)) - ((posY - 0.2f) * sinf((harvester.angle * i) + harvester.speedOfMechanism)) - 1.17f;
				transformationMlocarkaPoprzeczka.pos[1] = ((posX + 1.17f) * sinf((harvester.angle * i) + harvester.speedOfMechanism)) + ((posY - 0.2f) * cosf((harvester.angle * i) + harvester.speedOfMechanism)) + 0.2f;
				//element poprzeczny generowany proceduralnie
				Cube mlocarkaElementPoprzeczka(1.0f, transformationMlocarkaPoprzeczka);
				mlocarkaElementPoprzeczka.draw(shaderProgram);
			}

			posX = trans5.pos[0];
			posY = harvester.heightInMechanism + trans5.pos[1];
			float nextposX = ((posX + 1.17f) * cosf((harvester.angle)+ harvester.speedOfMechanism)) - ((posY - 0.2f) * sinf((harvester.angle)+ harvester.speedOfMechanism)) - 1.17f;
			float nextposY = ((posX + 1.17f) * sinf((harvester.angle)+harvester.speedOfMechanism)) + ((posY - 0.2f) * cosf((harvester.angle)+ harvester.speedOfMechanism)) + 0.2f;
			//cout << "x: " << posX << " newX: " << nextposX << " y: " << posY << " newY: " << nextposY << endl;
			harvester.angleDiffrence = ((nextposX - posX) * (nextposX - posX)) + ((nextposY - posY) * (nextposY - posY));
			//cout << angleDiffrence << endl;
			harvester.angleDiffrence = ((2.0f * harvester.heightInMechanism * harvester.heightInMechanism) - harvester.angleDiffrence) / (2.0f * harvester.heightInMechanism * harvester.heightInMechanism);
			//cout << angleDiffrence << endl;
			harvester.angleDiffrence = acosf(harvester.angleDiffrence);
			//cout << angleDiffrence << endl;
			harvester.angleDiffrence *= (180.0f / 3.1415f);
			if (nextposX > posX) harvester.angleDiffrence = 360.0f - harvester.angleDiffrence;
			//cout << angleDiffrence << endl;

			skybox.draw(glm::perspective(glm::radians(70.0f), 4.5f / 3.0f, 0.1f, 100.0f), camera.GetViewMatrix());

			//trapezoid test
			/*
			Transformation transformationTrapezoid({ 0.0f, -3.0f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 1.0f, 1.0f });
			Trapezoid trapezoidTest(1.0f, transformationTrapezoid);
			trapezoidTest.draw(shaderProgram);
			*/

			glBindVertexArray(0);

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
	unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
	if (image == nullptr)
		throw exception("Failed to load texture file");

	GLuint texture;
	glGenTextures(1, &texture);

	glActiveTexture(texId);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
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
