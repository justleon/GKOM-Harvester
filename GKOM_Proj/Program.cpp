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
#include "headers/Plane.h"
#include "headers/Harvester.h"
#include "headers/Lamp.h"
#include "headers/Skybox.h"
#include "headers/TextureManager.h"
#include "headers/ObjectCollection.h"
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

#define LIGHT_POSITION 25.0f, 35.0f, 45.0f
#define LIGHT_AMBIENT 0.2f, 0.2f, 0.2f
#define LIGHT_DIFFUSE 1.f, 1.0f, 1.0f
#define LIGHT_SPECULAR 0.9f, 0.9f, 0.9f
#define LIGHT_COLOR 1.0f, 1.0f, 0.8f

#define HARV_SIDE "textures/harv_side.png"
#define LADDER "textures/ladder.png"
#define BRICKS "textures/bricks_tileable.png"
#define WOOD_CONT "textures/container.jpg"
#define GRILL "textures/grill.png"
#define GRILL_RUST_HALF "textures/grill_half_rusted.png"
#define GRILL_RUST_SHI "textures/grill_rusted_shiny.png"
#define GRILL_RUST "textures/grill_very_rusted.png"
#define GROUND "textures/ground.png"

glm::vec3 lightPos(LIGHT_POSITION);
glm::vec3 light_ambient(LIGHT_AMBIENT);
glm::vec3 light_diffuse(LIGHT_DIFFUSE);
glm::vec3 light_specular(LIGHT_SPECULAR);
glm::vec3 light_color(LIGHT_COLOR);
glm::vec3 light_spec_half(0.3f, 0.3f, 0.3f);

const GLuint WIDTH = 1280, HEIGHT = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void LoadTextures();

//zmienne określające kombajn
int numberOfSidesInMechanism = 9;
int numberOfMechanisms = 5;
float lengthOfWheatPipe = 0.8f;

//parametry ruchu kombajnu
float maxSpeed = 10.0f;
float acceleration = 1.0f;
float rotSpeed = 10.0f;

Harvester harvester(numberOfSidesInMechanism, numberOfMechanisms, lengthOfWheatPipe);

//zmienna określające młyn
int numWings = 5;
float wingSpeed = 10.0f;
float wingAngle = 180 / numWings;

