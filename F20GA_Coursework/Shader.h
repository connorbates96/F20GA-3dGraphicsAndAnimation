#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Shader {

private:

	GLuint id;


	//Private Fucntions

	//Returns a string that contains the code from the file
	std::string loadShaderSource(char* fileName) {

		std::string temp = "";
		std::string src = "";

		std::ifstream in_file;

		//Vertex Shader
		in_file.open(fileName);

		if (in_file.is_open()) {
			while (std::getline(in_file, temp)) {
				src += temp + "\n";
			}
		}
		else {
			std::cout << "ERROR:: SHADER:: COULD_NOT_OPEN_FILE: " << fileName << "\n"; //Throw??
		}


		in_file.close();
		return src;
	}


	//Creates a shader and then compiles the shader using the code returned from loadShaderSource.
	//glGetShaderiv checks the compilation status to ensure it has succeded and prints out error if required
	GLuint loadShader(GLenum type, char* fileName) {

		char infoLog[512];
		GLint success;

		GLuint shader = glCreateShader(type);
		std::string str_src = this->loadShaderSource(fileName);
		const GLchar* src = str_src.c_str();
		glShaderSource(shader, 1, &src, NULL);
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success) {

			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR:: SHADER:: COULD_NOT_COMPILE_SHADER: " << fileName << "\n"; //Throw??
			std::cout << infoLog << "\n";
		}

		return shader;
	}



	void linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader) {

		char infoLog[512];
		GLint success;

		this->id = glCreateProgram();

		glAttachShader(this->id, vertexShader);

		if (geometryShader)
			glAttachShader(this->id, geometryShader);

		glAttachShader(this->id, fragmentShader);

		glLinkProgram(this->id);

		glGetProgramiv(this->id, GL_LINK_STATUS, &success);
		if (!success) {

			glGetProgramInfoLog(this->id, 512, NULL, infoLog);

			std::cout << "ERROR:: SHADER:: COULD_NOT_LINK_PROGRAM" << "\n";
			std::cout << infoLog << "\n";
		}

		glUseProgram(0);

	}



public:

	//Constructors/Destructors
	Shader(char* vertexFile = "", char* geometryFile = "", char* fragmentFile = "") {

		GLuint vertexShader = 0;
		GLuint geometryShader = 0;
		GLuint fragmentShader = 0;

		vertexShader = loadShader(GL_VERTEX_SHADER, vertexFile);

		if (geometryFile != "") {
			geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryFile);
		}

		fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentFile);

		this->linkProgram(vertexShader, geometryShader, fragmentShader);

		glDeleteShader(vertexShader);
		glDeleteShader(geometryShader);
		glDeleteShader(fragmentShader);
	}

	~Shader() {
		glDeleteProgram(this->id);
	}


	//Set uniform functions

	void use() {
		glUseProgram(this->id);
	}

	void unuse() {
		glUseProgram(0);
	}


	//Functions for specifying the value of a uniform variable for the current program object
	//i: integer
	//f: float
	void set1i(GLint value, const GLchar* name) {
		this->use();
		glUniform1i(glGetUniformLocation(this->id, name), value);
		this->unuse();
	}

	void set1f(GLfloat value, const GLchar* name) {
		this->use();
		glUniform1f(glGetUniformLocation(this->id, name), value);
		this->unuse();
	}

	void setVec2f(glm::fvec2 value, const GLchar* name) {
		this->use();
		glUniform2fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
		this->unuse();
	}

	void setVec3f(glm::fvec3 value, const GLchar* name) {
		this->use();
		glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
		this->unuse();
	}

	void setVec4f(glm::fvec4 value, const GLchar* name) {
		this->use();
		glUniform4fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
		this->unuse();
	}

	void setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE) {
		this->use();
		glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
		this->unuse();
	}

	void setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE) {
		this->use();
		glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
		this->unuse();
	}
};