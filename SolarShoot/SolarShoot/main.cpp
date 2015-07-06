#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <sstream>
#include <string>

using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "objects.hpp"
#include "SpaceObject.hpp"
#include "Skybox.h"

// http://r3dux.org/2011/05/simple-opengl-keyboard-and-mouse-fps-controls/
// http://r3dux.org/2012/12/a-c-camera-class-for-simple-opengl-fps-controls/

// Globals

// http://kengine.sourceforge.net/tutorial/g/stdmap-eng.htm
typedef std::map<std::string, SpaceObject*> SpaceObjectMap;
SpaceObjectMap spaceObjectMap;

const float TO_RADS = 3.141592654f / 180.0f; // The value of 1 degree in radians

GLFWwindow* window;
glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;
GLuint programID;
CSkybox skybox;

float angle = 0.0;
int windowWidth = 1024;
int windowHeight = 768;
int midWindowWidth = windowWidth / 2;
int midWindowHeight = windowHeight / 2;

float lightPositionMatrix[] = {-2.0f, 2.0f, -3.0f, 1.0f};

// Location of the sun (i.e. how far deep into the screen is it?)
GLfloat sunZLocation = -300.0f;
glm::mat4 LightTransformation;

// Camera rotation
GLfloat camXRot = 0.0f;
GLfloat camYRot = 0.0f;
GLfloat camZRot = 0.0f;

// Camera position
GLfloat camXPos = 0.0f;
GLfloat camYPos = 0.0f;
GLfloat camZPos = 0.0f;

// Camera movement speed
GLfloat camXSpeed = 0.0f;
GLfloat camYSpeed = 0.0f;
GLfloat camZSpeed = 0.0f;

// Hoding any keys down?
bool holdingForward = false;
bool holdingBackward = false;
bool holdingLeftStrafe = false;
bool holdingRightStrafe = false;
bool holdingLeftShift = false;
bool holdingLeftCtrl = false;


// How fast we move (higher values mean we move and strafe faster)
GLfloat movementSpeedFactor = 1.0f;
GLfloat sprintAddValue = 1.0f; // shall be added to the basic movement factor
GLfloat stepSubstractValue = 0.8f; // shall be removed from the base movement factor

// Method Stubs
void run();
void render();
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void drawGround();
void drawScene();
void calculateCameraMovement();
void createSpaceObjects(GLuint programID);

void drawTriangles();
void drawCube(float size);

// top methods

void sendMVP()
{
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model;
	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform, konstant fuer alle Eckpunkte
	glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "P"), 1, GL_FALSE, &Projection[0][0]);
}

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

// Function to convert degrees to radians
float toRads(const float& theAngleInDegrees)
{
	return theAngleInDegrees * TO_RADS;
}

// Function to move the camera the amount we've calculated in the calculateCameraMovement function
void moveCamera()
{
	camXPos += camXSpeed;
	camYPos += camYSpeed;
	camZPos += camZSpeed;
}

// Function to deal with mouse position changes, called whenever the mouse cursorm moves
void handleMouseMove(GLFWwindow* window, double mouseX, double mouseY)
{
	GLfloat vertMouseSensitivity = 10.0f;
	GLfloat horizMouseSensitivity = 10.0f;

	//cout << "Mouse cursor is at position (" << mouseX << ", " << mouseY << endl;

	int horizMovement = mouseX - midWindowWidth;
	int vertMovement = mouseY - midWindowHeight;

	camXRot += vertMovement / vertMouseSensitivity;
	camYRot += horizMovement / horizMouseSensitivity;

	// Control looking up and down with the mouse forward/back movement
	// Limit loking up to vertically up
	if (camXRot < -90.0f)
	{
		camXRot = -90.0f;
	}

	// Limit looking down to vertically down
	if (camXRot > 90.0f)
	{
		camXRot = 90.0f;
	}

	// Looking left and right. Keep the angles in the range -180.0f (anticlockwise turn looking behind) to 180.0f (clockwise turn looking behind)
	if (camYRot < -180.0f)
	{
		camYRot += 360.0f;
	}

	if (camYRot > 180.0f)
	{
		camYRot -= 360.0f;
	}

	// Reset the mouse position to the centre of the window each frame
	glfwSetCursorPos(window, midWindowWidth, midWindowHeight);
}

