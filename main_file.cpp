/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "myCube.h"
#include "myTeapot.h"
#include <vector>

#include "Furniture.h"
#include "Room.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

float speed_x=0;
float speed_y=0;
float aspectRatio=1;
float zoom=-5.5;

ShaderProgram *sp;


//Odkomentuj, żeby rysować kostkę
//float* vertices = myCubeVertices;
//float* normals = myCubeNormals;
//float* texCoords = myCubeTexCoords;
//float* colors = myCubeColors;
//int vertexCount = myCubeVertexCount;


//Odkomentuj, żeby rysować czajnik
// float* vertices = myTeapotVertices;
// float* normals = myTeapotVertexNormals;
// float* texCoords = myTeapotTexCoords;
// float* colors = myTeapotColors;
// int vertexCount = myTeapotVertexCount;

std::vector<glm::vec4> vertices;
std::vector<glm::vec4> normals;
std::vector<glm::vec2> texCoords;
std::vector<glm::vec4> colors;
std::vector<unsigned int> indices;
int vertexCount;
int selected = 0;
glm::vec3 up = { 0.0f, 1.0f, 0.0f }, eye = { 0.0f, 0.0f, 0.0f }, center = { 0.0f, 0.0f, 0.0f };

GLuint tex0;
GLuint tex1;
GLuint tex_floor;
GLuint tex_walls;

Furniture chair;
std::vector<Furniture *> tab;

Room room;

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

std::string mode = "c";

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    //if (action==GLFW_PRESS) {
       
    //}

	if (action == GLFW_PRESS)
	{
		if ((mods & GLFW_MOD_ALT != 0) && (key == GLFW_KEY_0)) 
		{
			// wybiera pierwszy obiekt (ustawia jakas zmienna selected na 0, ktora defiiowalaby ktory obiekt jest wybrany)
			selected = 0;
			printf("Picked furniture %d", selected);
		}
		if ((mods & GLFW_MOD_ALT != 0) && (key == GLFW_KEY_1))
		{
			// wybiera drugi obiekt
			selected = 1;
			printf("Picked furniture %d", selected);
		}
		if (key == GLFW_KEY_R) // wybrany tryb rotacji
		{
			mode = "r";
		}
		if (mode == "r"){
			if (key == GLFW_KEY_LEFT)
			{
				(*tab[selected]).rotate(glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // obraca wzgledem osi Y w lewo (chyba w lewo XD)
			}
			if (key == GLFW_KEY_RIGHT)
			{
				(*tab[selected]).rotate(glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			}
		}
		if (key == GLFW_KEY_M) // wybrany tryb przesuwania
		{
			mode = "m";
		}
		if (mode == "m"){
			if (key == GLFW_KEY_LEFT)
			{
				(*tab[selected]).translate(glm::vec3(0.0f, 0.0f, -0.1f)); // przesuwa w lewo
			}
			if (key == GLFW_KEY_RIGHT)
			{
				(*tab[selected]).translate(glm::vec3(0.0f, 0.0f, 0.1f)); // w prawo
			}
			if (key == GLFW_KEY_UP)
			{
				(*tab[selected]).translate(glm::vec3(0.1f, 0.0f, 0.0f));
			}
			if (key == GLFW_KEY_DOWN)
			{
				(*tab[selected]).translate(glm::vec3(-0.1f, 0.0f, 0.0f));
			}
		}
		if (key == GLFW_KEY_S) // tryb skalowania
		{
			mode = "s";
		}
		if (mode == "s"){
			if (key == GLFW_KEY_UP)
			{
				(*tab[selected]).scale(glm::vec3(1.5f, 1.5f, 1.5f)); // zwieksza
			}
			if (key == GLFW_KEY_DOWN)
			{
				(*tab[selected]).scale(glm::vec3(0.5f, 0.5f, 0.5f));
			}
		}
		if (key == GLFW_KEY_C) // tryb skalowania
		{
			mode = "c";
		}
		if (mode == "c"){
			// if (key==GLFW_KEY_LEFT) speed_x=-PI/2;
			// if (key==GLFW_KEY_RIGHT) speed_x=PI/2;
			// if (key==GLFW_KEY_UP) speed_y=PI/2;
			// if (key==GLFW_KEY_DOWN) speed_y=-PI/2;
			if (key==GLFW_KEY_LEFT_BRACKET) {center += glm::vec3(0.0f, 0.0f, 0.5f);}
			if (key==GLFW_KEY_RIGHT_BRACKET) {center += glm::vec3(0.0f, 0.0f, -0.5f);}
			if (key == GLFW_KEY_LEFT) {center += glm::vec3(-0.5f, 0.0f, 0.0f);}
			if (key==GLFW_KEY_RIGHT) {center += glm::vec3(0.5f, 0.0f, 0.0f);}
			if (key==GLFW_KEY_UP) {center += glm::vec3(0.0f, 0.5f, 0.0f);}
			if (key==GLFW_KEY_DOWN) {center += glm::vec3(0.0f, -0.5f, 0.0f);}
		}
	}

	if (action==GLFW_RELEASE) {
       if (key==GLFW_KEY_LEFT) speed_x=0;
       if (key==GLFW_KEY_RIGHT) speed_x=0;
       if (key==GLFW_KEY_UP) speed_y=0;
       if (key==GLFW_KEY_DOWN) speed_y=0;
    }
}

void windowResizeCallback(GLFWwindow* window,int width,int height) {
    if (height==0) return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);
}

