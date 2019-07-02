#pragma once

#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>
#include<algorithm>
#include<tuple>

#include<glew.h>
#include<glfw3.h>

#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Vertex.h"



struct MinMaxValues {
	float minX;
	float maxX;
	float minY;
	float maxY;
	float minZ;
	float maxZ;

	MinMaxValues() {
		minX = 0.0;
		maxX = 0.0;
		minY = 0.0;
		maxY = 0.0;
		minZ = 0.0;
		maxZ = 0.0;
	}
};

//Uses string stream to read in file and then depending on the prefix on that line,
//the remaining data on that line is then pushed back onto the corresponding vector.
// Once all lines have been read, transfer data into a Vertex vector and return it.
static std::tuple<std::vector<Vertex>, MinMaxValues>loadOBJ(const char* fileName){

	std::vector<glm::fvec3> vertex_positions;
	std::vector<glm::fvec2> vertex_texcoords;
	std::vector<glm::fvec3> vertex_normals;
	std::vector<GLint> vertex_indices;
	std::vector<GLint> vertex_normal_indices;
	std::vector<GLint> vertex_texcoord_indices;

	int nrOfVertices = 0;
	std::vector<Vertex> vertices;

	std::stringstream ss;
	std::ifstream in_file(fileName);
	std::string line;

	std::string prefix;
	glm::vec3 tempVec3;
	glm::vec2 tempVec2;
	GLint index;

	MinMaxValues minMax;

	if (in_file.is_open()){

		while (std::getline(in_file, line)){

			ss.clear();
			ss.str(line);
			ss >> prefix;

			if (prefix == "v"){

				ss >> tempVec3.x >> tempVec3.y >> tempVec3.z;
				vertex_positions.push_back(tempVec3);
				nrOfVertices++;

				if (tempVec3.x > minMax.maxX) minMax.maxX = tempVec3.x;
				if (tempVec3.x < minMax.minX) minMax.minX = tempVec3.x;
				if (tempVec3.y > minMax.maxY) minMax.maxY = tempVec3.y;
				if (tempVec3.y < minMax.minY) minMax.minY = tempVec3.y;
				if (tempVec3.z > minMax.maxZ) minMax.maxZ = tempVec3.z;
				if (tempVec3.z < minMax.minZ) minMax.minZ = tempVec3.z;

			}
			else if (prefix == "vt"){

				ss >> tempVec2.x >> tempVec2.y;
				vertex_texcoords.push_back(tempVec2);
			}
			else if (prefix == "vn"){

				ss >> tempVec3.x >> tempVec3.y >> tempVec3.z;
				vertex_normals.push_back(tempVec3);
			}
			else if (prefix == "f"){

				int counter = 0;

				while (ss >> index){

					if (counter == 0)
						vertex_indices.push_back(index);
					else if (counter == 1)
						vertex_texcoord_indices.push_back(index);
					else if (counter == 2)
						vertex_normal_indices.push_back(index);

					if (ss.peek() == '/'){

						counter++;
						ss.ignore(1, '/');
					}
					else if (ss.peek() == ' '){

						counter = 0;
						ss.ignore(1, ' ');
					}

					if (counter > 2)
						counter = 0;
				}
			}
		}

		for (size_t i = 0; i < vertex_indices.size(); i++){

			vertices.push_back(Vertex());
		}
		for (size_t i = 0; i < vertices.size(); i++){

			vertices[i].position = vertex_positions[vertex_indices[i] - 1];
		}
		for (size_t i = 0; i < vertex_texcoord_indices.size(); i++){

			vertices[i].texcoord = vertex_texcoords[vertex_texcoord_indices[i] - 1];
		}
		for (size_t i = 0; i < vertex_normal_indices.size(); i++){

			vertices[i].normal = vertex_normals[vertex_normal_indices[i] - 1];
		}
		for (size_t i = 0; i < nrOfVertices; i++){

			vertices[i].color = glm::vec3(1.f, 1.f, 1.f);
		}

	}
	else{
		std::cout << "ERROR::OBJLOADER::COULD_NOT_OPEN_FILE: " << fileName << "\n";
	}

	in_file.close();

	return std::make_tuple(vertices, minMax);
}