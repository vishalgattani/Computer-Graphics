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
	std::vector<glm::vec3>  tangents;
	std::vector<glm::vec3>  bitangents;
	std::vector<glm::vec2>  spherical_uvs;
	std::vector<glm::vec2>  cylindrical_uvs;
	std::vector<glm::vec2>  planar_uvs;
	std::vector<glm::vec2>  out_uv;
	std::vector<glm::vec2>  planar_builtin_textures;
	std::vector<glm::vec2>  cylindrical_builtin_textures;
	std::vector<glm::vec2>  spherical_builtin_textures;
	
	std::vector<glm::vec3>  temp_vertices;
	std::vector<glm::vec2>  temp_vertexuv;
    std::vector<glm::vec3>  temp_normals;

    std::vector<glm::vec4>  incircles;
	std::vector<unsigned int>  vertexIndices;

	float minx,miny,minz;
    float maxx,maxy,maxz;
    float centre[3],mx,my;
    float maxArea,minArea;
    float size,scale;
    float radius;
	unsigned int VAO,VBO, EBO;
	int noOfVertices,noOfFaces;
	GLuint vertexbuffer,uvbuffer,normalbuffer,tangentbuffer,bitangentbuffer,colorbuffer;
	GLuint Texture,TextureID;
	int textureType;

	unsigned int texturefile;

	GLuint prgmID;

	OBJLoader();
	bool loadOBJ(const char * path);
	void computeIn();
	void loadValues();
    void setNormals();
    void setProgramID(GLuint ID);
	void setupBuffers();
	void Draw();
	void addBoundingBox();
	void cylindricalTextures();
    void planarTextures();
    void sphericalTextures();
	GLuint texInit(char * texture);
	
	~OBJLoader();

};


#endif