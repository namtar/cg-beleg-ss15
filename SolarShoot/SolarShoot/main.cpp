#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>

using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "objects.hpp"
#include "SpaceObject.hpp"

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

// How fast we move (higher values mean we move and strafe faster)
GLfloat movementSpeedFactor = 1.0f;

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

void sendMVP() {
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
		cout << "Key pressed" << endl;
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

		default:
			// Do nothing...
			break;
		}
	}
	else
	{
		cout << "Key release" << endl;
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

	// glMatrixMode(GL_PROJECTION);
	// glLoadIdentity();
	// gluPerspective(45, windowWidth / windowHeight, 1.0, 500.0);
	Projection = glm::perspective(45.0f, (float) windowWidth / (float) windowHeight, 1.0f, 500.0f);
	// glMatrixMode(GL_MODELVIEW);
	glEnable(GL_CULL_FACE); // Do not draw polygons facing away from us
	glEnable(GL_DEPTH_TEST);
	glDepthFunc( GL_LESS );
	glClearDepth(1.0f); // Clear the entire depth of the depth buffer
	// glEnable(GL_LIGHTING);
	// glEnable(GL_LIGHT0);

	// float diffuseMatrix[] = {1.0f, 0.5f, 0.0f, 1.0f}; // 3 colors and alpha. In this a lamp with white color
	// float ambienceMatrix[] = {0.2f, 0.2f, 0.2f, 1.0f};
	// float specularLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// GLint specularMagnitude = 64; // Define how "tight" our specular highlights are (larger number = smaller specular highlight). The valid range is is 1 to 128 

	// glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseMatrix);
	// glLightfv(GL_LIGHT0, GL_AMBIENT, ambienceMatrix);
	// glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight); // reflection light from objects

	// Enable colour tracking of materials
	// glEnable(GL_COLOR_MATERIAL);

	// Define the shininess of the material we'll use to draw things
	// GLfloat materialSpecularReflectance[] = {1.0f, 1.0f, 1.0f, 1.0f};

	// Set Material properties to follow glColor values
	// glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Use our shiny material and magnitude
	// glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecularReflectance);
	// glMateriali(GL_FRONT, GL_SHININESS, specularMagnitude);

	// create space objects
	createSpaceObjects(programID);

	// fps stuff, not the best variance. Better would be to do this in a spearate thread.
	double starttime = glfwGetTime();
	double endtime = starttime;
	double intervalStart = starttime;

	double timeInterval = starttime - endtime;
	int frameCount = 0;

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
		
//		timeInterval = glfwGetTime() - intervalStart;
//		frameCount++;
//		if (timeInterval >= 1.0)
//		{
//			int fps = frameCount / (timeInterval / 1000.0f);
//			printf("FPD: %i\n", fps);
//			intervalStart = glfwGetTime();
//			frameCount = 0;
//		}
		View = glm::lookAt(glm::vec3(-camXPos, -camYPos, -camZPos), // Camera is at (0,0,-5), in World Space
						   glm::vec3(0,0,0),  // and looks at the origin
						   glm::vec3(0,1,0)); // Head is up (set to 0,-1,0 to look upside-down)
		

		drawScene();
		calculateCameraMovement();
		moveCamera();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
}

// Function to draw a grid of lines
void drawGround()
{
	GLfloat extent = 600.0f; // How far on the Z-Axis and X-Axis the ground extends
	GLfloat stepSize = 20.0f; // The size of the separation between points
	GLfloat groundLevel = -50.0f; // Where on the Y-Axis the ground is drawn

	// Set colour to white
	glColor3ub(255, 255, 255);

	// Draw our ground grid
	glBegin(GL_LINES);
	for (GLint loop = -extent; loop < extent; loop += stepSize)
	{
		// Draw lines along Z-Axis
		glVertex3f(loop, groundLevel, extent);
		glVertex3f(loop, groundLevel, -extent);

		// Draw lines across X-Axis
		glVertex3f(-extent, groundLevel, loop);
		glVertex3f(extent, groundLevel, loop);
	}
	glEnd();
}

