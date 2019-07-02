#pragma once

#include <iostream>
#include <string>
#include <glew.h>
#include <glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Texture {

private:
	GLuint id;
	int width;
	int height;
	int nrChannels;
	unsigned int type;


public:


	//Constructor - loads in texture file using stbi_load and then generates a texture with a corresponding texture id.
	// If the image has loaded properly, define the texture image using the loaded data and generate mimap and define texture parameters.
	Texture(const char* fileName, GLenum type) {

		if (this->id) {
			glDeleteTextures(1, &this->id);
		}


		this->type = type;

		//Load Image and assign width, height and number of channels
		unsigned char* image = stbi_load(fileName, &this->width, &this->height, &this->nrChannels, 4);

		//Generate Image and Get ID
		glGenTextures(1, &this->id); //gen 1 texture and get id for texture
		glBindTexture(type, this->id); // memory location and bind to texture0

		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT); //wrap s = x coord
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT); //wrap t = y coord
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); //equivalent to anti-aliasing
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //wrap t = y coord

																 //If image exists, define texture image and generate mipmap
		if (image) {
			glTexImage2D(type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(type); //creates same image but in different resolutions
		}
		else {
			std::cout << "ERROR:: TEXTURE:: TEXTURE LOADING FAILED" << "\n";
		}

		glActiveTexture(0);		//sets active texture to 0
		glBindTexture(type, 0); //no texture is binded
		stbi_image_free(image); //removes any background noise

	}

	~Texture() {
		glDeleteTextures(1, &this->id);
	}

	inline GLuint getID() const { return this->id; }

	//Set the active texture.
	void bind(const GLint textureUnit) {

		glActiveTexture(GL_TEXTURE0 + textureUnit); //texture unit offsets the active texure (0 gives texture0, 1 gives texture1 etc.)
		glBindTexture(this->type, this->id);				 //Binds the texture

	}

	void unbind() {

		glActiveTexture(0);
		glBindTexture(this->type, 0);
	}

	void loadFromFile(const char* fileName) {

		if (this->id) {
			glDeleteTextures(1, &this->id);
		}

		//Load Image and assign width, height and number of channels
		unsigned char* image = stbi_load(fileName, &this->width, &this->height, &this->nrChannels, 0);

		//Generate Image and Get ID
		glGenTextures(1, &this->id); //gen 1 texture and get id for texture
		glBindTexture(type, this->id); // memory location and bind to texture0

		glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_REPEAT); //wrap s = x coord
		glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_REPEAT); //wrap t = y coord
		glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); //equivalent to anti-aliasing
		glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //wrap t = y coord

																	   //If image exists, define texture image and generate mipmap
		if (image) {
			glTexImage2D(this->type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(this->type); //creates same image but in different resolutions
		}
		else {
			std::cout << "ERROR:: TEXTURE:: LOADFROMFILE:: TEXTURE LOADING FAILED" << "\n";
		}

		glActiveTexture(0);		//sets active texture to 0
		glBindTexture(this->type, 0); //no texture is binded
		stbi_image_free(image); //removes any background noise

	}






};