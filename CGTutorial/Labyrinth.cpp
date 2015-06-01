#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <list>
//OpenGL includes
#include "GL/glew.h" 
#include "GLFW/glfw3.h" 
#include "glm/glm.hpp" 
#include "glm/gtc/matrix_transform.hpp"
//
#include "shader.hpp" 
#include "objloader.hpp" 
#include "texture.hpp" 
#include "Objekt.hpp"
#include "Physics.hpp"

using namespace glm;

const int screenWidth = 1024;
const int screenHeight = 768;
double mouseXPos = (double) screenWidth / 2;
double mouseYPos = (double) screenHeight / 2;


bool foreward = true;
bool backward = false;
bool moveable = false;

bool doorOne = false;
bool doorTwo = false;
bool doorOpenOne = false;
bool doorOpenTwo = false;

int figuren = 0;

float yfigur1 = -0.5;
float yfigur2 = -0.5;
float yfigur3 = -0.5;
float yfigur4 = -0.5;

int tuerZ1 = -22;
int tuerZ2 = -3;

float zpos = 0, xrot = 0, ydrehen = 0;
mat4 lightTrf(1.0);
mat4 View;
//Position Kamera
glm::vec3 vUp(0.0f, 1.0f, 0.0f);
glm::vec3 vEye(15.0f, 0.0f, 11.0f);
glm::vec3 vView(14.5f, 0.0f, 11.0f);

void run(GLFWwindow* window);
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void mouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos);

//Steuerung
void rotateViewY(float fAngle);
void move(float fBy);
void setupLight();


int main(void)
{
	if (!glfwInit()) // Initialise GLFW und Fehlerausgabe
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	glfwSetErrorCallback(error_callback);
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "3d Cave", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	//glewExperimental = true; 

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// callbacks for keyboard and mouse
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, mouseCursorPosCallback);
	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);	
	glfwSetCursor(window, cursor);

	run(window);
	return 0;
}

