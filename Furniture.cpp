#include "Furniture.h"

void Furniture::loadModel() 
{
	Assimp::Importer imp;
	const aiScene* scene = imp.ReadFile(objPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
	std::string err = imp.GetErrorString();
	if(err.length() != 0){
		printf("Error While Loading Model %s : %s\n", objPath.c_str(), err.c_str());
	}
	aiMesh* mesh = scene->mMeshes[0];
	for (int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D vertex = mesh->mVertices[i];
		this->vertices.push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1));

		aiVector3D normal = mesh->mNormals[i];
		this->normals.push_back(glm::vec4(normal.x, normal.y, normal.z, 0));

		aiVector3D texCoords = mesh->mTextureCoords[0][i];
		this->uv.push_back(glm::vec2(texCoords.x, texCoords.y));
	}

	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace& face = mesh->mFaces[i];

		for (int j = 0; j < face.mNumIndices; j++) {
			this->indices.push_back(face.mIndices[j]);
		}
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
}

Furniture::Furniture(std::string obj, GLuint tex, glm::mat4 matrix)
{
	M = matrix;
	objPath = obj;
	texture = tex;
	this->loadModel();
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

void Furniture::drawModel(ShaderProgram *sp)
{
	sp->use();

	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));

	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices.data());

	glEnableVertexAttribArray(sp->a("normal"));  //Włącz przesyłanie danych do atrybutu normal
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals.data()); //Wskaż tablicę z danymi dla atrybutu normal

	glEnableVertexAttribArray(sp->a("texCoord0"));  //Włącz przesyłanie danych do atrybutu texCoord0
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, uv.data()); //Wskaż tablicę z danymi dla atrybutu texCoord0

	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord0"));
}