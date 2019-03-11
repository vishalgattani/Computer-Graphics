#ifndef OBJLOADER_H
#define OBJLOADER_H
#pragma once

#include <vector>
#include <glm/glm.hpp>



using namespace glm;
using namespace std;

class OBJLoader{
	public:
	std::vector<glm::vec3>  out_vertices; 
	std::vector<glm::vec3>  out_normals;
	std::vector<glm::vec3>  out_colors;
	std::vector<glm::vec3>  temp_vertices;
    std::vector<glm::vec3>  temp_normals;
    std::vector<glm::vec4>  incircles;
    
	std::vector<unsigned int>  vertexIndices;	
	unsigned int VAO,VBO, EBO;
	GLuint vertexbuffer,uvbuffer,normalbuffer;


	OBJLoader();
	bool loadOBJ(const char * path);
	void computeIn();
	void loadValues();
    void setNormals();
	void setupBuffers();
	void Draw();
	~OBJLoader();

};
/*
bool loadOBJ(
	const char * path, 
	std::vector<glm::vec3> & out_vertices, 
	std::vector<glm::vec3> & out_normals,
	std::vector<glm::vec3> & out_colors
	
);
*/

#endif