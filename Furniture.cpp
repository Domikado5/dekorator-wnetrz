#include "Furniture.h"

Furniture::Furniture(std::string obj, std::string tex)
{
	M = glm::mat4(1.0f);
	objPath = obj;
	texPath = tex;
}

void Furniture::loadModel() 
{
	// to be implemented
}

void Furniture::rotate(glm::mat4 M, float angle, glm::vec3 axis)
{
	this->M = glm::rotate(M, angle, axis);
}

void Furniture::scale(glm::mat4 M, glm::vec3 vec)
{
	this->M = glm::scale(M, vec);
}

void Furniture::translate(glm::mat4, glm::vec3 vec)
{
	this->M = glm::translate(M, vec);
}