// main

int main(void)
{
	/* Initialize the library */
	if (!glfwInit())
		return EXIT_FAILURE;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(windowWidth, windowHeight, "Solar Shoot", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	//	glewExperimental = true; // Needed for core profile

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return EXIT_FAILURE;
	}

	// GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);	
	// glfwSetCursor(window, cursor);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetErrorCallback(error_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	glfwSetKeyCallback(window, key_callback);
	// Specify the function which should execute when the mouse is moved
	glfwSetCursorPos(window, midWindowWidth, midWindowHeight);
	glfwSetCursorPosCallback(window, handleMouseMove);

	// finally run program
	run();

	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}

// method stub impls

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_W:
			holdingForward = true;
			break;

		case GLFW_KEY_S:
			holdingBackward = true;
			break;

		case GLFW_KEY_A:
			holdingLeftStrafe = true;
			break;

		case GLFW_KEY_D:
			holdingRightStrafe = true;
			break;

		case GLFW_KEY_LEFT_SHIFT:
			holdingLeftShift = true;
			break;

		case GLFW_KEY_LEFT_CONTROL:
			holdingLeftCtrl = true;
			break;

		default:
			// Do nothing...
			break;
		}
	}
	else
	{
		switch (key)
		{
		case GLFW_KEY_W:
			holdingForward = false;
			break;

		case GLFW_KEY_S:
			holdingBackward = false;
			break;

		case GLFW_KEY_A:
			holdingLeftStrafe = false;
			break;

		case GLFW_KEY_D:
			holdingRightStrafe = false;
			break;

		case GLFW_KEY_LEFT_SHIFT:
			holdingLeftShift = false;
			break;

		case GLFW_KEY_LEFT_CONTROL:
			holdingLeftCtrl = false;
			break;

		default:
			// Do nothing...
			break;
		}
	}
}

void run()
{
	// load shaders
	programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	glClearColor(0.0, 0.0, 0.8, 1.0); //background color and alpha
	// Shader auch benutzen !
	glUseProgram(programID);

	Projection = glm::perspective(45.0f, (float) windowWidth / (float) windowHeight, 1.0f, 50000.0f);
	// glMatrixMode(GL_MODELVIEW);
	glEnable(GL_CULL_FACE); // Do not draw polygons facing away from us
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearDepth(1.0f); // Clear the entire depth of the depth buffer

	// create space objects
	createSpaceObjects(programID);

	// fps stuff, not the best variance. Better would be to do this in a spearate thread.
	double starttime = glfwGetTime();
	double endtime = starttime;

	skybox.loadSkybox("material\\skyboxes\\jajlands2\\", "jajlands1_ft.bmp", "jajlands1_bk.bmp", "jajlands1_lf.bmp", "jajlands1_rt.bmp", "jajlands1_up.bmp", "jajlands1_dn.bmp");

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// limit fps to 60
		starttime = glfwGetTime();
		endtime = starttime;
		while (endtime - starttime < 1.0 / 60)
		{
			endtime = glfwGetTime();
		}

		View = glm::lookAt(glm::vec3(-camXPos, -camYPos, -camZPos), // Camera is at (0,0,-5), in World Space
		                   glm::vec3(0, 0, 0), // and looks at the origin
		                   glm::vec3(0, 1, 0)); // Head is up (set to 0,-1,0 to look upside-down)


		drawScene();
		calculateCameraMovement();
		moveCamera();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
}