void run(GLFWwindow* window)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//Shader laden
	GLuint programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	//GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//Shader benutzen
	glUseProgram(programID);

	setupLight();

	//anlegen der Objekte
	Objekt figur = Objekt("spongebob_bind.obj", "mauer.bmp", programID);

	Objekt figur1 = Objekt("spongebob_bind.obj", "mauer.bmp", programID);
	Objekt figur2 = Objekt("spongebob_bind.obj", "mauer.bmp", programID);
	Objekt figur3 = Objekt("teapot.obj", "mauer.bmp", programID);
	Objekt figur4 = Objekt("teapot.obj", "mauer.bmp", programID);

	Objekt boden = Objekt("mauer.bmp", programID);

	Objekt decke = Objekt("mauer.bmp", programID);

	Objekt wand1 = Objekt("mauer.bmp", programID);
	Objekt wand2 = Objekt("mauer.bmp", programID);
	Objekt wand3 = Objekt("mauer.bmp", programID);
	Objekt wand4 = Objekt("mauer.bmp", programID);
	Objekt wand5 = Objekt("mauer.bmp", programID);
	Objekt wand6 = Objekt("mauer.bmp", programID);
	Objekt wand7 = Objekt("mauer.bmp", programID);
	Objekt wand8 = Objekt("mauer.bmp", programID);
	Objekt wand9 = Objekt("mauer.bmp", programID);
	Objekt wand10 = Objekt("mauer.bmp", programID);
	Objekt wand11 = Objekt("mauer.bmp", programID);

	Objekt wand14 = Objekt("mauer.bmp", programID);
	Objekt wand15 = Objekt("mauer.bmp", programID);
	Objekt wand16 = Objekt("mauer.bmp", programID);
	Objekt wand17 = Objekt("mauer.bmp", programID);
	Objekt wand18 = Objekt("mauer.bmp", programID);
	Objekt wand19 = Objekt("mauer.bmp", programID);
	Objekt wand20 = Objekt("mauer.bmp", programID);
	Objekt wand21 = Objekt("mauer.bmp", programID);
	Objekt wand22 = Objekt("mauer.bmp", programID);
	Objekt wand23 = Objekt("mauer.bmp", programID);
	Objekt wand24 = Objekt("mauer.bmp", programID);
	Objekt wand25 = Objekt("mauer.bmp", programID);
	Objekt wand26 = Objekt("mauer.bmp", programID);
	Objekt wand27 = Objekt("mauer.bmp", programID);
	Objekt wand28 = Objekt("mauer.bmp", programID);
	Objekt wand29 = Objekt("mauer.bmp", programID);
	Objekt wand30 = Objekt("mauer.bmp", programID);
	Objekt wand31 = Objekt("mauer.bmp", programID);
	Objekt wand32 = Objekt("mauer.bmp", programID);

	Objekt wand33 = Objekt("mauer.bmp", programID);
	Objekt wand34 = Objekt("mauer.bmp", programID);
	Objekt wand35 = Objekt("mauer.bmp", programID);
	Objekt wand36 = Objekt("mauer.bmp", programID);

	Objekt wand37 = Objekt("mauer.bmp", programID);
	Objekt wand38 = Objekt("mauer.bmp", programID);
	Objekt wand39 = Objekt("mauer.bmp", programID);
	Objekt wand40 = Objekt("mauer.bmp", programID);
	Objekt wand41 = Objekt("mauer.bmp", programID);
	Objekt wand42 = Objekt("mauer.bmp", programID);
	Objekt wand43 = Objekt("mauer.bmp", programID);
	Objekt wand44 = Objekt("mauer.bmp", programID);
	Objekt wand45 = Objekt("mauer.bmp", programID);
	Objekt wand46 = Objekt("mauer.bmp", programID);
	Objekt wand47 = Objekt("mauer.bmp", programID);

	Objekt wand48 = Objekt("mauer.bmp", programID);
	Objekt wand49 = Objekt("mauer.bmp", programID);
	Objekt wand50 = Objekt("mauer.bmp", programID);
	Objekt wand51 = Objekt("mauer.bmp", programID);
	Objekt wand52 = Objekt("mauer.bmp", programID);

	Objekt lampe = Objekt("mauer.bmp", programID);

	Physics p = Physics();

	cout << "Finde die fuenf Figuren im Labyrinth\n";
	cout << "Drei sind in dem Labyrinth versteckt, fuer die Fuenfte musst du ";
	cout << "erst die vier anderen finden damit du die Wand verschieben kannst.\n\n";
	cout << "Bewegen kannst du dich mit\n w fuer foreward\n a links drehen\n d rechts drehen.\n";
	cout << "Um die Waende zu verschieben musst so nahm wie \nmoeglich an die Wand ran laufen und e druecken.\n\n";

	while (!glfwWindowShouldClose(window))
	{
		std::vector<vector<float>> kollisionsListe;
		std::vector<vector<float>> kollisionsListeBeweglicherObjekte;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; //Color(0.2, 0.2, 0.2)
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

		GLfloat lightColor0[] = {1.0f, 1.0f, 1.0f, 1.0f}; //Color (0.5, 0.5, 0.5)
		GLfloat lightPos0[] = {1.0f, 1.0f, 0.0f, 1.0f}; //Positioned at (4, 0, 8)
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
		glEnable(GL_COLOR_MATERIAL) ;
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE) ;

		mat4 Projection = glm::perspective(40.0f, 4.0f / 3.0f, 0.1f, 1000.0f);

		View = glm::lookAt(glm::vec3(vEye), glm::vec3(vView), glm::vec3(vUp));

		lampe.lampeBewegen(vView.x, vView.y, vView.z, ydrehen, lightTrf);

		figur.translate_skalieren(-15, 0, -24, 1, 1, 1);
		figur.objekt_drehen(0, 90, 0);
		figur.anlegen(Projection, View);

		figur1.translate_skalieren(-17, yfigur1, -6.3, 1, 1, 1);
		figur1.objekt_drehen(0, 90, 0);
		figur1.anlegen(Projection, View);

		figur2.translate_skalieren(-7, yfigur2, 12, 1, 1, 1);
		figur2.objekt_drehen(0, 90, 0);
		figur2.anlegen(Projection, View);

		figur3.translate_skalieren(27, yfigur3, 10);
		figur3.objekt_drehen(-90, 0, 0);
		figur3.anlegen(Projection, View);

		figur4.translate_skalieren(27, yfigur4, -24);
		figur4.objekt_drehen(-90, 0, 0);
		figur4.anlegen(Projection, View);

		boden.translate_skalieren(10, -1, -10, 40, 0.01, 35);
		boden.quadrat_anlegen(Projection, View);

		decke.translate_skalieren(10, 3, -10, 40, 0.01, 35);
		decke.quadrat_anlegen(Projection, View);

		//untere Wand
		wand1.translate_skalieren(0, 0, 20, 10, 3, 3);
		wand1.quadrat_anlegen(Projection, View);

		wand2.translate_skalieren(20, 0, 20, 10, 3, 3);
		wand2.quadrat_anlegen(Projection, View);

		wand3.translate_skalieren(-20, 0, 20, 10, 3, 3);
		wand3.quadrat_anlegen(Projection, View);

		//linke wand
		wand4.translate_skalieren(-30, 0, 10, 3, 3, 10);
		wand4.quadrat_anlegen(Projection, View);

		wand5.translate_skalieren(-30, 0, -10, 3, 3, 10);
		wand5.quadrat_anlegen(Projection, View);

		wand6.translate_skalieren(-30, 0, -30, 3, 3, 10);
		wand6.quadrat_anlegen(Projection, View);

		wand7.translate_skalieren(-7.5, 0, -28, 2.5, 3, 1);
		wand7.quadrat_anlegen(Projection, View);

		wand8.translate_skalieren(0, 0, -37, 10, 3, 3);
		wand8.quadrat_anlegen(Projection, View);

		wand9.translate_skalieren(20, 0, -34, 10, 3, 3);
		wand9.quadrat_anlegen(Projection, View);

		wand10.translate_skalieren(-20, 0, -37, 10, 3, 3);
		wand10.quadrat_anlegen(Projection, View);

		wand11.translate_skalieren(40, 0, -30, 10, 3, 3);
		wand11.quadrat_anlegen(Projection, View);

		//rechts wand
		wand14.translate_skalieren(42, 0, 10, 3, 3, 10);
		wand14.quadrat_anlegen(Projection, View);

		wand15.translate_skalieren(47, 0, -10, 3, 3, 10);
		wand15.quadrat_anlegen(Projection, View);

		wand16.translate_skalieren(50, 0, -30, 3, 3, 10);
		wand16.quadrat_anlegen(Projection, View);

		wand18.translate_skalieren(40, 0, 20, 10, 3, 3);
		wand18.quadrat_anlegen(Projection, View);

		// innenraum
		//U_Form
		wand21.translate_skalieren(-12, 0, 10, 10, 3, 1);
		wand21.quadrat_anlegen(Projection, View);

		wand22.translate_skalieren(-12, 0, 14, 10, 3, 1);
		wand22.quadrat_anlegen(Projection, View);

		wand23.translate_skalieren(-3, 0, 12, 1, 3, 1);
		wand23.quadrat_anlegen(Projection, View);
		//-Form
		wand24.translate_skalieren(-12, 0, 6, 10, 3, 1);
		wand24.quadrat_anlegen(Projection, View);

		//////////////////////////////////////////////////////////////////////////////////////////ROT1
		//i
		wand28.translate_skalieren(-23, 0, -5, 1, 3, 6);
		wand28.quadrat_anlegen(Projection, View);
		//-
		wand27.translate_skalieren(-12, 0, 2, 12, 3, 1);
		wand27.quadrat_anlegen(Projection, View);
		//i
		wand26.translate_skalieren(1, 0, 7, 1, 3, 6);
		wand26.quadrat_anlegen(Projection, View);
		//-
		wand25.translate_skalieren(10, 0, 14, 10, 3, 1);
		wand25.quadrat_anlegen(Projection, View);
		//i
		wand29.translate_skalieren(19, 0, 10, 1, 3, 3);
		wand29.quadrat_anlegen(Projection, View);
		//-
		wand30.translate_skalieren(13, 0, 8, 5, 3, 1);
		wand30.quadrat_anlegen(Projection, View);
		//i
		wand31.translate_skalieren(7, 0, 3, 1, 3, 6);
		wand31.quadrat_anlegen(Projection, View);
		//-
		wand32.translate_skalieren(18, 0, -4, 12, 3, 1);
		wand32.quadrat_anlegen(Projection, View);

		//////////////////////////////////////////////////////////////////////////////////////////////gelb2
		//i
		wand33.translate_skalieren(23, 0, 10, 1, 3, 3);
		wand33.quadrat_anlegen(Projection, View);
		//-
		wand34.translate_skalieren(27, 0, 14, 5, 3, 1);
		wand34.quadrat_anlegen(Projection, View);
		//i
		wand17.translate_skalieren(31, 0, 8, 1, 3, 6);
		wand17.quadrat_anlegen(Projection, View);
		//o
		wand35.translate_skalieren(22, 0, 1, 6, 3, 2);
		wand35.quadrat_anlegen(Projection, View);


		//////////////////////////////////////////////////////////////////////////////////////////////////////////orange3
		//i
		wand37.translate_skalieren(36, 0, -4, 1, 3, 3);
		wand37.quadrat_anlegen(Projection, View);
		//-
		wand36.translate_skalieren(22, 0, -8, 20, 3, 1);
		wand36.quadrat_anlegen(Projection, View);
		//i
		wand38.translate_skalieren(1, 0, -6, 1, 3, 3);
		wand38.quadrat_anlegen(Projection, View);
		//-
		wand39.translate_skalieren(-9, 0, -4, 9, 3, 1);
		wand39.quadrat_anlegen(Projection, View);
		//i
		wand40.translate_skalieren(-19, 0, -6, 1, 3, 3);
		wand40.quadrat_anlegen(Projection, View);
		//-
		wand41.translate_skalieren(-13, 0, -8, 5, 3, 1);
		wand41.quadrat_anlegen(Projection, View);
		//i
		wand42.translate_skalieren(-9, 0, -12, 1, 3, 3);
		wand42.quadrat_anlegen(Projection, View);
		//-
		wand43.translate_skalieren(-15, 0, -16, 7, 3, 1);
		wand43.quadrat_anlegen(Projection, View);
		//i
		wand44.translate_skalieren(-23, 0, -21, 1, 3, 6);
		wand44.quadrat_anlegen(Projection, View);
		//-
		wand45.translate_skalieren(-17, 0, -28, 7, 3, 1);
		wand45.quadrat_anlegen(Projection, View);


		/////////////////////////////////////////////////////////////////////
		//i
		wand46.translate_skalieren(8, 0, -23, 1, 3, 4);
		wand46.quadrat_anlegen(Projection, View);
		//o
		wand47.translate_skalieren(35, 0, 7, 1, 3, 4);
		wand47.quadrat_anlegen(Projection, View);
		//-
		wand48.translate_skalieren(2, 0, -28, 7, 3, 1);
		wand48.quadrat_anlegen(Projection, View);

		//-
		wand49.translate_skalieren(22, 0, -18, 19, 3, 1);
		wand49.quadrat_anlegen(Projection, View);
		//o
		wand50.translate_skalieren(22, 0, -24, 3, 3, 3);
		wand50.quadrat_anlegen(Projection, View);

		//bewegliche Objekte
		//i
		wand51.translate_skalieren(-9, 0, tuerZ1, 1, 3, 5);
		wand51.quadrat_anlegen(Projection, View);
		//i
		wand52.translate_skalieren(31, 0, tuerZ2, 1, 3, 5);
		wand52.quadrat_anlegen(Projection, View);

		//Liste von Objekte mit den eine Kollision überprüft werden soll
		kollisionsListe.push_back(wand1.pos());
		kollisionsListe.push_back(wand2.pos());
		kollisionsListe.push_back(wand3.pos());
		kollisionsListe.push_back(wand4.pos());
		kollisionsListe.push_back(wand5.pos());
		kollisionsListe.push_back(wand6.pos());
		kollisionsListe.push_back(wand7.pos());
		kollisionsListe.push_back(wand8.pos());
		kollisionsListe.push_back(wand9.pos());
		kollisionsListe.push_back(wand10.pos());
		kollisionsListe.push_back(wand11.pos());
		kollisionsListe.push_back(wand14.pos());
		kollisionsListe.push_back(wand15.pos());
		kollisionsListe.push_back(wand16.pos());

		kollisionsListe.push_back(wand18.pos());
		kollisionsListe.push_back(wand19.pos());
		kollisionsListe.push_back(wand20.pos());
		kollisionsListe.push_back(wand21.pos());
		kollisionsListe.push_back(wand22.pos());
		kollisionsListe.push_back(wand23.pos());
		kollisionsListe.push_back(wand24.pos());
		kollisionsListe.push_back(wand25.pos());
		kollisionsListe.push_back(wand26.pos());
		kollisionsListe.push_back(wand27.pos());
		kollisionsListe.push_back(wand28.pos());
		kollisionsListe.push_back(wand29.pos());
		kollisionsListe.push_back(wand30.pos());

		kollisionsListe.push_back(wand31.pos());
		kollisionsListe.push_back(wand32.pos());
		kollisionsListe.push_back(wand33.pos());
		kollisionsListe.push_back(wand34.pos());
		kollisionsListe.push_back(wand35.pos());
		kollisionsListe.push_back(wand36.pos());
		kollisionsListe.push_back(wand37.pos());
		kollisionsListe.push_back(wand38.pos());
		kollisionsListe.push_back(wand39.pos());
		kollisionsListe.push_back(wand40.pos());

		kollisionsListe.push_back(wand41.pos());
		kollisionsListe.push_back(wand42.pos());
		kollisionsListe.push_back(wand43.pos());
		kollisionsListe.push_back(wand44.pos());
		kollisionsListe.push_back(wand45.pos());
		kollisionsListe.push_back(wand46.pos());
		kollisionsListe.push_back(wand47.pos());
		kollisionsListe.push_back(wand48.pos());
		kollisionsListe.push_back(wand49.pos());
		kollisionsListe.push_back(wand50.pos());
		kollisionsListe.push_back(wand51.pos());
		kollisionsListe.push_back(wand52.pos());

		kollisionsListe.push_back(figur1.pos());
		kollisionsListe.push_back(figur2.pos());
		kollisionsListe.push_back(figur3.pos());
		kollisionsListe.push_back(figur4.pos());

		kollisionsListeBeweglicherObjekte.push_back(wand51.pos());
		kollisionsListeBeweglicherObjekte.push_back(wand52.pos());
		kollisionsListeBeweglicherObjekte.push_back(figur1.pos());
		kollisionsListeBeweglicherObjekte.push_back(figur2.pos());
		kollisionsListeBeweglicherObjekte.push_back(figur3.pos());
		kollisionsListeBeweglicherObjekte.push_back(figur4.pos());

		if (p.positionsAbfrage(-15, -24, vView.x, vView.y, vView.z))
		{
			break;
		}

		if (p.Kollision3DKamera(kollisionsListe, vView.x, vView.y, vView.z))
		{
			foreward = false;
			if (p.KollisionKameraObjektVorhanden(kollisionsListeBeweglicherObjekte, vView.x, vView.y, vView.z))
			{
				moveable = true;
				if (p.KollisionKameraObjekt(kollisionsListeBeweglicherObjekte, vView.x, vView.y, vView.z) == 0)
				{
					doorOne = true;
					if (figuren == 4)
					{
						cout << "Du kannst die Wand mit e verschieben\n";
					}
					else
					{
						cout << "Du hast nicht genug Figuren\n " << figuren << " / 3\n";
					}
				}
				else
				{
					doorOne = false;
				}
				if (p.KollisionKameraObjekt(kollisionsListeBeweglicherObjekte, vView.x, vView.y, vView.z) == 1)
				{
					doorTwo = true;
					cout << "Du kannst die Wand mit e verschieben\n";
				}
				else
				{
					doorTwo = false;
				}
				if (p.KollisionKameraObjekt(kollisionsListeBeweglicherObjekte, vView.x, vView.y, vView.z) == 2)
				{
					yfigur1 = -5;
					figuren += 1;
					cout << figuren << " / 4 Figuren\n";
					if (figuren == 4)
					{
						cout << "Jetzt kannst du auch die letzte Wand verschieben\n";
					}
				}
				if (p.KollisionKameraObjekt(kollisionsListeBeweglicherObjekte, vView.x, vView.y, vView.z) == 3)
				{
					yfigur2 = -5;
					figuren += 1;
					cout << figuren << " / 4 Figuren\n";
					if (figuren == 4)
					{
						cout << "Jetzt kannst du auch die letzte Wand verschieben\n";
					}
				}
				if (p.KollisionKameraObjekt(kollisionsListeBeweglicherObjekte, vView.x, vView.y, vView.z) == 4)
				{
					yfigur3 = -5;
					figuren += 1;
					cout << figuren << " / 4 Figuren\n";
					if (figuren == 4)
					{
						cout << "Jetzt kannst du auch die letzte Wand verschieben\n";
					}
				}
				if (p.KollisionKameraObjekt(kollisionsListeBeweglicherObjekte, vView.x, vView.y, vView.z) == 5)
				{
					yfigur4 = -5;
					figuren += 1;
					cout << figuren << " / 4 Figuren\n";
					if (figuren == 4)
					{
						cout << "Jetzt kannst du auch die letzte Wand verschieben\n";
					}
				}
			}
			else
			{
				moveable = false;
			}
		}
		else
		{
			doorOne = false;
			doorTwo = false;
			moveable = false;
			foreward = true;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	boden.~Objekt();
	wand1.~Objekt();
	wand2.~Objekt();
	wand3.~Objekt();
	wand4.~Objekt();
	wand5.~Objekt();
	wand6.~Objekt();
	wand7.~Objekt();
	wand8.~Objekt();
	wand9.~Objekt();
	wand10.~Objekt();

	wand21.~Objekt();
	wand22.~Objekt();
	wand23.~Objekt();
	wand24.~Objekt();
	wand25.~Objekt();
	wand26.~Objekt();
	wand27.~Objekt();
	wand28.~Objekt();
	wand29.~Objekt();
	wand30.~Objekt();

	wand31.~Objekt();
	wand32.~Objekt();
	wand33.~Objekt();
	wand34.~Objekt();
	wand35.~Objekt();
	wand36.~Objekt();
	wand37.~Objekt();
	wand38.~Objekt();
	wand39.~Objekt();

	wand40.~Objekt();
	wand41.~Objekt();
	wand42.~Objekt();
	wand43.~Objekt();
	wand44.~Objekt();
	wand45.~Objekt();
	wand46.~Objekt();
	wand47.~Objekt();
	wand48.~Objekt();
	wand49.~Objekt();
	wand50.~Objekt();

	wand51.~Objekt();
	wand52.~Objekt();

	figur1.~Objekt();
	figur2.~Objekt();
	figur3.~Objekt();
	figur4.~Objekt();

	boden.~Objekt();
	decke.~Objekt();
	

	glDeleteProgram(programID);
}

