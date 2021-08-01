#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shaderprogram.h"

class Room {
public:
	glm::mat4 M; // macierz modelu
	std::string floorPath; // sciezka do modelu podlogi
    std::string wallsPath; // sciezka do modelu podlogi
	GLuint floor_texture; // tekstura podlogi
    GLuint walls_texture; // tekstura scian
	std::vector<glm::vec4> floor_vertices; // tablica wierzcholkow
	std::vector<glm::vec4> floor_normals;
	std::vector<glm::vec2> floor_uv; // UV mapping (texture coordinates)
	std::vector<unsigned int> floor_indices;
    std::vector<glm::vec4> walls_vertices; // tablica wierzcholkow
	std::vector<glm::vec4> walls_normals;
	std::vector<glm::vec2> walls_uv; // UV mapping (texture coordinates)
	std::vector<unsigned int> walls_indices;

	Room(std::string, GLuint, std::string, GLuint); // konstruktor

	Room(){};

	void loadModel(); // ladowanie modelu
	void drawFloor(ShaderProgram *sp);
    void drawWalls(ShaderProgram *sp);
	void drawRoom(ShaderProgram *sp);
};