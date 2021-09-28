#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shaderprogram.h"

class Furniture {
public:
	glm::mat4 M; // macierz modelu
	glm::mat4 M_rotate;
	std::string objPath; // sciezka do obj
	GLuint texture; // tekstura
	std::vector<glm::vec4> vertices; // tablica wierzcholkow
	std::vector<glm::vec4> normals;
	std::vector<glm::vec2> uv; // koordynaty tekstur
	std::vector<unsigned int> indices;
	bool chosen;

	Furniture(std::string, GLuint); // konstruktor

	Furniture(){};

	void loadModel(); // ladowanie modelu
	void translate(glm::vec3); // przesuwanie
	void rotate(float, glm::vec3); // obracanie
	void scale(glm::vec3); // skalowanie
	void drawModel(ShaderProgram *sp); // rysowanie modelu
};