//camera
Camera camera(glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;
bool camInHarv = false;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float currentFrame = 0.0f;
float framePushed = 0.0f;

TextureManager texManager;

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
		
		LoadTextures();

		glEnable(GL_DEPTH_TEST);

		auto skybox = Skybox();

		// main event loop
		while (!glfwWindowShouldClose(window))
		{
			harvester.wheelAngle = 0.0f;
			harvester.wheelRotation = 1.0f;

			processInput(window);

			currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// Clear the colorbuffer
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Bind Textures using texture units
			//texManager.useTexture("textures/container.jpg");
			glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, texManager.getTextureID("textures/bricks_tileable.png"));
			//shaderProgram.setUniformInt("material.tex", 0);

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
			shaderProgram.setUniformFloat("material.shininess", 70.0f);

			shaderProgram.setUniformMat4("view", view);
			shaderProgram.setUniformMat4("projection", projection);

			//Create Collection of objects building a Harvester body.
			ObjectCollection kompoj;

			Transformation transformationMatrix({ -1.18f, -0.1f, -1.15f },
				90.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 0.03f, 0.25f, 0.1f });

			for (unsigned int i = 0; i < 20; i++)
			{
				transformationMatrix.pos[2] = harvester.firstTeethPosition + i * 0.1f;

				kompoj.addObject(std::shared_ptr<Object>(new 	Pyramid(0.5f, transformationMatrix, texManager.getTextureID(GRILL_RUST))));
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
			Lamp lightSource(15.0f, light);
			lightSource.draw(lampShader);
		
			ObjectCollection mlyn;

			//bryła młyna
			Transformation t1({ 0.0f, 6.0f, 0.0f },
				90.0f,
				{ 1.0f, 0.0f, 0.0f },
				{ 1.0f, 1.0f, 2.5f });
			mlyn.addObject(std::shared_ptr<Object>(new Cylinder(5.0f, t1, texManager.getTextureID(BRICKS))));

			//dach mlyna
			Transformation t2({ 0.0f, 14.0f, 0.0f },
				0.0f,
				{ 1.0f, 1.0f, 1.0f },
				{ 1.0f, 1.0f, 1.0f });
			mlyn.addObject(std::shared_ptr<Object>(new Pyramid(6.50f, t2, texManager.getTextureID(WOOD_CONT))));

			//wal młyna
			Transformation t4({ 0.0f, 8.0f, 3.0f },
				wingSpeed * currentFrame,
				{ 0.0f, 0.0f, 1.0f },
				{ 1.0f, 1.0f, 3.0f });
			mlyn.addObject(std::shared_ptr<Object>(new Cylinder(0.6f, t4, texManager.getTextureID(WOOD_CONT))));

			for (int i = 0; i < numWings; i++) {
				Transformation t3({ 0.0f, 8.0f, 3.7f + i * 0.001f },
					i * wingAngle + wingSpeed * currentFrame,
					{ 0.0f, 0.0f, 1.0f },
					{ 2.0f, 40.0f, 0.2f });
				mlyn.addObject(std::shared_ptr<Object>(new 	Cube(0.3f, t3, texManager.getTextureID(WOOD_CONT))));
			}

			mlyn.rotateWorld(35.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			mlyn.translateWorld(glm::vec3(-7.0f, 0.0f, -8.0f));
			mlyn.draw(shaderProgram);


			//pod�o�e
			Transformation trans1({ 0.0f, 9.89f, 0.0f },
				90.0f,
				{ 1.0f, 0.0f, 0.0f },
				{ 2000.0f, 2000.0f, 10.0f });
			Plane platform(2.0f, trans1, texManager.getTextureID(GROUND));
			platform.draw(shaderProgram);

			//ty� nagarniacza
			Transformation trans2({ -0.5f, 0.2f, 0.0f },
				0.0f,
				{ 1.0f, 0.0f, 0.0f },
				{ 0.1f, 1.2f, 5.0f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cube(0.5f, trans2, texManager.getTextureID(HARV_SIDE))));

			//wysi�gnik
			Transformation trans3({ -0.18f, 0.15f, 0.0f },
				30.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 0.9f, 0.15f, 0.4f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cube(0.8f, trans3, texManager.getTextureID(HARV_SIDE))));

			//cylinder trzymaj�cy wysi�gnik
			Transformation trans4({ 0.12f, 0.35f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 0.5f, 0.5f, 0.7f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cylinder(0.6f, trans4, texManager.getTextureID(HARV_SIDE))));

			//cylinder obracaj�cy maszyn� do m��cenia
			Transformation trans5({ -1.17f, 0.2f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.5f, 1.5f, 50.0f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cylinder(0.05f, trans5, texManager.getTextureID(HARV_SIDE))));

			//uchwyt na z�by
			Transformation trans8({ -0.82f, -0.1f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 1.0f },
				{ 0.9f, 0.025f, 3.6f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cube(0.7f, trans8, texManager.getTextureID(GRILL_RUST))));

			//lewa �ciana nagarnaicza - cube
			Transformation trans9({ -0.82f, 0.2f, 1.25f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.4f, 1.2f, 0.1f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cube(0.5f, trans9, texManager.getTextureID(HARV_SIDE))));

			//lewa �ciana nagarnaicza - cylinder
			Transformation trans10({ -1.17f, 0.2f, 1.25f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.2f, 1.2f, 0.12f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cylinder(0.5f, trans10, texManager.getTextureID(HARV_SIDE))));

			//prawa �ciana nagarnaicza - cube
			Transformation trans11({ -0.82f, 0.2f, -1.25f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.4f, 1.2f, 0.1f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cube(0.5f, trans11, texManager.getTextureID(HARV_SIDE))));

			//prawa �ciana nagarnaicza - cylinder
			Transformation trans12({ -1.17f, 0.2f, -1.25f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.2f, 1.2f, 0.12f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cylinder(0.5f, trans12, texManager.getTextureID(HARV_SIDE))));

			//big box nadwozie placeholder
			Transformation trans13({ 1.14f, 0.86f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 4.0f, 2.0f, 2.0f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cube(0.5f, trans13, texManager.getTextureID(HARV_SIDE))));

			//pod�oga w kabinie
			Transformation trans14({ -0.1f, 0.8f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 0.15f, 1.0f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cube(0.5f, trans14, texManager.getTextureID(HARV_SIDE))));

			//chyba lewy przedni pr�t kabiny 
			Transformation trans15({ -0.44f, 1.4f, 0.26f },
				18.0f,
				{ 0.15f, 0.0f, 0.5f },
				{ 0.12f, 2.5f, 0.12f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cube(0.5f, trans15, texManager.getTextureID(HARV_SIDE))));

			//chyba prawy przedni pr�t kabiny 
			Transformation trans16({ -0.44f, 1.4f, -0.26f },
				18.0f,
				{ -0.15f, 0.0f, 0.5f },
				{ 0.12f, 2.5f, 0.12f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cube(0.5f, trans16, texManager.getTextureID(HARV_SIDE))));

			//daszek kabiny
			Transformation trans17({ -0.19f, 2.0f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 2.1f, 0.10f, 1.9f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cube(0.5f, trans17, texManager.getTextureID(HARV_SIDE))));

			//chyba prawy tylni pr�t kabiny 
			Transformation trans18({ 0.2f, 1.62f, -0.26f },
				9.0f,
				{ -0.5f, 0.0f, 0.0f },
				{ 0.12f, 1.5f, 0.12f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cube(0.5f, trans18, texManager.getTextureID(HARV_SIDE))));

			//chyba lewy tylni pr�t kabiny 
			Transformation trans19({ 0.2f, 1.62f, 0.26f },
				9.0f,
				{ 0.5f, 0.0f, 0.0f },
				{ 0.12f, 1.5f, 0.12f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cube(0.5f, trans19, texManager.getTextureID(HARV_SIDE))));

			//rura wydechowa - rura
			Transformation trans20({ 1.6f, 1.36f, 0.26f },
				90.0f,
				{ 0.5f, 0.0f, 0.0f },
				{ 0.12f, 0.12f, 0.6f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cylinder(0.5f, trans20, texManager.getTextureID(GRILL_RUST_SHI))));

			//rura wydechowa - puszka na czubku rury
			Transformation trans21({ 1.6f, 1.56f, 0.26f },
				90.0f,
				{ 0.5f, 0.0f, 0.0f },
				{ 0.32f, 0.32f, 0.2f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cylinder(0.5f, trans21, texManager.getTextureID(GRILL_RUST_SHI))));

			//rura wydechowa zbożowa podstawa 1
			Transformation trans222({ 2.5f, 1.0f, 0.0f },
				90.0f,
				{ 2.0f, 0.0f, 0.0f },
				{ 0.15f, 0.15f, 0.4f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cylinder(1.0f, trans222, texManager.getTextureID(HARV_SIDE))));

			//rura wydechowa zbożowa łącznik
			Transformation trans223({ 2.5f, 1.2f, 0.0f },
				90.0f,
				{ 2.0f, 0.0f, 0.0f },
				{ 0.12f, 0.12f, 0.12f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Sphere(1.0f, trans223, texManager.getTextureID(HARV_SIDE))));

			//trans22 i trans23 do animacji obrotu rury zbożowej
			//rura wydechowa zbo�owa the base
			Transformation trans22({ 2.5f + (lengthOfWheatPipe/2.0f), 1.2f, 0.0f },
				90.0f,
				{ 0.0f, 2.0f, 0.0f },
				{ 0.15f, 0.15f, lengthOfWheatPipe });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cylinder(1.0f, trans22, texManager.getTextureID(HARV_SIDE))));

			//rura wydechowa zbo�owa the exit
			Transformation trans23({ 2.5f + lengthOfWheatPipe, 1.175f, 0.0f },
				90.0f,
				{ 2.0f, 0.0f, 0.0f },
				{ 0.20f, 0.2f, 0.20f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cylinder(1.0f, trans23, texManager.getTextureID(GRILL_RUST_HALF))));

			//big box nadwozie placeholder kube� na zbo�e
			Transformation trans24({ 2.64f, 0.66f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 2.0f, 1.0f, 1.5f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cube(0.5f, trans24, texManager.getTextureID(HARV_SIDE))));

			//smaller box podwozie placeholder prz�d
			Transformation trans25({ 0.8f, 0.35f, 0.0f },
				10.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 2.0f, 1.0f, 1.0f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cube(0.5f, trans25, texManager.getTextureID(HARV_SIDE))));

			//o� przednia do k� trajktora
			Transformation trans26({ 0.4f, 0.1f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 1.0f, 40.0f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cylinder(0.03f, trans26, texManager.getTextureID(GRILL))));

			//lewego przedniego ko�a
			Transformation trans27({ 0.4f, 0.1f, 0.6f },
				harvester.wheelRotation,
				{ 0.0f, harvester.wheelAngle + 0.5f, 0.0f},
				{ 1.0f, 1.0f, 1.0f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Wheel(0.5f, trans27, texManager.getTextureID(HARV_SIDE))));

			//prawego przedniego ko�a
			Transformation trans28({ 0.4f, 0.1f, -0.6f },
				harvester.wheelRotation,
				{ 0.0f, harvester.wheelAngle + 0.5f, 0.0f },
				{ 1.0f, 1.0f, 1.0f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Wheel(0.5f, trans28, texManager.getTextureID(HARV_SIDE))));

			//smaller box podwozie placeholder ty�
			Transformation trans29({ 1.8f, 0.35f, 0.0f },
				8.0f,
				{ 0.0f, 0.0f, -3.0f },
				{ 2.4f, 1.0f, 1.0f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cube(0.5f, trans29, texManager.getTextureID(HARV_SIDE))));

			//o� tylnia do k� trokatora
			Transformation trans30({ 2.2f, 0.03f, 0.0f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 1.0f, 1.0f, 30.0f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cylinder(0.03f, trans30, texManager.getTextureID(GRILL))));

			//test lewego tylnego ko�a
			Transformation trans31({ 2.2f, 0.03f, 0.4f },
				harvester.wheelRotation,
				{ 0.0f, harvester.wheelAngle + 0.5f, 0.0f },
				{ 0.7f, 0.7f, 0.7f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Wheel(0.5f, trans31, texManager.getTextureID(HARV_SIDE))));

			//test prawego tylnego ko�a
			Transformation trans32({ 2.2f, 0.03f, -0.4f },
				harvester.wheelRotation,
				{ 0.0f, harvester.wheelAngle + 0.5f, 0.0f },
				{ 0.7f, 0.7f, 0.7f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Wheel(0.5f, trans32, texManager.getTextureID(HARV_SIDE))));

			//pr�t pomi�dzy pod�og� kabiny a najwy�szym schodkiem
			Transformation trans33({ -0.18f, 0.7f, 0.22f },
				18.0f,
				{ -0.15f, 0.0f, 0.5f },
				{ 0.06f, 0.4f, 0.06f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cube(0.5f, trans33, texManager.getTextureID(LADDER))));

			//najwy�szy schodek
			Transformation trans34({ -0.01f, 0.6f, 0.23f },
				0.0f,
				{ 0.0f, 0.0f, 3.0f },
				{ 0.6f, 0.06f, 0.12f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cube(0.5f, trans34, texManager.getTextureID(LADDER))));

			//rurka drabinkowa bli�sza wykaszarce
			Transformation trans35({ -0.1f, 0.37f, 0.23f },
				90.0f,
				{ 0.5f, 0.0f, 0.0f },
				{ 0.12f, 0.12f, 1.6f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cylinder(0.3f, trans35, texManager.getTextureID(LADDER))));

			//rurka drabinkowa bli�sza korpusowi
			Transformation trans36({ 0.1f, 0.37f, 0.23f },
				90.0f,
				{ 0.5f, 0.0f, 0.0f },
				{ 0.12f, 0.12f, 1.6f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cylinder(0.3f, trans36, texManager.getTextureID(LADDER))));

			//schodek drugi od g�ry
			Transformation trans37({ 0.0f, 0.5f, 0.23f },
				90.0f,
				{ 0.0f, 0.5f, 0.0f },
				{ 0.12f, 0.12f, 0.8f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cylinder(0.3f, trans37, texManager.getTextureID(LADDER))));

			//schodek trzeci od g�ry
			Transformation trans38({ 0.0f, 0.4f, 0.23f },
				90.0f,
				{ 0.0f, 0.5f, 0.0f },
				{ 0.12f, 0.12f, 0.8f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cylinder(0.3f, trans38, texManager.getTextureID(LADDER))));

			//schodek czwarty od g�ry
			Transformation trans39({ 0.0f, 0.3f, 0.23f },
				90.0f,
				{ 0.0f, 0.5f, 0.0f },
				{ 0.12f, 0.12f, 0.8f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Cylinder(0.3f, trans39, texManager.getTextureID(LADDER))));

			//drzwi kombonojaju lewe
			Transformation trans40({ -0.06f, 1.0f, 0.22f },
				180.0f,
				{ 0.0f, 0.0f, 0.5f },
				{ 1.6f, 1.2f, 0.12f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Trapezoid(0.3f, trans40, texManager.getTextureID(HARV_SIDE))));


			//drzwi kombonojaju prawe
			Transformation trans41({ -0.06f, 1.0f, -0.22f },
				180.0f,
				{ 0.0f, 0.0f, 0.5f },
				{ 1.6f, 1.2f, 0.12f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Trapezoid(0.3f, trans41, texManager.getTextureID(HARV_SIDE))));

			//drzwi kombonojaju prawe trójkąt
			Transformation trans42({ -0.3f, 1.26f, -0.22f },
				50.0f,
				{ 0.0f, 0.0f, 0.5f },
				{ 0.9f, 2.0f, 0.12f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Triangle(0.3f, trans42, texManager.getTextureID(HARV_SIDE))));

			//drzwi kombonojaju lewe trójkąt
			Transformation trans43({ -0.3f, 1.26f, 0.22f },
				50.0f,
				{ 0.0f, 0.0f, 0.5f },
				{ 0.9f, 2.0f, 0.12f });
			kompoj.addObject(std::shared_ptr<Object>(new 	Triangle(0.3f, trans43, texManager.getTextureID(HARV_SIDE))));

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
					kompoj.addObject(std::shared_ptr<Object>(new 	Cube(1.0f, transformationMlocarka, texManager.getTextureID(GRILL))));
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
				kompoj.addObject(std::shared_ptr<Object>(new 	Cube(1.0f, transformationMlocarkaPoprzeczka, texManager.getTextureID(GRILL))));
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


			//kompoj.rotateWorld(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			

			//global Harvester draw
			harvester.move(deltaTime);
			if(camInHarv)
				camera.Position = harvester.Position + harvester.Front * 0.05f + glm::vec3(0.0f, 1.5f, 0.0f);
			kompoj.rotateWorld(harvester.rotation, glm::vec3(0.0f, 1.0f, 0.0f));
			kompoj.translateWorld(harvester.Position);

			kompoj.draw(shaderProgram);

			skybox.draw(glm::perspective(glm::radians(70.0f), 4.5f / 3.0f, 0.1f, 100.0f), camera.GetViewMatrix());


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

void LoadTextures()
{
	texManager.addTexture(HARV_SIDE);			//0
	texManager.addTexture(LADDER);				//1
	texManager.addTexture(BRICKS);				//2
	texManager.addTexture(WOOD_CONT);			//3
	texManager.addTexture(GRILL);				//4
	texManager.addTexture(GRILL_RUST_HALF);		//5
	texManager.addTexture(GRILL_RUST_SHI);		//6
	texManager.addTexture(GRILL_RUST);			//7
	texManager.addTexture(GROUND);				//8
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && (currentFrame - framePushed) > 0.2f)
		if (camInHarv) {
			camInHarv = false;
			framePushed = currentFrame;		
		}else {
			camInHarv = true;
			framePushed = currentFrame;
		}

	

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		harvester.accelerate(deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		harvester.decelerate(harvester.acceleration * 2.5f, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		harvester.turnLeft(deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		harvester.turnRight(deltaTime);
	
	if (!camInHarv) {
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
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			camera.ProcessKeyboard(DOWN, deltaTime);
	}
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