void drawScene()
{
	// clear screen and set lightposition
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// glLoadIdentity();
//	glLightfv(GL_LIGHT0, GL_POSITION, lightPositionMatrix); // the position must be set right after the loading of the identity matrix			

	// Move the camera to our location in space
	View = glm::mat4(1.0f);
	View = glm::rotate(View, camXRot, glm::vec3(1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, camYRot, glm::vec3(0.0f, 1.0f, 0.0f));
	View = glm::translate(View, glm::vec3(-camXPos, -camYPos, -camZPos));
	sendMVP();
	// glRotatef(camXRot, 1.0f, 0.0f, 0.0f); // Rotate our camera on the x-axis (looking up and down)
	// glRotatef(camYRot, 0.0f, 1.0f, 0.0f); // Rotate our camera on the  y-axis (looking left and right)
	// glTranslatef(-camXPos, -camYPos, -camZPos); // Translate the modelviewm matrix to the position of our camera

//	lightPositionMatrix[0] = (-camXPos);
//	lightPositionMatrix[1] = (-camYPos);
//	lightPositionMatrix[2] = (-camZPos);
//	glLightfv(GL_LIGHT0, GL_POSITION, lightPositionMatrix); // the position must be set right after the loading of the identity matrix

	// drawGround();
//	glPushMatrix();
//
//	glPopMatrix(); 
	// draw sun and set light to this location
	    // Move everything "into" the screen (i.e. move 300 units along the Z-axis into the screen) so that all positions are now relative to the location of the sun
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, sunZLocation));		
	glm::vec4 lightPos = LightTransformation * glm::vec4(0, 0, 0, 1);
//	glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), 0.0f, 0.0f, sunZLocation);
    glUniform3f(glGetUniformLocation(programID, "LightColor"), 1, 1, 0); // yellow light
	glUniform3f(glGetUniformLocation(programID, "ObjectColor"), 1, 1, 0); // draw yellow sun
 
    // Draw the sun (disable lighting so it's always drawn as bright as possible regardless of any lighting going on)
    // glColor3ub(255, 255, 0);
    // glDisable(GL_LIGHTING);
	Model = glm::scale(Model, glm::vec3(10.0f, 10.0f, 10.0f));
	sendMVP();
    drawSphere(35.0f, 35.0f);
// 	glScalef(1.0f, 1.0f, 1.0f);
//   glEnable(GL_LIGHTING);
 
    // Define our light position
    // *** IMPORTANT! *** A light position takes a FOUR component vector! The last component is w! If you leave off the last component, you get NO LIGHT!!!
    // GLfloat newLightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
 
    // glLightfv(GL_LIGHT0, GL_POSITION, newLightPos);  // Place the light where the sun is!

	/* Render here */
	render();
	
}

void render()
{
	spaceObjectMap.find("ship1")->second->draw(View, Projection);

	//glRotatef(angle, 1.0, 1.0, 1.0); // angle, x-axis, y-axis, z-axis
	//glTranslatef(-5.0, 1.0, -25.0);
	//drawCube(2.0);
	//angle += 0.5;
	//angle = fmod(angle, 360.0f);	

	//glTranslatef(0.8, 0.8, -10.0);
	//glScalef(1.0, 1.0, 1.0);
	//drawSphere(1, 1);
}

