#pragma once

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc/type_ptr.hpp>

#include "Shader.h"

class Material {

private:

	//These contain both the colours and the intensity.
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	//These are the textures
	GLint textureDiffuse;
	GLint textureSpecular;


public:

	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLint textureDiffuse, GLint textureSpecular) {

		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->textureDiffuse = textureDiffuse;
		this->textureSpecular = textureSpecular;
	}

	~Material() {}

	//Sends the information to the shader and saves it in the field with name given
	void sendToShader(Shader& program) {

		program.setVec3f(this->ambient, "material.ambient");
		program.setVec3f(this->diffuse, "material.diffuse");
		program.setVec3f(this->specular, "material.specular");
		program.set1i(this->textureDiffuse, "material.texture1Diffuse");
		program.set1i(this->textureSpecular, "material.texture1Specular");
	}

};