GLuint readTexture(const char* filename) {	
      GLuint tex;	
      glActiveTexture(GL_TEXTURE0); 	

      //Wczytanie do pamięci komputera
      std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
      unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
      //Wczytaj obrazek
      unsigned error = lodepng::decode(image, width, height, filename);
     
      //Import do pamięci karty graficznej
      glGenTextures(1,&tex); //Zainicjuj jeden uchwyt
      glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
      //Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
      glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());	

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	

      return tex;
    }


//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window,windowResizeCallback);
	glfwSetKeyCallback(window,keyCallback);

	sp=new ShaderProgram("v_simplest.glsl",NULL,"f_simplest.glsl");
	tex0 = readTexture("bricks.png");
	tex1 = readTexture("sky.png");
	tex_floor = readTexture("textures/floor_wooden.png");
	tex_walls = readTexture("textures/walls_old.png");

	room = Room("models/floor.obj", tex_floor, "models/walls.obj", tex_walls);

	chair = Furniture("models/chair.obj", tex0);
	tab = {&chair};
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************

    delete sp;
}


//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window,float angle_x,float angle_y) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V=glm::lookAt(
         glm::vec3(-5.0, 4.5, 0.0),
         glm::vec3(0.0f, 0.5f, 0.0f),
         glm::vec3(0.0f,1.0f,0.0f)); //Wylicz macierz widoku
	// glm::mat4 V = glm::lookAt(glm::vec3(0, 0, zoom), center, glm::vec3(0.0f,1.0f,0.0f));

    glm::mat4 P=glm::perspective(50.0f*PI/180.0f, aspectRatio, 0.01f, 50.0f); //Wylicz macierz rzutowania

	glm::mat4 M=glm::mat4(1.0f);
	
	// M=glm::scale(M, glm::vec3(0.5f, 0.5f, 0.5f));
	// M=glm::rotate(M,angle_y,glm::vec3(1.0f,0.0f,0.0f)); //Wylicz macierz modelu
	// M=glm::rotate(M,angle_x,glm::vec3(0.0f,1.0f,0.0f)); //Wylicz macierz modelu

    sp->use();//Aktywacja programu cieniującego
    //Przeslij parametry programu cieniującego do karty graficznej
	// Nwm czy to ponizej tez zakomentowac
    glUniformMatrix4fv(sp->u("P"),1,false,glm::value_ptr(P));
    glUniformMatrix4fv(sp->u("V"),1,false,glm::value_ptr(V));
    glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));

	room.drawRoom(sp);
	chair.drawModel(sp);

    glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1280, 720, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	//Główna pętla
	float angle_x=0; //Aktualny kąt obrotu obiektu
	float angle_y=0; //Aktualny kąt obrotu obiektu
	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
        angle_x+=speed_x*glfwGetTime(); //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
        angle_y+=speed_y*glfwGetTime(); //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
        glfwSetTime(0); //Zeruj timer
		drawScene(window,angle_x,angle_y); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
