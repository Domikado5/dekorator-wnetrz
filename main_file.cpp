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

#include <glew/include/GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "myCube.h"
#include "myTeapot.h"
#include <vector>
#include <iostream>
#include <fstream>

#include "Furniture.h"
#include "Room.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define MAX_OBJECTS 20

float speed_x=0;
float speed_y=0;
float aspectRatio=1;
float radius = 10.0f;
float fov = 50.0f;
// zmienne do obracania osią
float cam_rot_speed_x = 0;
float cam_rot_speed_y = 0;
float cam_rot_speed_z = 0;
// zmienne to poruszania kamerą
float cam_speed_x = 0;
float cam_speed_y = 0;

ShaderProgram *sp;

std::vector<glm::vec4> vertices;
std::vector<glm::vec4> normals;
std::vector<glm::vec2> texCoords;
std::vector<glm::vec4> colors;
std::vector<unsigned int> indices;
int vertexCount;
int selected = 0;

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

Furniture f[MAX_OBJECTS];

void saveProject(std::string filePath){ // save project progress to file
	for(int i = 0; i < tab.size(); i++){
		f[i] = (*tab[i]);
	}

	std::ofstream fstream_ob;
	fstream_ob.open(filePath, std::ios::out);
	fstream_ob.write((char *)&f, sizeof(f));
	fstream_ob.close();
	printf("Saved successfully.\n");
}

void loadProject(std::string filePath){ // load project from save file
	std::ifstream ifstream_ob;
	ifstream_ob.open(filePath, std::ios::in);
	ifstream_ob.read((char *) &f, sizeof(f));
	ifstream_ob.close();


	// const float *fSource;
	for(int i=0; i < tab.size(); i++){
		// fSource = (const float*)glm::value_ptr(f[i].M);
		(*tab[i]).M = f[i].M;
		(*tab[i]).M_rotate = f[i].M_rotate;
	}
	printf("Loaded successfully.\n");
}

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
		if (key == GLFW_KEY_F1)
		{
			saveProject("saves/save1.txt");
		}
		if (key == GLFW_KEY_F2)
		{
			loadProject("saves/save1.txt");
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
			if (key == GLFW_KEY_UP)
			{
				(*tab[selected]).rotate(glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // obraca wzgledem osi X w gore (chyba w gore XD)
			}
			if (key == GLFW_KEY_DOWN)
			{
				(*tab[selected]).rotate(glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			}
		}
		if (key == GLFW_KEY_T) // wybrany tryb przesuwania
		{
			mode = "t";
		}
		if (mode == "t"){
			if (key == GLFW_KEY_LEFT)
			{
				(*tab[selected]).translate(glm::vec3(0.0f, 0.0f, -0.1f)); // przesuwa w lewo
			}
			if (key == GLFW_KEY_RIGHT)
			{
				(*tab[selected]).translate(glm::vec3(0.0f, 0.0f, 0.1)); // w prawo
			}
			if (key == GLFW_KEY_UP)
			{
				(*tab[selected]).translate(glm::vec3(0.1f, 0.0f, 0.0f));
			}
			if (key == GLFW_KEY_DOWN)
			{
				(*tab[selected]).translate(glm::vec3(-0.1f, 0.0f, 0.0f));
			}
			if (key == GLFW_KEY_Z)
			{
				(*tab[selected]).translate(glm::vec3(0.0f, 0.1f, 0.0f));
			}
			if (key == GLFW_KEY_X)
			{
				(*tab[selected]).translate(glm::vec3(0.0f, -0.1f, 0.0f));
			}
		}
		if (key == GLFW_KEY_Y) // tryb skalowania
		{
			mode = "y";
		}
		if (mode == "y"){
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
			if (key == GLFW_KEY_LEFT) // jak nie bedzie dzialac to zmienic na GLFW_KEY_KP_4 (chodzi o sterowanie za pomoca numpada)
			{
				cam_rot_speed_x = -PI;
				cam_rot_speed_z = -PI;
			}
			if (key == GLFW_KEY_RIGHT)
			{
				cam_rot_speed_x = PI;
				cam_rot_speed_z = PI;
			}
			if (key == GLFW_KEY_UP)
			{
				cam_rot_speed_y = PI;
			}
			if (key == GLFW_KEY_DOWN)
			{
				cam_rot_speed_y = -PI;
			}
			if (key == GLFW_KEY_W)
			{
				cam_speed_y = 5.0f;
			}
			if (key == GLFW_KEY_S)
			{
				cam_speed_y = -5.0f;
			}
			if (key == GLFW_KEY_A)
			{
				cam_speed_x = -5.0f;
			}
			if (key == GLFW_KEY_D)
			{
				cam_speed_x = 5.0f;
			}
			if (key == GLFW_KEY_Z)
			{
				if (fov >= 5.0f) {fov -= 5.0f;}
			}
			if (key == GLFW_KEY_X)
			{
				if (fov <= 45.0f) {fov += 5.0f;}
			}
		}
	}

	if (action==GLFW_RELEASE) {
	   if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT)
	   {
		   cam_rot_speed_x = 0;
		   cam_rot_speed_z = 0;
	   }
	   if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN)
	   {
		   cam_rot_speed_y = 0;
	   }
	   if (key == GLFW_KEY_W || key == GLFW_KEY_S)
	   {
		   cam_speed_y = 0;
	   }
	   if (key == GLFW_KEY_A || key == GLFW_KEY_D)
	   {
		   cam_speed_x = 0;
	   }
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
void drawScene(GLFWwindow* window,float cam_angle_x, float cam_angle_y, float cam_angle_z, float cam_pos_z, float cam_pos_y) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float cam_x = sin(cam_angle_x) * radius;
	float cam_y = cam_angle_y * radius;
	float cam_z = cos(cam_angle_z) * radius;

	glm::mat4 V = glm::lookAt(glm::vec3(cam_x, cam_y, cam_z), glm::vec3(0.0f, cam_pos_y, cam_pos_z), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 P = glm::perspective(glm::radians(fov), aspectRatio, 1.0f, 50.0f); //Wylicz macierz rzutowania

	glm::mat4 M = glm::mat4(1.0f);

    sp->use();//Aktywacja programu cieniującego
    //Przeslij parametry programu cieniującego do karty graficznej
	// Nwm czy to ponizej tez zakomentowac
    glUniformMatrix4fv(sp->u("P"),1,false,glm::value_ptr(P));
    glUniformMatrix4fv(sp->u("V"),1,false,glm::value_ptr(V));
    glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));

	room.drawFloor(sp);
	room.drawWalls(sp);
	
	for(int i = 0; i < tab.size(); i++){
		(*tab[i]).drawModel(sp);
	}

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
	float cam_angle_x = 4.7f;
	float cam_angle_y = 0.5f;
	float cam_angle_z = 4.7f;
	float cam_pos_x = 0;
	float cam_pos_y = 0;
	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		double time = glfwGetTime();
		cam_angle_x += cam_rot_speed_x * time;
		cam_angle_y += cam_rot_speed_y * time;
		cam_angle_y = std::max(0.0f, cam_angle_y);
		cam_angle_y = std::min(1.5f, cam_angle_y);
		cam_angle_z += cam_rot_speed_z * time;
		cam_pos_x += cam_speed_x * time;
		cam_pos_y += cam_speed_y * time;
		cam_pos_y = std::max(0.0f, cam_pos_y);
        glfwSetTime(0); //Zeruj timer
		drawScene(window, cam_angle_x, cam_angle_y, cam_angle_z, cam_pos_x, cam_pos_y); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