// Function to calculate which direction we need to move the camera and by what amount
void calculateCameraMovement()
{
	// Break up our movement into components along the X, Y and Z axis
	float camMovementXComponent = 0.0f;
	float camMovementYComponent = 0.0f;
	float camMovementZComponent = 0.0f;

	if (holdingForward == true)
	{
		// Control X-Axis movement
		float pitchFactor = cos(toRads(camXRot));
		camMovementXComponent += (movementSpeedFactor * float(sin(toRads(camYRot)))) * pitchFactor;

		// Control Y-Axis movement
		camMovementYComponent += movementSpeedFactor * float(sin(toRads(camXRot))) * -1.0f;

		// Control Z-Axis movement
		float yawFactor = float(cos(toRads(camXRot)));
		camMovementZComponent += (movementSpeedFactor * float(cos(toRads(camYRot))) * -1.0f) * yawFactor;
	}

	if (holdingBackward == true)
	{
		// Control X-Axis movement
		float pitchFactor = cos(toRads(camXRot));
		camMovementXComponent += (movementSpeedFactor * float(sin(toRads(camYRot))) * -1.0f) * pitchFactor;

		// Control Y-Axis movement
		camMovementYComponent += movementSpeedFactor * float(sin(toRads(camXRot)));

		// Control Z-Axis movement
		float yawFactor = float(cos(toRads(camXRot)));
		camMovementZComponent += (movementSpeedFactor * float(cos(toRads(camYRot)))) * yawFactor;
	}

	if (holdingLeftStrafe == true)
	{
		// Calculate our Y-Axis rotation in radians once here because we use it twice
		float yRotRad = toRads(camYRot);

		camMovementXComponent += -movementSpeedFactor * float(cos(yRotRad));
		camMovementZComponent += -movementSpeedFactor * float(sin(yRotRad));
	}

	if (holdingRightStrafe == true)
	{
		// Calculate our Y-Axis rotation in radians once here because we use it twice
		float yRotRad = toRads(camYRot);

		camMovementXComponent += movementSpeedFactor * float(cos(yRotRad));
		camMovementZComponent += movementSpeedFactor * float(sin(yRotRad));
	}

	// After combining our movements for any & all keys pressed, assign them to our camera speed along the given axis
	camXSpeed = camMovementXComponent;
	camYSpeed = camMovementYComponent;
	camZSpeed = camMovementZComponent;

	// Cap the speeds to our movementSpeedFactor (otherwise going forward and strafing at an angle is twice as fast as just going forward!)
	// X Speed cap
	if (camXSpeed > movementSpeedFactor)
	{
		//cout << "high capping X speed to: " << movementSpeedFactor << endl;
		camXSpeed = movementSpeedFactor;
	}
	if (camXSpeed < -movementSpeedFactor)
	{
		//cout << "low capping X speed to: " << movementSpeedFactor << endl;
		camXSpeed = -movementSpeedFactor;
	}

	// Y Speed cap
	if (camYSpeed > movementSpeedFactor)
	{
		//cout << "low capping Y speed to: " << movementSpeedFactor << endl;
		camYSpeed = movementSpeedFactor;
	}
	if (camYSpeed < -movementSpeedFactor)
	{
		//cout << "high capping Y speed to: " << movementSpeedFactor << endl;
		camYSpeed = -movementSpeedFactor;
	}

	// Z Speed cap
	if (camZSpeed > movementSpeedFactor)
	{
		//cout << "high capping Z speed to: " << movementSpeedFactor << endl;
		camZSpeed = movementSpeedFactor;
	}
	if (camZSpeed < -movementSpeedFactor)
	{
		//cout << "low capping Z speed to: " << movementSpeedFactor << endl;
		camZSpeed = -movementSpeedFactor;
	}
}

void drawTriangles()
{
	float ratio;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float) height;
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-0.6f, -0.4f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.6f, -0.4f, 0.f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.6f, 0.f);
	glEnd();
}

void drawCube(float size)
{
	float difamb[] = {1.0,0.5,0.3,1.0};
	glBegin(GL_QUADS);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, difamb);
	// front face
	glNormal3f(0.0, 0.0, 1.0);
	//              glColor3f(1.0,0.0,0.0);
	glVertex3f(size / 2, size / 2, size / 2);
	glVertex3f(-size / 2, size / 2, size / 2);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glVertex3f(size / 2, -size / 2, size / 2);
	// left face
	glNormal3f(-1.0, 0.0, 0.0);
	//              glColor3f(0.0,1.0,0.0);
	glVertex3f(-size / 2, size / 2, size / 2);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glVertex3f(-size / 2, size / 2, -size / 2);
	// back face
	glNormal3f(0.0, 0.0, -1.0);
	//              glColor3f(0.0,0.0,1.0);
	glVertex3f(size / 2, size / 2, -size / 2);
	glVertex3f(-size / 2, size / 2, -size / 2);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glVertex3f(size / 2, -size / 2, -size / 2);
	// right face
	glNormal3f(1.0, 0.0, 0.0);
	//              glColor3f(1.0,1.0,0.0);
	glVertex3f(size / 2, size / 2, size / 2);
	glVertex3f(size / 2, -size / 2, size / 2);
	glVertex3f(size / 2, -size / 2, -size / 2);
	glVertex3f(size / 2, size / 2, -size / 2);
	// top face
	glNormal3f(0.0, 1.0, 0.0);
	//              glColor3f(1.0,0.0,1.0);
	glVertex3f(size / 2, size / 2, size / 2);
	glVertex3f(-size / 2, size / 2, size / 2);
	glVertex3f(-size / 2, size / 2, -size / 2);
	glVertex3f(size / 2, size / 2, -size / 2);
	// bottom face
	glNormal3f(0.0, -1.0, 0.0);
	//              glColor3f(0.0,1.0,1.0);
	glVertex3f(size / 2, -size / 2, size / 2);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glVertex3f(size / 2, -size / 2, -size / 2);
	glEnd();
}

void createSpaceObjects(GLuint programID)
{
	SpaceObject* ship1 = new SpaceObject(programID);
	ship1->translate(-5.0f, -2.0f, -15.0f);
	spaceObjectMap.insert(std::pair<string, SpaceObject*>("ship1", ship1));
}