void drawScene()
{	
	// Move the camera to our location in space
	View = glm::mat4(1.0f);
	View = glm::rotate(View, camXRot, glm::vec3(1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, camYRot, glm::vec3(0.0f, 1.0f, 0.0f));
	View = glm::translate(View, glm::vec3(-camXPos, -camYPos, -camZPos));
	sendMVP();
	skybox.renderSkybox();

	// draw sun and set light to this location
	// Move everything "into" the screen (i.e. move 300 units along the Z-axis into the screen) so that all positions are now relative to the location of the sun
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, sunZLocation));
	glm::vec4 lightPos = LightTransformation * glm::vec4(0, 0, 0, 1);
	//	glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), 0.0f, 0.0f, sunZLocation);
	glUniform3f(glGetUniformLocation(programID, "LightColor"), 1, 1, 1); // yellow light
	glUniform3f(glGetUniformLocation(programID, "ObjectColor"), 1, 1, 0); // draw yellow sun

	// Draw the sun
	Model = glm::scale(Model, glm::vec3(10.0f, 10.0f, 10.0f));
	sendMVP();
	drawSphere(35.0f, 35.0f);
	
	/* Render here */
	render();
}

void render()
{

//	spaceObjectMap.find("ship1")->second->draw(View, Projection);
//	typedef std::map<std::string, SpaceObject*>::iterator it_type;
	for(auto iterator = spaceObjectMap.begin(); iterator != spaceObjectMap.end(); iterator++)
	{
		//iterator->second->rotate(angle, 1.0f, 1.0f, 1.0f);
		iterator->second->translate(-0.5f, 1.0f, -25.0f);		

		iterator->second->draw(View, Projection);
	}
	/*angle += 0.5f;
	angle = fmod(angle, 360.0f);*/
}

// Function to calculate which direction we need to move the camera and by what amount
void calculateCameraMovement()
{
	// Break up our movement into components along the X, Y and Z axis
	float camMovementXComponent = 0.0f;
	float camMovementYComponent = 0.0f;
	float camMovementZComponent = 0.0f;

	GLfloat calculatedMovementSpeedFactor = movementSpeedFactor;
	if (holdingLeftShift && !holdingLeftCtrl)
	{
		// sprint
		calculatedMovementSpeedFactor += sprintAddValue;
	}
	else if (!holdingLeftShift && holdingLeftCtrl)
	{
		calculatedMovementSpeedFactor -= stepSubstractValue;
	}

	if (holdingForward == true)
	{
		// Control X-Axis movement
		float pitchFactor = cos(toRads(camXRot));
		camMovementXComponent += (calculatedMovementSpeedFactor * float(sin(toRads(camYRot)))) * pitchFactor;

		// Control Y-Axis movement
		camMovementYComponent += calculatedMovementSpeedFactor * float(sin(toRads(camXRot))) * -1.0f;

		// Control Z-Axis movement
		float yawFactor = float(cos(toRads(camXRot)));
		camMovementZComponent += (calculatedMovementSpeedFactor * float(cos(toRads(camYRot))) * -1.0f) * yawFactor;
	}

	if (holdingBackward == true)
	{
		// Control X-Axis movement
		float pitchFactor = cos(toRads(camXRot));
		camMovementXComponent += (calculatedMovementSpeedFactor * float(sin(toRads(camYRot))) * -1.0f) * pitchFactor;

		// Control Y-Axis movement
		camMovementYComponent += calculatedMovementSpeedFactor * float(sin(toRads(camXRot)));

		// Control Z-Axis movement
		float yawFactor = float(cos(toRads(camXRot)));
		camMovementZComponent += (calculatedMovementSpeedFactor * float(cos(toRads(camYRot)))) * yawFactor;
	}

	if (holdingLeftStrafe == true)
	{
		// Calculate our Y-Axis rotation in radians once here because we use it twice
		float yRotRad = toRads(camYRot);

		camMovementXComponent += -calculatedMovementSpeedFactor * float(cos(yRotRad));
		camMovementZComponent += -calculatedMovementSpeedFactor * float(sin(yRotRad));
	}

	if (holdingRightStrafe == true)
	{
		// Calculate our Y-Axis rotation in radians once here because we use it twice
		float yRotRad = toRads(camYRot);

		camMovementXComponent += calculatedMovementSpeedFactor * float(cos(yRotRad));
		camMovementZComponent += calculatedMovementSpeedFactor * float(sin(yRotRad));
	}

	// After combining our movements for any & all keys pressed, assign them to our camera speed along the given axis
	camXSpeed = camMovementXComponent;
	camYSpeed = camMovementYComponent;
	camZSpeed = camMovementZComponent;

	// Cap the speeds to our calculatedMovementSpeedFactor (otherwise going forward and strafing at an angle is twice as fast as just going forward!)
	// X Speed cap
	if (camXSpeed > calculatedMovementSpeedFactor)
	{
		//cout << "high capping X speed to: " << calculatedMovementSpeedFactor << endl;
		camXSpeed = calculatedMovementSpeedFactor;
	}
	if (camXSpeed < -calculatedMovementSpeedFactor)
	{
		//cout << "low capping X speed to: " << calculatedMovementSpeedFactor << endl;
		camXSpeed = -calculatedMovementSpeedFactor;
	}

	// Y Speed cap
	if (camYSpeed > calculatedMovementSpeedFactor)
	{
		//cout << "low capping Y speed to: " << calculatedMovementSpeedFactor << endl;
		camYSpeed = calculatedMovementSpeedFactor;
	}
	if (camYSpeed < -calculatedMovementSpeedFactor)
	{
		//cout << "high capping Y speed to: " << calculatedMovementSpeedFactor << endl;
		camYSpeed = -calculatedMovementSpeedFactor;
	}

	// Z Speed cap
	if (camZSpeed > calculatedMovementSpeedFactor)
	{
		//cout << "high capping Z speed to: " << calculatedMovementSpeedFactor << endl;
		camZSpeed = calculatedMovementSpeedFactor;
	}
	if (camZSpeed < -calculatedMovementSpeedFactor)
	{
		//cout << "low capping Z speed to: " << calculatedMovementSpeedFactor << endl;
		camZSpeed = -calculatedMovementSpeedFactor;
	}
}

