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
	std::vector<glm::vec2>  out_uv;
	
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

	unsigned int VAO,VBO, EBO;
	int noOfVertices,noOfFaces;
	GLuint vertexbuffer,uvbuffer,normalbuffer,texturebuffer;
	GLuint Texture,TextureID;
	int textureType;

	GLuint prgmID;

	OBJLoader();
	bool loadOBJ(const char * path, int texType);
	void computeIn();
	void loadValues();
    void setNormals();
    void setProgramID(GLuint ID);
	void setupBuffers();
	void Draw();
	void addBoundingBox();
	GLuint texInit(char *);
	
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

class Scene {
	vector<OBJLoader> objects;
	char texture1[256], texture2[256], texture3[256], texture4[256], texture5[256];
	GLuint floorTexture;

public:
	void drawAxes();
	void drawFloor();
};


#endif