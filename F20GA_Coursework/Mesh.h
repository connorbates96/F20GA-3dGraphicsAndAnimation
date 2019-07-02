#pragma once

#include <iostream>
#include <vector>

#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Primitives.h"


class Mesh {

private:

	unsigned nrOfVertices;
	unsigned nrOfIndices;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 ModelMatrix;


	//Creates then binds VAO, creates VBO then binds and sends data, creates EBO then binds and sends data Iif object contains indices)
	//Sets the attribute pointers which tells GPU where each piece of information will begin and the offset until the next piece.
	void initVAO(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices) {

		//Set variables
		this->nrOfVertices = nrOfVertices;
		this->nrOfIndices = nrOfIndices;

		//Create VAO
		glCreateVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		//GEN VBO AND BIND AND SEND DATA
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO); //simple buffer sends an array of floats
		glBufferData(GL_ARRAY_BUFFER, (this->nrOfVertices * sizeof(Vertex)), vertexArray, GL_STATIC_DRAW); //data sent to graphics card (GL_STATIC_DRAW means we don't want to change it too often, for this use GL_SYNAMIC_DRAW)

		//GEN EBO AND BIND AND SEND DATA
		if (this->nrOfIndices > 0) {
			glGenBuffers(1, &this->EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO); //used for indexing vertices
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (this->nrOfIndices * sizeof(GLuint)), indexArray, GL_STATIC_DRAW); //sending data from cpu to gpu is costly so only want to do it once
		}

		//SET VERTEXATTRIBPOINTERS AND ENABLE
		//Position
		//Called input assembly, This is where we tell the gpu what the data its getting is. Where are the textcoors, color, position in the long list of data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		//Colour
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);
		//TexCoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);
		//Normal
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);

		//BIND VAO 0
		glBindVertexArray(0);
	}

	void initVAO(Primitives* primitives) {

		//Set variables
		this->nrOfVertices = primitives->getNrOfVertices();
		this->nrOfIndices = primitives->getNrOfIndices();

		//Create VAO
		glCreateVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		//GEN VBO AND BIND AND SEND DATA
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO); //simple buffer sends an array of floats
		glBufferData(GL_ARRAY_BUFFER, (this->nrOfVertices * sizeof(Vertex)), primitives->getVertices(), GL_STATIC_DRAW); //data sent to graphics card (GL_STATIC_DRAW means we don't want to change it too often, for this use GL_SYNAMIC_DRAW)

		//GEN EBO AND BIND AND SEND DATA
		if (this->nrOfIndices > 0) {
			glGenBuffers(1, &this->EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO); //used for indexing vertices
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (this->nrOfIndices * sizeof(GLuint)), primitives->getIndices(), GL_STATIC_DRAW); //sending data from cpu to gpu is costly so only want to do it once
		}

		//SET VERTEXATTRIBPOINTERS AND ENABLE
		//Position
		//Called input assembly, This is where we tell the gpu what the data its getting is. Where are the textcoors, color, position in the long list of data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		//Colour
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);
		//TexCoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);
		//Normal
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);

		//BIND VAO 0
		glBindVertexArray(0);
	}


	void updateUniforms(Shader* shader) {

		shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
	}

	void updateModelMatrix() {

		this->ModelMatrix = glm::mat4(1.f); //creates an identity matrix
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->position); //moves the model in the worldview
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f)); //Rotates around the X axis
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f)); //Rotates around the Y axis
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f)); //Rotates around the Z axis
		this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale); //increases scale
	}


public:

	Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices,
		glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f)) {

		this->position = position;
		this->rotation = rotation;
		this->scale = scale;

		this->initVAO(vertexArray, nrOfVertices, indexArray, nrOfIndices);
		this->updateModelMatrix();
	}

	Mesh(Primitives* primitives, glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f)) {

		this->position = position;
		this->rotation = rotation;
		this->scale = scale;

		this->initVAO(primitives);
		this->updateModelMatrix();
	}


	~Mesh() {

		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		if (this->nrOfIndices > 0) {
			glDeleteBuffers(1, &this->EBO);
		}
	}

	void initObjModel(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices,
		glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {

		this->position = position;
		this->rotation = rotation;
		this->scale = scale;

		this->initVAO(vertexArray, nrOfVertices, indexArray, nrOfIndices);
		this->updateModelMatrix();

	}


	//Modifiers
	void setPosition(const glm::vec3 position) {

		this->position = position;
	}

	void setRotation(const glm::vec3 rotation) {

		this->rotation = rotation;
	}

	void setScale(const glm::vec3 scale) {

		this->scale = scale;
	}



	//Function

	void move(const glm::vec3 position) {

		this->position += position;
	}

	void rotate(const glm::vec3 rotation) {

		this->rotation += rotation;
	}

	void scaleChange(const glm::vec3 scale) {

		this->scale += scale;
	}

	float getHeight() {

		return this->position.y;
	}

	float getX() {
		return this->position.x;
	}

	float getZ() {
		return this->position.z;
	}


	void render(Shader* shader) {

		//Update uniforms
		this->updateModelMatrix();
		this->updateUniforms(shader);
		//updateUniforms unbinds the shader so use() is required.
		shader->use();

		//Bind vertex array object (VAO)
		glBindVertexArray(this->VAO);

		//Render
		if (this->nrOfIndices == 0)
			glDrawArrays(GL_TRIANGLES, 0, this->nrOfVertices);
		else
			glDrawElements(GL_TRIANGLES, this->nrOfIndices, GL_UNSIGNED_INT, 0);

	}
	

};


