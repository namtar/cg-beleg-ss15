#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"

// Globals
GLFWwindow* window;
float angle = 0.0;
int windowWidth = 1024;
int windowHeight = 768;

// Method Stubs
void run();
void render();
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void drawTriangles();
void drawCube(float size);

// top methods

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
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

	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);

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
}

void run()
{
	// load shaders
	GLuint programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	glClearColor(0.0, 0.0, 0.0, 1.0); //background color and alpha
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, windowWidth / windowHeight, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float lightPositionMatrix[] = {-2.0f, 2.0f, -3.0f, 1.0f};
	float diffuseMatrix[] = {1.0f, 0.0f, 0.0f, 1.0f}; // 3 colors and alpha. In this a lamp with white color
	float ambienceMatrix[] = {0.2f, 0.2f, 0.2f, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseMatrix);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambienceMatrix);

	// fps stuff, not the best variance. Better would be to do this in a spearate thread.
	double starttime = glfwGetTime();
	double endtime = starttime;	
	double intervalStart = starttime;	

	double timeInterval = starttime - endtime;
	int frameCount = 0;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{		
		starttime = glfwGetTime();
		endtime = starttime;
		while (endtime - starttime < 1.0 / 60)
		{
			endtime = glfwGetTime();
		}
		timeInterval = glfwGetTime() - intervalStart;
		frameCount++;
		if(timeInterval >= 1.0)
		{
			int fps = frameCount / (timeInterval / 1000.0f);			
			printf("FPD: %i\n", fps);
			intervalStart = glfwGetTime();
			frameCount = 0;
		}


		// clear screen and set lightposition
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glLightfv(GL_LIGHT0, GL_POSITION, lightPositionMatrix); // the position must be set right after the loading of the identity matrix			

		/* Render here */
		render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
}

void render()
{
	glTranslatef(0.0, 0.0, -5.0);
	glRotatef(angle, 1.0, 1.0, 1.0); // angle, x-axis, y-axis, z-axis
	drawCube(1.0);
	angle += 0.5;
	angle = fmod(angle, 360.0f);
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