void createSpaceObjects(GLuint programID)
{
	//	SpaceObject* ship1 = new SpaceObject(programID, "material/mandrill.bmp", "material/teapot.obj");
	//	SpaceObject* ship1 = new SpaceObject(programID, "material/mandrill.bmp", "material/SpaceShip.obj");
	//	ship1->translate(-5.0f, -2.0f, sunZLocation + 100);
	//	ship1->scale(1.0 / 500.0, 1.0 / 500.0, 1.0 / 500.0);
	//	spaceObjectMap.insert(std::pair<string, SpaceObject*>("ship1", ship1));
	glm::vec3 positions[5] = {};
	positions[0] = glm::vec3(-5.0f, -2.0f, sunZLocation + 100);
	positions[1] = glm::vec3(-15.0f, -2.0f, sunZLocation + 100);
	positions[2] = glm::vec3(-30.0f, -2.0f, sunZLocation + 100);
	positions[3] = glm::vec3(-50.0f, -2.0f, sunZLocation + 100);
	positions[4] = glm::vec3(-80.0f, -2.0f, sunZLocation + 100);

	// create 5 ships
	for (int i = 0; i < 5; i++)
	{
		glm::vec3 locationVec = positions[i];

		SpaceObject* ship = new SpaceObject(programID, "material/mandrill.bmp", "material/teapot.obj");
		ship->translate(locationVec[0], locationVec[1], locationVec[2]);
		ship->scale(1.0 / 500.0, 1.0 / 500.0, 1.0 / 500.0);
		
		std::stringstream ss;
		ss << "ship" << (i+1);
		spaceObjectMap.insert(std::pair<string, SpaceObject*>(ss.str(), ship));
	}
	
}