void rotateViewY(float fAngle)
{
	glm::mat4 mRotation = glm::rotate(glm::mat4(1.0f), fAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 vDir = vView - vEye;
	glm::vec4 vNewView = mRotation * glm::vec4(vDir, 1.0f);
	vView = glm::vec3(vNewView.x, vNewView.y, vNewView.z);
	vView += vEye;
}

void move(float fBy)
{
	glm::vec3 vDir = vView - vEye;
	vDir *= fBy;
	vEye += vDir;
	vView += vDir;
}


void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE); //schliet das Fenster bei der ESC Taste
		break;

	case GLFW_KEY_W:
		if (foreward)
		{
			zpos = 1;
			move(zpos);
		}
		break;

	case GLFW_KEY_A:
		// TODO: change to strave left
		xrot = 90;
		rotateViewY(xrot / 180 * 3.141592654f);
		break;

	case GLFW_KEY_D:
		// TODO: change to strave right
		xrot = 90;
		rotateViewY(-xrot / 180 * 3.141592654f);
		break;

	case GLFW_KEY_S:

		// TODO: move backwards
		if (backward)
		{
			zpos = -1;
			move(zpos);
		}
		break;

	case GLFW_KEY_E:
		if (moveable)
		{
			if (doorOne && figuren == 4)
			{
				if (doorOpenOne)
				{
					tuerZ1 = -32;
					doorOpenOne = false;
				}
				else
				{
					tuerZ1 = -22;
					doorOpenOne = true;
				}
			}
			if (doorTwo)
			{
				if (doorOpenTwo)
				{
					tuerZ2 = -14;
					doorOpenTwo = false;
				}
				else
				{
					tuerZ2 = -3;
					doorOpenTwo = true;
				}
			}
		}
		break;

	default:
		break;
	}
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) 
{	
	if(button == GLFW_MOUSE_BUTTON_LEFT) {
		cout << "Button: " << button << ", Action: " << action << ", Mods: " << mods << endl;
		cout << "Left Button" << endl;
	}
}

void mouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos) 
{
	// cout << "MousePos: (x, y)  " << xpos << ", " << ypos << endl;
	mouseXPos = xpos;
	mouseYPos = ypos;

	glfwSetCursorPos(window, screenWidth / 2, screenHeight / 2);
}

void setupLight()
{
	glEnable(GL_LIGHTING) ;

	//	glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION) ;
	//	glEnable(GL_COLOR_MATERIAL) ;
	//	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE) ;

	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	//	GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; //Color(0.2, 0.2, 0.2)
	//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	//
	//	GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.5, 0.5)
	//	GLfloat lightPos0[] = {4.0f, 0.0f, 8.0f, 1.0f}; //Positioned at (4, 0, 8)
	//	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	//	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	// @See: http://www.videotutorialsrock.com/opengl_tutorial/lighting/text.php
	// @See: http://www.glprogramming.com/red/chapter05.html#name9	

	//	glEnable(GL_LIGHTING);
	//	glEnable(GL_COLOR_MATERIAL);
	//	glEnable(GL_LIGHT0);
	//
	//	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE) ;
	//	GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
	//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	//	GLfloat ambient_light[] = {0.3, 0.3, 0.3, 1.0};
	//	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
}
