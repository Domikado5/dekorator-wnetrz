#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

class Furniture {
public:
	glm::mat4 M; // macierz modelu
	std::string objPath; // sciezka do obj
	std::string texPath; // sciezka do tekstury

	Furniture(std::string, std::string); // konstruktor

	void loadModel(); // ladowanie modelu
	void translate(glm::mat4, glm::vec3); // przesuwanie
	void rotate(glm::mat4, float, glm::vec3); // obracanie
	void scale(glm::mat4, glm::vec3); // skalowanie
};